#!/usr/local/bin/lua

for l in io.lines("/etc/passwd")
do
	print(l)
end
