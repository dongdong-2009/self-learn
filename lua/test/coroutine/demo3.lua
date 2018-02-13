#!/usr/local/bin/lua

co = coroutine.create(function(a,b,c)
	print("one>",a,b,c)
	var1,var2,var3 = coroutine.yield(a*a,b+b,c*3)
	
	print("two>",var1,var2,var3)
end)

print("main>",coroutine.resume(co,3,4,5),"\n-----------\n")
print("main>",coroutine.resume(co,"hello ","world"),"\n-----------\n")
