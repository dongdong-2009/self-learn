/* Copyright Joyent, Inc. and other Node contributors. All rights reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 */

#include "uv.h"
#include "internal.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <errno.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/uio.h>
#include <sys/un.h>
#include <unistd.h>

#if defined(__APPLE__)
# include <sys/event.h>
# include <sys/time.h>
# include <sys/select.h>

/* Forward declaration */
typedef struct uv__stream_select_s uv__stream_select_t;

struct uv__stream_select_s {
  uv_stream_t* stream;
  uv_thread_t thread;
  uv_sem_t sem;
  uv_mutex_t mutex;
  uv_async_t async;
  int events;
  int fake_fd;
  int int_fd;
  int fd;
};
#endif /* defined(__APPLE__) */

static void uv__stream_connect(uv_stream_t*);
static void uv__write(uv_stream_t* stream);
static void uv__read(uv_stream_t* stream);
static void uv__stream_io(uv_loop_t* loop, uv__io_t* w, unsigned int events);


/* Used by the accept() EMFILE party trick. */
static int uv__open_cloexec(const char* path, int flags) {
  int fd;

#if defined(__linux__)
  fd = open(path, flags | UV__O_CLOEXEC);
  if (fd != -1)
    return fd;

  if (errno != EINVAL)
    return -1;

  /* O_CLOEXEC not supported. */
#endif

  fd = open(path, flags);
  if (fd != -1)
    uv__cloexec(fd, 1);

  return fd;
}


static size_t uv__buf_count(uv_buf_t bufs[], int bufcnt) {
  size_t total = 0;
  int i;

  for (i = 0; i < bufcnt; i++) {
    total += bufs[i].len;
  }

  return total;
}


void uv__stream_init(uv_loop_t* loop,
                     uv_stream_t* stream,
                     uv_handle_type type) {
  uv__handle_init(loop, (uv_handle_t*)stream, type);
  stream->read_cb = NULL;
  stream->read2_cb = NULL;
  stream->alloc_cb = NULL;
  stream->close_cb = NULL;
  stream->connection_cb = NULL;
  stream->connect_req = NULL;
  stream->shutdown_req = NULL;
  stream->accepted_fd = -1;
  stream->delayed_error = 0;
  ngx_queue_init(&stream->write_queue);
  ngx_queue_init(&stream->write_completed_queue);
  stream->write_queue_size = 0;

  if (loop->emfile_fd == -1)
    loop->emfile_fd = uv__open_cloexec("/", O_RDONLY);

#if defined(__APPLE__)
  stream->select = NULL;
#endif /* defined(__APPLE_) */

  uv__io_init(&stream->io_watcher, uv__stream_io, -1);
}


#if defined(__APPLE__)
void uv__stream_osx_select(void* arg) {
  uv_stream_t* stream;
  uv__stream_select_t* s;
  char buf[1024];
  fd_set sread;
  fd_set swrite;
  fd_set serror;
  int events;
  int fd;
  int r;
  int max_fd;

  stream = arg;
  s = stream->select;
  fd = stream->io_watcher.fd;

  if (fd > s->int_fd)
    max_fd = fd;
  else
    max_fd = s->int_fd;

  while (1) {
    /* Terminate on semaphore */
    if (uv_sem_trywait(&s->sem) == 0)
      break;

    /* Watch fd using select(2) */
    FD_ZERO(&sread);
    FD_ZERO(&swrite);
    FD_ZERO(&serror);

    if (uv_is_readable(stream))
      FD_SET(fd, &sread);
    if (uv_is_writable(stream))
      FD_SET(fd, &swrite);
    FD_SET(fd, &serror);
    FD_SET(s->int_fd, &sread);

    /* Wait indefinitely for fd events */
    r = select(max_fd + 1, &sread, &swrite, &serror, NULL);
    if (r == -1) {
      if (errno == EINTR)
        continue;

      /* XXX: Possible?! */
      abort();
    }

    /* Ignore timeouts */
    if (r == 0)
      continue;

    /* Empty socketpair's buffer in case of interruption */
    if (FD_ISSET(s->int_fd, &sread))
      while (1) {
        r = read(s->int_fd, buf, sizeof(buf));

        if (r == sizeof(buf))
          continue;

        if (r != -1)
          break;

        if (errno == EAGAIN || errno == EWOULDBLOCK)
          break;

        if (errno == EINTR)
          continue;

        abort();
      }

    /* Handle events */
    events = 0;
    if (FD_ISSET(fd, &sread))
      events |= UV__POLLIN;
    if (FD_ISSET(fd, &swrite))
      events |= UV__POLLOUT;
    if (FD_ISSET(fd, &serror))
      events |= UV__POLLERR;

    uv_mutex_lock(&s->mutex);
    s->events |= events;
    uv_mutex_unlock(&s->mutex);

    if (events != 0)
      uv_async_send(&s->async);
  }
}


void uv__stream_osx_interrupt_select(uv_stream_t* stream) {
  /* Notify select() thread about state change */
  uv__stream_select_t* s;
  int r;

  s = stream->select;

  /* Interrupt select() loop
   * NOTE: fake_fd and int_fd are socketpair(), thus writing to one will
   * emit read event on other side
   */
  do
    r = write(s->fake_fd, "x", 1);
  while (r == -1 && errno == EINTR);

  assert(r == 1);
}


void uv__stream_osx_select_cb(uv_async_t* handle, int status) {
  uv__stream_select_t* s;
  uv_stream_t* stream;
  int events;

  s = container_of(handle, uv__stream_select_t, async);
  stream = s->stream;

  /* Get and reset stream's events */
  uv_mutex_lock(&s->mutex);
  events = s->events;
  s->events = 0;
  uv_mutex_unlock(&s->mutex);

  assert(0 == (events & UV__POLLERR));

  /* Invoke callback on event-loop */
  if ((events & UV__POLLIN) && uv__io_active(&stream->io_watcher, UV__POLLIN))
    uv__stream_io(stream->loop, &stream->io_watcher, UV__POLLIN);

  if ((events & UV__POLLOUT) && uv__io_active(&stream->io_watcher, UV__POLLOUT))
    uv__stream_io(stream->loop, &stream->io_watcher, UV__POLLOUT);
}


void uv__stream_osx_cb_close(uv_handle_t* async) {
  uv__stream_select_t* s;

  s = container_of(async, uv__stream_select_t, async);
  free(s);
}


int uv__stream_try_select(uv_stream_t* stream, int fd) {
  /*
   * kqueue doesn't work with some files from /dev mount on osx.
   * select(2) in separate thread for those fds
   */

  struct kevent filter[1];
  struct kevent events[1];
  struct timespec timeout;
  uv__stream_select_t* s;
  int fds[2];
  int ret;
  int kq;

  kq = kqueue();
  if (kq == -1) {
    fprintf(stderr, "(libuv) Failed to create kqueue (%d)\n", errno);
    return uv__set_sys_error(stream->loop, errno);
  }

  EV_SET(&filter[0], fd, EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0, 0);

  /* Use small timeout, because we only want to capture EINVALs */
  timeout.tv_sec = 0;
  timeout.tv_nsec = 1;

  ret = kevent(kq, filter, 1, events, 1, &timeout);
  SAVE_ERRNO(close(kq));

  if (ret == -1)
    return uv__set_sys_error(stream->loop, errno);

  if ((events[0].flags & EV_ERROR) == 0 || events[0].data != EINVAL)
    return 0;

  /* At this point we definitely know that this fd won't work with kqueue */
  s = malloc(sizeof(*s));
  if (s == NULL)
    return uv__set_artificial_error(stream->loop, UV_ENOMEM);

  s->fd = fd;

  if (uv_async_init(stream->loop, &s->async, uv__stream_osx_select_cb)) {
    SAVE_ERRNO(free(s));
    return uv__set_sys_error(stream->loop, errno);
  }

  s->async.flags |= UV__HANDLE_INTERNAL;
  uv__handle_unref(&s->async);

  if (uv_sem_init(&s->sem, 0))
    goto fatal1;

  if (uv_mutex_init(&s->mutex))
    goto fatal2;

  /* Create fds for io watcher and to interrupt the select() loop. */
  if (socketpair(AF_UNIX, SOCK_STREAM, 0, fds))
    goto fatal3;

  s->fake_fd = fds[0];
  s->int_fd = fds[1];

  if (uv_thread_create(&s->thread, uv__stream_osx_select, stream))
    goto fatal4;

  s->stream = stream;
  stream->select = s;

  return 0;

fatal4:
  close(s->fake_fd);
  close(s->int_fd);
  s->fake_fd = -1;
  s->int_fd = -1;
fatal3:
  uv_mutex_destroy(&s->mutex);
fatal2:
  uv_sem_destroy(&s->sem);
fatal1:
  uv_close((uv_handle_t*) &s->async, uv__stream_osx_cb_close);
  return uv__set_sys_error(stream->loop, errno);
}
#endif /* defined(__APPLE__) */


int uv__stream_open(uv_stream_t* stream, int fd, int flags) {
  assert(fd >= 0);
  stream->flags |= flags;

  if (stream->type == UV_TCP) {
    if ((stream->flags & UV_TCP_NODELAY) && uv__tcp_nodelay(fd, 1))
      return uv__set_sys_error(stream->loop, errno);

    /* TODO Use delay the user passed in. */
    if ((stream->flags & UV_TCP_KEEPALIVE) && uv__tcp_keepalive(fd, 1, 60))
      return uv__set_sys_error(stream->loop, errno);
  }

#if defined(__APPLE__)
  {
    uv__stream_select_t* s;
    int r;

    r = uv__stream_try_select(stream, fd);
    if (r == -1)
      return r;

    s = stream->select;
    if (s != NULL)
      fd = s->fake_fd;
  }
#endif /* defined(__APPLE__) */

  stream->io_watcher.fd = fd;

  return 0;
}


void uv__stream_destroy(uv_stream_t* stream) {
  uv_write_t* req;
  ngx_queue_t* q;

  assert(!uv__io_active(&stream->io_watcher, UV__POLLIN | UV__POLLOUT));
  assert(stream->flags & UV_CLOSED);

  if (stream->connect_req) {
    uv__req_unregister(stream->loop, stream->connect_req);
    uv__set_artificial_error(stream->loop, UV_ECANCELED);
    stream->connect_req->cb(stream->connect_req, -1);
    stream->connect_req = NULL;
  }

  while (!ngx_queue_empty(&stream->write_queue)) {
    q = ngx_queue_head(&stream->write_queue);
    ngx_queue_remove(q);

    req = ngx_queue_data(q, uv_write_t, queue);
    uv__req_unregister(stream->loop, req);

    if (req->bufs != req->bufsml)
      free(req->bufs);

    if (req->cb) {
      uv__set_artificial_error(req->handle->loop, UV_ECANCELED);
      req->cb(req, -1);
    }
  }

  while (!ngx_queue_empty(&stream->write_completed_queue)) {
    q = ngx_queue_head(&stream->write_completed_queue);
    ngx_queue_remove(q);

    req = ngx_queue_data(q, uv_write_t, queue);
    uv__req_unregister(stream->loop, req);

    if (req->cb) {
      uv__set_sys_error(stream->loop, req->error);
      req->cb(req, req->error ? -1 : 0);
    }
  }

  if (stream->shutdown_req) {
    uv__req_unregister(stream->loop, stream->shutdown_req);
    uv__set_artificial_error(stream->loop, UV_ECANCELED);
    stream->shutdown_req->cb(stream->shutdown_req, -1);
    stream->shutdown_req = NULL;
  }
}


/* Implements a best effort approach to mitigating accept() EMFILE errors.
 * We have a spare file descriptor stashed away that we close to get below
 * the EMFILE limit. Next, we accept all pending connections and close them
 * immediately to signal the clients that we're overloaded - and we are, but
 * we still keep on trucking.
 *
 * There is one caveat: it's not reliable in a multi-threaded environment.
 * The file descriptor limit is per process. Our party trick fails if another
 * thread opens a file or creates a socket in the time window between us
 * calling close() and accept().
 */
static int uv__emfile_trick(uv_loop_t* loop, int accept_fd) {
  int fd;
  int r;

  if (loop->emfile_fd == -1)
    return -1;

  close(loop->emfile_fd);

  for (;;) {
    fd = uv__accept(accept_fd);

    if (fd != -1) {
      close(fd);
      continue;
    }

    if (errno == EINTR)
      continue;

    if (errno == EAGAIN || errno == EWOULDBLOCK)
      r = 0;
    else
      r = -1;

    loop->emfile_fd = uv__open_cloexec("/", O_RDONLY);

    return r;
  }
}


void uv__server_io(uv_loop_t* loop, uv__io_t* w, unsigned int events) {
  static int use_emfile_trick = -1;
  uv_stream_t* stream;
  int fd;
  int r;

  stream = container_of(w, uv_stream_t, io_watcher);
  assert(events == UV__POLLIN);
  assert(stream->accepted_fd == -1);
  assert(!(stream->flags & UV_CLOSING));

  if (stream->accepted_fd == -1)
    uv__io_start(stream->loop, &stream->io_watcher, UV__POLLIN);

  /* connection_cb can close the server socket while we're
   * in the loop so check it on each iteration.
   */
  while (uv__stream_fd(stream) != -1) {
    assert(stream->accepted_fd == -1);
    fd = uv__accept(uv__stream_fd(stream));

    if (fd == -1) {
      switch (errno) {
#if EWOULDBLOCK != EAGAIN
      case EWOULDBLOCK:
#endif
      case EAGAIN:
        return; /* Not an error. */

      case ECONNABORTED:
        continue; /* Ignore. */

      case EMFILE:
      case ENFILE:
        if (use_emfile_trick == -1) {
          const char* val = getenv("UV_ACCEPT_EMFILE_TRICK");
          use_emfile_trick = (val == NULL || atoi(val) != 0);
        }

        if (use_emfile_trick) {
          SAVE_ERRNO(r = uv__emfile_trick(loop, uv__stream_fd(stream)));
          if (r == 0)
            continue;
        }

        /* Fall through. */

      default:
        uv__set_sys_error(loop, errno);
        stream->connection_cb(stream, -1);
        continue;
      }
    }

    stream->accepted_fd = fd;
    stream->connection_cb(stream, 0);

    if (stream->accepted_fd != -1) {
      /* The user hasn't yet accepted called uv_accept() */
      uv__io_stop(loop, &stream->io_watcher, UV__POLLIN);
      return;
    }

    if (stream->type == UV_TCP && (stream->flags & UV_TCP_SINGLE_ACCEPT)) {
      /* Give other processes a chance to accept connections. */
      struct timespec timeout = { 0, 1 };
      nanosleep(&timeout, NULL);
    }
  }
}


int uv_accept(uv_stream_t* server, uv_stream_t* client) {
  uv_stream_t* streamServer;
  uv_stream_t* streamClient;
  int saved_errno;
  int status;

  /* TODO document this */
  assert(server->loop == client->loop);

  saved_errno = errno;
  status = -1;

  streamServer = (uv_stream_t*)server;
  streamClient = (uv_stream_t*)client;

  if (streamServer->accepted_fd < 0) {
    uv__set_sys_error(server->loop, EAGAIN);
    goto out;
  }

  switch (streamClient->type) {
    case UV_NAMED_PIPE:
    case UV_TCP:
      if (uv__stream_open(streamClient, streamServer->accepted_fd,
            UV_STREAM_READABLE | UV_STREAM_WRITABLE)) {
        /* TODO handle error */
        close(streamServer->accepted_fd);
        streamServer->accepted_fd = -1;
        goto out;
      }
      break;

    case UV_UDP:
      if (uv_udp_open((uv_udp_t*) client, streamServer->accepted_fd)) {
        close(streamServer->accepted_fd);
        streamServer->accepted_fd = -1;
        goto out;
      }
      break;

    default:
      assert(0);
  }

  uv__io_start(streamServer->loop, &streamServer->io_watcher, UV__POLLIN);
  streamServer->accepted_fd = -1;
  status = 0;

out:
  errno = saved_errno;
  return status;
}


int uv_listen(uv_stream_t* stream, int backlog, uv_connection_cb cb) {
  int r;

  switch (stream->type) {
    case UV_TCP:
      r = uv_tcp_listen((uv_tcp_t*)stream, backlog, cb);
      break;

    case UV_NAMED_PIPE:
      r = uv_pipe_listen((uv_pipe_t*)stream, backlog, cb);
      break;

    default:
      assert(0);
      return -1;
  }

  if (r == 0)
    uv__handle_start(stream);

  return r;
}


static void uv__drain(uv_stream_t* stream) {
  uv_shutdown_t* req;

  assert(ngx_queue_empty(&stream->write_queue));
  assert(stream->write_queue_size == 0);

  uv__io_stop(stream->loop, &stream->io_watcher, UV__POLLOUT);

  /* Shutdown? */
  if ((stream->flags & UV_STREAM_SHUTTING) &&
      !(stream->flags & UV_CLOSING) &&
      !(stream->flags & UV_STREAM_SHUT)) {
    assert(stream->shutdown_req);

    req = stream->shutdown_req;
    stream->shutdown_req = NULL;
    uv__req_unregister(stream->loop, req);

    if (shutdown(uv__stream_fd(stream), SHUT_WR)) {
      /* Error. Report it. User should call uv_close(). */
      uv__set_sys_error(stream->loop, errno);
      if (req->cb) {
        req->cb(req, -1);
      }
    } else {
      uv__set_sys_error(stream->loop, 0);
      ((uv_handle_t*) stream)->flags |= UV_STREAM_SHUT;
      if (req->cb) {
        req->cb(req, 0);
      }
    }
  }
}


static size_t uv__write_req_size(uv_write_t* req) {
  size_t size;

  size = uv__buf_count(req->bufs + req->write_index,
                       req->bufcnt - req->write_index);
  assert(req->handle->write_queue_size >= size);

  return size;
}


static void uv__write_req_finish(uv_write_t* req) {
  uv_stream_t* stream = req->handle;

  /* Pop the req off tcp->write_queue. */
  ngx_queue_remove(&req->queue);
  if (req->bufs != req->bufsml) {
    free(req->bufs);
  }
  req->bufs = NULL;

  /* Add it to the write_completed_queue where it will have its
   * callback called in the near future.
   */
  ngx_queue_insert_tail(&stream->write_completed_queue, &req->queue);
  uv__io_feed(stream->loop, &stream->io_watcher);
}


static int uv__handle_fd(uv_handle_t* handle) {
  switch (handle->type) {
    case UV_NAMED_PIPE:
    case UV_TCP:
      return ((uv_stream_t*) handle)->io_watcher.fd;

    case UV_UDP:
      return ((uv_udp_t*) handle)->io_watcher.fd;

    default:
      return -1;
  }
}


static void uv__write(uv_stream_t* stream) {
  struct iovec* iov;
  ngx_queue_t* q;
  uv_write_t* req;
  int iovcnt;
  ssize_t n;

start:

  assert(uv__stream_fd(stream) >= 0);

  if (ngx_queue_empty(&stream->write_queue)) {
    assert(stream->write_queue_size == 0);
    return;
  }

  q = ngx_queue_head(&stream->write_queue);
  req = ngx_queue_data(q, uv_write_t, queue);
  assert(req->handle == stream);

  /*
   * Cast to iovec. We had to have our own uv_buf_t instead of iovec
   * because Windows's WSABUF is not an iovec.
   */
  assert(sizeof(uv_buf_t) == sizeof(struct iovec));
  iov = (struct iovec*) &(req->bufs[req->write_index]);
  iovcnt = req->bufcnt - req->write_index;

  /*
   * Now do the actual writev. Note that we've been updating the pointers
   * inside the iov each time we write. So there is no need to offset it.
   */

  if (req->send_handle) {
    struct msghdr msg;
    char scratch[64];
    struct cmsghdr *cmsg;
    int fd_to_send = uv__handle_fd((uv_handle_t*) req->send_handle);

    assert(fd_to_send >= 0);

    msg.msg_name = NULL;
    msg.msg_namelen = 0;
    msg.msg_iov = iov;
    msg.msg_iovlen = iovcnt;
    msg.msg_flags = 0;

    msg.msg_control = (void*) scratch;
    msg.msg_controllen = CMSG_LEN(sizeof(fd_to_send));

    cmsg = CMSG_FIRSTHDR(&msg);
    cmsg->cmsg_level = SOL_SOCKET;
    cmsg->cmsg_type = SCM_RIGHTS;
    cmsg->cmsg_len = msg.msg_controllen;

    /* silence aliasing warning */
    {
      void* pv = CMSG_DATA(cmsg);
      int* pi = pv;
      *pi = fd_to_send;
    }

    do {
      n = sendmsg(uv__stream_fd(stream), &msg, 0);
    }
    while (n == -1 && errno == EINTR);
  } else {
    do {
      if (iovcnt == 1) {
        n = write(uv__stream_fd(stream), iov[0].iov_base, iov[0].iov_len);
      } else {
        n = writev(uv__stream_fd(stream), iov, iovcnt);
      }
    }
    while (n == -1 && errno == EINTR);
  }

  if (n < 0) {
    if (errno != EAGAIN && errno != EWOULDBLOCK) {
      /* Error */
      req->error = errno;
      stream->write_queue_size -= uv__write_req_size(req);
      uv__write_req_finish(req);
      return;
    } else if (stream->flags & UV_STREAM_BLOCKING) {
      /* If this is a blocking stream, try again. */
      goto start;
    }
  } else {
    /* Successful write */

    while (n >= 0) {
      uv_buf_t* buf = &(req->bufs[req->write_index]);
      size_t len = buf->len;

      assert(req->write_index < req->bufcnt);

      if ((size_t)n < len) {
        buf->base += n;
        buf->len -= n;
        stream->write_queue_size -= n;
        n = 0;

        /* There is more to write. */
        if (stream->flags & UV_STREAM_BLOCKING) {
          /*
           * If we're blocking then we should not be enabling the write
           * watcher - instead we need to try again.
           */
          goto start;
        } else {
          /* Break loop and ensure the watcher is pending. */
          break;
        }

      } else {
        /* Finished writing the buf at index req->write_index. */
        req->write_index++;

        assert((size_t)n >= len);
        n -= len;

        assert(stream->write_queue_size >= len);
        stream->write_queue_size -= len;

        if (req->write_index == req->bufcnt) {
          /* Then we're done! */
          assert(n == 0);
          uv__write_req_finish(req);
          /* TODO: start trying to write the next request. */
          return;
        }
      }
    }
  }

  /* Either we've counted n down to zero or we've got EAGAIN. */
  assert(n == 0 || n == -1);

  /* Only non-blocking streams should use the write_watcher. */
  assert(!(stream->flags & UV_STREAM_BLOCKING));

  /* We're not done. */
  uv__io_start(stream->loop, &stream->io_watcher, UV__POLLOUT);
}


static void uv__write_callbacks(uv_stream_t* stream) {
  uv_write_t* req;
  ngx_queue_t* q;

  while (!ngx_queue_empty(&stream->write_completed_queue)) {
    /* Pop a req off write_completed_queue. */
    q = ngx_queue_head(&stream->write_completed_queue);
    req = ngx_queue_data(q, uv_write_t, queue);
    ngx_queue_remove(q);
    uv__req_unregister(stream->loop, req);

    /* NOTE: call callback AFTER freeing the request data. */
    if (req->cb) {
      uv__set_sys_error(stream->loop, req->error);
      req->cb(req, req->error ? -1 : 0);
    }
  }

  assert(ngx_queue_empty(&stream->write_completed_queue));

  /* Write queue drained. */
  if (ngx_queue_empty(&stream->write_queue))
    uv__drain(stream);
}


static uv_handle_type uv__handle_type(int fd) {
  struct sockaddr_storage ss;
  socklen_t len;
  int type;

  memset(&ss, 0, sizeof(ss));
  len = sizeof(ss);

  if (getsockname(fd, (struct sockaddr*)&ss, &len))
    return UV_UNKNOWN_HANDLE;

  len = sizeof type;

  if (getsockopt(fd, SOL_SOCKET, SO_TYPE, &type, &len))
    return UV_UNKNOWN_HANDLE;

  if (type == SOCK_STREAM) {
    switch (ss.ss_family) {
      case AF_UNIX:
        return UV_NAMED_PIPE;
      case AF_INET:
      case AF_INET6:
        return UV_TCP;
      }
  }

  if (type == SOCK_DGRAM &&
      (ss.ss_family == AF_INET || ss.ss_family == AF_INET6))
    return UV_UDP;

  return UV_UNKNOWN_HANDLE;
}


static void uv__read(uv_stream_t* stream) {
  uv_buf_t buf;
  ssize_t nread;
  struct msghdr msg;
  struct cmsghdr* cmsg;
  char cmsg_space[64];
  int count;

  /* Prevent loop starvation when the data comes in as fast as (or faster than)
   * we can read it. XXX Need to rearm fd if we switch to edge-triggered I/O.
   */
  count = 32;

  /* XXX: Maybe instead of having UV_STREAM_READING we just test if
   * tcp->read_cb is NULL or not?
   */
  while ((stream->read_cb || stream->read2_cb)
      && (stream->flags & UV_STREAM_READING)
      && (count-- > 0)) {
    assert(stream->alloc_cb);
    buf = stream->alloc_cb((uv_handle_t*)stream, 64 * 1024);

    assert(buf.len > 0);
    assert(buf.base);
    assert(uv__stream_fd(stream) >= 0);

    if (stream->read_cb) {
      do {
        nread = read(uv__stream_fd(stream), buf.base, buf.len);
      }
      while (nread < 0 && errno == EINTR);
    } else {
      assert(stream->read2_cb);
      /* read2_cb uses recvmsg */
      msg.msg_flags = 0;
      msg.msg_iov = (struct iovec*) &buf;
      msg.msg_iovlen = 1;
      msg.msg_name = NULL;
      msg.msg_namelen = 0;
      /* Set up to receive a descriptor even if one isn't in the message */
      msg.msg_controllen = 64;
      msg.msg_control = (void *) cmsg_space;

      do {
        nread = recvmsg(uv__stream_fd(stream), &msg, 0);
      }
      while (nread < 0 && errno == EINTR);
    }

#define INVOKE_READ_CB(stream, status, buf, type)                             \
    do {                                                                      \
      if ((stream)->read_cb != NULL)                                          \
        (stream)->read_cb((stream), (status), (buf));                         \
      else                                                                    \
        (stream)->read2_cb((uv_pipe_t*) (stream), (status), (buf), (type));   \
    }                                                                         \
    while (0)

    if (nread < 0) {
      /* Error */
      if (errno == EAGAIN || errno == EWOULDBLOCK) {
        /* Wait for the next one. */
        if (stream->flags & UV_STREAM_READING) {
          uv__io_start(stream->loop, &stream->io_watcher, UV__POLLIN);
        }
        uv__set_sys_error(stream->loop, EAGAIN);
        INVOKE_READ_CB(stream, 0, buf, UV_UNKNOWN_HANDLE);
      } else {
        /* Error. User should call uv_close(). */
        uv__set_sys_error(stream->loop, errno);
        INVOKE_READ_CB(stream, -1, buf, UV_UNKNOWN_HANDLE);
        assert(!uv__io_active(&stream->io_watcher, UV__POLLIN) &&
               "stream->read_cb(status=-1) did not call uv_close()");
      }
      return;
    } else if (nread == 0) {
      /* EOF */
      uv__io_stop(stream->loop, &stream->io_watcher, UV__POLLIN);
      if (!uv__io_active(&stream->io_watcher, UV__POLLOUT))
        uv__handle_stop(stream);
      uv__set_artificial_error(stream->loop, UV_EOF);
      INVOKE_READ_CB(stream, -1, buf, UV_UNKNOWN_HANDLE);
      return;
    } else {
      /* Successful read */
      ssize_t buflen = buf.len;

      if (stream->read_cb) {
        stream->read_cb(stream, nread, buf);
      } else {
        assert(stream->read2_cb);

        /*
         * XXX: Some implementations can send multiple file descriptors in a
         * single message. We should be using CMSG_NXTHDR() to walk the
         * chain to get at them all. This would require changing the API to
         * hand these back up the caller, is a pain.
         */

        for (cmsg = CMSG_FIRSTHDR(&msg);
             msg.msg_controllen > 0 && cmsg != NULL;
             cmsg = CMSG_NXTHDR(&msg, cmsg)) {

          if (cmsg->cmsg_type == SCM_RIGHTS) {
            if (stream->accepted_fd != -1) {
              fprintf(stderr, "(libuv) ignoring extra FD received\n");
            }

            /* silence aliasing warning */
            {
              void* pv = CMSG_DATA(cmsg);
              int* pi = pv;
              stream->accepted_fd = *pi;
            }

          } else {
            fprintf(stderr, "ignoring non-SCM_RIGHTS ancillary data: %d\n",
                cmsg->cmsg_type);
          }
        }


        if (stream->accepted_fd >= 0) {
          stream->read2_cb((uv_pipe_t*)stream, nread, buf,
              uv__handle_type(stream->accepted_fd));
        } else {
          stream->read2_cb((uv_pipe_t*)stream, nread, buf, UV_UNKNOWN_HANDLE);
        }
      }

      /* Return if we didn't fill the buffer, there is no more data to read. */
      if (nread < buflen) {
        return;
      }
    }
  }
}


int uv_shutdown(uv_shutdown_t* req, uv_stream_t* stream, uv_shutdown_cb cb) {
  assert((stream->type == UV_TCP || stream->type == UV_NAMED_PIPE) &&
         "uv_shutdown (unix) only supports uv_handle_t right now");
  assert(uv__stream_fd(stream) >= 0);

  if (!(stream->flags & UV_STREAM_WRITABLE) ||
      stream->flags & UV_STREAM_SHUT ||
      stream->flags & UV_CLOSED ||
      stream->flags & UV_CLOSING) {
    uv__set_artificial_error(stream->loop, UV_ENOTCONN);
    return -1;
  }

  /* Initialize request */
  uv__req_init(stream->loop, req, UV_SHUTDOWN);
  req->handle = stream;
  req->cb = cb;
  stream->shutdown_req = req;
  stream->flags |= UV_STREAM_SHUTTING;

  uv__io_start(stream->loop, &stream->io_watcher, UV__POLLOUT);

  return 0;
}


static void uv__stream_io(uv_loop_t* loop, uv__io_t* w, unsigned int events) {
  uv_stream_t* stream;

  stream = container_of(w, uv_stream_t, io_watcher);

  assert(stream->type == UV_TCP ||
         stream->type == UV_NAMED_PIPE ||
         stream->type == UV_TTY);
  assert(!(stream->flags & UV_CLOSING));

  if (stream->connect_req) {
    uv__stream_connect(stream);
    return;
  }

  if (events & UV__POLLIN) {
    assert(uv__stream_fd(stream) >= 0);

    uv__read(stream);

    if (uv__stream_fd(stream) == -1)
      return; /* read_cb closed stream. */
  }

  if (events & UV__POLLOUT) {
    assert(uv__stream_fd(stream) >= 0);
    uv__write(stream);
    uv__write_callbacks(stream);
  }
}


/**
 * We get called here from directly following a call to connect(2).
 * In order to determine if we've errored out or succeeded must call
 * getsockopt.
 */
static void uv__stream_connect(uv_stream_t* stream) {
  int error;
  uv_connect_t* req = stream->connect_req;
  socklen_t errorsize = sizeof(int);

  assert(stream->type == UV_TCP || stream->type == UV_NAMED_PIPE);
  assert(req);

  if (stream->delayed_error) {
    /* To smooth over the differences between unixes errors that
     * were reported synchronously on the first connect can be delayed
     * until the next tick--which is now.
     */
    error = stream->delayed_error;
    stream->delayed_error = 0;
  } else {
    /* Normal situation: we need to get the socket error from the kernel. */
    assert(uv__stream_fd(stream) >= 0);
    getsockopt(uv__stream_fd(stream),
               SOL_SOCKET,
               SO_ERROR,
               &error,
               &errorsize);
  }

  if (error == EINPROGRESS)
    return;

  stream->connect_req = NULL;
  uv__req_unregister(stream->loop, req);

  if (req->cb) {
    uv__set_sys_error(stream->loop, error);
    req->cb(req, error ? -1 : 0);
  }
}


int uv_write2(uv_write_t* req,
              uv_stream_t* stream,
              uv_buf_t bufs[],
              int bufcnt,
              uv_stream_t* send_handle,
              uv_write_cb cb) {
  int empty_queue;

  assert(bufcnt > 0);

  assert((stream->type == UV_TCP || stream->type == UV_NAMED_PIPE ||
      stream->type == UV_TTY) &&
      "uv_write (unix) does not yet support other types of streams");

  if (uv__stream_fd(stream) < 0) {
    uv__set_sys_error(stream->loop, EBADF);
    return -1;
  }

  if (send_handle) {
    if (stream->type != UV_NAMED_PIPE || !((uv_pipe_t*)stream)->ipc) {
      uv__set_sys_error(stream->loop, EOPNOTSUPP);
      return -1;
    }
  }

  empty_queue = (stream->write_queue_size == 0);

  /* Initialize the req */
  uv__req_init(stream->loop, req, UV_WRITE);
  req->cb = cb;
  req->handle = stream;
  req->error = 0;
  req->send_handle = send_handle;
  ngx_queue_init(&req->queue);

  if (bufcnt <= (int) ARRAY_SIZE(req->bufsml))
    req->bufs = req->bufsml;
  else
    req->bufs = malloc(sizeof(uv_buf_t) * bufcnt);

  memcpy(req->bufs, bufs, bufcnt * sizeof(uv_buf_t));
  req->bufcnt = bufcnt;
  req->write_index = 0;
  stream->write_queue_size += uv__buf_count(bufs, bufcnt);

  /* Append the request to write_queue. */
  ngx_queue_insert_tail(&stream->write_queue, &req->queue);

  /* If the queue was empty when this function began, we should attempt to
   * do the write immediately. Otherwise start the write_watcher and wait
   * for the fd to become writable.
   */
  if (stream->connect_req) {
    /* Still connecting, do nothing. */
  }
  else if (empty_queue) {
    uv__write(stream);
  }
  else {
    /*
     * blocking streams should never have anything in the queue.
     * if this assert fires then somehow the blocking stream isn't being
     * sufficiently flushed in uv__write.
     */
    assert(!(stream->flags & UV_STREAM_BLOCKING));
    uv__io_start(stream->loop, &stream->io_watcher, UV__POLLOUT);
  }

  return 0;
}


/* The buffers to be written must remain valid until the callback is called.
 * This is not required for the uv_buf_t array.
 */
int uv_write(uv_write_t* req, uv_stream_t* stream, uv_buf_t bufs[], int bufcnt,
    uv_write_cb cb) {
  return uv_write2(req, stream, bufs, bufcnt, NULL, cb);
}


static int uv__read_start_common(uv_stream_t* stream,
                                 uv_alloc_cb alloc_cb,
                                 uv_read_cb read_cb,
                                 uv_read2_cb read2_cb) {
  assert(stream->type == UV_TCP || stream->type == UV_NAMED_PIPE ||
      stream->type == UV_TTY);

  if (stream->flags & UV_CLOSING) {
    uv__set_sys_error(stream->loop, EINVAL);
    return -1;
  }

  /* The UV_STREAM_READING flag is irrelevant of the state of the tcp - it just
   * expresses the desired state of the user.
   */
  stream->flags |= UV_STREAM_READING;

#if defined(__APPLE__)
  /* Notify select() thread about state change */
  if (stream->select != NULL)
    uv__stream_osx_interrupt_select(stream);
#endif /* defined(__APPLE__) */

  /* TODO: try to do the read inline? */
  /* TODO: keep track of tcp state. If we've gotten a EOF then we should
   * not start the IO watcher.
   */
  assert(uv__stream_fd(stream) >= 0);
  assert(alloc_cb);

  stream->read_cb = read_cb;
  stream->read2_cb = read2_cb;
  stream->alloc_cb = alloc_cb;

  uv__io_start(stream->loop, &stream->io_watcher, UV__POLLIN);
  uv__handle_start(stream);

  return 0;
}


int uv_read_start(uv_stream_t* stream, uv_alloc_cb alloc_cb,
    uv_read_cb read_cb) {
  return uv__read_start_common(stream, alloc_cb, read_cb, NULL);
}


int uv_read2_start(uv_stream_t* stream, uv_alloc_cb alloc_cb,
    uv_read2_cb read_cb) {
  return uv__read_start_common(stream, alloc_cb, NULL, read_cb);
}


int uv_read_stop(uv_stream_t* stream) {
  uv__io_stop(stream->loop, &stream->io_watcher, UV__POLLIN);
  uv__handle_stop(stream);
  stream->flags &= ~UV_STREAM_READING;

#if defined(__APPLE__)
  /* Notify select() thread about state change */
  if (stream->select != NULL)
    uv__stream_osx_interrupt_select(stream);
#endif /* defined(__APPLE__) */

  stream->read_cb = NULL;
  stream->read2_cb = NULL;
  stream->alloc_cb = NULL;
  return 0;
}


int uv_is_readable(const uv_stream_t* stream) {
  return stream->flags & UV_STREAM_READABLE;
}


int uv_is_writable(const uv_stream_t* stream) {
  return stream->flags & UV_STREAM_WRITABLE;
}


#if defined(__APPLE__)
int uv___stream_fd(uv_stream_t* handle) {
  uv__stream_select_t* s;

  s = handle->select;
  if (s != NULL)
    return s->fd;

  return handle->io_watcher.fd;
}
#endif /* defined(__APPLE__) */


void uv__stream_close(uv_stream_t* handle) {
#if defined(__APPLE__)
  /* Terminate select loop first */
  if (handle->select != NULL) {
    uv__stream_select_t* s;

    s = handle->select;

    uv_sem_post(&s->sem);
    uv__stream_osx_interrupt_select(handle);
    uv_thread_join(&s->thread);
    uv_sem_destroy(&s->sem);
    uv_mutex_destroy(&s->mutex);
    close(s->fake_fd);
    close(s->int_fd);
    uv_close((uv_handle_t*) &s->async, uv__stream_osx_cb_close);

    handle->select = NULL;
  }
#endif /* defined(__APPLE__) */

  uv_read_stop(handle);
  uv__io_close(handle->loop, &handle->io_watcher);

  close(handle->io_watcher.fd);
  handle->io_watcher.fd = -1;

  if (handle->accepted_fd >= 0) {
    close(handle->accepted_fd);
    handle->accepted_fd = -1;
  }

  assert(!uv__io_active(&handle->io_watcher, UV__POLLIN | UV__POLLOUT));
}
