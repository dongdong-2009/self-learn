#!/usr/local/bin/lua

--可变参数
function add(...)
	local s = 0
	--{...}表示由所有变长参数组成的table，三个点表示表达式
	for i,v in pairs{...}
	do
		if type(v) ~= "number" 
		then
			return "请检查参数是否为数字"
		end
		s = s + v 
	end
	return s
end

print(add(1,2,3,'4',5,6,7,8,9))