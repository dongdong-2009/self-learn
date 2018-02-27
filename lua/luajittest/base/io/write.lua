local file = "txt"
local fd = io.open(file,"a+")

io.output(fd)					
io.write("\n--------------------\n") --fd:write("\noooooooooooooooooooo\n")

io.close(fd)	--fd:close()
