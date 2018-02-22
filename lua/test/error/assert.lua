#!/usr/local/bin/lua

n = io.read()
--[[
参数：
	1、表达式，返回值为nil则打印参数2
	2、string，出错后的提示语句
--]]
assert(tonumber(n),"Invalid input: " .. n .. " is not a number")

print("ok")