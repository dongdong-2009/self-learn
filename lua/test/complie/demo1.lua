#!/usr/local/bin/lua

i = 32
local i = 0

-- loadstring�ڱ���ʱ���漰�ʷ����ȫ�ֻ����б��������ַ���
f = loadstring("i=i+1;print(i)")

-- �հ����������ֲ�����
g = function()
	i = i + 1
	print(i)
end
f()	-- 33
g() -- 1