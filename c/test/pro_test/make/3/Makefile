####################################################################
# 1、命令中的每条shell命令都在单独的shell中执行
# 2、在命令中定义的变量属于shell变量
# 3、要使他们相互关联可以使用[;|\]
# 4、$使用的是makefile中的变量
# 5、$$使用的是shell中的变量
####################################################################

.PHONY : 1 2 3 4 5 6 7
.ONESHELL:		#加上该命令可以使命令中不同命令shell进程关联


1 :
	@export foo=bar;echo "foo=[$$foo]"

2 : 
	@export foo=bar;\
	echo "foo=[$$foo]"

3 :
	@export foo=bar
	@echo "foo=[$$foo]"		#$$使用的是shell中的变量

h = hello		#此处定义的是makefile变量,可以在命令中{$h}直接使用
4 :
	@foo=bar	#该命令属于自己的shell进程中，别的命令中不能使用
	@echo "foo=[${foo}]"
	@echo "h=[${h}]"

5 : 
	#这是测试		#不加@注释也会打印

a = aa
a = aaa
b = bb
b := bbb
c =			#代表c不为空 
c ?= ccc 
d = dd
d += ddd
6 :
	@echo "[a = ${a}]"
	@echo "[b = ${b}]"
	@echo "[c = ${c}]"
	@echo "[d = ${d}]"

LIST = one two three
7 : 
	@for i in $(LIST);do\
		echo $$i;\
	done

a b : 
	touch $@





