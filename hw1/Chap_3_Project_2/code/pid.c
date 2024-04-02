/**
 * Kernel module that communicates with /proc file system.
 *
 * This provides the base logic for Project 2 - displaying task information
 */

#include <linux/init.h>
#include <linux/slab.h>
#include <linux/sched.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <linux/vmalloc.h>
#include <asm/uaccess.h>

#define BUFFER_SIZE 128
#define PROC_NAME "pid"

/* the current pid */
static long l_pid;

/**
 * Function prototypes
 */
static ssize_t proc_read(struct file *file, char __user *usr_buf, size_t count, loff_t *pos);
static ssize_t proc_write(struct file *file, const char __user *usr_buf, size_t count, loff_t *pos);

static struct proc_ops proc_ops = {
        .proc_read = proc_read,
        .proc_write = proc_write,
};

/**
 * This function is called each time the /proc/pid is read.
 * 
 * This function is called repeatedly until it returns 0, so
 * there must be logic that ensures it ultimately returns 0
 * once it has collected the data that is to go into the 
 * corresponding /proc file.
 */
static ssize_t proc_read(struct file *file, char __user *usr_buf, size_t count, loff_t *pos)
{
        int rv = 0;
        char buffer[BUFFER_SIZE];
        static int completed = 0;
        struct task_struct *tsk = NULL;

        if (completed) {
                completed = 0;
                return 0;
        }

        tsk = pid_task(find_vpid(l_pid), PIDTYPE_PID);
        if (!tsk) {
                printk(KERN_ALERT "Invalid PID\n");
                return 0;
        }

        rv = snprintf(buffer, BUFFER_SIZE, "command = [%s], pid = [%ld], state = [%ld]\n", tsk->comm, l_pid, (long)task_state_to_char(tsk));

        // copies the contents of kernel buffer to userspace usr_buf 
        if (copy_to_user(usr_buf, buffer, rv)) {
                return -EFAULT;
        }

        completed = 1;

        return rv;
}

/**
 * This function is called each time we write to the /proc file system.
 */
static ssize_t proc_write(struct file *file, const char __user *usr_buf, size_t count, loff_t *pos)
{
        char *k_mem;

        // allocate kernel memory
        k_mem = kmalloc(count, GFP_KERNEL);
        if (!k_mem) {
                return -ENOMEM;
        }

        /* copies user space usr_buf to kernel buffer */
        if (copy_from_user(k_mem, usr_buf, count)) {
                kfree(k_mem);
                return -EFAULT;
        }

        // Convert the string representation of PID to long integer
        if (kstrtol(k_mem, 10, &l_pid)) {
                kfree(k_mem);
                return -EINVAL;
        }

        kfree(k_mem);

        return count;
}

static int __init taskinfo_init(void)
{
        // Create the /proc file
        proc_create(PROC_NAME, 0666, NULL, &proc_ops);
        printk(KERN_INFO "/proc/%s created\n", PROC_NAME);
        return 0;
}

static void __exit taskinfo_exit(void)
{
        // Remove the /proc file
        remove_proc_entry(PROC_NAME, NULL);
        printk(KERN_INFO "/proc/%s removed\n", PROC_NAME);
}

module_init(taskinfo_init);
module_exit(taskinfo_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Task Information Module");
MODULE_AUTHOR("Jerry");
