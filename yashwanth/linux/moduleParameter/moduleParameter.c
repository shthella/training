#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/moduleparam.h>

MODULE_LICENSE("Dual BSD/GPL");
MODULE_AUTHOR("Your Name");
MODULE_DESCRIPTION("A simple kernel module with parameters");
MODULE_VERSION("1.0");

static int my_int_param = 123;
static char *my_str_param = "default";
static bool my_bool_param = true;

module_param(my_int_param, int, S_IRUGO);      
MODULE_PARM_DESC(my_int_param, "An integer parameter\n");

module_param(my_str_param, charp, S_IRUGO);    
MODULE_PARM_DESC(my_str_param, "A string parameter\n");

module_param(my_bool_param, bool, S_IRUGO);    
MODULE_PARM_DESC(my_bool_param, "A boolean parameter\n");

static int demo_init(void)
{
    printk(KERN_INFO "Module parameters example: int=%d, str=%s, bool=%d\n", my_int_param, my_str_param, my_bool_param);
    printk(KERN_INFO "The process is \"%s\" (pid %i)\n", current->comm, current->pid);
    return 0;
}

static void demo_exit(void)
{
    printk(KERN_INFO "Goodbye, module parameters example!\n");
    printk(KERN_INFO "The process is \"%s\" (pid %i)\n", current->comm, current->pid);
}

module_init(demo_init);
module_exit(demo_exit);

