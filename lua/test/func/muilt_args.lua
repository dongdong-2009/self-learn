#!/usr/local/bin/lua

--�ɱ����
function add(...)
	local s = 0
	--{...}��ʾ�����б䳤������ɵ�table���������ʾ���ʽ
	for i,v in pairs{...}
	do
		if type(v) ~= "number" 
		then
			return "��������Ƿ�Ϊ����"
		end
		s = s + v 
	end
	return s
end

print(add(1,2,3,'4',5,6,7,8,9))