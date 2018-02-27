local tblTest1 = { 1, a = 2, 3 }
print("Test1 " .. table.getn(tblTest1))

local tblTest2 = { 1, nil }
print("Test2 " .. table.getn(tblTest2))

local tblTest3 = { 1, nil, 2 }
print("Test3 " .. table.getn(tblTest3))

local tblTest4 = { 1, nil, 2, nil }
print("Test4 " .. table.getn(tblTest4))

local tblTest5 = { 1, nil, 2, nil, 3, nil }
print("Test5 " .. table.getn(tblTest5))

local tblTest6 = { 1, nil, 2, nil, 3, nil, 4, nil }
print("Test6 " .. table.getn(tblTest6))
