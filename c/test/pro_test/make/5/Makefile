##############################################################
#					   测试函数
##############################################################

.PHONY : all clean 1 2 3 4 5 6 7 8 9

root = $(shell ls /)
1:
	@echo "\33[32m${root}\33[0m" 

wild = $(wildcard /home/fyql/new_sdk/sydk/kernel/*.mk)
2:
	@echo "\33[32m${wild}\33[0m" 

sub = $(subst ee,EE,feet on the street)
3:
	@echo "\33[32m${sub}\33[0m" 

OUT = a.js b.js
OUT_1 = $(OUT:.js=.min.js)
4:
	@echo "${OUT_1}"

list = $(foreach var,a b c,${var}.o)
5:
	@echo "=============5==============="
	@echo ${list}

#函数定义
#echo "\33[32m${0}\n\33[0m"
#echo "^------------>$^\n"
#echo "<------------>$<\n"
#echo "@------------>$@\n"
#echo "[1]------------>$1\n\n\n"
#ls $1
#echo "0------------>$0\n"
#echo "1------------>$(1)\n"
#echo "2------------>$(2)\n"
define target
	ls $(1)
	echo "\33[42m===============\n\33[0m"
endef
define resever
	${2} ${1}
endef
6:
	@$(call target,${dir})
	@echo $(call resever,hello,world)
#@$(call target,/ /etc /usr)
#@$(call target,1,2,3)
#@$(call target,main)

res = $(origin cmd)
res1 = $(origin res)
res2 = $(origin CC)
res3 = $(origin bletch)
7:
	@echo "cli------------->${res}\n"
	@echo "file------------>${res1}\n"
	@echo "CC------------->${res2}\n"
	@echo "bletch------------->${res3}\n"


clean:
	echo "\33[0m"
	-rm -rf *.o

