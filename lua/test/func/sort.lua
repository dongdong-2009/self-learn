#!/usr/local/bin/lua

network = {
	{name = "akon",ip = "192.168.0.1"},
	{name = "xiaoze",ip = "192.168.0.2"},
	{name = "houxixi",ip = "192.168.0.3"},
	{name = "lll",ip = "192.168.0.4"}
}

table.sort(network,function(a,b) return (a.name < b.name) end)

for key,var in pairs(network)
do
	--print(type(key),"--->",type(var))
	for key1,var1 in pairs(var)
	do
		print(key1,"-->",var1)
	end
	print("==============")
end
