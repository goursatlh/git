obj-m := module_test.o force_rmmod.o killd.o

CURRENT_PATH := $(shell pwd)

VERSION_NUM :=$(shell uname -r)

LINUX_PATH :=/usr/src/linux-headers-3.16.0-30-generic
all:
	make -C $(LINUX_PATH) M=$(CURRENT_PATH) modules
clean:
	make -C $(LINUX_PATH) M=$(CURRENT_PATH) clean 
