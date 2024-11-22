#include <linux/module.h>
#include <linux/init.h>
#include <linux/pci.h>

#define PCI_VENDOR_ID 0x8086
#define PCI_DEVICE_ID 0xa0ef

/* Meta Information */
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Yashwanth");
MODULE_DESCRIPTION("Driver for the Realtek Semiconductor Co., Ltd. RTS525A PCI Express Card Reader");

static struct pci_device_id pci_io_ids[] = {
	{PCI_DEVICE(PCI_VENDOR_ID, PCI_DEVICE_ID)},
	{}
};

MODULE_DEVICE_TABLE(pci, pci_io_ids);

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
	printk("pci_io - Now I am in the probe function.\n");
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
