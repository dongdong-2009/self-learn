#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
#include <dirent.h>
#include <errno.h>

static int l_dir(lua_State *L)
{
		DIR *dir;
		struct dirent *entry;
		int i = 0;
		// 如果给定虚拟栈中索引处的元素可以转换为字符串，则返回转换后的字符串，否则报错。
		const char *path = luaL_checkstring(L, 1);

		/* open directory */
		dir = opendir(path);
		if(dir == NULL) {
				// 出错返回"nil"加上一个描述错误信息的字符串。
				lua_pushnil(L);
				lua_pushstring(L, strerror(errno));
				return 2;    // "nil"加上字符串，共两个返回值。
		}

		/* create result table */
		lua_newtable(L);
		i = 1;
		while((entry = readdir(dir)) != NULL)    // 逐一读取目录中的文件。
		{
				lua_pushnumber(L, i++);  /* push key */
				lua_pushstring(L, entry->d_name);  /* push value */
				lua_settable(L, -3);    // t[k] = v
		}

		closedir(dir);

		return 1;    // 返回值只有一个，"table"。
}

int main(void)
{
		lua_State *L = luaL_newstate();
		luaL_openlibs(L);

		// 将C函数"l_dir"定义为Lua的全局变量"mydir"。
		lua_register(L, "mydir", l_dir);

		// 打印"/home/"目录下的所有文件。
		const char* testfunc = "for i, v in pairs(mydir('/home')) do print(i, v) end";

		if(luaL_dostring(L, testfunc))    // 执行Lua命令。
				printf("Failed to invoke.\n");

		lua_close(L);

		return 0;
}
