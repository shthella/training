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
	{ 0xf147da98, "class_unregister" },
	{ 0xa1c4e556, "sysfs_remove_file_ns" },
	{ 0x71b7020e, "device_destroy" },
	{ 0xedf41f4c, "kobject_put" },
	{ 0xafa08004, "sysfs_create_file_ns" },
	{ 0x26d6900d, "kobject_create_and_add" },
	{ 0xfaeb0dcd, "kernel_kobj" },
	{ 0x201d0f70, "class_destroy" },
	{ 0xd20e521a, "device_create" },
	{ 0x6bc3fbc0, "__unregister_chrdev" },
	{ 0xa640d12, "__class_create" },
	{ 0xe9e73cc4, "__register_chrdev" },
	{ 0x56470118, "__warn_printk" },
	{ 0x362ef408, "_copy_from_user" },
	{ 0x88db9f48, "__check_object_size" },
	{ 0xb44ad4b3, "_copy_to_user" },
	{ 0xc5850110, "printk" },
	{ 0xcbd4898c, "fortify_panic" },
	{ 0xa916b694, "strnlen" },
	{ 0xe914e41e, "strcpy" },
	{ 0x3c3ff9fd, "sprintf" },
	{ 0xbdfb6dbb, "__fentry__" },
};

MODULE_INFO(depends, "");


MODULE_INFO(srcversion, "0578D1CE688823E2A654823");
