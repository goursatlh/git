obj-m := module_test.o force_rmmod.o killd.o rcu_test.o memory_test.o meltdown_mod.o systimer.o crash.o

CURRENT_PATH := $(shell pwd)

VERSION_NUM :=$(shell uname -r)

#LINUX_PATH :=/usr/src/linux-headers-3.13.0-34-generic
LINUX_PATH :=/usr/src/linux-headers-$(VERSION_NUM)
a = "hello"
c := $a
d := $b
all:
	make -C $(LINUX_PATH) M=$(CURRENT_PATH) modules

test:
	@echo "this is test"
	@echo $(MAKECMDGOALS)
	@echo $(b)
	@echo $(c)
	
clean:
	make -C $(LINUX_PATH) M=$(CURRENT_PATH) clean 

b = $a
a = "world"
