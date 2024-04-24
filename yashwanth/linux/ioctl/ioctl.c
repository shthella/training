#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/device.h>
#include <linux/cdev.h>
#include <linux/ioctl.h>

#define DEVICE_NAME "mychardev"
#define IOCTL_SET_MSG _IOW(0, 0, char *)

static char msg[256] = "Hello from kernel!\n";
static int major_number;
static struct class *mychardev_class = NULL;
static struct cdev mychardev_cdev;

static int mychardev_open(struct inode *inode, struct file *file) {
    printk(KERN_INFO "mychardev: Device opened\n");
    return 0;
}

static int mychardev_release(struct inode *inode, struct file *file) {
    printk(KERN_INFO "mychardev: Device released\n");
    return 0;
}

static ssize_t mychardev_read(struct file *file, char __user *buf, size_t count, loff_t *offset) {
    int bytes_read = 0;
    while (count && msg[*offset] != '\0') {
        put_user(msg[*offset], buf++);
        count--;
        bytes_read++;
        (*offset)++;
    }
    return bytes_read;
}

static ssize_t mychardev_write(struct file *file, const char __user *buf, size_t count, loff_t *offset) {
    int i = 0;
    while (count && i < 255) {
        get_user(msg[i], buf++);
        count--;
        i++;
    }
    msg[i] = '\0';
    return i;
}

static long mychardev_ioctl(struct file *file, unsigned int cmd, unsigned long arg) {
    int ret = 0;
    char *ioctl_msg = NULL;

    if (_IOC_TYPE(cmd) != 0) return -ENOTTY;

    switch (cmd) {
        case IOCTL_SET_MSG:
            ioctl_msg = (char *)arg;
            ret = copy_from_user(msg, ioctl_msg, sizeof(msg));
            if (ret != 0) return -EFAULT;
            break;
        default:
            return -ENOTTY;
    }
    return ret;
}

static const struct file_operations mychardev_fops = {
    .owner = THIS_MODULE,
    .open = mychardev_open,
    .release = mychardev_release,
    .read = mychardev_read,
    .write = mychardev_write,
    .unlocked_ioctl = mychardev_ioctl,
};

static int __init mychardev_init(void) {
    printk(KERN_INFO "mychardev: Initializing character device driver\n");

    if (alloc_chrdev_region(&major_number, 0, 1, DEVICE_NAME) < 0) {
        printk(KERN_ALERT "mychardev: Failed to allocate major number\n");
        return -1;
    }

    cdev_init(&mychardev_cdev, &mychardev_fops);
    if (cdev_add(&mychardev_cdev, major_number, 1) < 0) {
        printk(KERN_ALERT "mychardev: Failed to add device to system\n");
        unregister_chrdev_region(major_number, 1);
        return -1;
    }

    mychardev_class = class_create(THIS_MODULE, DEVICE_NAME);
    if (IS_ERR(mychardev_class)) {
        printk(KERN_ALERT "mychardev: Failed to register device class\n");
        cdev_del(&mychardev_cdev);
        unregister_chrdev_region(major_number, 1);
        return -1;
    }

    if (device_create(mychardev_class, NULL, major_number, NULL, DEVICE_NAME) == NULL) {
        printk(KERN_ALERT "mychardev: Failed to create device\n");
        class_destroy(mychardev_class);
        cdev_del(&mychardev_cdev);
        unregister_chrdev_region(major_number, 1);
        return -1;
    }

    printk(KERN_INFO "mychardev: Character device driver initialized\n");
    return 0;
}

static void __exit mychardev_exit(void) {
    device_destroy(mychardev_class, major_number);
    class_unregister(mychardev_class);
    class_destroy(mychardev_class);
    cdev_del(&mychardev_cdev);
    unregister_chrdev_region(major_number, 1);
    printk(KERN_INFO "mychardev: Exiting character device driver\n");
}

module_init(mychardev_init);
module_exit(mychardev_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Your Name");
MODULE_DESCRIPTION("A simple character device driver with IOCTL and sysfs attributes");

