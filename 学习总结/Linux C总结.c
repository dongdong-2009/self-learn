֪ʶ��:
1��vim�н�����ƶ���ĳ����shift+K�򿪸ùؼ��ֵ�man�ֲᣬ����q����
2��һЩ���õ�c99��׼��ϵͳ��
	__FUNCTION__:��ӡ��ǰ���ں�����
	__LINE__����ӡ��ǰ��������
	��ע��
	����ռ�õ���ʱ�䣬��ռ�ñ���ʱ��
	�겻ռ�õ���ʱ�䣬ռ�ñ���ʱ��
3��if(func()):func�ķ���ֵ0����٣���0������

	
---------------------------------------------------------------------
---------------------------------------------------------------------
һ����׼io��io��һ��ʵ�ֵĻ�����FILE*ʼ�չᴩ
	1�������������ã�������������Ĵ����Ǽ����£������Ǻϲ�ϵͳ����
		�л��壺���е�ʱ��ˢ�£����˵�ʱ��ˢ�£�ǿ��ˢ�£�stdout����Ϊ�漰���ն��豸��
		ȫ���壺���˵�ʱ��ˢ�£�ǿ��ˢ�£�Ĭ�ϣ�ֻҪ�豸�����ն��豸��
		�޻��壺��Ҫ�����������stderr
		fllush(NULL):ǿ��ˢ�»�������ˢ�����д��ļ���
		fllush(stdout)��ˢ�����������
		setvbuf:���û�����
	2���ļ�ָ����غ���
		<1>fopen�ķ���ֵ���ڶ���
			ulimit -a���鿴�ܴ򿪵��ļ�����
			ulimit -n������һ���߿��Դ��ļ��ĸ���
			localtime�ķ���ָ��Ϊ��̬������Ҫ����ʹ�ã���Ȼ�ᱻ���¸�ֵ1
			ע:
				��������ָ�룬�л������������ֵָ��ѿռ�
				��������ָ�룬û�л������������ֵָ��ȷ���������Ǿ�̬��
		<2>fseek(FILE *stream,long offset,int whence):�ļ�ƫ��
			ftell(FILE *stream):�õ���ǰ�ļ�ָ���λ��
			rewind(FILE *stream):���ļ��ף��൱��fseek�ķ�װ
			fseeko��ftello�Ƿ��ԣ�
				ע�⣺3�ַ�����ֹ����
					#define _FILE_OFFSET_BITS 64
					gcc -D_FILE_OFFSET_BITS 64
					��makefile�м��� CFLAGS+=-D_FILE_OFFSET_BITS=64 -D_GNU_SOURCE
			�ն��ļ���һ��ʼ��ƫ�Ƶ��ļ�����С����ʵ�ʴ�СΪ0��
				�ö��̷ֶ߳���ס�ļ�����
		<3>getline(char **lineptr,size_t *n,FILE *stream):��ȡ�ļ�һ��
			ע��˭�򿪣�:˭�ͷ�----�ȴ��ļ����ܶ��ļ�����getline
			@1��ָ�������һ��
			@2������һ�еĴ�С
			@3�����ļ����ļ�ָ��
		<4>
			void *malloc(size_t size);
				�����ʼ������Ŀռ䣨memset��
			void *calloc(size_t nmemb, size_t size);
				���ڴ�Ķ�̬�洢������n�鳤��Ϊsize�ֽڵ��������򣬲��ҳ�ʼ���������ռ�
				@1��@2�ĸ���
				@2��������ڴ�Ĵ�С
			void *realloc(void *ptr, size_t size); 
				���¸��Է�����ڴ�����С������ı�����
				realloc�·�����ڴ��ַ�;ɵĿ��ܲ�ͬ�����Է���ֵ�������¸�ֵ��ԭ���Ŀռ�ָ��
				@1���Ѿ�������ڴ��С
				@2���µĴ�С
				���sizeΪ0���൱��free		
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
			@1:�����Ļ���
			@2��ÿ��Ҫ���Ĵ�С
			@3��Ҫ�����ܴ�С
			@4��Ҫ�����ļ����ļ�ָ��
			size_t fwrite(const void *ptr, size_t size, size_t nmemb,FILE *stream);
	3��errno
		û�г���ʱ����ֵ���ᱻ�������
		�κκ��������ὲerrno��ֵ��Ϊ0����<errno.h>�ж���ĳ�������Ϊ0��
		fprintf(stderr,"fopen() failed,errno = %d\n",errno)�����Դ�ӡerrno��ֵ
		perror("fopen()");<==>fprintf(stderr,"fopen():%s\n",stderror(errno));
		
����ϵͳio�����ļ�io��fdʼ�չᴩ
	1������
		int open(const char *pathname, int flags);
			ע������NONBLOCK��Ĭ������
				��create�����Σ�û��create������
		int close(int fd);
		int open(const char *pathname, int flags, mode_t mode)��
		int creat(const char *pathname, mode_t mode);
		
		ssize_t read(int fd, void *buf, size_t count);
			ÿ��һ���ļ�ָ�붼��ƫ��count
		ssize_t write(int fd, const void *buf, size_t count);
			ÿдһ���ļ�дָ�붼��ƫ��count
		off_t lseek(int fd, off_t offset, int whence);
			Ҫ������ƫ�ƶ���
			lseek��fseek��ftell�Ľ��
		
		//�ļ��������ض���
		int dup(int oldfd);
		int dup2(int oldfd, int newfd);
			dup2(fd,1);<===>close(1);dup(fd);
			��oldfd����newfdʱ�������κβ�����ֱ�ӷ���newfd
		int dup3(int oldfd, int newfd, int flags);
		
		//ͬ��
		void sync(void);
		int fsync(int fd);
		int fdatasync(int fd);
			ֻˢ���ݣ���ˢ������
		
		int truncate(const char *path, off_t length);
			�Ѵ򿪵�ĳ���ļ��ضϵ��̶�����
		
		//�ܼҼ�����
		int ioctl(int d, int request, ...);
		int fcntl(int fd, int cmd, ... /* arg */ );
		/dev/fd/Ŀ¼�а����˵�ǰ���̴򿪵��ļ�������
		
	2��	����
		<1>�ļ�������
			�ļ�����������ʹ�õ�ǰ���õ���С��
			Ĭ��һ���߳̿��Դ�1024���ļ���1024�γ�һ��int[1024]���飬ÿһ������Ԫ�ش��
				��һ���ļ����ɵ�һ���ṹ����׵�ַ���ýṹ����pos����Ϊ�ļ�inode��������
				����ڽ��̿ռ䣬���������Ա����ָ��ͬһ���ṹ�壩
				
		<2>��ʱ�ļ�
			char *tmpnam(char *s);
				����ֵָ��̬�洢������tmpnam�ĺ���ʹ�ûḲ��
				tmpnam�Ĳ�����Ϊ�������������֣������ļ�����ԭ��
			FILE *tmpfile(void);
				����һ���ļ���ָ�룬��ָ��һ��Ψһ����ʱ�ļ������ļ��Զ�д�ķ�ʽ�򿪣�
					������ļ����������ö���ɾ��ʱ�������ʱ�ļ�����ɾ����������NULL��
					����errno������ԭ�ӻ�			
	
	ע��*****************************************��
		1����׼io���ļ�io���ܻ��ʹ��
		2��
			int fileno(FILE *stream);	��׼io------>�ļ�io
			FILE *fdopen(int fd, const char *mode);	�ļ�io----->��׼io
		3��strace xx/file���鿴�ļ�ִ�е�ϵͳ����
		4���´������ļ�Ȩ�ޣ�0666&~umask��0666��umask��λ��umask���Բ鿴�ļ����ڸǵ�Ȩ�ޣ�
		5��scanf�����سɹ���ֵ�ĸ���
		6��printf�����سɹ�������ֽڸ���
		
�����ļ�ϵͳ
	1������
		//Ŀ¼����
		DIR *opendir(const char *name);
		int readdir(unsigned int fd, struct old_linux_dirent *dirp,unsigned int count);
		int closedir(DIR *dirp);
		
		int mkdir(const char *pathname, mode_t mode);
		int rmdir(const char *pathname);
		
		//�鿴�ļ�״̬
		int stat(const char *path, struct stat *buf);
			struct stat�ṹ���е�st_mode��һ��16λ��λͼ��������
				�ļ����ͣ�
					S_ISRE(m):�Ƿ��������ļ�
					S_ISDIR(m):�Ƿ���Ŀ¼
					...
					umsk 0022��ֱ�Ӹ���umask��ֵ����ֹ����Ȩ�޹��ɵ��ļ�
		int fstat(int fd, struct stat *buf);
		int lstat(const char *path, struct stat *buf);
			���ڷ��������ļ���չʾ����Ϣ�Ǹ������ļ�����Ϣ
			
		//�����ļ�Ȩ��
		int chmod(const char *path, mode_t mode);
		int fchmod(int fd, mode_t mode);
		
		//�ı乤��·��
		int chdir(const char *path);
		int fchdir(int fd);
		
		//��ȡ��ǰ����·��
		char *get_current_dir_name(void);
		
		//������·��
		int chroot(const char *path);
		
		//�����ļ��������޸ĵ�ʱ��
		int utime(const char *filename, const struct utimbuf *times);
		
		//Ŀ¼����
		int glob(const char *pattern, int flags,
                int (*errfunc) (const char *epath, int eerrno),
                glob_t *pglob);
			@1��ģʽ��֧��ͨ���
			@2��һ��д0
			@3��������Ϣ����ϣ���õ�ֱ��дNULL��ʹ�õ��ǻص�������
			@4���ṹ��ָ��
		void globfree(glob_t *pglob);
			�����Ҫ�ͷ�
			
		//du���ļ�����Ŀ¼��ռ�Ĵ�С��Ĭ�ϵ�λΪK
		char *strrchr(const char *s, int c);
			��sָ����ַ��������ұߵ�c
			int64_t <===> long long
		**************����Խ�ݹ��ı��������ܷ��ھ�̬��*************
		
		
		//ʱ�亯��
		time_t time(time_t *t);
			��ȡʱ���
		struct tm *localtime(const time_t *timep);
			@1��ʱ���
		struct tm *localtime_r(const time_t *timep, struct tm *result);
		time_t mktime(struct tm *tm);
			������ṹ��
		size_t strftime(char *s, size_t max, const char *format,
                       const struct tm *tm);
					   
		//�������
		//----->/etc/passwd
		int getpwuid_r(uid_t uid, struct passwd *pwd,char *buf, size_t buflen, struct passwd **result);
			У���û�����û�д��û��򷵻ؿ�ָ��
		struct passwd *getpwnam(const char *name);
			��ѯ�û���Ϣ
		//----->/etc/shadow
		struct spwd *getspnam(const char *name);
			�õ�shadow�ļ���һ��
			@1���û���
		char *crypt(const char *key, const char *salt);
			@1������
			@2�����ʴ�
		char *getpass( const char *prompt);
			�ر��ն˻��Եõ�����
			@1����ʾ��Ϣ
		
		makefileע�⣺	
			��Ҫdefine�ģ�CFLAGS+=-D_XOPEN_SOURCE���������ֱ�Ӹ������
			��Ҫ���Ӷ�̬��ģ�LDFLAGS+=-lcrypt
			
		//�ҹ��Ӻ���
		int atexit(void (*function)(void));
			ִ��˳��͵���˳���෴
			@1�����Ӻ���ָ��
		void exit(int status);
			�������ظ�������status&0377
			exit�Ժ���at_exit��on_exitע��Ĺ��Ӻ���������ʽִ��
			
		//�����н���http://vopit.blog.51cto.com/2400931/440453
		int getopt(int argc, char * const argv[],const char *optstring);
			@1����������
			@2��������ɵ��ַ�����ָ�룬����ָ��
			@3��"-y:mdH:MS"
				-	ʶ���ѡ�����
				��	��������Ĳ���
			optarg�������Ĳ���������ֱ���õı�����
			����ֵ��������argv����һ��ѡ�����ĸ
				getopt()ÿ�ε��û���η��������д���Ĳ�����
			��  ��û�в���������һ�ε���ʱ��getopt()������-1��
			��  ��������һ������optstring����Ĳ���������һ����ѡֵ��������ֵʱ������'?'��
			��  ��optstring����':'��ͷʱ��ȱֵ����������»᷵��':'��������'?' 
			
			getopt(argc,argv,"-y:mdH:MS");
			ִ�У�./a.out  -y4 -m -d -H24 -M -S
			
			***************ע****************
			���ж̲�����getopt�������Ϊ���֣�
			����1. ����ֵ�Ĳ��������Ķ��弴�ǲ�������
			����2. �����ֵ�Ĳ��������Ķ������ڲ�����������ټ�һ��ð�š�
			����3. ��ѡֵ�Ĳ��������Ķ������ڲ���������������ð�� ��
				4. ����ֵ�Ĳ���������д����1��a�ǲ���ֵ�Ĳ��������ǿ���-1 -a�ֿ�д��Ҳ����-1a��-a1��д��
	����		5. ���������Ⱥ�˳��'-1a -c cvalue -ddvalue'��'-d -c cvalue -a1'�Ľ��������һ���ġ�
	����		6. Ҫע���ѡֵ�Ĳ�����ֵ�����֮�䲻���пո񣬱���д��-ddvalue�����ĸ�ʽ�����д��-d dvalue�����ĸ�ʽ�ͻ��������

		//��������
		��32λ�Ĳ���ϵͳ�У�ÿ�����̵������ַ�ռ���4G
		��������λ��3G~4G�Ŀ�ͷ��
		char *getenv(const char *name);
			extern char **environ:�ⲿ��������
			char *getenv("PWD");--->��ȡ��������PWD��ֵ
		int setenv(const char *name, const char *value, int overwrite);
			���û�������
			@1��Ҫ���ӵĻ�������������
			@2��Ҫ���ӵĻ���������ֵ
			@3��
				0�������������name�Ѿ����ڣ����ı价��������ֵ
				��0�������������name�Ѿ����ڣ�����ı�û���������ֵ
		int unsetenv(const char *name);
			ȡ����������
		int putenv(char *string);
			@1��name=value
			���ӻ��߸ı价������
			
		//��ת
		goto ���ܿ纯����ת
		int setjmp(jmp_buf env);
			����������ֵ����
				����0��������ת��
				���ط�0���ӱ�ĵط�������
		void longjmp(jmp_buf env, int val);
		
	2��
		1��/tmpĿ¼����ճסλ��t��	
		2��Ȩ��
			Ҫɾ��һ��·��������ļ���ֻ��Ҫ������һ��Ŀ¼��дȨ��
			Ŀ¼Ȩ��
				r�����г�Ŀ¼������
				w������Ŀ¼����ӻ���ɾ���ļ�
				x���ܽ���Ŀ¼
			��ǰĿ¼�е��ļ�����ճסλȨ��ʱ��ֻ�ܽ����ļ����û�ɾ���Լ����ļ����𵽱���������
		3��������������errno������ʹ��perror����
		4��int uname(struct utsname *buf);
			��ȡ�ں���Ϣ
			
�������̿���
	1������
		//fork��дʱ����
		pid_t fork(void);
			pid_t�����̱�ʶ����һֱ�ۼ�
		pid_t getpid(void);
			��ȡ���̺�
		pid_t getppid(void);
			��ȡ�����̺�
			
		//exec
		int execl(const char *path, const char *arg, ...);
			execl("/bin/date","date","+%s",NULL);
			@1�������ȫ·��
			@2������
			@3������Ĳ���
			@4��NULL
			*************ע**************
			ֱ��ȡ����ǰ���ڽ��̣����ǳ�����-1
			
		//wait
		pid_t wait(int *status);
			������ʬ
		pid_t waitpid(pid_t pid, int *status, int options);
			@1��ָ��Ҫwait���ӽ���
			@2������״̬
			��������ʬ
		
		//system
		int system(const char *command);
			�൱��fork��execl��wait�����
			
		//��ʱ��rootȨ��
		int atoi(const char *nptr);
			�ַ���----->����
		int setuid(uid_t uid);
			���õ�ǰ�û���uid
		int execvp(const char *file, char *const argv[]);
			@1��������Բ���ȫ·����
			@2������Ӳ������ַ������׵�ַ
		
		//��д�Լ���shell����
		char *strsep(char **stringp, const char *delim);
			�ַ�������
			@1��Ҫ������ַ���
			@2���ָ������ַ����׵�ַ
			ע��
				* This function modifies its first argument.
				* This function cannot be used on constant strings.
				* The identity of the delimiting character is lost.
			����ֵ
				�Էָ�����ȡ�����ַ���
		glob(tok,GLOB_NOCHECK|GLOB_APPEND*i,NULL,&res->globres);
			@1����stdin��ȡ��һ������ĳһ���ָ�����ȡ����һ��
			@2���ӵڶ�����ʼ��׷�ӵķ�ʽ����gl_pathv������ָ�룩��gl_pathcÿ�μ�1
		execvp(cmd.globres.gl_pathv[0],cmd.globres.gl_pathv);
			@1������
			@2������+�����ϳɵ��ַ������׵�ַ
			
		//�ػ�����
		1��
			void openlog(const char *ident, int option, int facility);
				openlog("mydaemon",LOG_PID,LOG_DAEMON);
				@1���Զ�������
				@2��һ��д���̺ţ�LOG_PID��
				@3����Դ��LOG_DAEMON�����ػ����̣�
			int syslog(int type, char *bufp, int len);
				syslog(LOG_WARNING,"open():%s",strerror(errno));
				syslog(LOG_ERR,"daemonize() failed.");
				syslog(LOG_INFO,"daemonize() successed.");
				@1����������
				@2��Ҫд������,���ܼ�\n
				LOG_DEBUG���ᱻд����־
				����LOG_INFO�ĲŻ�д����־
			closelog();
			ϵͳ���Ӵ��·����/var/log/messages
		2��fork�ӽ���
		3������������
		4���ر�0��1��2
		5��fd = open("/dev/null",O_RDWR);
			dup2(fd,0);
			dup2(fd,1);
			dup2(fd,2);
		6��pid_t setsid(void)
			���õĽ��̻��Ϊ�Ự���leader������������նˣ����ܱ��Ự���leader����
		7��umask(0);
		8��chdir("/");
			�л���ǰ����·��
			
		ע**************
			����̨���������ն�ʱ�������ɱ����̨����
	2������
		1��main(int argc,char **argv,char **environ)
			@3:���Բ�д
		2��������ֹ
			������ֹ��
				��main��������
				����exit����(�⺯��)
				����_exit��_Exit(ϵͳ����)
				���һ���̴߳����������̷���
				���һ���̵߳���pthread_exit
			�쳣��ֹ��
				����abort����ɱ��
				�ӵ�һ���źŲ���ֹ
				���һ���̶߳���ȡ������������Ӧ
		3��init
			���Ƚ��̣���ʱ�ں����൱�ڿ�
		4����fork����֮ǰ�����fflush(NULL)
			fork��ʱ�򻺳���Ҳ�ᱻ��ֵ
		5��http://blog.csdn.net/xy010902100449/article/details/44851453��fork��������ʲô��
				�û����򿴲��������������ַ���û�ֻ�����߼���ַ������Ϊ���̵ĵ�ַ�ռ�Ϊ0
			��max�������ַ��Χ��R+0��R+max��RΪ����ַ,��ַӳ�䣭�������ַ�ռ���ʹ�õ���
			����ַ�任���ڴ��е������ַ�Ĺ��̡����ڴ����Ԫ��MMU������ɡ�
				�߼���ַ��CPU�����ɵĵ�ַ��CPU�������߼���ַ����Ϊ :p ��ҳ�ţ� ������ÿ��
			ҳ�������ڴ��еĻ�ַ��������Ϊҳ���������d ��ҳƫ�ƣ���ͬ��ַ���ϣ�����ȷ
			�������ڴ��豸�������ڴ��ַ��
				�����ַ���ڴ浥Ԫ�������ĵ�ַ��


**************************Posix��׼����Ĳ���**********************************
---------------------------�����+�ź�-----------------���߳�------------------
1��	����֮����໥ͨ�űȽϸ��ӣ���Ҫ�����ܵ��������ڴ�Ƚ��̼�ͨ�ż���
	�߳��������̣��̹߳�����ͬ���ڴ�ռ䣬������ͨ���ϰ�����ͬ���߳̿��Դ�ȡ�ڴ���ͬһ������
2��	shell��system�ڲ���������fork��Linux��fork�Ǵ������̵�Ψһ;��
	�ڸ������У�fork���ش����ӽ��̵Ľ��̺�
	���ӽ����У�fork����0
	����fork����һ����ֵ
	�����ԭ��������
		��ǰ�Ľ������Ѿ��ﵽ��ϵͳ�涨�����ޣ���ʱerrno������ΪEAGAIN
		ϵͳ�ڴ治�㣬��ʱerrno������ΪENOMEM
		������ʺ�С
3�����̼�ͨ������
	һ�������ռ����������ݣ�����һ������ͬʱ���ӽ��Ȳ�������Ϣ
	��ʱ������Ϣ������ϵͳ�������������Եȵ�Ӧ�˳������շ�����
4�����̼�ͨ�ţ�IPC��Inter-Process Communication����ʽ http://www.cnblogs.com/andtt/articles/2136279.html
	�ļ������������Ľ��̶����Ի������������ڱ��أ����ڹ�������ݼ�
	�ܵ�/�����ܵ�����������ʹ��ר�õ��ļ�����������������֮�䴫�����ݡ�
		�����ܵ���ֻ���ڸ��ӽ���֮�����
		�����ܵ���������ͬһ�����ϵ����������ԵȽ���֮�����
		�����ڱ���
		�򵥵����ݹ����������ƣ����������ߺ�������
		ע���ܵ���Ҫ����fd��fd[0]:���ˣ�fd[1]��д��
		�����ܵ�ʵ��˫��ͨ��
	*�ź�������
	*��Ϣ����
	*�����ڴ棺ͨ����ͬһ���ڴ��ж�д���ݹ�����Ϣ
		�����ڱ���
		�κ����͵�Э���������ʺ���Ҫ��ȫ�Ե����
	�źţ�ͨ���ж�֪ͨӦ�˳���ĳһ��������̼�ͨ��Ψһ���첽ͨ�Ż��ƣ�
		�����ڱ��ص�
		�޷����ź��д������ݣ������ź���Ҫ���ڽ��̹���
	�׽��֣������������ù���֮��ʹ��һ�������/���������������
		�����ڱ��ػ�Զ��
		FTP��ssh��Apache Web Server���������
----------------------------------���߳�------------------------------------
1��ÿ���߳�ӵ���Լ��Ķ�����ջ�ռ䣬ӵ�ж�����ִ������
2���Ա�
	�߳�ִ�п���С������������Դ�Ĺ���ͱ����������෴
	�߳��ʺ�����SMP���������У������̿��Կ�����Ǩ��
	һ����Գ���������ȿ��ǵ�����/���߳�ģʽ����β��Ƕ����/���߳�ģʽ
*******************************************************************************
�ġ������+�źţ��źű��ʣ�http://www.cnblogs.com/hoys/archive/2012/08/19/2646377.html��
	http://blog.chinaunix.net/uid-26000296-id-3416281.html
	1������
		//�����źź���
		#include <signal.h>
		typedef void (*sighandler_t)(int);
		sighandler_t signal(int signum, sighandler_t handler);
		void (*singal(int signum,void (*handler)(int)))(int);
			signum : ָ���źŵ�ֵ��
			handler: ָ�����ǰ���ź�ֵ�Ĵ���
			���Ժ��Ը��ź�(������ΪSIG_IGN)��
			���Բ���ϵͳĬ�Ϸ�ʽ�����ź�(������ΪSIG_DFL)��
			Ҳ�����Լ�ʵ�ִ���ʽ(����ָ��һ��������ַ)��
			����ֵ��
				�ɹ����������һ��Ϊ��װ�ź�signum������signal()ʱ��handlerֵ
				ʧ�ܣ��򷵻�SIG_ERR
		signal(SIGINT, SIG_DFL);---->���źŵĶ����ָ�Ĭ��
			����ֵ���ɵ���Ϊ
			�ź������Ժ��źŲ�δ�����������ź����˷��ɴ����ź�
			�źŻ���������ϵͳ����
			�źŵ���Ϊ���ں˲��õģ����ɿ���34) SIGRTMIN֮ǰ�Ķ��ǲ��ɿ��ź�
			�ź������ֹ���������û���ر��ָ�����źŵĹ�����������Ĭ�϶���
				ʹ��Ĭ�϶���
				�����ź�
				�����źţ�ִ���Լ�������źŴ�����
				SIGKILL��SIGSTOP�Ȳ��ܺ���Ҳ���ܲ�����Ϊ�����û�ɱ����ֹͣ�����ṩ�˱�Ҫ����
			�źŵĲ��ɿ������֣������unixϵͳ�У���
				�źŴ�����������ð�װ�ģ�����ÿ�δ����źź󣬾ͽ��źŵ���Ӧ����ΪĬ�϶���
					����취����ÿ���źŴ�������β��һ�ε���signal()�����°�װ�ź�
				������ʱ������ͬ���ź��ٴη����������룩
			Linux֧�ֲ��ɿ��źţ����ǶԲ��ɿ��źŻ������˸Ľ�
				�ڵ������źŴ�����֮�󲻱����µ��ø��źŵİ�װ����
				Linux�µĲ��ɿ��ź�������Ҫ���źſ��ܶ�ʧ�����룩
			������źŰ�װ������signal()	�źŷ��ͺ���kill()	���ܴ�����Ϣ���źŴ�����
			���ڵ��źŰ�װ������sigaction()	�źŷ��ͺ���sigqueue()	�ܴ�����Ϣ���źŴ�����
			�źŰ�װ����signal��sigaction�����ܰ�SIGRTMIN��ǰ���źű�ɿɿ��ź�
			���Ƕ�SIGRTMIN�Ժ���źŶ�֧���Ŷӻ���
		
		//ʵ�ֿɿ��źŵĺ����ӿ�http://www.cnblogs.com/wblyuyang/archive/2012/11/13/2768923.html
		int sigaction(int signum, const struct sigaction *act,struct sigaction *oldact);
			sig:Ϊ�źŵ�ֵ��
				����Ϊ��SIGKILL��SIGSTOP ����κ�һ���ض���Ч���ź�
				Ϊ�������źŶ����Լ��Ĵ��������������źŰ�װ����
			act:ָ��ṹ��sigaction��һ��ʵ����ָ�룬
				�ڽṹ�� sigaction��ʵ���У�ָ���˶��ض��źŵĴ�������Ϊ�գ�
				���̻���ȱʡ��ʽ���źŴ���
				����act ��Ϊ��Ҫ��
				���а����˶�ָ���źŵĴ����ź������ݵ���Ϣ��
				�źŴ�����ִ�й�����Ӧ���ε���Щ�����ȵ�.
			oldact: ָ��Ķ�����������ԭ������Ӧ�źŵĴ���
				��ָ��oldactΪNULL��
				���oldact���ǿ�ָ��, sigaction����ԭ�ȶԸ��źŵĶ���д����ָ���λ�á�
				���oldact �ǿ�ָ��, ��sigaction�����Ͳ���Ҫ�������������ˡ�
			����Ѳ���act, oldact����ΪNULL����ô�ú��������ڼ���źŵ���Ч�ԡ�
			struct sigaction {
               void     (*sa_handler)(int);
               void     (*sa_sigaction)(int, siginfo_t *, void *);
               sigset_t   sa_mask;
               int        sa_flags;
               void     (*sa_restorer)(void);
			};
			ǰ��������ָ���ѡһ
			sa_flags=SA_SIGINFOʱ��ϵͳ��ʹ��sa_sigaction������Ϊ�źŴ�����������
			ʹ��sa_handler��Ϊ�źŴ�����
			sa_mask ��Ա����ָ�����źŴ�����ִ���ڼ���Ҫ�����ε��źţ��ر��ǵ�ĳ
			���źű�����ʱ��������ᱻ�Զ�������̵��ź����룬������źŴ�����ִ
			���ڼ�����źŲ����ٶȷ�����
		//�źż�&�ź�������
		int sigemptyset(sigset_t *set);
			���Ҫ���Ƶ��źż�
		int sigfillset(sigset_t *set);
			�������е��ź�
		int sigaddset(sigset_t *set, int signum);
			����ĳһ���źţ����sigemptysetʹ��
		int sigdelset(sigset_t *set, int signum);
			ɾ����ĳһ���źŵĿ���
		int sigismember(const sigset_t *set, int signum);
			�ж�ĳ����Ա�Ƿ񱻿���
		int sigprocmask(int how, const sigset_t *set, sigset_t *oldset);
			��ʱ��ϣ���ڽӵ��ź�ʱ������ֹͣ��ǰִ�У�ȥ�����źţ�ͬʱҲ��ϣ�����Ը��ź�
			������ʱһ��ʱ��ȥ�����źŴ����������������ͨ�������ź�ʵ�ֵ�
			sigprocmask(SIG_UNBLOCK,&set,&saveset);
			@1��ָ���ź��޸ĵķ�ʽ
				SIG_BLOCK//��set��ָ����źż��а������źżӵ���ǰ���ź������С����ź������set�źż����л������
				SIG_UNBLOCK//��set��ָ����źż��а������źŴӵ�ǰ���ź�������ɾ�������ź������set�����������
				SIG_SETMASK //��set��ֵ�趨Ϊ�µĽ����ź����롣��set���ź���������˸�ֵ������
			@2��Ϊָ���źż���ָ�룬�ڴ�רָ������źż�����������ȡ���ڵ�����ֵ���ɽ�����ΪNULL��
			@3��Ҳ��ָ���źż���ָ�룬�ڴ˴��ԭ�����źż�������������ź������д���ʲô�źš�
			����ֵ���ɹ�ִ��ʱ������0��ʧ�ܷ���-1��errno����ΪEINVAL��
		int sigpending(sigset_t *set);
			���ں�ȡ��pending״̬����set����
		
		//�ź�����
		int sigsuspend(const sigset_t *mask);
			ʵ���˽������--->�ȴ�---->������ԭ�Ӳ���
			sigprocmask(SIG_SETMASK,&oset,&tmpset);
			pause();
			sigprocmask(SIG_SETMASK,&tmpset,NULL);
		
		//�źŷ��ͺ���
		int kill(pid_t pid, int sig);
		int raise(int sig);
			����̱������ź�
			������ϵͳ���ã��ᱻalarm���
			sleep()<===>alarm+pause
			int nanosleep(const struct timespec *req, struct timespec *rem);
		int sigqueue(pid_t pid, int sig, const union sigval value);			
		unsigned int alarm(unsigned int seconds);
			ר��ΪSIGALRM�źŶ��裬
			��ָ����ʱ��seconds��󣬽�����̱�����SIGALRM�źţ��ֳ�Ϊ����ʱ�䡣
			���̵���alarm���κ���ǰ��alarm()���ö�����Ч��
			�������secondsΪ�㣬��ô�����ڽ����ٰ����κ�����ʱ�䡣
			����ֵ��
				�������alarm()ǰ���������Ѿ�����������ʱ�䣬�򷵻���һ������ʱ���ʣ��ʱ�䣬
				���򷵻�0��
		void abort(void);
			��ɱ������core�ļ�
		int setitimer(int which, const struct itimerval *new_value,struct itimerval *old_value);
			setitimer()ΪLinux��API������C���Ե�Standard Library��setitimer()���������ܣ�һ��ָ��
			һ��ʱ��󣬲�ִ��ĳ��function������ÿ���һ��ʱ���ִ��ĳ��function
		
		//��������
		int system(const char *command);
			����һ���źţ�SIGCHLD�������������źţ�SIGINT��SIGQUIT��
		int usleep(useconds_t usec);
	2������
		1���źţ��ź�������жϣ��źŵ���Ӧ�������ж� 
		2���ź������첽�¼�����֪��ʲôʱ���������˲�֪�����ʲô�������������֣���ѯ�����ࣩ��֪ͨ��
		3��kill -l:�鿴�źţ�1-31����׼�źţ�34-64ʵʱ�źţ�
		4��core�ļ���http://www.cnblogs.com/dongzhiquan/archive/2012/01/20/2328355.html
			ulimit -c 0 ����ֹ����core�ļ�
			ulimit -a ���������core�ļ���ѡ���Ƿ��
			�������������ֳ�
			����źŲ�����ֹ��core�ֳ�
		5���źſ�ݼ�
			ctrl -c ��SIGIINT----->��ֹ�������е�����ǰ̨����
			ctrl -z ��SIGSTP------>��������ǰ̨����
			ctrl -\ ��SIGOUT------>��ֹ����ǰ̨���̲�����core�ļ�
			ctrl -d �����Ƿ����źţ�����һ������Ķ�����ֵ����ʾEOF����ֹ���룬�˳�shell
			ctrl -l ��clear screen
		6�������뺯��
			���е�ϵͳ���ö��ǿ�����ģ�һ���ֿ⺯��Ҳ�ǿ�����ģ���memcopy\memmove��
		7���źŵ���Ӧ���̣�32λϵͳΪ����
			1���ں�Ϊÿ������ά������λͼ��resource����mask��ȫ1����pending��ȫ0��
			2��mask���ź������֣���ʾ��ǰ�ź�״̬����ʼֵΪ1
			3��pending����ǰ����λͼ��Ĭ��ȫ0��Ҫ��Ӧ�ĸ��źţ��Ѷ�Ӧ��λ��1
			4�������յ��ź�֮�󣬽�mask&pendingΪ1��λ��ʾ���źſ�����Ӧ�������ں�֮��Ὣmask
				ȫ��λ��0����ֹ���룩�����ں˻ص��û�̬��ʱ���ٽ�maskȫ����Ϊ1�������������
				��һ�����ɱ������ʱ�����źŵ���Ӧ��kernel--->userʱ��Ӧ��
			5������һ���źţ���mask��Ӧλ��0
			6�����ܴ��źŴ����������������������mask��λ�Ĳ��裬�����޷���Ӧ����ź�
		8��volatile:ȥ��ֵʵ�ʵ�λ��ȡֵ
			gcc file.c -'o1' : �Ż�����
			free(NULL)���ᱨ��
			sig_atomic_t���ź�ԭ�ӣ��������Լ�
�塢���߳�
=================================�Աȶ���̲���====================================
����̲�������ʵ�ֺ��׼��
���̲߳������ȱ�׼����ʵ��
===================================================================================
	1������
		//�����ͺ���
		int pthread_equal(pthread_t t1, pthread_t t2);
			�Ƚ������߳�id
		pthread_t pthread_self(void);
			�鿴��ǰ�̵߳��̺߳�
			
		//�����߳�
		int pthread_create(pthread_t *thread, const pthread_attr_t *attr,void *(*start_routine) (void *), void *arg);
			@1��������̱߳�ʶ
			@2����������ԣ�һ��дNULL��
			@3���������ֵ��߳�
			@4�����ֵ��̴߳��Ĳ���
		return NULL ����������̹߳��Ӻ���
		
		//�߳�����
		int pthread_attr_init(pthread_attr_t *attr);
		int pthread_attr_destroy(pthread_attr_t *attr);
		int pthread_attr_setstacksize(pthread_attr_t *attr, size_t stacksize);
		int pthread_attr_getstacksize(pthread_attr_t *attr, size_t *stacksize);
		
		//�߳�ͬ������
		int pthread_mutexattr_init(pthread_mutexattr_t *attr);
		int pthread_mutexattr_destroy(pthread_mutexattr_t *attr);
		int pthread_mutexattr_getpshared(const pthread_mutexattr_t *restrict attr, int *restrict pshared);
		int pthread_mutexattr_setpshared(pthread_mutexattr_t *attr,int pshared);
		
		//�߳�ȡ������ʬ���߳�һ����ȡ��������ʬ
		int pthread_cancel(pthread_t thread);
		int pthread_setcancelstate(int state, int *oldstate);
			@1���Ե�ǰ�����߳�����ȡ��״̬
				PTHREAD_CANCEL_ENABLE������
				PTHREAD_CANCEL_DISABLE���ܾ�
				ΪNULLʱ���൱�ڲ�ѯ��ǰ�̵߳�״̬
			@2������ɵ�״̬
		int pthread_setcanceltype(int type, int *oldtype);
			@1���Ե�ǰ�����߳�����ȡ����ʽ
				PTHREAD_CANCEL_DEFERRED ���߳̽��յ�ȡ��������ֱ�����е�����ȡ���㡱��ȡ���� 
				PTHREAD_CANCEL_ASYNCHRONOUS ���߳̽��յ�ȡ������������ȡ����
		void pthread_testcancel(void);
			�ڵ�ǰ�̴߳���һ��ȡ���㣬�����ǰ�̲߳���ȡ���������������Ч
		int pthread_join(pthread_t thread, void **retval);
			ȡ��������״̬������Ͳ�����
			����ȡ����
				�첽cancel
				�Ƴ�cancel(Ĭ�ϣ��Ƴٵ�cancel������Ӧ����POSIX�����cancel�㶼��������ϵͳ����)
		
		int pthread_detach(pthread_t thread);
			�̷߳��룬�Ѿ�������̲߳����ٶ�����ʬ�����Զ��ͷ�
		
		//ջ������
		void pthread_cleanup_push(void (*routine)(void *),void *arg);
			@1�����õ�ջ������ָ��
			@2�����������Ĳ���
		void pthread_cleanup_pop(int execute);
			@1��Ϊ��ִ�У�1����Ϊ�ٲ�ִ�У�0��������pthread_exit()֮��Ĭ��Ϊ�棬��ִ��
			��������һ�Ժ�
			
		//�߳�ͬ��
		//�����������ƴ��룬ʵ�ֻ��⣨�Զ����ķ�ʽռ��ĳһ�δ��룩
		int pthread_mutex_destroy(pthread_mutex_t *mutex);
		int pthread_mutex_init(pthread_mutex_t *restrict mutex,const pthread_mutexattr_t *restrict attr);
		pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
			�������ĳ�ʼ�������ְ취
		int pthread_once(pthread_once_t *once_control,void (*init_routine)(void))
			��̬ģ��ֻ����һ��
		//��������
		int pthread_cond_destroy(pthread_cond_t *cond);
		int pthread_cond_init(pthread_cond_t *restrict cond,const pthread_condattr_t *restrict attr);
		pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
		int pthread_cond_broadcast(pthread_cond_t *cond);
			������еȴ�
		int pthread_cond_signal(pthread_cond_t *cond);
			������һ���ȴ�
		int pthread_cond_wait(pthread_cond_t *restrict cond,pthread_mutex_t *restrict mutex);
			����--->�ȴ�--->����
			int pthread_mutex_unlock(pthread_mutex_t *mutex);
			int sched_yield(void); ���õ���������������������̵ܶ�sleep
			int pthread_mutex_lock(pthread_mutex_t *mutex);
		int pthread_cond_timedwait(pthread_cond_t *restrict cond,pthread_mutex_t *restrict mutex,const struct timespec *restrict abstime);
	
	2������
		1���̸߳��һ�����������еĺ�����posix��һ�ױ�׼��������ʵ��
		2���̱߳�ʶ��pthread_t�����Ͳ�ȷ����
			ps axm : --�����߳�
			ps ax -L:��Linuxģʽ�鿴�������̹߳�ϵ
			LWP:���������̼��߳�
		3��������ж��̵߳�Դ��ʱ��makefile��Ҫ���룺
			CFLAGS+=-pthread
			LDFLAGS+=-pthread
		4��exit��ʹ������ֹ���߳�Ҳ��������
			�߳̿��Ի�����ʬ
			�̵߳ĵ���ȡ���ڵ������ĵ��Ȳ���
		5���̵߳���ֹ
			�̴߳����������̷��أ������߳��˳���
			�̱߳�ͬһ�����е������߳�ȡ��
			�̵߳���pthread_exit()����
		6��һ�����̴����Ķ���̣߳�ջ�ռ��Ƕ����ģ�������ǹ��õģ������ܵ�ջ�ռ�����
			32λ������C����������ַΪ0~4G
			user̬��0x08048000~3G
			64λ������C���������ַΪ128T
		7���ź�������
		8����д��
			������������
			д����������
			
�����߼�IO��������IO��
******************�����м�relay*******************************************************
1�����ļ�ʱ�ᱨ�ļٴ�
	EAGAIN���������ı���һ��ʱ����������ݾͷ��أ���ʾ�����ٶ�
	EINTR�������ı��������Ǳ��źŴ����
**************************************************************************************
	1���ļ���
		int fileno(FILE *stream);	fp---->fd
		int lockf(int fd, int cmd, off_t len);
			@2�������ļ�״̬
				F_LOCK������
				F_ULOCK������
			@3���ӵ�ǰλ�ü����ĳ��ȣ�Ϊ0ʱ���Ὣ�����ļ���ס
	2��IO��·ѭ��
		2.1 select�����ü�������---->����----->�鿴���ӽ����
			int fcntl(int fd, int cmd, ... /* arg */ );
				fd1_save = fcntl(fd1,F_GETFL);
					���浱ǰ״̬
				fcntl(fd1,F_SETFL,fd1_save|O_NONBLOCK);
					���õ�ǰ״̬�����������
			//���ü�������
				����д����0
					FD_ZERO(&rset);
					FD_ZERO(&wset);
				���¼��м���Ҫ�������ļ�������
					FD_SET(fsm12.sfd,&rset);
					FD_SET(fsm21.dfd,&wset);
			//����
				int select(int nfds, fd_set *readfds, fd_set *writefds,
							fd_set *exceptfds, struct timeval *timeout);
					select(max(fd1,fd2)+1,&rset,&wset,NULL,NULL)
					@1���̶�ȡ���м������ļ�������������fd+1
					@2�����¼���
					@3��д�¼���
					@4�������쳣
					@5�����ó�ʱ�������þ�����
					�����ӵ�ĳһ���ļ��������ж�����д�Ĳ���ʱ�������˳������ߵ�����
						�˳�ʱ�����˳�ʱ�¼��������˳�
			//�鿴���ӽ��
				FD_ISSET(fd2,&rset)���ļ��������ڶ��¼�����
				FD_ISSET(fd1,&wset)���ļ���������д�¼�����
		2.2 poll�����ļ�������Ϊ��λ��֯�¼���
			int poll(struct pollfd *fds, nfds_t nfds, int timeout);
				struct pollfd {
					int   fd;         /* file descriptor */
					short events;     /* requested events */
					short revents;    /* returned events */
				};
				@1���ṹ������ָ��
				@2�����鳤�ȣ����Ϊ1024��
				@3�����ó�ʱ
					0��������
					-1����������
					>0�����õĳ�ʱʱ��
			//���ü�������
			pfd[0].events = 0;
				���ļ��������������¼�����
			pfd[0].events |= POLLIN;
				���ļ���������Ӽ����¼������¼���
			pfd[0].events |= POLLOUT;
				���ļ����������д�¼�
			//����
			poll(pfd,2,-1)��
			//�鿴���ӽ��
			pfd[0].revents & POLLIN || pfd[1].revents & POLLOUT
				0�ܶ�����1��д
				
		2.3 epoll�������ں�̬��������ֲ��
			int epoll_create(int size);
				@1���������0����
			int epoll_ctl(int epfd, int op, int fd, struct epoll_event *event);
				@1�����溯���ķ���ֵ
			int epoll_wait(int epfd, struct epoll_event *events,int maxevents, int timeout);
				@3���ȴ��¼�����
			//��ʼ������
			struct epoll_event ev;
			epfd = epoll_create(10);
				����epoll����
			ev.events = 0;
			ev.data.fd = fd1;
			epoll_ctl(epfd,EPOLL_CTL_ADD,fd1,&ev);
				���¼������fd1
			ev.events = 0;
			ev.data.fd = fd1;
			epoll_ctl(epfd,EPOLL_CTL_ADD,fd2,&ev);
				���¼������fd2
			//���ü�������
			ev.events = 0;
			ev.data.fd = fd1;
			ev.events |= EPOLLIN;
				���ļ���������Ӷ��¼�
			//����
			epoll_wait(epfd,&ev,1,-1)
			//�鿴���ӽ��
			λ�������ж��߼�ֵ
	3���첽IO
		ssize_t readv(int fd, const struct iovec *iov, int iovcnt);
		ssize_t writev(int fd, const struct iovec *iov, int iovcnt);
			
	4���ڴ�ӳ��		http://blog.chinaunix.net/uid-24517893-id-164217.html
		void *mmap(void *addr, size_t length, int prot, int flags,int fd, off_t offset);
			@1��ָ����ӳ����ڴ���ʼ��ַ��ͨ����Ϊ NULL��������ϵͳ�Զ�ѡ����ַ��ӳ��ɹ��󷵻ظõ�ַ��
			@2�������ļ��ж��Ĳ���ӳ�䵽�ڴ�
			@3�� ӳ������ı�����ʽ������Ϊ���¼��ַ�ʽ����ϣ�
                    PROT_EXEC ӳ������ɱ�ִ��
                    PROT_READ ӳ������ɱ���ȡ
                    PROT_WRITE ӳ������ɱ�д��
                    PROT_NONE ӳ�������ܴ�ȡ
			@4��Ӱ��ӳ������ĸ������ԡ��ڵ���mmap()ʱ����Ҫָ��MAP_SHARED ��MAP_PRIVATE��
			@5��Ҫӳ�䵽�ڴ��е��ļ�������
			@6���ļ�ӳ���ƫ������ͨ������Ϊ0��������ļ���ǰ����ʼ��Ӧ��offset������PAGE_SIZE����������
			����ֵ��
				��ӳ��ɹ��򷵻�ӳ�������ڴ���ʼ��ַ�����򷵻�MAP_FAILED(��1)
				����ԭ�����errno �С�
		int munmap(void *addr, size_t length);
	5���ܵ�
		int pipe(int pipefd[2]);
			0��1д
	*****************����IPC����**********************************
	xxxget():����
	xxxop():����
	xxxctl():����
	**************************************************************
	6����Ϣ����
		struct msgbuf {
               long mtype;       /* message type, must be > 0 */
               char mtext[1];    /* message data */
           };
		 ע�������Сʱ��Ӧ��ȥ��longռ���ֽڣ�char mtext[1]-->�䳤����
		key_t ftok(const char *pathname, int proj_id);
			@1��Э�������涨��·��
			@2��Э�������涨������
		int msgget(key_t key, int msgflg);
			msgget(key,IPC_CREAT|0600);
			@1��ftok�ķ���ֵ
			@2��IPC_CREAT����������Ȩ��
		int msgsnd(int msqid, const void *msgp, size_t msgsz, int msgflg);	
		ssize_t msgrcv(int msqid, void *msgp, size_t msgsz, long msgtyp,int msgflg);
			@1��msget�ķ���ֵ
			@2����������
			@3���������ݵĴ�С
		msgctl(msgid,IPC_RMID,NULL);
			����
	7���ź�������
	8�������ڴ�
	
�ߡ������׽��� socket
	********************************************************************
	1�������ڿ��������ݴ���
	2������
		1���ֽ���
			�����ֽ���host
			�����ֽ���network
			������ֽ���ת��
			uint16_t htons(uint16_t hostshort);
			uint16_t ntohs(uint16_t netshort);
		2������
			�����
			__attribute__((packed))__�����óɲ�����
		3�����ͳ���
			int 32_t	uint 32_t	int 64_t
	3��int socket(int domain, int type, int protocol);
		@1��Э����
		@2����������
		@3��Э�����е�һ��Э��
	���⣺
		FILE *fdopen(int fd, const char *mode);
			��fd--->ת��Ϊfp
	********************************************************************
	1��tcp����ʽ��
		server�ˣ������ˣ�����������
		1��ȡ��socket
		2����socket�󶨵�ַ
		3����socket��Ϊ����ģʽ
		4����������
		5����/����Ϣ
		6���ر�
		
		client�ˣ������ˣ�����������
		1��ȡ��socket
		2����socket�󶨵�ַ
		3����������
		4����/����Ϣ
		5���ر�
		
		//struct sockaddr��struct sockaddr_in
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
		
		================s��=====================
		1��int socket(int domain, int type, int protocol);
			��ȡsocket��ʶ��
			
		2���������ȡsocket����
		setsockopt(sd,SOL_SOCKET,SO_REUSEADDR,&val,sizeof(val)
		int setsockopt(int sockfd, int level, int optname,const void *optval, socklen_t optlen);
			@1��socket����ֵ
			@2������ָ��ѡ����������
				SOL_SOCKET: �����׽ӿ�
				IPPROTO_IP: IPv4�׽ӿ�
				IPPROTO_IPV6: IPv6�׽ӿ�
				IPPROTO_TCP: TCP�׽ӿ�
			@3��ѡ������
				SO_REUSEADDR
					�������ñ��ص�ַ�Ͷ˿ڣ�������Ѿ���ʹ�õĵ�ַ����˿ںţ�
			@4��������һ��������ָ��
			@5��@4�ĳ���
		int getsockopt(int sockfd, int level, int optname,void *optval, socklen_t *optlen);
		
		3���󶨶˿�
		bind(sd,(void *)&laddr,sizeof(laddr)��
		//�����ʽip��ַת��Ϊ����
		int inet_pton(int af, const char *src, void *dst);
			@1��Э����
			@2��Ҫת����ip��ַ��0.0.0.0��
			@3��ת���������͵�ַ��ָ��
		const char *inet_ntop(int af, const void *src,char *dst, socklen_t size);
			�����͵�ip��ַת��Ϊ���ʽ��
		//�󶨶˿�
		int bind(int sockfd, const struct sockaddr *addr,socklen_t addrlen);
			@1��socket����ֵ
			@2����ַ�Ľṹ��ָ�루�ڲ�������ip��ַ���˿ںš�Э����ȣ�
				��man 7 ip����Э���ڱ䣬����ʱ��Ҫǿת
			@3��@2�ĳ���
		
		4������Ϊ����ģʽ��������������accept����
		listen(sd,200)��
		int listen(int sockfd, int backlog);
			@1����һ���õ���socket
			@2���������еĳ��ȣ�tcp�����д��ڰ����ӳص�������ں���Ҫ����
			
		5����������
		accept(sd,(void *)&raddr,&raddr_len);
		int accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen);
			@1��socket����ֵ
			@2����ַ�ṹ��ĵ�ַ����ҪǿתΪstruct sockaddr��
		
		6����������
		ssize_t send(int sockfd, const void *buf, size_t len, int flags);
		
		================c��=====================
		1����ȡsocket
		2���󶨵�ַ����ʡ��
		3����������
			int connect(int sockfd, const struct sockaddr *addr,socklen_t addrlen);
		4��������
		5��������
		6���ر�socket
	
	2��udp����ʽ���䣩
		server�ˣ����������հ���һ��������������
		1��ȡ��socket
		2����socketȡ�õ�ַ
		3����/����Ϣ
		4���ر�
		client�ˣ������ˣ��ȷ�����һ��
		1��ȡ��socket
		2����socketȡ�õ�ַ����ʡ�ԣ�
		3����/����Ϣ
		4���ر�
		
		===============s��========================
		1����ȡsocket
			sd = socket(AF_INET,SOCK_DGRAM,0/*IPPROTO_UDP*/);
			0�������Э�����еĵ�һ��Э�飬��ʽ�ĵ�һ����UDP
		2���󶨶˿�
			bind(sd,(void *)&laddr,sizeof(laddr))��
		3����������
			recvfrom(sd,&rbuf,sizeof(rbuf),0,(void *)&raddr,&raddr_len);
		4���ر�socket
		===============c��========================
		1�����socket
			socket(AF_INET,SOCK_DGRAM,0/*IPPROTO_UDP*/);
		2����������
			sendto(sd,&sbuf,sizeof(sbuf),0,(void *)&raddr,sizeof(raddr))��
		3���ر�socket
		
	********************************************************************
	udpЭ�飺�������㲥���ಥ���鲥��http://www.bkjia.com/cjjc/995939.html
		�㲥���㲥��ַ��ȫ���㲥��255.255.255.255	�����㲥�������ع㲥��Ϣ���ᱻ·����ת��
			�㲥������Internet������д���
		�ಥ��������Internet������д���
			�ಥ��Ҫ����������Ƶ�����ϻ���
			�ಥʹ��D���ַ��224.0.0.0~239.255.255.255��������Ϊ
				�ֲ����Ӷಥ��ַ��224.0.0.0~224.0.0.255��Ϊ·��Э���������;�����ĵ�ַ��·��
					������ת�����෶Χ�ڵ�IP��
				Ԥ���ಥ��ַ:224.0.1.0~238.255.255.255��������ȫ��Χ����internet��������Э��
				����Ȩ�޶ಥ��ַ��239.0.0.0~239.255.255.255���ɹ���֯�ڲ�ʹ�ã�������˽��IP
					��ַ����������internet�������ƶಥ��Χ
			�ಥ��ʵ������������setsockopt()��getsockopt()��ѡ����IP�㣨man 7 ip��
			getsockopt()/setsockopt()��ѡ��		�� ��
			IP_MULTICAST_TTL					���öಥ�����ݵ�TTLֵ
			IP_ADD_MEMBERSHIP					��ָ���ӿ��ϼ����鲥��
			IP_DROP_MEMBERSHIP					�˳��鲥��
			IP_MULTICAST_IF						��ȡĬ�Ͻӿڻ����ýӿ�
			IP_MULTICAST_LOOP					��ֹ�鲥���ݻ���
	********************************************************************
	3���㲥��broadcast��
		----------------->server��
		inet_pton(AF_INET,"0.0.0.0",&laddr.sin_addr.s_addr);
		1����ȡsocket
			sd = socket(AF_INET,SOCK_DGRAM,0/*IPPROTO_UDP*/);
		2�����ù㲥
			setsockopt(sd,SOL_SOCKET,SO_BROADCAST,&val,sizeof(val);
		3���󶨹㲥ip��ַ���ض��˿ڣ���1989��
			bind(sd,(void *)&laddr,sizeof(laddr))��
		4����������
			recvfrom(sd,&rbuf,sizeof(rbuf),0,(void *)&raddr,&raddr_len)��
		---------------->client��
		inet_pton(AF_INET,"255.255.255.255",&raddr.sin_addr);
		1����ȡsocket
		2�����ù㲥
			setsockopt(sd,SOL_SOCKET,SO_BROADCAST,&val,sizeof(val);
		3�������������㲥��ַ
			sendto(sd,&sbuf,sizeof(sbuf),0,(void *)&raddr,sizeof(raddr);
		4���ر�socket
			
	4���ಥ���鲥��
		----------------->server��
		IP_ADD_MEMBERSHIP (since Linux 1.2)
        Join a multicast group.  Argument is an ip_mreqn structure.
			struct ip_mreqn {
				struct in_addr imr_multiaddr; /* IP multicast group
												   address */
				struct in_addr imr_address;   /* IP address of local
												   interface */
				int            imr_ifindex;   /* interface index */
			};
		1����ȡsocket
		2�����öಥ
			inet_pton(AF_INET,MGROUP,&mreq.imr_multiaddr);
				���öಥ��ַ
			inet_pton(AF_INET,"0.0.0.0",&mreq.imr_address);
				���ñ��ص�ַ
			mreq.imr_ifindex = if_nametoindex("eth0");
				���ñ��ص�ַ����ӿ�����
			setsockopt(sd,IPPROTO_IP,IP_ADD_MEMBERSHIP,&mreq,sizeof(mreq);
				@4��
		3���󶨶˿ںͱ���ip��ַ
			inet_pton(AF_INET,"0.0.0.0",&laddr.sin_addr.s_addr);
			bind(sd,(void *)&laddr,sizeof(laddr);
		4����������
			recvfrom(sd,&rbuf,sizeof(rbuf),0,(void *)&raddr,&raddr_len);
		----------------->client��
		1����ȡsocket
		2������ಥ��
			#define MGROUP      "224.2.2.2"
			inet_pton(AF_INET,MGROUP,&mreq.imr_multiaddr);
			inet_pton(AF_INET,"0.0.0.0",&mreq.imr_address);
			mreq.imr_ifindex = if_nametoindex("eth0");
			recvfrom(sd,&rbuf,sizeof(rbuf),0,(void *)&raddr,&raddr_len);
		3����������
			raddr.sin_port = htons(atoi(RCVPORT));
			inet_pton(AF_INET,MGROUP,&raddr.sin_addr);
			sendto(sd,&sbuf,sizeof(sbuf),0,(void *)&raddr,sizeof(raddr);
		4���ر�socket	
			
		****************************************************************
		unsigned if_nametoindex(const char *ifname);
			������������---->����
		ip ad sh���鿴������������
		ip ro sh���鿴·�����
		ip ro add default via�����һ��Ĭ��·��
		eog��ͼƬ�������
		accept���������⣬ͬһʱ��ֻ����Ӧһ��sd
		getaddrinfo����������
		tcgetattr�����ü���
		****************************************************************		
			
				
			
	
	
		