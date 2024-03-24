#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/proc_fs.h>
#include <linux/jiffies.h>
#include <linux/uaccess.h>

#define BUFFER_SIZE 128
#define PROC_NAME "jiffies"

ssize_t proc_read(struct file *file, char __user *usr_buf, size_t count, loff_t *pos);

// Declare the read function
static struct proc_ops proc_ops = {
    .proc_read = proc_read,
};

// Begin call this function when the module loaded
int proc_init(void)
{
    proc_create(PROC_NAME, 0666, NULL, &proc_ops);
    printk(KERN_INFO "The /proc/jiffies loaded!\n");
    return 0;
}

void proc_exit(void)
{
    remove_proc_entry(PROC_NAME, NULL);
    printk(KERN_INFO "The /proc/jiffies unloaded!\n");
}

// Implementation of proc_read
ssize_t proc_read(struct file *file, char __user *usr_buf, size_t count, loff_t *pos)
{
    int rv = 0;
    char buffer[BUFFER_SIZE];
    static int completed = 0;
    if (completed)
    {
        completed = 0;
        return 0;
    }
    completed = 1;
    rv = sprintf(buffer, "Now the jiffies value is %lu\n", jiffies);
    if (copy_to_user(usr_buf, buffer, rv))
    {
        return -EFAULT;
    }
    return rv;
}

module_init(proc_init);
module_exit(proc_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Display the jiffies value");
MODULE_AUTHOR("Jerry");
