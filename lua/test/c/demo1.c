#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>

static int l_sin(lua_State *L)
{
		// 如果给定虚拟栈中索引处的元素可以转换为数字，则返回转换后的数字，否则报错。
		double d = luaL_checknumber(L, 1);
		lua_pushnumber(L, sin(d));  /* push result */

		/* 这里可以看出，C可以返回给Lua多个结果，
		 * 通过多次调用lua_push*()，之后return返回结果的数量。
		 */
		return 1;  /* number of results */
}

int main(void)
{
		lua_State *L = luaL_newstate();    // 创建Lua状态机。
		luaL_openlibs(L);    // 打开Lua状态机"L"中的所有Lua标准库。

		/* 这两句话还有更简单的方法：
		 * lua_register(L, "mysin", l_sin)
		 * 将C函数"l_sin"定义为Lua的全局变量"mysin"。
		 * 其实现是如下宏：
		 * #define lua_register(L,n,f) \
		 *      (lua_pushcfunction(L, f), lua_setglobal(L, n))
		 */
		lua_pushcfunction(L, l_sin);    // 将C函数转换为Lua的"function"并压入虚拟栈。
		lua_setglobal(L, "mysin");    // 弹出栈顶元素，并在Lua中用名为"mysin"的全局变量存储。

		const char* testfunc = "print(mysin(3.14 / 2))";

		if(luaL_dostring(L, testfunc))    // 执行Lua命令。
				printf("Failed to invoke.\n");
		lua_close(L);    // 关闭Lua状态机。

		return 0;
}
