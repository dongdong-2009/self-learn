functor = {}
function func1(self, arg)
  print ("called from", arg)
end

setmetatable(functor, {__call = func1})

functor("functor11")  --> called from functor11
print(functor)      --> output：0x00076fc8 （后面这串数字可能不一样）
