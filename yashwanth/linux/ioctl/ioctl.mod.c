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
	{ 0xc665c010, "class_unregister" },
	{ 0xbfa81801, "device_destroy" },
	{ 0xe7a5b252, "cdev_del" },
	{ 0x8abc8099, "class_destroy" },
	{ 0x134f00b9, "device_create" },
	{ 0x1c89aa5e, "__class_create" },
	{ 0x6091b333, "unregister_chrdev_region" },
	{ 0x7a288aa6, "cdev_add" },
	{ 0x8c3a7cb1, "cdev_init" },
	{ 0xe3ec2f2b, "alloc_chrdev_region" },
	{ 0x362ef408, "_copy_from_user" },
	{ 0xc5850110, "printk" },
	{ 0x167e7f9d, "__get_user_1" },
	{ 0xc3aaf0a9, "__put_user_1" },
	{ 0xbdfb6dbb, "__fentry__" },
};

MODULE_INFO(depends, "");


MODULE_INFO(srcversion, "467470C84FE0D2A52B3547A");
