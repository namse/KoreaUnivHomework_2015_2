#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/proc_fs.h>
#include <asm/uaccess.h>

#define min(a,b) \
({ __typeof__ (a) _a = (a); \
__typeof__ (b) _b = (b); \
_a < _b ? _a : _b; })
#define PROC_DIRNAME "myproc"
#define PROC_FILENAME "myproc"
#define MAX_LOG_LENGTH (1024*512)
static struct proc_dir_entry *proc_dir;
static struct proc_dir_entry *proc_file;


// Enough large buffer so we don't need circular buffer.
static char stringBuffer[MAX_LOG_LENGTH];
static size_t bufferOffset; // the length of stirngBuffer.

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
	
	return count;
}

static ssize_t my_read(struct file *filp, char *buffer, size_t buffer_length, loff_t * offset)
{
	size_t length = 0;
	printk(KERN_INFO "offset : %lld,  buffer_length : %u, bufferOffset : %u\n", *offset, buffer_length, bufferOffset);

	if( *offset >= bufferOffset ) // if they try to read over the length of stringBuffer?
		return 0;
	else if( *offset + buffer_length > bufferOffset ) // if offset is lower than bufferOffset but buffer_length is over the total length,
	{
		length = min(buffer_length, bufferOffset - *offset); // than just read what we can read
		printk(KERN_INFO "buffer Length : %u",buffer_length);
		printk(KERN_INFO "buferOffset - sOffset = %d",bufferOffset - *offset);	
	}	
	else
		length = buffer_length;
	if(copy_to_user(buffer, stringBuffer + *offset, length) ) {
		return -EFAULT;
	}
	printk(KERN_INFO "length : %d",length);
	*offset += length;
	return length;
}

static const struct file_operations myproc_fops = {
	.owner = THIS_MODULE,
	.open = my_open,
	.write = my_write,
	.read = my_read,
};


static int __init simple_init(void){
	printk(KERN_INFO"Simple Module Init!!\n");

	bufferOffset = 0;	
	proc_dir = proc_mkdir(PROC_DIRNAME, NULL);
	proc_file = proc_create(PROC_FILENAME, 0600, proc_dir, &myproc_fops);
	return 0;
}

static void __exit simple_exit(void){
	printk(KERN_INFO"Simple Module Exit!!\n");
}

module_init(simple_init);
module_exit(simple_exit);

MODULE_AUTHOR("NamSeHyun");
MODULE_DESCRIPTION("SIMPLE");
MODULE_LICENSE("GPL");
MODULE_VERSION("NEW");
