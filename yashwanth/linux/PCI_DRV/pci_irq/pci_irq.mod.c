#include <linux/module.h>
#define INCLUDE_VERMAGIC
#include <linux/build-salt.h>
#include <linux/elfnote-lto.h>
#include <linux/export-internal.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

#ifdef CONFIG_UNWINDER_ORC
#include <asm/orc_header.h>
ORC_HEADER;
#endif

BUILD_SALT;
BUILD_LTO_INFO;

MODULE_INFO(vermagic, VERMAGIC_STRING);
MODULE_INFO(name, KBUILD_MODNAME);

__visible struct module __this_module
__section(".gnu.linkonce.this_module") = {
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



static const char ____versions[]
__used __section("__versions") =
	"\x20\x00\x00\x00\xde\xac\xb4\x3b"
	"__pci_register_driver\0\0\0"
	"\x1c\x00\x00\x00\xf2\xeb\xf9\xcf"
	"pcim_enable_device\0\0"
	"\x1c\x00\x00\x00\x6b\x14\xc0\x6a"
	"pcim_iomap_regions\0\0"
	"\x18\x00\x00\x00\xb9\x27\x48\xb9"
	"devm_kmalloc\0\0\0\0"
	"\x1c\x00\x00\x00\xe2\x84\x80\x19"
	"pcim_iomap_table\0\0\0\0"
	"\x14\x00\x00\x00\xf3\xf5\x8a\xa7"
	"ioread32\0\0\0\0"
	"\x24\x00\x00\x00\x2c\x95\x7a\x9d"
	"devm_request_threaded_irq\0\0\0"
	"\x14\x00\x00\x00\x2e\x37\x8d\x84"
	"iowrite8\0\0\0\0"
	"\x20\x00\x00\x00\x4a\xa9\x9f\x45"
	"pci_unregister_driver\0\0\0"
	"\x14\x00\x00\x00\xbb\x6d\xfb\xbd"
	"__fentry__\0\0"
	"\x10\x00\x00\x00\x7e\x3a\x2c\x12"
	"_printk\0"
	"\x1c\x00\x00\x00\xca\x39\x82\x5b"
	"__x86_return_thunk\0\0"
	"\x18\x00\x00\x00\x3a\x0a\xd8\xfc"
	"module_layout\0\0\0"
	"\x00\x00\x00\x00\x00\x00\x00\x00";

MODULE_INFO(depends, "");

MODULE_ALIAS("pci:v00008086d0000A0EFsv*sd*bc*sc*i*");

MODULE_INFO(srcversion, "5922E5E293F1EEAC0742924");
