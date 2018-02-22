-- meta class
Rectangle = {area = 0,length = 0,braedth = 0}

--派生的方法
function Rectangle:new(o,length,breadth)
	o = o or {}
	setmetatable(o,self)
	self.__index = self
	self.length = length or 0
	self.breadth = breadth or 0
	self.area = length*breadth
	return o
end

--派生的方法printArea
function Rectangle:printArea()
	print("矩形的面积为 : ",self.area)
end

r = Rectangle:new(nil,10,20)

r:printArea()

