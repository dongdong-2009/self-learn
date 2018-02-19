#!/usr/local/bin/lua

i = 32
local i = 0

-- loadstring在编译时不涉及词法语，在全局环境中编译它的字符串
f = loadstring("i=i+1;print(i)")

-- 闭包函数操作局部变量
g = function()
	i = i + 1
	print(i)
end
f()	-- 33
g() -- 1