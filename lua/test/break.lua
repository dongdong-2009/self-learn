#!/usr/local/bin/lua

m = {1,2,3,4,5}

for key,val in pairs(m)
do
	if key > 3 then
		print("End")
		break	
		--print("End") --breakֻ������������һ�����
	end
	print(string.format("%s-->%s",key,val))
end
