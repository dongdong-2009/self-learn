local status,err = pcall(function() error({code=304,msg="I am error"}) end)

print(status,"--->",err.code,"--->",err.msg)

local sta,err1 = pcall(function() a = "a" + 1 end)
print(err1)