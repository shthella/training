obj-m += helloWorld.o

CROSS_COMPILE = x86_64-linux-gnu-

CC := $(CROSS_COMPILE)gcc-12

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) ARCH=x86_64 CROSS_COMPILE=$(CROSS_COMPILE) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean

