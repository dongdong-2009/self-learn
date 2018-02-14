#!/usr/local/bin/lua

package.path = package.path..";./mod/?.lua"
--print(package.path)

local akon = require("akon")

local s1 = akon:new(1,2)

print(s1:get_circumference())
print(s1:get_square())
