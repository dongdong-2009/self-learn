--����һ���࣬��ʾ�ı���
local RectAngle = { length, width, area} --�������������Ա����

function RectAngle: new (len,wid) --�����½�ʵ����New����
	local o = {
	--�趨�������ֵ
	 length = len or 0,
	 width = wid or 0,
	 area =len*wid
	}
	setmetatable(o,{__index = self} )--������ı�ӳ�䵽��new�����ı���
	return o
end
    
function RectAngle:getInfo()--��ȡ������Ϣ�ķ���
    return self.length,self.width,self.area
end
a = RectAngle:new(10,20)
print(a:getInfo())      -- �����10    20    200
b = RectAngle:new(10,10)
print(b:getInfo())      -- �����10    10    100
print(a:getInfo())      -- �����10    20    200