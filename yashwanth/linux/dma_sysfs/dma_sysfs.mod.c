#include <linux/build-salt.h>
#include <linux/module.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

BUILD_SALT;

MODULE_INFO(vermagic, VERMAGIC_STRING);
MODULE_INFO(name, KBUILD_MODNAME);

__visible struct module __this_module
__section(.gnu.linkonce.this_module) = {
	.name = KBUILD_MODNAME,
	.init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
	.exit = cleanup_module,
#endif
	.arch = MODULE_ARCH_INIT,
};

#ifdef CONFIG_RETPOLINE
MODULE_INFO(retpoline, "Y");
#endif

static const struct modversion_info ____versions[]
__used __section(__versions) = {
	{ 0x6e5e9ea5, "module_layout" },
	{ 0x37a0cba, "kfree" },
	{ 0xd81fa9c5, "dma_release_channel" },
	{ 0x201d0f70, "class_destroy" },
	{ 0x71b7020e, "device_destroy" },
	{ 0x7f8bbb5f, "device_create_file" },
	{ 0xd20e521a, "device_create" },
	{ 0xa640d12, "__class_create" },
	{ 0xeeccba3c, "dma_request_chan" },
	{ 0xeb233a45, "__kmalloc" },
	{ 0x7cd8d75e, "page_offset_base" },
	{ 0xe8c97f94, "dma_direct_map_page" },
	{ 0x97651e6c, "vmemmap_base" },
	{ 0x4c9d28b0, "phys_base" },
	{ 0x56470118, "__warn_printk" },
	{ 0xf59725d6, "dev_driver_string" },
	{ 0x9cb986f2, "vmalloc_base" },
	{ 0xa263ceb0, "dma_direct_unmap_page" },
	{ 0x9f109929, "dma_ops" },
	{ 0xdecd0b29, "__stack_chk_fail" },
	{ 0xc5850110, "printk" },
	{ 0x6de13801, "wait_for_completion" },
	{ 0x2ea2c95c, "__x86_indirect_thunk_rax" },
	{ 0x362ef408, "_copy_from_user" },
	{ 0x88db9f48, "__check_object_size" },
	{ 0x29361773, "complete" },
	{ 0xbdfb6dbb, "__fentry__" },
};

MODULE_INFO(depends, "");


MODULE_INFO(srcversion, "4D33F89B043AAFE76643945");
