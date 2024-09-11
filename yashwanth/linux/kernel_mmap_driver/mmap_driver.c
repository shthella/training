#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/mm.h>
#include <linux/io.h>
#include <linux/slab.h>
#include <linux/device.h>

#define DEVICE_NAME "mmap_driver"
#define CLASS_NAME "mmap_class"
#define MEM_SIZE 4096 // 4KB

static int major;
static struct class *mmap_class = NULL;
static struct device *mmap_device = NULL;
static unsigned long phys_addr = 0x0; // Placeholder for the physical address

// Open file operation
static int mmap_driver_open(struct inode *inodep, struct file *filep) {
    printk(KERN_INFO "mmap_driver: Device opened\n");
    return 0;
}

// Close file operation
static int mmap_driver_release(struct inode *inodep, struct file *filep) {
    printk(KERN_INFO "mmap_driver: Device closed\n");
    return 0;
}

// Memory mapping operation
static int mmap_driver_mmap(struct file *filep, struct vm_area_struct *vma) {
    unsigned long offset = vma->vm_pgoff << PAGE_SHIFT;
    unsigned long size = vma->vm_end - vma->vm_start;
    
    if (remap_pfn_range(vma, vma->vm_start, offset >> PAGE_SHIFT, size, vma->vm_page_prot)) {
        printk(KERN_ERR "mmap_driver: Failed to remap region\n");
        return -EIO;
    }
    
    printk(KERN_INFO "mmap_driver: Memory mapped\n");
    return 0;
}

static struct file_operations fops = {
    .open = mmap_driver_open,
    .release = mmap_driver_release,
    .mmap = mmap_driver_mmap,
};

// Driver initialization
static int __init mmap_driver_init(void) {
    // Register device number
    major = register_chrdev(0, DEVICE_NAME, &fops);
    if (major < 0) {
        printk(KERN_ERR "mmap_driver: Failed to register a major number\n");
        return major;
    }

    // Register device class
    mmap_class = class_create(THIS_MODULE, CLASS_NAME);
    if (IS_ERR(mmap_class)) {
        unregister_chrdev(major, DEVICE_NAME);
        printk(KERN_ERR "mmap_driver: Failed to register device class\n");
        return PTR_ERR(mmap_class);
    }

    // Register device driver
    mmap_device = device_create(mmap_class, NULL, MKDEV(major, 0), NULL, DEVICE_NAME);
    if (IS_ERR(mmap_device)) {
        class_destroy(mmap_class);
        unregister_chrdev(major, DEVICE_NAME);
        printk(KERN_ERR "mmap_driver: Failed to create the device\n");
        return PTR_ERR(mmap_device);
    }

    printk(KERN_INFO "mmap_driver: Device created successfully\n");
    return 0;
}

// Driver exit
static void __exit mmap_driver_exit(void) {
    device_destroy(mmap_class, MKDEV(major, 0));
    class_unregister(mmap_class);
    class_destroy(mmap_class);
    unregister_chrdev(major, DEVICE_NAME);
    printk(KERN_INFO "mmap_driver: Device unregistered\n");
}

module_init(mmap_driver_init);
module_exit(mmap_driver_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Memory Map Driver");
MODULE_AUTHOR("Pavani");

