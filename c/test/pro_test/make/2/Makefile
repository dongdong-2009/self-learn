.PHONY:all clean  
.RECIPEPREFIX = >

all : b.txt c.txt
	@cat b.txt c.txt >> a.txt

e.txt : d.txt
	cp d.txt e.txt

#前置依赖和命令至少得存在一个
d.txt :
	echo "I am d" >> d.txt


#如果当前目录中有一个clean文件就不会执行make clean命令，必须申明clean为为目标
clean:
	-rm -rf *.o
