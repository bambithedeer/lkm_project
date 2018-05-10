#include <linux/module.h>  /* Needed by all kernel modules */
#include <linux/kernel.h>  /* Needed for loglevels (KERN_WARNING, KERN_EMERG, KERN_INFO, etc.) */
#include <linux/init.h>    /* Needed for __init and __exit macros. */
#include <net/sock.h>
#include <linux/skbuff.h>
#include <linux/netlink.h>
#include <linux/vmalloc.h>
#include <linux/list.h>

typedef struct key_value{
	int key;
	int value;
	int size;
	struct list_head hdr;
} key_value;

static struct sock *socket;
static struct key_value * store;

static void callback(struct sk_buff *sockBuff){

	int curr_size = store->size;
	struct key_value * iter;
	struct key_value * received = (key_value *)sockBuff->data;
	struct key_value * entry;

	printk(KERN_NOTICE "\n");
	printk(KERN_NOTICE "TEST_LKM -- Message received with key = %d and value = %d\n", received->key, received->value);
	printk(KERN_NOTICE "\n");

	if (store->size == 0){

		printk(KERN_NOTICE "\n");
		printk(KERN_NOTICE "TEST_LKM -- List Empty\n");
		printk(KERN_NOTICE "\n");

		printk(KERN_NOTICE "\n");
		printk(KERN_NOTICE "TEST_LKM -- Adding new entry to list\n");
		printk(KERN_NOTICE "\n");

		entry = kmalloc(sizeof(struct key_value), GFP_KERNEL);
		entry->key = received->key;
		entry->value = received->value;
		entry->size = 0;
		INIT_LIST_HEAD(&(entry->hdr));
		list_add_tail(&(entry->hdr), &(store->hdr));

		printk(KERN_NOTICE "\n");
		printk(KERN_NOTICE "TEST_LKM -- Added entry with Key: %d and Value: %d\n", entry->key, entry->value);
		printk(KERN_NOTICE "TEST_LKM -- Current size: %d\n", store->size);
		printk(KERN_NOTICE "\n");

		store->size += 1;

		return;
	}
	else{

		printk(KERN_NOTICE "\n");
		printk(KERN_NOTICE "TEST_LKM -- Starting search for received entry\n");
		printk(KERN_NOTICE "\n");

		list_for_each_entry_reverse(iter, &(store->hdr), hdr){

			printk(KERN_NOTICE "\n");
			printk(KERN_NOTICE "TEST_LKM -- Iter Key: %d, Value: %d\n", iter->key, iter->value);
			printk(KERN_NOTICE "\n");

			if (iter->key == received->key){

				printk(KERN_NOTICE "\n");
				printk(KERN_NOTICE "TEST_LKM -- Found entry for Key: %d. Current value: %d, updating value\n", iter->key, iter->value);
				printk(KERN_NOTICE "\n");

				iter->value = received->value;

				printk(KERN_NOTICE "\n");
				printk(KERN_NOTICE "TEST_LKM -- Updated entry with Key: %d with Value: %d\n", iter->key, iter->value);
				printk(KERN_NOTICE "TEST_LKM -- Current size: %d\n", store->size);
				printk(KERN_NOTICE "\n");

				return;
			}
			else if (iter->key > received->key){

				printk(KERN_NOTICE "\n");
				printk(KERN_NOTICE "TEST_LKM -- No entry for received, adding it now...\n");
				printk(KERN_NOTICE "\n");

				entry = kmalloc(sizeof(struct key_value), GFP_KERNEL);
				entry->key = received->key;
				entry->value = received->value;
				entry->size = 0;
				INIT_LIST_HEAD(&(entry->hdr));
				list_add(&(entry->hdr), &(iter->hdr));
				store->size += 1;

				printk(KERN_NOTICE "\n");
				printk(KERN_NOTICE "TEST_LKM -- Added entry with Key: %d and Value: %d\n", entry->key, entry->value);
				printk(KERN_NOTICE "TEST_LKM -- Current size: %d\n", store->size);
				printk(KERN_NOTICE "\n");

				return;
			}
		}
		if (curr_size == store->size){

			printk(KERN_NOTICE "\n");
			printk(KERN_NOTICE "TEST_LKM -- No entry for received, adding it now - end\n");
			printk(KERN_NOTICE "\n");

			entry = kmalloc(sizeof(struct key_value), GFP_KERNEL);
			entry->key = received->key;
			entry->value = received->value;
			entry->size = 0;
			INIT_LIST_HEAD(&(entry->hdr));
			list_add(&(entry->hdr), &(store->hdr));
			store->size += 1;

			printk(KERN_NOTICE "\n");
			printk(KERN_NOTICE "TEST_LKM -- Added entry with Key: %d and Value: %d\n", entry->key, entry->value);
			printk(KERN_NOTICE "TEST_LKM -- Current size: %d\n", store->size);
			printk(KERN_NOTICE "\n");

		}
		return;
	}
}

static int __init wasLoaded(void){

	struct netlink_kernel_cfg cfg = {
		.input = callback,
	};
	store = kmalloc(sizeof(struct key_value *), GFP_KERNEL);
	if (!store){
		printk(KERN_ERR "Store pointer was not initialized properly\n");
	}
	store->key = -1;
	store->value = -1;
	store->size = 0;
	INIT_LIST_HEAD(&(store->hdr));
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
MODULE_DESCRIPTION("Key value store that is accessible from userland");
