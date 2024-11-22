#include <linux/module.h>
#include <linux/init.h>
#include <linux/pci.h>
#include <linux/gpio/driver.h>

#define PCI_VENDOR_ID 0x8086
#define PCI_DEVICE_ID 0xA0EF
#define PCI_GPIO_STATE 0xFC
#define PCI_DIRECTION 0xF8
#define PCI_OFFSET_IRQ 0xF9

/* Meta Information */
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Yashwanth");
MODULE_DESCRIPTION("Driver for the Realtek Semiconductor Co., Ltd. RTS525A PCI Express Card Reader");

static struct pci_device_id pci_io_ids[] = {
	{PCI_DEVICE(PCI_VENDOR_ID, PCI_DEVICE_ID)},
	{}
};

MODULE_DEVICE_TABLE(pci, pci_io_ids);

struct pci_io_gpiochip {
	void __iomem *ptr_bar0;
	struct gpio_chip chip;
};

static void pci_set_multiple(struct gpio_chip *chip, unsigned long *mask, unsigned long *bits) {
	u32 val;
	struct pci_io_gpiochip *gpio = gpiochip_get_data(chip);

	val = ioread32(gpio->ptr_bar0 + PCI_GPIO_STATE);
	val = (val & *mask) | *bits;
	iowrite32(val, gpio->ptr_bar0 + PCI_GPIO_STATE);
}

static void pci_set(struct gpio_chip *chip, unsigned int gpio_nr, int value) {
	unsigned long mask = 0;
        unsigned long bits = 0;
	mask = (1 << gpio_nr);
	bits = (value << gpio_nr);
	pci_set_multiple(chip, &mask, &bits);
}

static int pci_get_multiple(struct gpio_chip * chip, unsigned long *mask, unsigned long *bits) {
	u32 val;
	struct pci_io_gpiochip *gpio = gpiochip_get_data(chip);

	val = ioread32(gpio->ptr_bar0 + PCI_GPIO_STATE);
	*bits = (val & *mask);
	return 0;
}

static int pci_get(struct gpio_chip *chip, unsigned int gpio_nr) {
	unsigned long mask = 0;
        unsigned long bits = 0;
	
	mask = (1 < gpio_nr);
	pci_get_multiple(chip, &mask, &bits);
	return (bits > 0) ? 1 : 0;
}

static int pci_get_direction(struct gpio_chip *chip, unsigned int gpio_nr) {
	u8 val;
	struct pci_io_gpiochip *gpio = gpiochip_get_data(chip);

	val = ioread8(gpio->ptr_bar0 + PCI_DIRECTION);
	val = val & (1<<(gpio_nr/8));
	return (val > 0) ? GPIO_LINE_DIRECTION_OUT : GPIO_LINE_DIRECTION_IN;
}

static int pci_set_direction_input(struct gpio_chip *chip, unsigned int gpio_nr) {
	u8 val;
	struct pci_io_gpiochip *gpio = gpiochip_get_data(chip);

	val = ioread8(gpio->ptr_bar0 + PCI_DIRECTION);
	val &= ~(1 << (gpio_nr/8));
	iowrite8(val, gpio->ptr_bar0 + PCI_DIRECTION);
	return 0;
}

static int pci_set_direction_output(struct gpio_chip *chip, unsigned int gpio_nr, int value) {
	u8 val;
	struct pci_io_gpiochip *gpio = gpiochip_get_data(chip);

	val = ioread8(gpio->ptr_bar0 + PCI_DIRECTION);
	val |= (1 << (gpio_nr/8));
	iowrite8(val, gpio->ptr_bar0 + PCI_DIRECTION);
	pci_set(chip, gpio_nr, val);
	return 0;
}

static const struct gpio_chip template_chip = {
	.label = "pci_io",
	.owner = THIS_MODULE,
	/* Function to control GPIOs */
	.get_direction = pci_get_direction,
	.direction_input = pci_set_direction_input,
	.direction_output = pci_set_direction_output,
	.set = pci_set,
	.set_multiple = pci_set_multiple,
	.get = pci_get,
	.get_multiple = pci_get_multiple,
	.base = -1,
	.ngpio = 32,
	.can_sleep = true,
};

/**
 * Function call when a PCI device is registered
 *
 * @param dev   pointer to the PCI device
 * @param id    pointer to the correspondig id table's entry
 *
 * @return      0 on success
 * 		negative error code on failure
 **/
static int pci_probe(struct pci_dev *dev, const struct pci_device_id *id) {
	struct pci_io_gpiochip *my_data;
	int status;

	printk("pci_io - Now I am in the probe function.\n");
	
	status = pci_resource_len(dev, 0);
	printk("pci_io - Size of BAR0 is %d bytes\n", status);
	if (status != 16384) {
		printk("pci_io - Error! BAR0 has wrong size!\n");
		return -1;
	}

	printk("pci_io - BAR0 is mapped to ox%llx\n", pci_resource_start(dev, 0));

	status = pcim_enable_device(dev);
	if (status < 0) {
		printk("pci_io - Error! Could not enable the device\n");
		return status;
	}

	status = pcim_iomap_regions(dev, BIT(0), KBUILD_MODNAME);
	if (status < 0) {
		printk("pci_io - Error! BAR0 is already in use!\n");
		return status;
	}

	my_data = devm_kzalloc(&dev->dev, sizeof(struct pci_io_gpiochip), GFP_KERNEL);
	if (my_data == NULL) {
		printk("pci_io - Error! Out of memory\n");
		return -ENOMEM;
	}

	my_data->ptr_bar0 = pcim_iomap_table(dev)[0];
	if (my_data->ptr_bar0 == NULL) {
		printk("pci_io - Error! Invalid pointer for BAR0\n");
		return -1;
	}
	
	my_data->chip = template_chip;
	if (gpiochip_add_data(&my_data->chip, my_data) < 0) {
		printk("pci_io - Error! Can't add GPIO Chip\n");
		return -1;
	}
	
	pci_set_drvdata(dev, my_data);

	return 0;
}

/**
 * Function call when PCI device is unref=gistered
 *
 * @param dev    pointer to the PCI device
 **/
static void pci_remove(struct pci_dev *dev) {
	struct pci_io_gpiochip * my_data = pci_get_drvdata(dev);
	printk("pci_io - Now I am in the remove function.\n");
	if (my_data)
		gpiochip_remove(&my_data->chip);
}

/* PCI driver struct */
static struct pci_driver pci_io_driver = {
	.name ="pci_driver",
	.id_table = pci_io_ids,
	.probe = pci_probe,
	.remove = pci_remove,
};

/**
 * Function call when the module is loaded into the kernel
 **/
static int my_init(void) {
	printk("pci_io - Registering the PCI Device.\n");
	return pci_register_driver(&pci_io_driver);
}

/** 
 * Function call when the module is removed into from the kernel
 **/
static void my_exit(void) {
	printk("pci_io - Unregistering the PCI Device.\n");
	return pci_unregister_driver(&pci_io_driver);
}

module_init(my_init);
module_exit(my_exit);
