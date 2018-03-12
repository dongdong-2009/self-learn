arr = {1,2,3,4}
arr.name = function()
	print('akon')
end

print(#(arr))
print(table.getn(arr))
arr.name()


brr = {"a","b"}			--2
brr1 = {a = "akon",b = "bkon"}   --0
print(#(brr).."----"..#(brr1))
print(next(brr1).."-----"..next(brr))  --next返回表的第一个元素的下标
print(next(brr1).."-----"..next(brr))  --next返回表的下标

--判断一个表是否为空
function isTableEmpty(t)
    return t == nil or next(t) == nil
end
