obj-m   :=hack_open.o
EXTRA_CFLAGS := -Dsymname=sys_call_table
KDIR   := /lib/modules/$(shell uname -r)/build
PWD   := $(shell pwd)
default:
    make -C $(KDIR) SUBDIRS=$(PWD) modules
clean:
    rm -rf .*.cmd *.mod.c *.o *.ko .tmp* *.symvers