/*********************************************************************
  功能：创建一个连接mysql，并创建mkk_test的实例
  作者：穆昆空
  时间：2016-12-05
 *********************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <mysql/mysql.h>

MYSQL 		*conn;			//mysql连接
MYSQL_RES 	*res;			//mysql记录集
MYSQL_ROW	row;			//字符串数组，mysql记录行

#define MAX_BUF_SIZE	1024

const char *host = "localhost";
const char *user = "root";
const char *passwd = "111111";
const char *db_name = "mkk_test";
const unsigned int port = 3306;

void print_mysql_error(const char *msg) 
{ // 打印最后一次错误
	if (msg)
		printf("%s: %s\n", msg, mysql_error(conn));
	else
	    puts(mysql_error(conn));
}

int executesql(const char * sql)
{
	if(mysql_real_query(conn,sql,strlen(sql)))
		return -1;
	return 0;
}

int init_mysql(void)
{
	conn = mysql_init(NULL);
	if(conn == NULL)
	{
		return -1;
	}

	if(!mysql_real_connect(conn,host,user,passwd,db_name,port,NULL,0))
		return -1;

	//检测数据库是否可用
	if(executesql("set names utf8"))
		return -1;

	return 0;
}

int main(void)
{
	if(init_mysql())
	{
		print_mysql_error(NULL);
		return -1;
	}
	printf("connect mkk_test success\n");

	//在事先命令行建好的表中插入一条数据
	char sql[MAX_BUF_SIZE];
	sprintf(sql, "INSERT INTO `test`(`name`) VALUES('testname')");
	if(executesql(sql))
	{
		print_mysql_error(NULL);
	}

	if (executesql("SELECT * from `test`")) // 句末没有分号
		print_mysql_error(NULL);

	res = mysql_store_result(conn); // 从服务器传送结果集至本地，mysql_use_result直接使用服务器上的记录集

	int iNum_rows = mysql_num_rows(res); // 得到记录的行数
	int iNum_fields = mysql_num_fields(res); // 得到记录的列数
	printf("共%d个记录，每个记录%d字段\n", iNum_rows, iNum_fields);

	puts("id\tname\n");
	while ((row=mysql_fetch_row(res))) // 打印结果集
	     printf("%s\t%s\n", row[0], row[1]); // 第一，第二字段
	
	mysql_free_result(res); // 释放结果集
	mysql_close(conn);

	return 0;
}












