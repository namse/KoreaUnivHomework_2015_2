#define __KERNEL__
#include <linux/module.h>
#include <linux/kernel.h>
#include <asm/uaccess.h>
#include <linux/skbuff.h>
#include <linux/in.h>
#include <linux/ip.h>
#include <linux/tcp.h>
#include <linux/icmp.h>
#include <linux/netdevice.h>
#include <linux/netfilter.h>
#include <linux/netfilter_ipv4.h>
#include <linux/if_arp.h>
#include <linux/if_ether.h>
#include <linux/if_packet.h>
static struct nf_hook_ops netfilter_ops;

#define PROC_DIRNAME "myproc"
#define PROC_FILENAME "myproc"
#define MAX_LOG_LENGTH (1024)
#define MAX_PORT_NUMBER (10000)
static struct proc_dir_entry *proc_dir;
static struct proc_dir_entry *proc_file;
static char stringBuffer[MAX_LOG_LENGTH];
static size_t bufferOffset; // the length of stirngBuffer.
static short isPortBlocked[MAX_PORT_NUMBER]; // 0 : non-blocked, !0 : blocked
unsigned int main_hook(unsigned int hooknum,
				  struct sk_buff *skb,
				  const struct net_device *in,
				  const struct net_device *out,
				  int (*okfn)(struct sk_buff*))
{
	
	if(!skb) return NF_ACCEPT;

	struct iphdr* ipHeader = (struct iphdr *)skb_network_header(skb);

	if (!ipHeader) return NF_ACCEPT;
	
	// not tcp -> pass
	if (ipHeader->protocol != IPPROTO_TCP) {
		return NF_ACCEPT;
	}

	struct tcphdr *tcpHeader= (struct tcphdr *)((__u32 *)ipHeader+ ipHeader->ihl);
	unsigned short int destPort = htons((unsigned short int) tcpHeader->dest);

	printk(KERN_INFO "port : %d\n", destPort);
	if (isPortBlocked[destPort]) {
		printk(KERN_INFO "port %lu droped\n",destPort);
		return NF_DROP;
	}
	return NF_ACCEPT;
}

static int my_open(struct inode *inode, struct file* file)
{
	printk(KERN_INFO "module file open");
	return 0;
}

static ssize_t my_write(struct file *file, const char __user *user_buffer, size_t count, loff_t *ppos)
{
	// if buffer is full, don't copy them and just return count. it's fake.
	if(bufferOffset + count > MAX_LOG_LENGTH)
		return count;
	if( copy_from_user(stringBuffer + bufferOffset, user_buffer, count) ) {
		return -EFAULT;
	}
	bufferOffset += count;
	
	// port only >=1000
	if(bufferOffset >= 4)
	{
		stringBuffer[4] = 0;
		long port;
		kstrtol(stringBuffer, 10, &port);
		printk(KERN_INFO "port : %ld was %d, than become %d\n", port, isPortBlocked[port], isPortBlocked[port] != 0 ? 0 : 1);
		if(port >= 1000){
			isPortBlocked[port] = isPortBlocked[port] != 0 ? 0 : 1;
		}
		bufferOffset = 0;
	}

	return count;
}

static const struct file_operations myproc_fops = {
	.owner = THIS_MODULE,
	.open = my_open,
	.write = my_write,
};


static int __init simple_init(void){
	printk(KERN_INFO"Simple Module Init!!\n");

	netfilter_ops.hook = main_hook;
	netfilter_ops.pf = PF_INET;
	netfilter_ops.hooknum = NF_INET_LOCAL_IN;
	netfilter_ops.priority = NF_IP_PRI_FIRST;

	nf_register_hook(&netfilter_ops);

	proc_dir = proc_mkdir(PROC_DIRNAME, NULL);
	proc_file = proc_create(PROC_FILENAME, 0600, proc_dir, &myproc_fops);

	return 0;
}

static void __exit simple_exit(void){
	printk(KERN_INFO"Simple Module Exit!!\n");

	nf_unregister_hook(&netfilter_ops);
	remove_proc_entry(PROC_FILENAME, proc_dir);
	return;
}

module_init(simple_init);
module_exit(simple_exit);

MODULE_AUTHOR("NamSeHyun");
MODULE_DESCRIPTION("SIMPLE");
MODULE_LICENSE("GPL");
MODULE_VERSION("NEW");
