#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <linux/slab.h>
#include <linux/uaccess.h>
#include <linux/dmaengine.h>
#include <linux/dma-mapping.h>
#include <linux/sysfs.h>

static struct dma_chan *dma_chan;
static dma_addr_t src_dma_addr, dst_dma_addr;
static void *src_buf, *dst_buf;
static size_t buffer_size = 1024; // Default buffer size

// Callback for DMA completion
static void dma_complete_callback(void *completion)
{
    complete(completion);
}

// Sysfs store function for copying memory using DMA
static ssize_t dma_memcopy_store(struct device *dev, struct device_attribute *attr, const char *buf, size_t count)
{
    int ret;
    struct dma_async_tx_descriptor *tx;
    DECLARE_COMPLETION_ONSTACK(completion);

    if (count != buffer_size) {
        pr_err("Size mismatch, must be %zu bytes\n", buffer_size);
        return -EINVAL;
    }

    // Copy from userspace to src_buf
    if (copy_from_user(src_buf, buf, count)) {
        pr_err("Failed to copy data from userspace\n");
        return -EFAULT;
    }

    // Prepare DMA transfer
    tx = dmaengine_prep_dma_memcpy(dma_chan, dst_dma_addr, src_dma_addr, buffer_size, 0);
    if (!tx) {
        pr_err("Failed to prepare DMA memcpy\n");
        return -EIO;
    }

    // Set the callback and issue the DMA transaction
    tx->callback = dma_complete_callback;
    tx->callback_param = &completion;

    dmaengine_submit(tx);
    dma_async_issue_pending(dma_chan);

    // Wait for completion
    wait_for_completion(&completion);

    pr_info("DMA memcopy complete\n");

    return count;
}

// Sysfs attribute
static DEVICE_ATTR_WO(dma_memcopy);

// Driver initialization
static int __init dma_driver_init(void)
{
    int ret;
    struct class *dma_class;
    struct device *dma_device;

    // Allocate buffers
    src_buf = kmalloc(buffer_size, GFP_KERNEL);
    dst_buf = kmalloc(buffer_size, GFP_KERNEL);
    if (!src_buf || !dst_buf) {
        pr_err("Failed to allocate buffers\n");
        return -ENOMEM;
    }

    // Get DMA channel
    dma_chan = dma_request_chan(NULL, "memcpy");
    if (IS_ERR(dma_chan)) {
        pr_err("Failed to request DMA channel\n");
        ret = PTR_ERR(dma_chan);
        goto free_buffers;
    }

    // Map buffers for DMA
    src_dma_addr = dma_map_single(dma_chan->device->dev, src_buf, buffer_size, DMA_TO_DEVICE);
    dst_dma_addr = dma_map_single(dma_chan->device->dev, dst_buf, buffer_size, DMA_FROM_DEVICE);
    if (dma_mapping_error(dma_chan->device->dev, src_dma_addr) ||
        dma_mapping_error(dma_chan->device->dev, dst_dma_addr)) {
        pr_err("Failed to map DMA buffers\n");
        ret = -EIO;
        goto release_channel;
    }

    // Create class and device
    dma_class = class_create(THIS_MODULE, "dma_memcopy_class");
    if (IS_ERR(dma_class)) {
        pr_err("Failed to create class\n");
        ret = PTR_ERR(dma_class);
        goto unmap_buffers;
    }

    dma_device = device_create(dma_class, NULL, 0, NULL, "dma_memcopy_device");
    if (IS_ERR(dma_device)) {
        pr_err("Failed to create device\n");
        ret = PTR_ERR(dma_device);
        goto destroy_class;
    }

    // Create sysfs attribute
    ret = device_create_file(dma_device, &dev_attr_dma_memcopy);
    if (ret) {
        pr_err("Failed to create sysfs file\n");
        goto destroy_device;
    }

    pr_info("DMA driver loaded\n");
    return 0;

destroy_device:
    device_destroy(dma_class, 0);
destroy_class:
    class_destroy(dma_class);
unmap_buffers:
    dma_unmap_single(dma_chan->device->dev, src_dma_addr, buffer_size, DMA_TO_DEVICE);
    dma_unmap_single(dma_chan->device->dev, dst_dma_addr, buffer_size, DMA_FROM_DEVICE);
release_channel:
    dma_release_channel(dma_chan);
free_buffers:
    kfree(src_buf);
    kfree(dst_buf);
    return ret;
}

// Driver cleanup
static void __exit dma_driver_exit(void)
{
    dma_unmap_single(dma_chan->device->dev, src_dma_addr, buffer_size, DMA_TO_DEVICE);
    dma_unmap_single(dma_chan->device->dev, dst_dma_addr, buffer_size, DMA_FROM_DEVICE);
    dma_release_channel(dma_chan);
    kfree(src_buf);
    kfree(dst_buf);
    pr_info("DMA driver unloaded\n");
}

module_init(dma_driver_init);
module_exit(dma_driver_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Your Name");
MODULE_DESCRIPTION("DMA Memcopy Driver with Sysfs");

