local function init()
    return 1, "lua"
end

print((init()), 2)   -->output  1  2
print(2, (init()))   -->output  2  1
