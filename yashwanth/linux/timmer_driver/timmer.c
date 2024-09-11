#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/device.h>
#include <linux/kobject.h>
#include <linux/sysfs.h>
#include <linux/delay.h>

#define DRIVER_NAME "msleep_timer_driver"
static struct kobject *msleep_kobj;
static int msleep_val = 1000;  // Default sleep time in ms

// Sysfs write function to take input and trigger msleep
static ssize_t msleep_store(struct kobject *kobj, struct kobj_attribute *attr, const char *buf, size_t count)
{
    int ret;
    // Parse input to integer (milliseconds)
    ret = kstrtoint(buf, 10, &msleep_val);
    if (ret < 0)
        return ret;

    // Sleep for the specified milliseconds
    pr_info("Sleeping for %d ms\n", msleep_val);
    msleep(msleep_val);

    return count;
}

// Sysfs show function (optional if you want to read the current msleep_val)
static ssize_t msleep_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf)
{
    return sprintf(buf, "%d\n", msleep_val);
}

// Define sysfs attribute for msleep
static struct kobj_attribute msleep_attribute = __ATTR(msleep_time, 0664, msleep_show, msleep_store);

// Create sysfs node on driver initialization
static int __init msleep_driver_init(void)
{
    int retval;

    // Create a kobject under /sys/kernel/
    msleep_kobj = kobject_create_and_add("msleep_timer", kernel_kobj);
    if (!msleep_kobj)
        return -ENOMEM;

    // Create the file associated with the sysfs attribute
    retval = sysfs_create_file(msleep_kobj, &msleep_attribute.attr);
    if (retval)
        kobject_put(msleep_kobj);

    pr_info("%s: Driver loaded, sysfs node created at /sys/kernel/msleep_timer/msleep_time\n", DRIVER_NAME);
    return retval;
}

// Clean up and remove sysfs node
static void __exit msleep_driver_exit(void)
{
    kobject_put(msleep_kobj);
    pr_info("%s: Driver unloaded\n", DRIVER_NAME);
}

module_init(msleep_driver_init);
module_exit(msleep_driver_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Pavani");
MODULE_DESCRIPTION("Timer driver with msleep() using sysfs");

