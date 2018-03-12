obj = { x = 20 }

function obj:fun1()
    print(self.x)
end

function obj.fun2()
	print("aaa")
end

--obj.fun1()    
obj.fun1()	--冒号定义的table方法只能用：引用
obj:fun2()	--obj.fun2()，点号定义的方法可以用两种方式引用
