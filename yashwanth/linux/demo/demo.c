#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/string.h>

#define BUFFER_SIZE 1024

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Your Name");

static char message_buffer[BUFFER_SIZE] = {0};
static int message_size = 0;
static char empty_buffer[] = "Buffer is empty";

static int dev_open(struct inode *inodep, struct file *filep) {
    printk(KERN_INFO "Device opened\n");
    return 0;
}

static int dev_release(struct inode *inodep, struct file *filep) {
    printk(KERN_INFO "Device closed\n");
    return 0;
}

static ssize_t dev_read(struct file *filep, char *buffer, size_t len, loff_t *offset) {
    int bytes_read = 0;
    
    if (message_size == 0) {
        printk(KERN_INFO "Buffer is empty. No data to read\n");
        return 0;
    }

    if (len > message_size)
        len = message_size;

    if (copy_to_user(buffer, message_buffer, len)) {
        return -EFAULT;
    } else {
        bytes_read = len;
        memset(message_buffer, 0, BUFFER_SIZE);  // Clear the buffer
        message_size = 0;
        printk(KERN_INFO "Read %d bytes from device\n", bytes_read);
    }
    
    return bytes_read;
}


static ssize_t dev_write(struct file *filep, const char *buffer, size_t len, loff_t *offset) {
    if (len > BUFFER_SIZE - message_size)
        return -ENOMEM;

    if (copy_from_user(message_buffer + message_size, buffer, len)) {
        return -EFAULT;
    } else {
        message_size += len;
        printk(KERN_INFO "Wrote %zu bytes to device\n", len);
    }
    
    return len;
}

static struct file_operations fops = {
    .open = dev_open,
    .read = dev_read,
    .write = dev_write,
    .release = dev_release,
};

static int __init file_op_init(void) {
    printk(KERN_INFO "File operation module loaded\n");
    register_chrdev(0, "file_op", &fops);
    return 0;
}

static void __exit file_op_exit(void) {
    unregister_chrdev(0, "file_op");
    printk(KERN_INFO "File operation module unloaded\n");
}

module_init(file_op_init);
module_exit(file_op_exit);

