--[[
元表的__index指向函数，在表中找不到键值，则调用元表的index指向的方法，参数是表名和键值
--]]
mytable = setmetatable({key1 = "value1"}, {
  __index = function(mytable, key)
    if key == "key2" then
      return "metatablevalue"
    else
      return nil
    end
  end
})

print(mytable.key1,mytable.key2)