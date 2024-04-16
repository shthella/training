#include <linux/module.h>
#include <linux/fs.h>
#include <linux/cdev.h>

#define DEVICE_NAME "mychardev"

static dev_t dev_num;      
static struct cdev my_cdev;

static int mychardev_open(struct inode *inode, struct file *file)
{
    printk(KERN_INFO "mychardev: Device opened\n");
    return 0;
}

static int mychardev_release(struct inode *inode, struct file *file)
{
    printk(KERN_INFO "mychardev: Device closed\n");
    return 0;
}

static struct file_operations mychardev_fops = {
    .open = mychardev_open,
    .release = mychardev_release,
};

static int __init mychardev_init(void)
{
    int ret;


    ret = alloc_chrdev_region(&dev_num, 0, 1, DEVICE_NAME);
    if (ret < 0) {
        printk(KERN_ERR "mychardev: Failed to allocate device number\n");
        return ret;
    }


    cdev_init(&my_cdev, &mychardev_fops);
    ret = cdev_add(&my_cdev, dev_num, 1);
    if (ret < 0) {
        unregister_chrdev_region(dev_num, 1);
        printk(KERN_ERR "mychardev: Failed to add character device\n");
        return ret;
    }

    printk(KERN_INFO "mychardev: Device registered with major number %d\n", MAJOR(dev_num));
    return 0;
}

static void __exit mychardev_exit(void)
{
    cdev_del(&my_cdev);
    unregister_chrdev_region(dev_num, 1);
    printk(KERN_INFO "mychardev: Device unregistered\n");
}

module_init(mychardev_init);
module_exit(mychardev_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Your Name");
MODULE_DESCRIPTION("A simple character device driver");

