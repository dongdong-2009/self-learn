file = io.open("txt", "r")    -- 使用 io.open() 函数，以只读模式打开文件

for line in file:lines() do         -- 使用 file:lines() 函数逐行读取文件
   print(line)
end

file:close()
