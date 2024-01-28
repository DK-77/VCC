#include <linux/module.h>
#define INCLUDE_VERMAGIC
#include <linux/build-salt.h>
#include <linux/elfnote-lto.h>
#include <linux/export-internal.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

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


static const struct modversion_info ____versions[]
__used __section("__versions") = {
	{ 0x92997ed8, "_printk" },
	{ 0xbdfb6dbb, "__fentry__" },
	{ 0x6a2ed50b, "find_get_pid" },
	{ 0xebb286b, "pid_task" },
	{ 0xc9ee9343, "get_task_mm" },
	{ 0x668b19a1, "down_read" },
	{ 0x2ff39f5e, "find_vma" },
	{ 0x4c9d28b0, "phys_base" },
	{ 0x7cd8d75e, "page_offset_base" },
	{ 0x53b954a2, "up_read" },
	{ 0xee66e3fc, "mmput" },
	{ 0x5b8239ca, "__x86_return_thunk" },
	{ 0x21cad704, "param_ops_ulong" },
	{ 0xd6375041, "param_ops_int" },
	{ 0x517f61ad, "module_layout" },
};

MODULE_INFO(depends, "");

