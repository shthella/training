#include <linux/module.h>
#include <linux/fs.h>
#include <linux/cdev.h>


#define DEVICE_NAME "sculldev"

static dev_t dev_num;      
static struct cdev scull_cdev;

static int scull_open(struct inode *inode, struct file *file)
{
    printk(KERN_INFO "sculldev: Device opened\n");
    return 0;
}

static int scull_release(struct inode *inode, struct file *file)
{
    printk(KERN_INFO "sculldev: Device closed\n");
    return 0;
}

static struct file_operations scull_fops = {
    .open = scull_open,
    .release = scull_release,
};

static int __init scull_init(void)
{
    int ret;
    ret = alloc_chrdev_region(&dev_num, 0, 1, DEVICE_NAME);
    if (ret < 0) {
        printk(KERN_ERR "sculldev: Failed to allocate device number\n");
        return ret;
    }

    cdev_init(&scull_cdev, &scull_fops);
    ret = cdev_add(&scull_cdev, dev_num, 1);
    if (ret < 0) {
        unregister_chrdev_region(dev_num, 1);
        printk(KERN_ERR "sculldev: Failed to add character device\n");
        return ret;
    }

    printk(KERN_INFO "sculldev: Device registered with major number %d\n", MAJOR(dev_num));
    return 0;
}

static void __exit scull_exit(void)
{
    cdev_del(&scull_cdev);
    unregister_chrdev_region(dev_num, 1);
    printk(KERN_INFO "sculldev: Device unregistered\n");
}

module_init(scull_init);
module_exit(scull_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Your Name");
MODULE_DESCRIPTION("A simple character device driver using SCULL");

