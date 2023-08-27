#include <linux/module.h>
#include <linux/init.h>

static int __init helloworld_init(void){
    printk(KERN_ALERT "Hello world module init\n");
    return 0;
}

static void __exit helloworld_exit(void){
    printk(KERN_ALERT "Hello world module exit\n");
}

module_init(helloworld_init);
module_exit(helloworld_exit);


