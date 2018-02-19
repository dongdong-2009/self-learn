#!/usr/local/bin/lua

function newCounter()
	local i = 1
	return function()
		i = i * 2		--非局部变量i
		return i
	end
end

c1 = newCounter()
c2 = newCounter()
print(type(c1))
for i=1,10,2 
do
	print(c1())		--2,4,8,16,32
end
print("===========")
print(c2())		--c2和c1是两个不同的closure，拥有各自的非局部变量i