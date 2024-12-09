#include <linux/module.h>
#include <linux/init.h>
#include <linux/interrupt.h>
#include <linux/gpio.h>
#include <linux/irq.h>
#include <linux/kernel.h>

#define GPIO_BUTTON_0 50
#define GPIO_BUTTON_1 51
#define GPIO_LED 7

static int irq_button_0;
static int irq_button_1;

static irqreturn_t button_irq_handler(int irq, void *dev_id)
{
    static bool led_state = false;

    led_state = !led_state; // Toggle LED state
    gpio_set_value(GPIO_LED, led_state);

    printk(KERN_INFO "Button interrupt handled, LED state: %d\n", led_state);
    return IRQ_HANDLED;
}

static int __init zedboard_gpio_init(void)
{
    int ret;

    // Request GPIO for LED and Buttons
    if ((ret = gpio_request(GPIO_LED, "LED")) ||
        (ret = gpio_request(GPIO_BUTTON_0, "Button_0")) ||
        (ret = gpio_request(GPIO_BUTTON_1, "Button_1"))) {
        printk(KERN_ERR "GPIO request failed\n");
        return ret;
    }

    gpio_direction_output(GPIO_LED, 0);
    gpio_direction_input(GPIO_BUTTON_0);
    gpio_direction_input(GPIO_BUTTON_1);

    irq_button_0 = gpio_to_irq(GPIO_BUTTON_0);
    irq_button_1 = gpio_to_irq(GPIO_BUTTON_1);

    if ((ret = request_irq(irq_button_0, button_irq_handler, IRQF_TRIGGER_RISING, "button0_handler", NULL)) ||
        (ret = request_irq(irq_button_1, button_irq_handler, IRQF_TRIGGER_RISING, "button1_handler", NULL))) {
        printk(KERN_ERR "IRQ request failed\n");
        return ret;
    }

    printk(KERN_INFO "ZedBoard GPIO driver loaded\n");
    return 0;
}

static void __exit zedboard_gpio_exit(void)
{
    free_irq(irq_button_0, NULL);
    free_irq(irq_button_1, NULL);
    gpio_free(GPIO_BUTTON_0);
    gpio_free(GPIO_BUTTON_1);
    gpio_free(GPIO_LED);

    printk(KERN_INFO "ZedBoard GPIO driver unloaded\n");
}

module_init(zedboard_gpio_init);
module_exit(zedboard_gpio_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Yashwanth");
MODULE_DESCRIPTION("ZedBoard GPIO Interrupt Driver");

