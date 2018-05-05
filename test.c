#include <linux/module.h>  /* Needed by all kernel modules */
#include <linux/kernel.h>  /* Needed for loglevels (KERN_WARNING, KERN_EMERG, KERN_INFO, etc.) */
#include <linux/init.h>    /* Needed for __init and __exit macros. */
#include <net/sock.h>
#include <linux/skbuff.h>
#include <linux/netlink.h>
#include <linux/vmalloc.h>

typedef struct key_value{
	int key;
	int value;
	struct key_value * prev;
	struct key_value * next;
} key_value;

static struct sock *socket;
static struct key_value store;

static void callback(struct sk_buff *sockBuff){
	struct key_value * received = (key_value *)sockBuff->data;
	struct key_value * curr = &store;

	printk(KERN_NOTICE "\n");
	printk(KERN_NOTICE "TEST_LKM -- Message recieved with key = %d and value = %d\n", received->key, received->value);
	printk(KERN_NOTICE "\n");


	curr = &store;
	while (curr != NULL){
		printk(KERN_NOTICE "key = %d and value = %d\n", curr->key, curr->value);
		curr = curr->next;
	}
	return;
}

static int __init wasLoaded(void){

	struct netlink_kernel_cfg cfg = {
		.input = callback,
	};

	store.key = 0;
	store.value = 0;
	store.next = &store;
	store.prev = &store;

	socket = netlink_kernel_create(&init_net, 20, &cfg);

	if (socket == NULL){
		printk(KERN_ERR "Could not establish a netlink\n");
		return -1;
	}
	printk(KERN_NOTICE "Test module initialized\n");
	return 0;
}

static void __exit wasUnloaded(void) {
	netlink_kernel_release(socket);
	printk(KERN_NOTICE "Test module removed\n");
}

module_init(wasLoaded);
module_exit(wasUnloaded);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Andrew Baumbach");
MODULE_DESCRIPTION("Outline for linux kernel module");
