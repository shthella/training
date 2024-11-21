#include <linux/module.h>
#include <linux/init.h>
#include <linux/pci.h>

#define PCI_VENDOR_ID 0x8086
#define PCI_DEVICE_ID 0x9a04
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

struct drv_data {
	void __iomem *ptr_bar0;
};

irqreturn_t my_irq_handler (int irq, void *data) {
	struct drv_data *my_data;
	printk("pci_io - Now I am in the IRQ service routine!\n");
	my_data = (struct drv_data *) data;

	if (my_data != NULL)
		iowrite8((1 | (1<<1)), my_data->ptr_bar0 + PCI_OFFSET_IRQ);
	return IRQ_HANDLED;
}

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
	struct drv_data *my_data;
	int status;

	printk("pci_io - Now I am in the probe function.\n");
	
	status = pci_resource_len(dev, 0);
	printk("pci_io - Size of BAR0 is %d bytes\n", status);
	if (status != 256) {
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

	my_data = devm_kzalloc(&dev->dev, sizeof(struct drv_data), GFP_KERNEL);
	if (my_data == NULL) {
		printk("pci_io - Error! Out of memory\n");
		return -ENOMEM;
	}

	my_data->ptr_bar0 = pcim_iomap_table(dev)[0];
	if (my_data->ptr_bar0 == NULL) {
		printk("pci_io - Error! Invalid pointer for BAR0\n");
		return -1;
	}

	printk("pci_io - GPIO state DWord: 0x%x\n", ioread32(my_data->ptr_bar0 + PCI_GPIO_STATE));
	
	/* Let's set up the interrupt */
	if (dev->irq) {
		status = devm_request_irq(&dev->dev, dev->irq, my_irq_handler, 0, KBUILD_MODNAME, my_data);
		if (status) {
			printk("pci_io - Error requesting IRq\n");
			return -1;
		}
		printk("pci_io - Requesting IRQ %d was successful\n", dev->irq);

		/* Setup PCI Device */
		iowrite8((1 | (1<<1)), my_data->ptr_bar0 + PCI_OFFSET_IRQ);
	}
	
	return 0;
}

/**
 * Function call when PCI device is unref=gistered
 *
 * @param dev    pointer to the PCI device
 **/
static void pci_remove(struct pci_dev *dev) {
	printk("pci_io - Now I am in the remove function.\n");
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
