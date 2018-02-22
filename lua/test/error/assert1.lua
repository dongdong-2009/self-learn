local file,msg

repeat
	print("Enter a file name:")
	local name = io.read()
	if not name then
		return
	end
	file,msg = io.open(name,"r")
	if file then
		print(msg,"--->",file)
	end
until file
