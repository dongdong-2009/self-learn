local ffi = require "ffi"

local myffi = ffi.load('myffi')		--加载c的so库

ffi.cdef[[
int add(int x,int y);
]]

local res = myffi.add(1,2)
print(res)
