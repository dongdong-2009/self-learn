#!/usr/local/bin/lua

co = coroutine.create(function()
	local i = 1
	while(true)
	do
		coroutine.yield(i)
		i = i + 1
		print("[coroutine][status]",coroutine.status(co),"[thread num]",coroutine.running())
	end
end)

for j=10,1,-1
do
	sta,var = coroutine.resume(co)
	print("[main]<",sta,"><",var,">","[status]",coroutine.status(co),"[thread num]",coroutine.running())
end
