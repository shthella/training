#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>

#define DEVICE_NAME "my_char_dev"
#define BUF_LEN 80

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Your Name");

static int major_num;
static char msg[BUF_LEN];
static char *msg_ptr;

static int device_open(struct inode *inode, struct file *file) {
    printk(KERN_INFO "Device opened\n");
    return 0;
}

static int device_release(struct inode *inode, struct file *file) {
    printk(KERN_INFO "Device released\n");
    return 0;
}

static ssize_t device_read(struct file *filp, char *buffer, size_t length, loff_t *offset) {
    int bytes_read = 0;
    if (*msg_ptr == 0)
        return 0;
    while (length && *msg_ptr) {
        put_user(*(msg_ptr++), buffer++);
        length--;
        bytes_read++;
    }
    return bytes_read;
}

static ssize_t device_write(struct file *filp, const char *buffer, size_t length, loff_t *offset) {
    int bytes_written = 0;
    while (length && (bytes_written < BUF_LEN)) {
        get_user(msg[bytes_written], buffer++);
        bytes_written++;
        length--;
    }
    msg_ptr = msg;
    return bytes_written;
}

static struct file_operations fops = {
    .open = device_open,
    .read = device_read,
    .write = device_write,
    .release = device_release,
};

static int __init char_dev_init(void) {
    major_num = register_chrdev(0, DEVICE_NAME, &fops);
    if (major_num < 0) {
        printk(KERN_ALERT "Failed to register a major number\n");
        return major_num;
    }
    printk(KERN_INFO "Registered correctly with major number %d\n", major_num);
    return 0;
}

static void __exit char_dev_exit(void) {
    unregister_chrdev(major_num, DEVICE_NAME);
    printk(KERN_INFO "Unregistered the device\n");
}

module_init(char_dev_init);
module_exit(char_dev_exit);

