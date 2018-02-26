--[[
由第一行的打印结果可以看出函数string.gmatch()返回的类型是一个函数，这一点比较类似于以前所讲的一个函数io.lines()，有兴趣的朋友可以看一下。
由函数string.gmatch()返回的迭代函数每一次调用都会得到新的匹配结果。
关于匹配符的含义%a表示查找一个字母，%w表示查找一个数字或者字母，%d表示查找一个数字，而后面的+表示1个或者多个，即%a+表示查找连续的1个及以上的字母字符串，除了这几个外还有很多类型的匹配符，以后还要仔细去说，在这里不着用过多的篇幅。
知道了匹配符的含义自然就能了解(%w+)=(%w+)的意义了，它就是用来寻找形如key=value的字符串的。
从最后一组输出结果可以看出，当查找的字符串不包含所要匹配的字符时，返回的迭代函数调用时会返回nil。

作者：AlbertS
链接：https://www.jianshu.com/p/ef0bafd9b6a2
來源：简书
著作权归作者所有。商业转载请联系作者获得授权，非商业转载请注明出处。
http://blog.csdn.net/suifcd/article/details/78928559
--]]
local str = "12ab34,eed,56"
local func_itor = string.gmatch(str, "%d+")

-- 查看func_itor类型
print("func_itor is ", func_itor)
-- 第一次调用函数func_itor
print("func_itor ret is ", func_itor())

-- 再次调用函数func_itor
print("func_itor ret is ", func_itor())

-- 使用循环来打印
local sourcestr = "hello world from Lua"
local index = 1
print("\noutput capture using loop:")
for word in string.gmatch(sourcestr, "%a+") do
    print(index,"   ", word)
    index = index + 1
end

-- 查找属性对
local attrstr = "from=world, to=Lua, name=AlbertS"
print("\noutput attr pair capture using loop:")
for k,v in string.gmatch(attrstr, "(%w+)=(%w+)") do
    print(k,"  ", v)
end

local nonumstr = "fadfasd,.;p[];'asd"
local func_numitor = string.gmatch(nonumstr, "%d+")
local numret = func_numitor()
print("\nnumret ret is", numret)

