#KERNEL_DIR=/lib/modules/$(shell uname -r)/build
KERNEL_DIR=/home/echo/download/linux-3.0

obj-m = simplemodule.o

KDIR :=$(KERNEL_DIR)
PWD :=$(shell pwd)

export CROSS_COMPILE=arm-linux-gnueabi-
export ARCH=arm


all:
	$(MAKE) -C $(KDIR) SUBDIRS=$(PWD) modules

clean:
	@rm -rf *.ko
	@rm -rf *.mod.*
	@rm -rf *.cmd
	@rm -rf *.o
	@rm -rf *.ko.*
	@rm -rf modules.order
	@rm -rf Module.sysvers
	@rm -rf .tmp_versions
