include Make.def
include .config

.PHONY:all clean

ifeq (${version},1.0)
VERSION = 1.0
else
VERSION = other
endif

all:
	@echo ${VERSION}
	@echo ${def}
