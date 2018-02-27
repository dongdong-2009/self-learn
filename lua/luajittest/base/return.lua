local function foo()
	print("before")
	do return end
	print("after")
end

foo()
