#include <linux/module.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>
#include <linux/device.h>
#include <linux/sysfs.h>
#include <linux/kobject.h>

#define DEVICE_NAME "mychardev"
#define CLASS_NAME "mycharclass"
#define IOCTL_MAGIC 'k'
#define IOCTL_HELLO _IOR(IOCTL_MAGIC, 1, char*)

static int majorNumber;
static struct class* myCharClass = NULL;
static struct device* myCharDevice = NULL;
static struct kobject *kobj_ref;

// Buffer for device operations
static char message[256] = {0};
static short size_of_message;

// File operation prototypes
static int dev_open(struct inode *, struct file *);
static int dev_release(struct inode *, struct file *);
static ssize_t dev_read(struct file *, char *, size_t, loff_t *);
static ssize_t dev_write(struct file *, const char *, size_t, loff_t *);
static long dev_ioctl(struct file *file, unsigned int cmd, unsigned long arg);
static ssize_t sysfs_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf);
static ssize_t sysfs_store(struct kobject *kobj, struct kobj_attribute *attr, const char *buf, size_t count);

// File operations structure
static struct file_operations fops = {
    .open = dev_open,
    .read = dev_read,
    .write = dev_write,
    .release = dev_release,
    .unlocked_ioctl = dev_ioctl,
};

// Sysfs attributes
struct kobj_attribute mychar_attr = __ATTR(mychar_attr, 0660, sysfs_show, sysfs_store);

static int __init mychar_init(void) {
    printk(KERN_INFO "Initializing the MyCharDevice\n");

    // Allocate a major number
    majorNumber = register_chrdev(0, DEVICE_NAME, &fops);
    if (majorNumber < 0) {
        printk(KERN_ALERT "MyCharDevice failed to register a major number\n");
        return majorNumber;
    }
    printk(KERN_INFO "MyCharDevice: registered correctly with major number %d\n", majorNumber);

    // Register the device class
    myCharClass = class_create(THIS_MODULE, CLASS_NAME);
    if (IS_ERR(myCharClass)) {
        unregister_chrdev(majorNumber, DEVICE_NAME);
        printk(KERN_ALERT "Failed to register device class\n");
        return PTR_ERR(myCharClass);
    }
    printk(KERN_INFO "MyCharDevice: device class registered correctly\n");

    // Register the device driver
    myCharDevice = device_create(myCharClass, NULL, MKDEV(majorNumber, 0), NULL, DEVICE_NAME);
    if (IS_ERR(myCharDevice)) {
        class_destroy(myCharClass);
        unregister_chrdev(majorNumber, DEVICE_NAME);
        printk(KERN_ALERT "Failed to create the device\n");
        return PTR_ERR(myCharDevice);
    }
    printk(KERN_INFO "MyCharDevice: device class created correctly\n");

    // Create a sysfs entry
    kobj_ref = kobject_create_and_add("mychar_kobject", kernel_kobj);
    if (sysfs_create_file(kobj_ref, &mychar_attr.attr)) {
        printk(KERN_ALERT "Failed to create sysfs entry\n");
        kobject_put(kobj_ref);
        device_destroy(myCharClass, MKDEV(majorNumber, 0));
        class_destroy(myCharClass);
        unregister_chrdev(majorNumber, DEVICE_NAME);
        return -ENOMEM;
    }

    return 0;
}

static void __exit mychar_exit(void) {
    kobject_put(kobj_ref);
    sysfs_remove_file(kernel_kobj, &mychar_attr.attr);
    device_destroy(myCharClass, MKDEV(majorNumber, 0));
    class_unregister(myCharClass);
    class_destroy(myCharClass);
    unregister_chrdev(majorNumber, DEVICE_NAME);
    printk(KERN_INFO "MyCharDevice: Goodbye from the user!\n");
}

static int dev_open(struct inode *inodep, struct file *filep) {
    printk(KERN_INFO "MyCharDevice: Device has been opened\n");
    return 0;
}

static ssize_t dev_read(struct file *filep, char *buffer, size_t len, loff_t *offset) {
    int error_count = 0;
    error_count = copy_to_user(buffer, message, size_of_message);

    if (error_count == 0) {
        printk(KERN_INFO "MyCharDevice: Sent %d characters to the user\n", size_of_message);
        return (size_of_message = 0);
    } else {
        printk(KERN_INFO "MyCharDevice: Failed to send %d characters to the user\n", error_count);
        return -EFAULT;
    }
}

static ssize_t dev_write(struct file *filep, const char *buffer, size_t len, loff_t *offset) {
    copy_from_user(message, buffer, len);
    size_of_message = strlen(message);
    printk(KERN_INFO "MyCharDevice: Received %zu characters from the user\n", len);
    return len;
}

static long dev_ioctl(struct file *file, unsigned int cmd, unsigned long arg) {
    switch(cmd) {
        case IOCTL_HELLO:
            printk(KERN_INFO "MyCharDevice: IOCTL_HELLO received\n");
            copy_to_user((char *)arg, "Hello from kernel!", 19);
            break;
        default:
            return -EINVAL;
    }
    return 0;
}

static int dev_release(struct inode *inodep, struct file *filep) {
    printk(KERN_INFO "MyCharDevice: Device successfully closed\n");
    return 0;
}

// Sysfs show function
static ssize_t sysfs_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf) {
    return sprintf(buf, "%s\n", message);
}

// Sysfs store function
static ssize_t sysfs_store(struct kobject *kobj, struct kobj_attribute *attr, const char *buf, size_t count) {
    sprintf(message, "%s", buf);
    size_of_message = strlen(message);
    return count;
}

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Your Name");
MODULE_DESCRIPTION("A simple Linux char driver with IOCTL and sysfs");
MODULE_VERSION("0.1");

module_init(mychar_init);
module_exit(mychar_exit);

