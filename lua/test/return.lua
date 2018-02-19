#!/usr/local/bin/lua

function add(a,b)
	local var1 = tonumber(a)
	local var2 = tonumber(b)
	--print(var1,var2)
	if type(var1) == "nil" or type(b) == "nil" then
		return "Please check input"
		--print() --return只能是一个语句块的最后一条语句
	end
	return a + b
end

--[[a = io.read()
b = io.read()--]]

print(add('10','20'))