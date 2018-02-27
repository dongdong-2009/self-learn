local file = io.read()

fd = io.input(file)
local i = 1
repeat
	line = io.read()
	if line == nil then
		break
	end
	print(string.format("[%d]%s",i,line))
	i = i + 1
until (false)

io.close(fd)
