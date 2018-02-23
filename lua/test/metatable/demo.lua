--[[
原表中不能存在__metatable键值
__index键值指向表
访问的表t中不存在foo键，则在表t的元表的__index中查找
--]]
other = {foo = 3}
t = setmetatable({},{__index = other})

print(t.foo)