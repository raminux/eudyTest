#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/debugfs.h>
#include <linux/fs.h>  // file_operations
#include <linux/errno.h>
#include <linux/uaccess.h>
#include <linux/string.h>

struct dentry *dir_dentry;

ssize_t id_read(struct file *file,
	char __user *buf, /* The buffer to fill with data */
	size_t len, /* The length of the buffer */
	loff_t *ppos) /* Our offset in the file */
{
	char *ID = "0c7a48b3b807";

	return simple_read_from_buffer(buf, len, ppos, ID, 12);
}

ssize_t id_write(struct file *file,
	const char __user *buf, /* The buffer to fill with data */
	size_t len, /* The length of the buffer */
	loff_t *ppos) /* Our offset in the file */
{
	char *ID = "0c7a48b3b807";
	char data[12];

	ssize_t ret = simple_write_to_buffer(data, 12, ppos, buf, len);
	if (ret >= 0) {
		if (strncmp(buf, ID, len) == 0) {
			pr_debug("I get your ID\n");
		}
	}
	return ret;
}

const struct file_operations id_fops = {
	.owner = THIS_MODULE,
	.write = id_write,
	.read = id_read,
};


static int __init Task08_init(void)
{
	dir_dentry = debugfs_create_dir("eudyptula", NULL);
	if (!dir_dentry) {
		pr_debug("debugfs not supported in this kernel");
		return -ENODEV;
	}
	
	debugfs_create_file("id", 0666, dir_dentry, NULL, &id_fops);

	pr_debug("Hello, world!\n");
	return 0;
}
static void __exit Task08_exit(void)
{
	debugfs_remove_recursive(dir_dentry);
	pr_debug("Goodbye, world!\n");

}
module_init(Task08_init);
module_exit(Task08_exit);
MODULE_LICENSE("GPL");
