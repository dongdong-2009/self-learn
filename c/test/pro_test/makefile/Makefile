m := I am $(mkk)
mkk := mukunkong

objects = a.o
objects += b.o

ifeq ($(mkk),mukunkong)
	a = a
else
	a = b
endif

ifeq '$(a)' 'a'
	r = yes
else
	r = no
endif

fun = $(1) + $(2)
foo = $(call fun,1,2)

pwd := $(shell pwd)

.PHONY:all   print

all:
	@echo $(m)	#@将会导致echo本身命令不会输出,:=会使前面的变量引用不能使用后面定义的变量
	@echo $(objects)
	@echo CFLAGS=$(CFLAGS)
	@echo a = $(a)
	@echo result = $(r)
	@echo foo = $(foo)
	@echo $(origin <)
	@echo $(pwd)

print:
	@echo "I am here printing..."








