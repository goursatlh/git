obj-m := module_test.o force_rmmod.o killd.o rcu_test.o memory_test.o meltdown_mod.o

CURRENT_PATH := $(shell pwd)

VERSION_NUM :=$(shell uname -r)

#LINUX_PATH :=/usr/src/linux-headers-3.13.0-34-generic
LINUX_PATH :=/usr/src/linux-headers-$(VERSION_NUM)
all:
	make -C $(LINUX_PATH) M=$(CURRENT_PATH) modules
clean:
	make -C $(LINUX_PATH) M=$(CURRENT_PATH) clean 
