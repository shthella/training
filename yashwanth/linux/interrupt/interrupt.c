#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/interrupt.h>
#include <linux/workqueue.h>
#include <linux/spinlock.h>

#define IRQ_LINE 1  // Replace with an appropriate IRQ line

static spinlock_t my_spinlock;
static unsigned long flags;
static int shared_data = 0;

// Tasklet and Workqueue structures
static struct tasklet_struct my_tasklet;
static struct workqueue_struct *my_wq;
static struct work_struct my_work;

// Tasklet function
void tasklet_handler(unsigned long data)
{
    printk(KERN_INFO "Tasklet: Executing bottom half\n");

    // Modify shared data using spinlock in tasklet
    spin_lock_irqsave(&my_spinlock, flags);
    shared_data++;
    printk(KERN_INFO "Tasklet: Shared data incremented to %d\n", shared_data);
    spin_unlock_irqrestore(&my_spinlock, flags);
}

// Workqueue function
static void workqueue_handler(struct work_struct *work)
{
    printk(KERN_INFO "Workqueue: Executing deferred work\n");

    // Modify shared data using spinlock in workqueue
    spin_lock_irqsave(&my_spinlock, flags);
    shared_data += 10;
    printk(KERN_INFO "Workqueue: Shared data incremented to %d\n", shared_data);
    spin_unlock_irqrestore(&my_spinlock, flags);
}

// Interrupt handler
static irqreturn_t my_irq_handler(int irq, void *dev_id)
{
    printk(KERN_INFO "IRQ Handler: Interrupt received\n");

    // Simulate modifying shared data in interrupt context
    spin_lock_irqsave(&my_spinlock, flags);
    shared_data++;
    printk(KERN_INFO "IRQ Handler: Shared data incremented to %d\n", shared_data);
    spin_unlock_irqrestore(&my_spinlock, flags);

    // Schedule tasklet
    tasklet_schedule(&my_tasklet);

    // Queue workqueue task
    queue_work(my_wq, &my_work);

    return IRQ_HANDLED;
}

// Module init
static int __init my_module_init(void)
{
    int ret;

    // Initialize spinlock
    spin_lock_init(&my_spinlock);

    // Initialize tasklet
    tasklet_init(&my_tasklet, tasklet_handler, 0);

    // Initialize workqueue
    my_wq = create_singlethread_workqueue("my_wq");
    INIT_WORK(&my_work, workqueue_handler);

    // Request IRQ line (replace IRQ_LINE with actual IRQ)
    ret = request_irq(IRQ_LINE, my_irq_handler, IRQF_SHARED, "my_irq_handler", (void *)(my_irq_handler));
    if (ret) {
        printk(KERN_ERR "Failed to request IRQ\n");
        return ret;
    }

    printk(KERN_INFO "Module loaded: Spinlock, Tasklet, and Workqueue example\n");
    return 0;
}

// Module exit
static void __exit my_module_exit(void)
{
    // Free the IRQ
    free_irq(IRQ_LINE, (void *)(my_irq_handler));

    // Kill the tasklet
    tasklet_kill(&my_tasklet);

    // Destroy the workqueue
    destroy_workqueue(my_wq);

    printk(KERN_INFO "Module unloaded\n");
}

module_init(my_module_init);
module_exit(my_module_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Pavani");
MODULE_DESCRIPTION("Spinlock, Tasklet, and Workqueue in Interrupt Handler Example");

