#include <linux/module.h>

static int __init hello_sarath(void)
{
	pr_info("\nHello World.\n");
	return 0;
}


static  void __exit bye_sarath(void)
{
	pr_info("\nBye Sarath ");
}


module_init(hello_sarath);
module_exit(bye_sarath);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("SARATH");
MODULE_DESCRIPTION("simple hello world module");
MODULE_INFO(board,"beaglebone");

