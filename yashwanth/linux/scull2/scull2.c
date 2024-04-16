#include <linux/module.h>
#include <linux/fs.h>
#include <linux/cdev.h>

#define DEVICE_NAME "sculldev"
#define SCULL_NR_DEVS 4  

static int SCULL_MAJOR = 0;
#define SCULL_MINOR 0

struct scull_dev {
    struct cdev cdev;
   
};

static struct scull_dev *scull_devices;

static int scull_open(struct inode *inode, struct file *filp)
{
    struct scull_dev *dev = container_of(inode->i_cdev, struct scull_dev, cdev);
    filp->private_data = dev; 
    printk(KERN_INFO "sculldev: Device opened\n");
    return 0;
}

static int scull_release(struct inode *inode, struct file *filp)
{
    printk(KERN_INFO "sculldev: Device closed\n");
    return 0;
}

static struct file_operations scull_fops = {
    .owner = THIS_MODULE,
    .open = scull_open,
    .release = scull_release,
    
};

static int __init scull_init(void)
{
    int result;
    dev_t dev = MKDEV(SCULL_MAJOR, SCULL_MINOR);

    if (SCULL_MAJOR) {
        result = register_chrdev_region(dev, SCULL_NR_DEVS, DEVICE_NAME);
    } else {
        result = alloc_chrdev_region(&dev, SCULL_MINOR, SCULL_NR_DEVS, DEVICE_NAME);
        SCULL_MAJOR = MAJOR(dev);
    }
    if (result < 0) {
        printk(KERN_WARNING "sculldev: can't get major %d\n", SCULL_MAJOR);
        return result;
    }

    scull_devices = kmalloc(SCULL_NR_DEVS * sizeof(struct scull_dev), GFP_KERNEL);
    if (!scull_devices) {
        unregister_chrdev_region(dev, SCULL_NR_DEVS);
        return -ENOMEM;
    }

    
    for (int i = 0; i < SCULL_NR_DEVS; i++) {
        
    }

    printk(KERN_INFO "sculldev: Device registered with major number %d\n", SCULL_MAJOR);
    return 0;
}

static void __exit scull_exit(void)
{
    dev_t dev = MKDEV(SCULL_MAJOR, SCULL_MINOR);

    
    for (int i = 0; i < SCULL_NR_DEVS; i++) {
        
    }

    kfree(scull_devices);
    unregister_chrdev_region(dev, SCULL_NR_DEVS);
    printk(KERN_INFO "sculldev: Device unregistered\n");
}

module_init(scull_init);
module_exit(scull_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Your Name");
MODULE_DESCRIPTION("A simple character device driver using SCULL");

