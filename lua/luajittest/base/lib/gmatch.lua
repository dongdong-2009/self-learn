#!/usr/local/bin/luajit

str = "i am akon"

for k,v in string.gmatch(str)
do
	print(k,"--->",v)
end
