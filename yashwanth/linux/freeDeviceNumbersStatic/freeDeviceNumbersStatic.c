#include <linux/module.h>
#include <linux/fs.h>
#include <linux/cdev.h>


#define DEVICE_NAME "mychardev"
#define MAJOR_NUMBER 250  

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


    ret = register_chrdev(MAJOR_NUMBER, DEVICE_NAME, &mychardev_fops);
    if (ret < 0) {
        printk(KERN_ERR "mychardev: Failed to register device: %d\n", ret);
        return ret;
    }
    printk(KERN_INFO "mychardev: Device registered with major number %d\n", MAJOR_NUMBER);
    return 0;
}

static void __exit mychardev_exit(void)
{
    
    unregister_chrdev(MAJOR_NUMBER, DEVICE_NAME);
    printk(KERN_INFO "mychardev: Device unregistered\n");
}

module_init(mychardev_init);
module_exit(mychardev_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Your Name");
MODULE_DESCRIPTION("A simple character device driver");

