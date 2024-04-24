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
	{ 0x8d7e33a7, "module_layout" },
	{ 0xbfa81801, "device_destroy" },
	{ 0xe7a5b252, "cdev_del" },
	{ 0x6091b333, "unregister_chrdev_region" },
	{ 0x8abc8099, "class_destroy" },
	{ 0x1e41de40, "sysfs_remove_file_ns" },
	{ 0xedf41f4c, "kobject_put" },
	{ 0xbd465f5a, "sysfs_create_file_ns" },
	{ 0x26d6900d, "kobject_create_and_add" },
	{ 0x12324102, "kernel_kobj" },
	{ 0x134f00b9, "device_create" },
	{ 0x1c89aa5e, "__class_create" },
	{ 0x7a288aa6, "cdev_add" },
	{ 0x8c3a7cb1, "cdev_init" },
	{ 0xe3ec2f2b, "alloc_chrdev_region" },
	{ 0x3c3ff9fd, "sprintf" },
	{ 0xbcab6ee6, "sscanf" },
	{ 0xc5850110, "printk" },
	{ 0xbdfb6dbb, "__fentry__" },
};

MODULE_INFO(depends, "");


MODULE_INFO(srcversion, "54D8292764B8B8E057B1C17");
