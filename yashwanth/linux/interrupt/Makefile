obj-m += interrupt.o

KERNELDIR ?= /lib/modules/$(shell uname -r)/build
PWD := $(shell pwd)

all: module user_program

module:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules

user_program: user_program.c
	gcc -o user_program user_program.c

clean:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) clean
	rm -f user_program
