# Kernel source directory
KERNEL_SOURCE := /lib/modules/$(shell uname -r)/build

# Module name
MODULE_NAME := zedboard_gpio_driver

# Build target
obj-m += $(MODULE_NAME).o

EXTRA_FLAGS += -U__STRICT_ANSI__

# Default build rule
all:
	$(MAKE) -C $(KERNEL_SOURCE) M=$(PWD) modules

# Clean rule
clean:
	$(MAKE) -C $(KERNEL_SOURCE) M=$(PWD) clean

