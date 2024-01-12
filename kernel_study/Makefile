#obj-m := module_test.o force_rmmod.o killd.o rcu_test.o memory_test.o meltdown_mod.o systimer.o crash.o deadlock_test.o atomic_opt.o softlockup_test.o timer_test.o

#obj-m := file_ops_in_irq.o
#obj-m := sockex1_kern.o
#obj-m := mem_test.o
obj-m := vir2phy_kern.o
CURRENT_PATH := $(shell pwd)

VERSION_NUM :=$(shell uname -r)

#LINUX_PATH :=/usr/src/linux-headers-3.13.0-34-generic
LINUX_PATH :=/usr/src/linux-headers-$(VERSION_NUM)
all:
	make -C $(LINUX_PATH) M=$(CURRENT_PATH) modules

test:
	@echo $(MAKECMDGOALS)
	
clean:
	make -C $(LINUX_PATH) M=$(CURRENT_PATH) clean 

