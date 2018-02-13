#!/usr/local/bin/lua

co = coroutine.create(function()
	local i = 1
	while(true)
	do
		local res = coroutine.yield(i)
		i = i + 1
		print(res,"Coroutine exec")
	end
end)

for j=10,1,-1
do
	print("main",coroutine.resume(co))
end
