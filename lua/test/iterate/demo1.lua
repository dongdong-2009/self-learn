#!/usr/local/bin/lua

tab = {"hello","akon","aze"}

function values (t)
	local i = 0
	return function()
		i = i + 1
		return t[i]
	end
end

--[[iter = values(tab)
while true
do
	local element = iter()
	if element == nil then
		break
	end
	print(element)
end--]]

for element in values(tab)
do
	print(element)
end