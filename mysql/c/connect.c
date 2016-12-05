#include <stdio.h>
#include <stdlib.h>
#include <mysql/mysql.h>


/*
	注意事项：
		1、在编译的时候需要链接mysqlclient库
 */


/*
   错误处理api
   		// 返回错误码
		unsigned int mysql_errno(MYSQL *connection);
		// 返回错误详细信息
		char* mysql_error(MYSQL *connection);
 */
void error(MYSQL* conn)
{
	printf("Connection error %d: %s\n", mysql_errno(conn), mysql_error(conn));
}

int main(int argc,char **argv)
{
	MYSQL *conn;
	
	//1、初始化连接句柄
	conn = mysql_init(NULL);
	if(conn == NULL)
	{
		error(conn);
		exit(1);
	}
	printf("Init mysql success!\n");

	//2、真正的连接数据库
	//参数：句柄、mysql所在主机ip、用户名、密码、库名，其余默认
	conn = mysql_real_connect(conn,"127.0.0.1","root","111111","cstack",0,NULL,0);
	if(conn == NULL)
	{
		error(conn);
		exit(1);
	}
	printf("Connect mysql cstack success\n");

	//3、关闭数据库连接
	mysql_close(conn);

	exit(0);
}
