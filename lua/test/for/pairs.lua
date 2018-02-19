#!/usr/local/bin/lua

m = {['name']="akon";['age']="20"}
m.sex = 'man'

for key,val in pairs(m) do
	print(string.format("%s===>%s",key,val))
end

arr = {"a","b","c"}
for key,val in ipairs(arr)
do
	print(string.format("%s====>%s",key,val))
end
