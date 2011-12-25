KERNELBUILD := /lib/modules/$(shell uname -r)/build

obj-m += hidefile.o

default: hidefile

hidefile:
	make -C $(KERNELBUILD) M=$(shell pwd) modules
install:
	insmod hidefile.ko
uninstall:
	rmmod hidefile.ko
clean:
	rm -rf *.ko *.o hide
	rm -rf *mod* Module*
	rm -rf .*.cmd .tmp*