##############################################################
#					   测试函数
##############################################################

.PHONY : all clean 1 2 3 4 5 

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
	

clean:
	-rm -rf *.o
