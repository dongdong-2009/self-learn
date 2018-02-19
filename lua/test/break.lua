#!/usr/local/bin/lua

m = {1,2,3,4,5}

for key,val in pairs(m)
do
	if key > 3 then
		print("End")
		break	
		--print("End") --break只能是语句块的最后一条语句
	end
	print(string.format("%s-->%s",key,val))
end
