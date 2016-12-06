知识点:
1、vim中将光标移动至某处，shift+K打开该关键字的man手册，两次q返回
2、一些常用的c99标准的系统宏
	__FUNCTION__:打印当前所在函数名
	__LINE__：打印当前所在行数
	备注：
	函数占用调用时间，不占用编译时间
	宏不占用调用时间，占用编译时间
3、if(func()):func的返回值0代表假，非0代表真

	
---------------------------------------------------------------------
---------------------------------------------------------------------
一、标准io：io是一切实现的基础，FILE*始终贯穿
	1、缓冲区的作用：大多数缓冲区的存在是件好事，作用是合并系统调用
		行缓冲：换行的时候刷新，满了的时候刷新，强制刷新（stdout，因为涉及到终端设备）
		全缓冲：满了的时候刷新，强制刷新（默认，只要设备不是终端设备）
		无缓冲：需要立即输出，如stderr
		fllush(NULL):强制刷新缓冲区（刷新所有打开文件）
		fllush(stdout)：刷新输出缓冲区
		setvbuf:设置缓冲区
	2、文件指针相关函数
		<1>fopen的返回值放在堆中
			ulimit -a：查看能打开的文件个数
			ulimit -n：设置一个线可以打开文件的个数
			localtime的返回指针为静态区，需要马上使用，不然会被重新赋值1
			注:
				函数返回指针，有互逆操作，返回值指向堆空间
				函数返回指针，没有互逆操作，返回值指向不确定，可能是静态区
		<2>fseek(FILE *stream,long offset,int whence):文件偏移
			ftell(FILE *stream):得到当前文件指针的位置
			rewind(FILE *stream):置文件首，相当于fseek的封装
			fseeko和ftello是方言：
				注意：3种方法防止报错
					#define _FILE_OFFSET_BITS 64
					gcc -D_FILE_OFFSET_BITS 64
					在makefile中加入 CFLAGS+=-D_FILE_OFFSET_BITS=64 -D_GNU_SOURCE
			空洞文件：一开始就偏移到文件最后大小处（实际大小为0）
				用多线程分段锁住文件下载
		<3>getline(char **lineptr,size_t *n,FILE *stream):获取文件一行
			注：谁打开，:谁释放----先打开文件才能对文件进行getline
			@1：指向读到的一行
			@2：读到一行的大小
			@3：打开文件的文件指针
		<4>
			void *malloc(size_t size);
				不会初始化分配的空间（memset）
			void *calloc(size_t nmemb, size_t size);
				在内存的动态存储区分配n块长度为size字节的连续区域，并且初始化该连续空间
				@1：@2的个数
				@2：分配的内存的大小
			void *realloc(void *ptr, size_t size); 
				重新给以分配的内存分配大小，不会改变内容
				realloc新分配的内存地址和旧的可能不同，所以返回值必须重新赋值给原来的空间指针
				@1：已经分配的内存大小
				@2：新的大小
				如果size为0，相当于free		
			void free(void *ptr);
		<5>
			int fputc(int c, FILE *stream);
			int fputs(const char *s, FILE *stream);
			int putchar(int c);
			int puts(const char *s);
			int fgetc(FILE *stream);
			char *fgets(char *s, int size, FILE *stream);
			int getchar(void);
			char *gets(char *s);
			int ungetc(int c, FILE *stream);
		<6>
			size_t fread(void *ptr, size_t size, size_t nmemb, FILE *stream);
			@1:读到的缓存
			@2：每次要读的大小
			@3：要读的总大小
			@4：要读的文件的文件指针
			size_t fwrite(const void *ptr, size_t size, size_t nmemb,FILE *stream);
	3、errno
		没有出错时，其值不会被例程清除
		任何函数都不会讲errno的值置为0（在<errno.h>中定义的常量都不为0）
		fprintf(stderr,"fopen() failed,errno = %d\n",errno)；可以打印errno的值
		perror("fopen()");<==>fprintf(stderr,"fopen():%s\n",stderror(errno));
		
二、系统io或者文件io：fd始终贯穿
	1、函数
		int open(const char *pathname, int flags);
			注：不加NONBLOCK，默认阻塞
				有create用两参，没有create用三参
		int close(int fd);
		int open(const char *pathname, int flags, mode_t mode)；
		int creat(const char *pathname, mode_t mode);
		
		ssize_t read(int fd, void *buf, size_t count);
			每读一次文件指针都快偏移count
		ssize_t write(int fd, const void *buf, size_t count);
			每写一次文件写指针都会偏移count
		off_t lseek(int fd, off_t offset, int whence);
			要从哪里偏移多少
			lseek是fseek和ftell的结合
		
		//文件描述符重定向
		int dup(int oldfd);
		int dup2(int oldfd, int newfd);
			dup2(fd,1);<===>close(1);dup(fd);
			当oldfd等于newfd时不进行任何操作，直接返回newfd
		int dup3(int oldfd, int newfd, int flags);
		
		//同步
		void sync(void);
		int fsync(int fd);
		int fdatasync(int fd);
			只刷数据，不刷亚数据
		
		int truncate(const char *path, off_t length);
			把打开的某个文件截断到固定长度
		
		//管家级函数
		int ioctl(int d, int request, ...);
		int fcntl(int fd, int cmd, ... /* arg */ );
		/dev/fd/目录中包含了当前进程打开的文件描述符
		
	2、	概念
		<1>文件描述符
			文件描述符优先使用当前可用的最小的
			默认一个线程可以打开1024个文件（1024形成一个int[1024]数组，每一个数组元素存放
				打开一个文件生成的一个结构体的首地址，该结构体中pos变量为文件inode，该数组
				存放在进程空间，两个数组成员可能指向同一个结构体）
				
		<2>临时文件
			char *tmpnam(char *s);
				返回值指向静态存储区，对tmpnam的后续使用会覆盖
				tmpnam的操作分为两步，产生名字，创建文件，不原子
			FILE *tmpfile(void);
				返回一个文件流指针，它指向一个唯一的临时文件，该文件以读写的方式打开，
					当这个文件的所有引用都被删除时，这个临时文件将被删除，出错返回NULL，
					并置errno，操作原子化			
	
	注（*****************************************）
		1、标准io和文件io不能混合使用
		2、
			int fileno(FILE *stream);	标准io------>文件io
			FILE *fdopen(int fd, const char *mode);	文件io----->标准io
		3、strace xx/file：查看文件执行的系统调用
		4、新创建的文件权限：0666&~umask（0666与umask按位或，umask可以查看文件被掩盖的权限）
		5、scanf：返回成功赋值的个数
		6、printf：返回成功输出的字节个数
		
三：文件系统
	1、函数
		//目录操作
		DIR *opendir(const char *name);
		int readdir(unsigned int fd, struct old_linux_dirent *dirp,unsigned int count);
		int closedir(DIR *dirp);
		
		int mkdir(const char *pathname, mode_t mode);
		int rmdir(const char *pathname);
		
		//查看文件状态
		int stat(const char *path, struct stat *buf);
			struct stat结构体中的st_mode是一个16位的位图，包含：
				文件类型：
					S_ISRE(m):是否是正规文件
					S_ISDIR(m):是否是目录
					...
					umsk 0022：直接更改umask的值，防止产生权限过松的文件
		int fstat(int fd, struct stat *buf);
		int lstat(const char *path, struct stat *buf);
			对于符号链接文件，展示的信息是该链接文件的信息
			
		//更改文件权限
		int chmod(const char *path, mode_t mode);
		int fchmod(int fd, mode_t mode);
		
		//改变工作路径
		int chdir(const char *path);
		int fchdir(int fd);
		
		//获取当前工作路径
		char *get_current_dir_name(void);
		
		//制作假路径
		int chroot(const char *path);
		
		//更改文件最后读和修改的时间
		int utime(const char *filename, const struct utimbuf *times);
		
		//目录解析
		int glob(const char *pattern, int flags,
                int (*errfunc) (const char *epath, int eerrno),
                glob_t *pglob);
			@1：模式，支持通配符
			@2：一般写0
			@3：出错信息，不希望得到直接写NULL（使用的是回调函数）
			@4：结构体指针
		void globfree(glob_t *pglob);
			最后需要释放
			
		//du：文件或者目录所占的大小，默认单位为K
		char *strrchr(const char *s, int c);
			找s指向的字符串中最右边的c
			int64_t <===> long long
		**************：跨越递归点的变量，不能放在静态区*************
		
		
		//时间函数
		time_t time(time_t *t);
			获取时间戳
		struct tm *localtime(const time_t *timep);
			@1：时间戳
		struct tm *localtime_r(const time_t *timep, struct tm *result);
		time_t mktime(struct tm *tm);
			会调整结构体
		size_t strftime(char *s, size_t max, const char *format,
                       const struct tm *tm);
					   
		//加密相关
		//----->/etc/passwd
		int getpwuid_r(uid_t uid, struct passwd *pwd,char *buf, size_t buflen, struct passwd **result);
			校验用户名，没有此用户则返回空指针
		struct passwd *getpwnam(const char *name);
			查询用户信息
		//----->/etc/shadow
		struct spwd *getspnam(const char *name);
			得到shadow文件的一行
			@1：用户名
		char *crypt(const char *key, const char *salt);
			@1：口令
			@2：杂质串
		char *getpass( const char *prompt);
			关闭终端回显得到密码
			@1：提示信息
		
		makefile注意：	
			需要define的：CFLAGS+=-D_XOPEN_SOURCE，后面可以直接跟多个宏
			需要连接动态库的：LDFLAGS+=-lcrypt
			
		//挂钩子函数
		int atexit(void (*function)(void));
			执行顺序和调用顺序相反
			@1：钩子函数指针
		void exit(int status);
			函数返回给父进程status&0377
			exit以后以at_exit或on_exit注册的钩子函数以逆序方式执行
			
		//命令行解析http://vopit.blog.51cto.com/2400931/440453
		int getopt(int argc, char * const argv[],const char *optstring);
			@1：参数个数
			@2：参数组成的字符串的指针，二级指针
			@3："-y:mdH:MS"
				-	识别非选项参数
				：	处理参数的参数
			optarg：参数的参数（可以直接用的变量）
			返回值：返回在argv中下一个选项的字母
				getopt()每次调用会逐次返回命令行传入的参数。
			　  当没有参数的最后的一次调用时，getopt()将返回-1。
			　  当解析到一个不在optstring里面的参数，或者一个必选值参数不带值时，返回'?'。
			　  当optstring是以':'开头时，缺值参数的情况下会返回':'，而不是'?' 
			
			getopt(argc,argv,"-y:mdH:MS");
			执行：./a.out  -y4 -m -d -H24 -M -S
			
			***************注****************
			其中短参数在getopt定义里分为三种：
			　　1. 不带值的参数，它的定义即是参数本身。
			　　2. 必须带值的参数，它的定义是在参数本身后面再加一个冒号。
			　　3. 可选值的参数，它的定义是在参数本身后面加两个冒号 。
				4. 不带值的参数可以连写，象1和a是不带值的参数，它们可以-1 -a分开写，也可以-1a或-a1连写。
	　　		5. 参数不分先后顺序，'-1a -c cvalue -ddvalue'和'-d -c cvalue -a1'的解析结果是一样的。
	　　		6. 要注意可选值的参数的值与参数之间不能有空格，必须写成-ddvalue这样的格式，如果写成-d dvalue这样的格式就会解析错误。

		//环境变量
		在32位的操作系统中，每个进程的虚拟地址空间是4G
		环境变量位于3G~4G的开头处
		char *getenv(const char *name);
			extern char **environ:外部环境变量
			char *getenv("PWD");--->获取环境变量PWD的值
		int setenv(const char *name, const char *value, int overwrite);
			设置环境变量
			@1：要增加的环境变量的名字
			@2：要增加的环境变量的值
			@3：
				0：如果环境变量name已经存在，不改变环境变量的值
				非0：如果环境变量name已经存在，将会改变该环境变量的值
		int unsetenv(const char *name);
			取消环境变量
		int putenv(char *string);
			@1：name=value
			增加或者改变环境变量
			
		//跳转
		goto 不能跨函数跳转
		int setjmp(jmp_buf env);
			有两个返回值返回
				返回0：设置跳转点
				返回非0：从别的地方跳回来
		void longjmp(jmp_buf env, int val);
		
	2、
		1、/tmp目录具有粘住位（t）	
		2、权限
			要删除一个路径下面的文件，只需要具有上一级目录的写权限
			目录权限
				r：能列出目录的内容
				w：能在目录中添加或者删除文件
				x：能进入目录
			当前目录中的文件具有粘住位权限时，只能建立文件的用户删除自己的文件，起到保护的作用
		3、函数报错不设置errno，不能使用perror函数
		4、int uname(struct utsname *buf);
			获取内核信息
			
三、进程控制
	1、函数
		//fork：写时拷贝
		pid_t fork(void);
			pid_t：进程标识，会一直累加
		pid_t getpid(void);
			获取进程号
		pid_t getppid(void);
			获取父进程号
			
		//exec
		int execl(const char *path, const char *arg, ...);
			execl("/bin/date","date","+%s",NULL);
			@1：命令的全路径
			@2：命令
			@3：命令的参数
			@4：NULL
			*************注**************
			直接取代当前所在进程，除非出错返回-1
			
		//wait
		pid_t wait(int *status);
			阻塞收尸
		pid_t waitpid(pid_t pid, int *status, int options);
			@1：指定要wait的子进程
			@2：回填状态
			非阻塞收尸
		
		//system
		int system(const char *command);
			相当于fork、execl、wait的组合
			
		//临时加root权限
		int atoi(const char *nptr);
			字符串----->整数
		int setuid(uid_t uid);
			设置当前用户的uid
		int execvp(const char *file, char *const argv[]);
			@1：命令（可以不是全路径）
			@2：命令加参数的字符串的首地址
		
		//编写自己的shell窗口
		char *strsep(char **stringp, const char *delim);
			字符串处理
			@1：要处理的字符串
			@2：分隔符的字符串首地址
			注：
				* This function modifies its first argument.
				* This function cannot be used on constant strings.
				* The identity of the delimiting character is lost.
			返回值
				以分隔符截取到的字符串
		glob(tok,GLOB_NOCHECK|GLOB_APPEND*i,NULL,&res->globres);
			@1：从stdin获取的一行中以某一个分隔符截取到的一个
			@2：从第二个开始以追加的方式加入gl_pathv（二级指针），gl_pathc每次加1
		execvp(cmd.globres.gl_pathv[0],cmd.globres.gl_pathv);
			@1：命令
			@2：命令+参数合成的字符串的首地址
			
		//守护进程
		1、
			void openlog(const char *ident, int option, int facility);
				openlog("mydaemon",LOG_PID,LOG_DAEMON);
				@1：自定义名称
				@2：一般写进程号（LOG_PID）
				@3：来源（LOG_DAEMON代表守护进程）
			int syslog(int type, char *bufp, int len);
				syslog(LOG_WARNING,"open():%s",strerror(errno));
				syslog(LOG_ERR,"daemonize() failed.");
				syslog(LOG_INFO,"daemonize() successed.");
				@1：参数类型
				@2：要写的内容,不能加\n
				LOG_DEBUG不会被写入日志
				大于LOG_INFO的才会写入日志
			closelog();
			系统日子存放路径：/var/log/messages
		2、fork子进程
		3、结束父进程
		4、关闭0、1、2
		5、fd = open("/dev/null",O_RDWR);
			dup2(fd,0);
			dup2(fd,1);
			dup2(fd,2);
		6、pid_t setsid(void)
			调用的进程会成为会话组的leader，并脱离控制终端，不能被会话组的leader调用
		7、umask(0);
		8、chdir("/");
			切换当前工作路径
			
		注**************
			当后台进程连上终端时，输入会杀死后台进程
	2、概念
		1、main(int argc,char **argv,char **environ)
			@3:可以不写
		2、进程终止
			正常终止：
				从main函数返回
				调用exit函数(库函数)
				调用_exit或_Exit(系统调用)
				最后一个线程从其启动例程返回
				最后一个线程调用pthread_exit
			异常终止：
				调用abort（自杀）
				接到一个信号并终止
				最后一个线程对其取消请求作出响应
		3、init
			祖先进程，此时内核是相当于库
		4、在fork进程之前最好先fflush(NULL)
			fork的时候缓冲区也会被赋值
		5、http://blog.csdn.net/xy010902100449/article/details/44851453（fork到底做了什么）
				用户程序看不见真正的物理地址，用户只生成逻辑地址，且认为进程的地址空间为0
			到max。物理地址范围从R+0到R+max，R为基地址,地址映射－将程序地址空间中使用的逻
			辑地址变换成内存中的物理地址的过程。由内存管理单元（MMU）来完成。
				逻辑地址：CPU所生成的地址。CPU产生的逻辑地址被分为 :p （页号） 它包含每个
			页在物理内存中的基址，用来作为页表的索引；d （页偏移），同基址相结合，用来确
			定送入内存设备的物理内存地址。
				物理地址：内存单元所看到的地址。


**************************Posix标准下面的并发**********************************
---------------------------多进程+信号-----------------多线程------------------
1、	进程之间的相互通信比较复杂，需要借助管道、共享内存等进程间通信技术
	线程隶属进程，线程共享相同的内存空间，不存在通信障碍，不同的线程可以存取内存中同一个变量
2、	shell和system内部都调用了fork，Linux中fork是创建进程的唯一途径
	在父进程中，fork返回创建子进程的进程号
	在子进程中，fork返回0
	出错，fork返回一个负值
	出错的原因有两个
		当前的进程数已经达到了系统规定的上限，这时errno内设置为EAGAIN
		系统内存不足，这时errno被设置为ENOMEM
		出错概率很小
3、进程间通信情形
	一个进程收集或生成数据，而另一个进程同时监视进度并分析信息
	即时交换信息的聊天系统，其中有两个对等的应运程序互相收发数据
4、进程间通信（IPC：Inter-Process Communication）方式 http://www.cnblogs.com/andtt/articles/2136279.html
	文件：任意数量的进程都可以互操作，适用于本地，用于共享大数据集
	管道/命名管道（单工）：使用专用的文件描述符在两个进程之间传输数据。
		匿名管道：只能在父子进程之间进行
		命名管道：可以在同一主机上的任意两个对等进程之间进行
		适用于本地
		简单的数据共享或命令控制，比如生产者和消费者
		注：管道需要两个fd，fd[0]:读端，fd[1]：写端
		两个管道实现双向通信
	*信号量数组
	*消息队列
	*共享内存：通过在同一段内存中读写数据共享信息
		适用于本地
		任何类型的协作，尤其适合需要安全性的情况
	信号：通过中断通知应运程序某一情况（进程间通信唯一的异步通信机制）
		适用于本地的
		无法在信号中传输数据，所以信号主要用于进程管理
	套接字：完成特殊的设置过程之后，使用一般的输入/输出操作传输数据
		适用于本地或远程
		FTP、ssh、Apache Web Server等网络服务
----------------------------------多线程------------------------------------
1、每个线程拥有自己的独立的栈空间，拥有独立的执行序列
2、对比
	线程执行开销小，但不利于资源的管理和保护，进程相反
	线程适合于在SMP机器上运行，而进程可以跨主机迁移
	一般而言程序设计优先考虑单进程/多线程模式，其次才是多进程/单线程模式
*******************************************************************************
四、多进程+信号（信号本质：http://www.cnblogs.com/hoys/archive/2012/08/19/2646377.html）
	http://blog.chinaunix.net/uid-26000296-id-3416281.html
	1、函数
		//定义信号函数
		#include <signal.h>
		typedef void (*sighandler_t)(int);
		sighandler_t signal(int signum, sighandler_t handler);
		void (*singal(int signum,void (*handler)(int)))(int);
			signum : 指定信号的值，
			handler: 指定针对前面信号值的处理，
			可以忽略该信号(参数设为SIG_IGN)；
			可以采用系统默认方式处理信号(参数设为SIG_DFL)；
			也可以自己实现处理方式(参数指定一个函数地址)。
			返回值：
				成功：返回最后一次为安装信号signum而调用signal()时的handler值
				失败：则返回SIG_ERR
		signal(SIGINT, SIG_DFL);---->将信号的动作恢复默认
			返回值：旧的行为
			信号埋完以后，信号并未结束，并且信号来了方可处理信号
			信号会打断阻塞的系统调用
			信号的行为是内核布置的，不可靠，34) SIGRTMIN之前的都是不可靠信号
			信号有三种关联动作，没有特别的指定，信号的关联动作是其默认动作
				使用默认动作
				忽略信号
				捕获信号：执行自己定义的信号处理函数
				SIGKILL和SIGSTOP既不能忽略也不能捕获，这为超级用户杀死和停止进程提供了必要保障
			信号的不可靠性体现（早起的unix系统中）：
				信号处理程序不是永久安装的，进程每次处理信号后，就将信号的响应设置为默认动作
					解决办法：在每次信号处理函数结尾再一次调用signal()，重新安装信号
				不能暂时屏蔽相同的信号再次发生（可重入）
			Linux支持不可靠信号，但是对不可靠信号机制做了改进
				在调用完信号处理函数之后不必重新调用该信号的安装函数
				Linux下的不可靠信号问题主要是信号可能丢失（重入）
			早起的信号安装函数：signal()	信号发送函数kill()	不能传递信息给信号处理函数
			现在的信号安装函数：sigaction()	信号发送函数sigqueue()	能传递信息给信号处理函数
			信号安装函数signal和sigaction都不能把SIGRTMIN以前的信号变成可靠信号
			但是对SIGRTMIN以后的信号都支持排队机制
		
		//实现可靠信号的函数接口http://www.cnblogs.com/wblyuyang/archive/2012/11/13/2768923.html
		int sigaction(int signum, const struct sigaction *act,struct sigaction *oldact);
			sig:为信号的值，
				可以为除SIGKILL及SIGSTOP 外的任何一个特定有效的信号
				为这两个信号定义自己的处理函数，将导致信号安装错误。
			act:指向结构体sigaction的一个实例的指针，
				在结构体 sigaction的实例中，指定了对特定信号的处理，可以为空，
				进程会以缺省方式对信号处理；
				参数act 最为重要，
				其中包含了对指定信号的处理、信号所传递的信息、
				信号处理函数执行过程中应屏蔽掉哪些函数等等.
			oldact: 指向的对象用来保存原来对相应信号的处理，
				可指定oldact为NULL。
				如果oldact不是空指针, sigaction将把原先对该信号的动作写到它指向的位置。
				如果oldact 是空指针, 则sigaction函数就不需要再做其它设置了。
			如果把参数act, oldact都设为NULL，那么该函数可用于检查信号的有效性。
			struct sigaction {
               void     (*sa_handler)(int);
               void     (*sa_sigaction)(int, siginfo_t *, void *);
               sigset_t   sa_mask;
               int        sa_flags;
               void     (*sa_restorer)(void);
			};
			前两个函数指针二选一
			sa_flags=SA_SIGINFO时，系统将使用sa_sigaction函数作为信号处理函数，否则
			使用sa_handler作为信号处理函数
			sa_mask 成员用来指定在信号处理函数执行期间需要被屏蔽的信号，特别是当某
			个信号被处理时，它自身会被自动放入进程的信号掩码，因此在信号处理函数执
			行期间这个信号不会再度发生。
		//信号集&信号屏蔽字
		int sigemptyset(sigset_t *set);
			清空要控制的信号集
		int sigfillset(sigset_t *set);
			控制所有的信号
		int sigaddset(sigset_t *set, int signum);
			控制某一个信号，配合sigemptyset使用
		int sigdelset(sigset_t *set, int signum);
			删除对某一个信号的控制
		int sigismember(const sigset_t *set, int signum);
			判断某个成员是否被控制
		int sigprocmask(int how, const sigset_t *set, sigset_t *oldset);
			有时候不希望在接到信号时就立刻停止当前执行，去处理信号，同时也不希望忽略该信号
			而是延时一段时间去调用信号处理函数，这种情况是通过阻塞信号实现的
			sigprocmask(SIG_UNBLOCK,&set,&saveset);
			@1：指定信号修改的方式
				SIG_BLOCK//将set所指向的信号集中包含的信号加到当前的信号掩码中。即信号掩码和set信号集进行或操作。
				SIG_UNBLOCK//将set所指向的信号集中包含的信号从当前的信号掩码中删除。即信号掩码和set进行与操作。
				SIG_SETMASK //将set的值设定为新的进程信号掩码。即set对信号掩码进行了赋值操作。
			@2：为指向信号集的指针，在此专指新设的信号集，如果仅想读取现在的屏蔽值，可将其置为NULL。
			@3：也是指向信号集的指针，在此存放原来的信号集。可用来检测信号掩码中存在什么信号。
			返回值：成功执行时，返回0。失败返回-1，errno被设为EINVAL。
		int sigpending(sigset_t *set);
			扎内核取出pending状态放入set集中
		
		//信号驱动
		int sigsuspend(const sigset_t *mask);
			实现了解除阻塞--->等待---->阻塞的原子操作
			sigprocmask(SIG_SETMASK,&oset,&tmpset);
			pause();
			sigprocmask(SIG_SETMASK,&tmpset,NULL);
		
		//信号发送函数
		int kill(pid_t pid, int sig);
		int raise(int sig);
			向进程本身发送信号
			阻塞的系统调用，会被alarm打断
			sleep()<===>alarm+pause
			int nanosleep(const struct timespec *req, struct timespec *rem);
		int sigqueue(pid_t pid, int sig, const union sigval value);			
		unsigned int alarm(unsigned int seconds);
			专门为SIGALRM信号而设，
			在指定的时间seconds秒后，将向进程本身发送SIGALRM信号，又称为闹钟时间。
			进程调用alarm后，任何以前的alarm()调用都将无效。
			如果参数seconds为零，那么进程内将不再包含任何闹钟时间。
			返回值：
				如果调用alarm()前，进程中已经设置了闹钟时间，则返回上一个闹钟时间的剩余时间，
				否则返回0。
		void abort(void);
			自杀并产生core文件
		int setitimer(int which, const struct itimerval *new_value,struct itimerval *old_value);
			setitimer()为Linux的API，并非C语言的Standard Library，setitimer()有两个功能，一是指定
			一段时间后，才执行某个function，二是每间格一段时间就执行某个function
		
		//其他函数
		int system(const char *command);
			阻塞一个信号（SIGCHLD），忽略两个信号（SIGINT和SIGQUIT）
		int usleep(useconds_t usec);
	2、概念
		1、信号：信号是软件中断，信号的响应依赖于中断 
		2、信号属于异步事件，不知道什么时候来，来了不知道造成什么，处理方法有两种：查询法（多），通知法
		3、kill -l:查看信号（1-31：标准信号，34-64实时信号）
		4、core文件：http://www.cnblogs.com/dongzhiquan/archive/2012/01/20/2328355.html
			ulimit -c 0 ：阻止生成core文件
			ulimit -a ：检查生成core文件的选项是否打开
			用来保留出错现场
			多半信号产生终止和core现场
		5、信号快捷键
			ctrl -c ：SIGIINT----->终止正在运行的所有前台进程
			ctrl -z ：SIGSTP------>挂起所有前台进程
			ctrl -\ ：SIGOUT------>终止所有前台进程并生成core文件
			ctrl -d ：不是发送信号，而是一个特殊的二进制值，表示EOF，终止输入，退出shell
			ctrl -l ：clear screen
		6、可重入函数
			所有的系统调用都是可重入的，一部分库函数也是可重入的（如memcopy\memmove）
		7、信号的响应过程（32位系统为例）
			1、内核为每个进程维护两个位图（resource）：mask（全1）、pending（全0）
			2、mask：信号屏蔽字，表示当前信号状态，初始值为1
			3、pending：当前进程位图，默认全0，要响应哪个信号，把对应的位置1
			4、进程收到信号之后，将mask&pending为1的位表示该信号可以响应，进入内核之后会将mask
				全部位置0（防止重入），从内核回到用户态的时候，再将mask全部置为1（在这个过程中
				有一个不可避免的延时），信号的响应是kernel--->user时响应的
			5、忽略一个信号：将mask相应位置0
			6、不能从信号处理函数中随便往外跳，会错过mask复位的步骤，导致无法响应这个信号
		8、volatile:去数值实际的位置取值
			gcc file.c -'o1' : 优化编译
			free(NULL)不会报错
			sig_atomic_t：信号原子，自增，自减
五、多线程
=================================对比多进程并发====================================
多进程并发是先实现后标准化
多线程并发是先标准化后实现
===================================================================================
	1、函数
		//概念型函数
		int pthread_equal(pthread_t t1, pthread_t t2);
			比较两个线程id
		pthread_t pthread_self(void);
			查看当前线程的线程号
			
		//创建线程
		int pthread_create(pthread_t *thread, const pthread_attr_t *attr,void *(*start_routine) (void *), void *arg);
			@1：回填的线程标识
			@2：回填的属性（一般写NULL）
			@3：创建的兄弟线程
			@4：给兄弟线程传的参数
		return NULL ：不会调用线程钩子函数
		
		//线程属性
		int pthread_attr_init(pthread_attr_t *attr);
		int pthread_attr_destroy(pthread_attr_t *attr);
		int pthread_attr_setstacksize(pthread_attr_t *attr, size_t stacksize);
		int pthread_attr_getstacksize(pthread_attr_t *attr, size_t *stacksize);
		
		//线程同步属性
		int pthread_mutexattr_init(pthread_mutexattr_t *attr);
		int pthread_mutexattr_destroy(pthread_mutexattr_t *attr);
		int pthread_mutexattr_getpshared(const pthread_mutexattr_t *restrict attr, int *restrict pshared);
		int pthread_mutexattr_setpshared(pthread_mutexattr_t *attr,int pshared);
		
		//线程取消和收尸，线程一般先取消，再收尸
		int pthread_cancel(pthread_t thread);
		int pthread_setcancelstate(int state, int *oldstate);
			@1：对当前所在线程设置取消状态
				PTHREAD_CANCEL_ENABLE：允许
				PTHREAD_CANCEL_DISABLE：拒绝
				为NULL时，相当于查询当前线程的状态
			@2：保存旧的状态
		int pthread_setcanceltype(int type, int *oldtype);
			@1：对当前所在线程设置取消方式
				PTHREAD_CANCEL_DEFERRED ：线程接收到取消操作后，直到运行到“可取消点”后取消。 
				PTHREAD_CANCEL_ASYNCHRONOUS ：线程接收到取消操作后，立即取消。
		void pthread_testcancel(void);
			在当前线程创建一个取消点，如果当前线程不能取消，则这个函数无效
		int pthread_join(pthread_t thread, void **retval);
			取消有两种状态：允许和不允许
			允许取消：
				异步cancel
				推迟cancel(默认，推迟到cancel点再响应，在POSIX定义的cancel点都是阻塞的系统调用)
		
		int pthread_detach(pthread_t thread);
			线程分离，已经分离的线程不能再对其收尸，会自动释放
		
		//栈的清理
		void pthread_cleanup_push(void (*routine)(void *),void *arg);
			@1：调用的栈清理函数指针
			@2：给函数传的参数
		void pthread_cleanup_pop(int execute);
			@1：为真执行（1），为假不执行（0），放在pthread_exit()之后默认为真，都执行
			两函数是一对宏
			
		//线程同步
		//互斥量：限制代码，实现互斥（以独立的方式占有某一段代码）
		int pthread_mutex_destroy(pthread_mutex_t *mutex);
		int pthread_mutex_init(pthread_mutex_t *restrict mutex,const pthread_mutexattr_t *restrict attr);
		pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
			互斥量的初始化有两种办法
		int pthread_once(pthread_once_t *once_control,void (*init_routine)(void))
			动态模块只加载一次
		//条件变量
		int pthread_cond_destroy(pthread_cond_t *cond);
		int pthread_cond_init(pthread_cond_t *restrict cond,const pthread_condattr_t *restrict attr);
		pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
		int pthread_cond_broadcast(pthread_cond_t *cond);
			打断所有等待
		int pthread_cond_signal(pthread_cond_t *cond);
			随机打断一个等待
		int pthread_cond_wait(pthread_cond_t *restrict cond,pthread_mutex_t *restrict mutex);
			解锁--->等待--->加锁
			int pthread_mutex_unlock(pthread_mutex_t *mutex);
			int sched_yield(void); 出让调度器，不会产生颠簸，很短的sleep
			int pthread_mutex_lock(pthread_mutex_t *mutex);
		int pthread_cond_timedwait(pthread_cond_t *restrict cond,pthread_mutex_t *restrict mutex,const struct timespec *restrict abstime);
	
	2、概念
		1、线程概念：一个正在运行中的函数，posix是一套标准，而不是实现
		2、线程标识：pthread_t（类型不确定）
			ps axm : --代表线程
			ps ax -L:以Linux模式查看进程与线程关系
			LWP:轻量级进程即线程
		3、编译带有多线程的源码时，makefile需要加入：
			CFLAGS+=-pthread
			LDFLAGS+=-pthread
		4、exit会使进程终止，线程也不存在了
			线程可以互相收尸
			线程的调度取决于调度器的调度策略
		5、线程的终止
			线程从其启动例程返回，返回线程退出码
			线程被同一进程中的其它线程取消
			线程调用pthread_exit()函数
		6、一个进程创建的多个线程，栈空间是独立的，代码段是共用的，上限受到栈空间限制
			32位机器的C程序的虚拟地址为0~4G
			user态：0x08048000~3G
			64位机器的C程序虚拟地址为128T
		7、信号量数组
		8、读写锁
			读锁：共享锁
			写锁：互斥量
			
六、高级IO（非阻塞IO）
******************数据中继relay*******************************************************
1、读文件时会报的假错
	EAGAIN：非阻塞的报错，一段时间读不到内容就返回，提示可以再读
	EINTR：阻塞的报错，可能是被信号打断了
**************************************************************************************
	1、文件锁
		int fileno(FILE *stream);	fp---->fd
		int lockf(int fd, int cmd, off_t len);
			@2：设置文件状态
				F_LOCK：加锁
				F_ULOCK：解锁
			@3：从当前位置加锁的长度，为0时，会将整个文件锁住
	2、IO多路循环
		2.1 select（设置监视任务---->监视----->查看监视结果）
			int fcntl(int fd, int cmd, ... /* arg */ );
				fd1_save = fcntl(fd1,F_GETFL);
					保存当前状态
				fcntl(fd1,F_SETFL,fd1_save|O_NONBLOCK);
					设置当前状态，加入非阻塞
			//设置监视任务
				读和写集清0
					FD_ZERO(&rset);
					FD_ZERO(&wset);
				给事件中加入要监听的文件描述符
					FD_SET(fsm12.sfd,&rset);
					FD_SET(fsm21.dfd,&wset);
			//监听
				int select(int nfds, fd_set *readfds, fd_set *writefds,
							fd_set *exceptfds, struct timeval *timeout);
					select(max(fd1,fd2)+1,&rset,&wset,NULL,NULL)
					@1：固定取所有监听的文件描述符中最大的fd+1
					@2：读事件集
					@3：写事件集
					@4：错误异常
					@5：设置超时，不设置就死等
					当监视到某一个文件描述符有读或者写的操作时，函数退出，或者当设置
						了超时，到了超时事件，函数退出
			//查看监视结果
				FD_ISSET(fd2,&rset)：文件描述符在读事件集中
				FD_ISSET(fd1,&wset)：文件描述符在写事件集中
		2.2 poll（以文件描述符为单位组织事件）
			int poll(struct pollfd *fds, nfds_t nfds, int timeout);
				struct pollfd {
					int   fd;         /* file descriptor */
					short events;     /* requested events */
					short revents;    /* returned events */
				};
				@1：结构体数组指针
				@2：数组长度（最大为1024）
				@3：设置超时
					0：非阻塞
					-1：阻塞死等
					>0：设置的超时时间
			//设置监视任务
			pfd[0].events = 0;
				对文件描述符监听的事件清零
			pfd[0].events |= POLLIN;
				给文件描述符添加监视事件（读事件）
			pfd[0].events |= POLLOUT;
				给文件描述符添加写事件
			//监视
			poll(pfd,2,-1)；
			//查看监视结果
			pfd[0].revents & POLLIN || pfd[1].revents & POLLOUT
				0能读或者1能写
				
		2.3 epoll（基于内核态，不可移植）
			int epoll_create(int size);
				@1：任意大于0的数
			int epoll_ctl(int epfd, int op, int fd, struct epoll_event *event);
				@1：上面函数的返回值
			int epoll_wait(int epfd, struct epoll_event *events,int maxevents, int timeout);
				@3：等待事件个数
			//初始化操作
			struct epoll_event ev;
			epfd = epoll_create(10);
				创建epoll任务
			ev.events = 0;
			ev.data.fd = fd1;
			epoll_ctl(epfd,EPOLL_CTL_ADD,fd1,&ev);
				将事件中添加fd1
			ev.events = 0;
			ev.data.fd = fd1;
			epoll_ctl(epfd,EPOLL_CTL_ADD,fd2,&ev);
				将事件中添加fd2
			//设置监视任务
			ev.events = 0;
			ev.data.fd = fd1;
			ev.events |= EPOLLIN;
				给文件描述符添加读事件
			//监视
			epoll_wait(epfd,&ev,1,-1)
			//查看监视结果
			位操作，判断逻辑值
	3、异步IO
		ssize_t readv(int fd, const struct iovec *iov, int iovcnt);
		ssize_t writev(int fd, const struct iovec *iov, int iovcnt);
			
	4、内存映射		http://blog.chinaunix.net/uid-24517893-id-164217.html
		void *mmap(void *addr, size_t length, int prot, int flags,int fd, off_t offset);
			@1：指向欲映射的内存起始地址，通常设为 NULL，代表让系统自动选定地址，映射成功后返回该地址。
			@2：代表将文件中多大的部分映射到内存
			@3： 映射区域的保护方式。可以为以下几种方式的组合：
                    PROT_EXEC 映射区域可被执行
                    PROT_READ 映射区域可被读取
                    PROT_WRITE 映射区域可被写入
                    PROT_NONE 映射区域不能存取
			@4：影响映射区域的各种特性。在调用mmap()时必须要指定MAP_SHARED 或MAP_PRIVATE。
			@5：要映射到内存中的文件描述符
			@6：文件映射的偏移量，通常设置为0，代表从文件最前方开始对应，offset必须是PAGE_SIZE的整数倍。
			返回值：
				若映射成功则返回映射区的内存起始地址，否则返回MAP_FAILED(－1)
				错误原因存于errno 中。
		int munmap(void *addr, size_t length);
	5、管道
		int pipe(int pipefd[2]);
			0读1写
	*****************三大IPC机制**********************************
	xxxget():创建
	xxxop():操作
	xxxctl():销毁
	**************************************************************
	6、消息队列
		struct msgbuf {
               long mtype;       /* message type, must be > 0 */
               char mtext[1];    /* message data */
           };
		 注：计算大小时，应该去掉long占的字节，char mtext[1]-->变长数组
		key_t ftok(const char *pathname, int proj_id);
			@1：协议中随便规定的路径
			@2：协议中随便规定的数字
		int msgget(key_t key, int msgflg);
			msgget(key,IPC_CREAT|0600);
			@1：ftok的返回值
			@2：IPC_CREAT创建并设置权限
		int msgsnd(int msqid, const void *msgp, size_t msgsz, int msgflg);	
		ssize_t msgrcv(int msqid, void *msgp, size_t msgsz, long msgtyp,int msgflg);
			@1：msget的返回值
			@2：回填数据
			@3：回填数据的大小
		msgctl(msgid,IPC_RMID,NULL);
			销毁
	7、信号量数组
	8、共享内存
	
七、网络套接字 socket
	********************************************************************
	1、适用于跨主机数据传输
	2、问题
		1、字节序
			主机字节序：host
			网络字节序：network
			解决：字节序转换
			uint16_t htons(uint16_t hostshort);
			uint16_t ntohs(uint16_t netshort);
		2、对齐
			解决：
			__attribute__((packed))__：设置成不对齐
		3、类型长度
			int 32_t	uint 32_t	int 64_t
	3、int socket(int domain, int type, int protocol);
		@1：协议族
		@2：传输类型
		@3：协议族中的一个协议
	额外：
		FILE *fdopen(int fd, const char *mode);
			将fd--->转换为fp
	********************************************************************
	1、tcp（流式）
		server端：被动端，先运行起来
		1、取得socket
		2、给socket绑定地址
		3、将socket置为监听模式
		4、接受连接
		5、收/发消息
		6、关闭
		
		client端：主动端，先运行起来
		1、取得socket
		2、给socket绑定地址
		3、发起连接
		4、收/发消息
		5、关闭
		
		//struct sockaddr与struct sockaddr_in
		man 7 ip
		struct sockaddr_in {
               sa_family_t    sin_family; /* address family: AF_INET */
               in_port_t      sin_port;   /* port in network byte order */
               struct in_addr sin_addr;   /* internet address */
           };
           /* Internet address. */
           struct in_addr {
               uint32_t       s_addr;     /* address in network byte order */
           };
		
		================s端=====================
		1、int socket(int domain, int type, int protocol);
			获取socket标识符
			
		2、设置与获取socket属性
		setsockopt(sd,SOL_SOCKET,SO_REUSEADDR,&val,sizeof(val)
		int setsockopt(int sockfd, int level, int optname,const void *optval, socklen_t optlen);
			@1：socket返回值
			@2：级别，指定选项代码的类型
				SOL_SOCKET: 基本套接口
				IPPROTO_IP: IPv4套接口
				IPPROTO_IPV6: IPv6套接口
				IPPROTO_TCP: TCP套接口
			@3：选项名称
				SO_REUSEADDR
					允许重用本地地址和端口，允许绑定已经被使用的地址（或端口号）
			@4：可以是一个整数的指针
			@5：@4的长度
		int getsockopt(int sockfd, int level, int optname,void *optval, socklen_t *optlen);
		
		3、绑定端口
		bind(sd,(void *)&laddr,sizeof(laddr)；
		//将点分式ip地址转换为整型
		int inet_pton(int af, const char *src, void *dst);
			@1：协议族
			@2：要转换的ip地址（0.0.0.0）
			@3：转换后存放整型地址的指针
		const char *inet_ntop(int af, const void *src,char *dst, socklen_t size);
			将整型的ip地址转换为点分式的
		//绑定端口
		int bind(int sockfd, const struct sockaddr *addr,socklen_t addrlen);
			@1：socket返回值
			@2：地址的结构体指针（内部定义了ip地址、端口号、协议族等）
				差man 7 ip，随协议在变，调用时需要强转
			@3：@2的长度
		
		4、设置为监听模式不会阻塞，会在accept阻塞
		listen(sd,200)；
		int listen(int sockfd, int backlog);
			@1：第一步得到的socket
			@2：监听队列的长度（tcp连接中存在半连接池的情况，内核需要处理）
			
		5、接收数据
		accept(sd,(void *)&raddr,&raddr_len);
		int accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen);
			@1：socket返回值
			@2：地址结构体的地址，需要强转为struct sockaddr型
		
		6、发送数据
		ssize_t send(int sockfd, const void *buf, size_t len, int flags);
		
		================c端=====================
		1、获取socket
		2、绑定地址（可省）
		3、发起连接
			int connect(int sockfd, const struct sockaddr *addr,socklen_t addrlen);
		4、发数据
		5、收数据
		6、关闭socket
	
	2、udp（报式传输）
		server端：被动端先收包的一方，先运行起来
		1、取得socket
		2、给socket取得地址
		3、收/发消息
		4、关闭
		client端：主动端，先发包的一端
		1、取得socket
		2、给socket取得地址（可省略）
		3、发/收消息
		4、关闭
		
		===============s端========================
		1、获取socket
			sd = socket(AF_INET,SOCK_DGRAM,0/*IPPROTO_UDP*/);
			0：代表该协议族中的第一个协议，报式的第一个是UDP
		2、绑定端口
			bind(sd,(void *)&laddr,sizeof(laddr))；
		3、接收数据
			recvfrom(sd,&rbuf,sizeof(rbuf),0,(void *)&raddr,&raddr_len);
		4、关闭socket
		===============c端========================
		1、获得socket
			socket(AF_INET,SOCK_DGRAM,0/*IPPROTO_UDP*/);
		2、发送数据
			sendto(sd,&sbuf,sizeof(sbuf),0,(void *)&raddr,sizeof(raddr))；
		3、关闭socket
		
	********************************************************************
	udp协议：单播、广播、多播（组播）http://www.bkjia.com/cjjc/995939.html
		广播：广播地址（全网广播：255.255.255.255	子网广播），本地广播消息不会被路由器转发
			广播不能再Internet上面进行传输
		多播：可以在Internet上面进行传输
			多播主要用于网上视频、网上会议
			多播使用D类地址（224.0.0.0~239.255.255.255），划分为
				局部连接多播地址：224.0.0.0~224.0.0.255，为路由协议和其他用途保留的地址，路由
					器并不转发此类范围内的IP包
				预留多播地址:224.0.1.0~238.255.255.255，可用于全球范围（如internet）或网络协议
				管理权限多播地址：239.0.0.0~239.255.255.255，可供组织内部使用，类似于私有IP
					地址，不能用于internet，可限制多播范围
			多播的实现依靠函数：setsockopt()和getsockopt()，选项是IP层（man 7 ip）
			getsockopt()/setsockopt()的选项		含 义
			IP_MULTICAST_TTL					设置多播组数据的TTL值
			IP_ADD_MEMBERSHIP					在指定接口上加入组播组
			IP_DROP_MEMBERSHIP					退出组播组
			IP_MULTICAST_IF						获取默认接口或设置接口
			IP_MULTICAST_LOOP					禁止组播数据回送
	********************************************************************
	3、广播（broadcast）
		----------------->server端
		inet_pton(AF_INET,"0.0.0.0",&laddr.sin_addr.s_addr);
		1、获取socket
			sd = socket(AF_INET,SOCK_DGRAM,0/*IPPROTO_UDP*/);
		2、设置广播
			setsockopt(sd,SOL_SOCKET,SO_BROADCAST,&val,sizeof(val);
		3、绑定广播ip地址到特定端口（如1989）
			bind(sd,(void *)&laddr,sizeof(laddr))；
		4、接收数据
			recvfrom(sd,&rbuf,sizeof(rbuf),0,(void *)&raddr,&raddr_len)；
		---------------->client端
		inet_pton(AF_INET,"255.255.255.255",&raddr.sin_addr);
		1、获取socket
		2、设置广播
			setsockopt(sd,SOL_SOCKET,SO_BROADCAST,&val,sizeof(val);
		3、发送数据至广播地址
			sendto(sd,&sbuf,sizeof(sbuf),0,(void *)&raddr,sizeof(raddr);
		4、关闭socket
			
	4、多播（组播）
		----------------->server端
		IP_ADD_MEMBERSHIP (since Linux 1.2)
        Join a multicast group.  Argument is an ip_mreqn structure.
			struct ip_mreqn {
				struct in_addr imr_multiaddr; /* IP multicast group
												   address */
				struct in_addr imr_address;   /* IP address of local
												   interface */
				int            imr_ifindex;   /* interface index */
			};
		1、获取socket
		2、设置多播
			inet_pton(AF_INET,MGROUP,&mreq.imr_multiaddr);
				设置多播地址
			inet_pton(AF_INET,"0.0.0.0",&mreq.imr_address);
				设置本地地址
			mreq.imr_ifindex = if_nametoindex("eth0");
				设置本地地址网络接口索引
			setsockopt(sd,IPPROTO_IP,IP_ADD_MEMBERSHIP,&mreq,sizeof(mreq);
				@4：
		3、绑定端口和本地ip地址
			inet_pton(AF_INET,"0.0.0.0",&laddr.sin_addr.s_addr);
			bind(sd,(void *)&laddr,sizeof(laddr);
		4、接收数据
			recvfrom(sd,&rbuf,sizeof(rbuf),0,(void *)&raddr,&raddr_len);
		----------------->client端
		1、获取socket
		2、加入多播组
			#define MGROUP      "224.2.2.2"
			inet_pton(AF_INET,MGROUP,&mreq.imr_multiaddr);
			inet_pton(AF_INET,"0.0.0.0",&mreq.imr_address);
			mreq.imr_ifindex = if_nametoindex("eth0");
			recvfrom(sd,&rbuf,sizeof(rbuf),0,(void *)&raddr,&raddr_len);
		3、发送数据
			raddr.sin_port = htons(atoi(RCVPORT));
			inet_pton(AF_INET,MGROUP,&raddr.sin_addr);
			sendto(sd,&sbuf,sizeof(sbuf),0,(void *)&raddr,sizeof(raddr);
		4、关闭socket	
			
		****************************************************************
		unsigned if_nametoindex(const char *ifname);
			网卡索引名字---->索引
		ip ad sh：查看网卡的索引号
		ip ro sh：查看路由情况
		ip ro add default via：添加一个默认路由
		eog：图片浏览工具
		accept：天生互斥，同一时刻只能响应一个sd
		getaddrinfo：域名解析
		tcgetattr：设置键盘
		****************************************************************		
			
				
			
	
	
		