#include <stdio.h>
#include <uv.h>

static void  test(struct uv_timer_s *time, int aa)
{
	static int num = 0;
	printf("\n\n\nhello[%d]\n\n\n",num++);
}

int main() {
#if 0
    uv_loop_t *loop = uv_loop_new();

    printf("Now quitting.\n");
    uv_run(loop, UV_RUN_DEFAULT);
#endif
	uv_timer_t ti;
	uv_timer_init(uv_default_loop(),&ti);
	uv_timer_start(&ti,test,1000,1000);
	uv_run(uv_default_loop(),UV_RUN_DEFAULT);
    return 0;
}
