local fd = io.open("txt")

print(io.type(fd))
if fd then
	print("open file ok\n")
	fd:close()
	print(io.type(fd))
else
	print("open file failed")
end
