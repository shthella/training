# GPIO chip comptiable Linux PCI(e) Driver
obj-m += pci_gpio.o

CROSS_COMPILE = x86_64-linux-gnu-

CC := $(CROSS_COMPILE)gcc-13

EXTRA_CFLAGS += -U__STRICT_ANSI__

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) ARCH=x86_64 CROSS_COMPILE=$(CROSS_COMPILE) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean

