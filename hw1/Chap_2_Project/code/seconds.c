#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/proc_fs.h>
#include <asm/uaccess.h>
#include <linux/jiffies.h>
#include <linux/param.h>

#define BUFFER_SIZE 128
#define PROC_NAME "seconds"

unsigned long int volatile f_jiffies, l_jiffies;
const int hz = HZ;

ssize_t proc_read(struct file *file, char __user *usr_buf, size_t count, loff_t *pos);

static struct proc_ops proc_ops = {
    .proc_read = proc_read,
};

int proc_init(void)
{
    proc_create(PROC_NAME, 0666, NULL, &proc_ops);
    f_jiffies = jiffies;
    printk(KERN_INFO "seconds loaded!\n");
    return 0;
}

void proc_exit(void)
{
    remove_proc_entry(PROC_NAME, NULL);
    printk(KERN_INFO "seconds unloaded!\n");
}

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
    l_jiffies = jiffies;
    rv = snprintf(buffer, BUFFER_SIZE, "The running time is %ld s\n", ((l_jiffies - f_jiffies) / hz));
    if (copy_to_user(usr_buf, buffer, rv))
    {
        return -EFAULT;
    }
    return rv;
}

module_init(proc_init);
module_exit(proc_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Show the program execution time");
MODULE_AUTHOR("Jerry");
