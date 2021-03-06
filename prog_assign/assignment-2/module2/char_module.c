#include <linux/init.h>
#include <linux/module.h>
#include <linux/types.h>
#include <linux/stat.h>
#include <linux/fcntl.h>
#include <linux/unistd.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <asm/uaccess.h>
#define BUFFER_SIZE 5


static int char_driver_init(void);
static void char_driver_exit(void);
int char_driver_open (struct inode *pinode, struct file *pfile);
int char_driver_close (struct inode *pinode, struct file *pfile);
ssize_t char_driver_read (struct file *pfile, char __user *buffer, size_t length, loff_t *offset);
ssize_t char_driver_write (struct file *pfile, const char __user *buffer, size_t length, loff_t *offset);


static char device_buffer[BUFFER_SIZE];

static int major;
static int num_open = 0;
static int num_close = 0;
const char* DEVICE_NAME = "my_char_driver";
loff_t m_offset = 0;

struct file_operations m_fops = {
        .owner   = THIS_MODULE,
	.read = char_driver_read,
	.write = char_driver_write,
	.open = char_driver_open,
	.release = char_driver_close
};

/* add module_init and module_exit to point to the corresponding init and exit function*/
module_init(char_driver_init);
module_exit(char_driver_exit); 

 /* *buffer is the userspace buffer to where you are writing the data you want to be read from the device file*/
        /*  length is the length of the userspace buffer*/
        /*  current position of the opened file*/
        /* copy_to_user function. source is device_buffer (the buffer defined at the start of the code) and destination is the userspace                buffer *buffer */
	
ssize_t char_driver_read (struct file *pfile, char __user *buffer, size_t length, loff_t *offset)
{
	/* copy data to user buffer, returns 0 on success */
	if( copy_to_user(buffer, device_buffer, m_offset) != 0 ) 
		return -EFAULT;
	printk(KERN_ALERT "char_module READ: Data (%llu) copied to user buffer from device buffer.\n", m_offset);
	printk(KERN_ALERT "char_module READ: offset (%llu)\n", m_offset);
        return m_offset;
}

	/* *buffer is the userspace buffer where you are writing the data you want to be written in the device file*/
        /*  length is the length of the userspace buffer*/
        /*  current position of the opened file*/
        /* copy_from_user function. destination is device_buffer (the buffer defined at the start of the code) and source is the userspace              buffer *buffer */
	/* return value is negative when buffer overflows */
ssize_t char_driver_write (struct file *pfile, const char __user *buffer, size_t length, loff_t *offset)
{
        size_t u_buffer_len = strlen(buffer);
	if( u_buffer_len + m_offset >= BUFFER_SIZE )
		u_buffer_len = (size_t)BUFFER_SIZE - m_offset;
	/* copy data to user buffer, returns 0 on success */
	if( copy_from_user(device_buffer + m_offset, buffer, u_buffer_len) != 0 ) 
		return -EFAULT;
	/* increase the offset by amount written, and return amount written. */
        m_offset += u_buffer_len;
       	printk(KERN_ALERT "char_module WRITE: Data (%lu) copied from user buffer to device buffer.\n", u_buffer_len);
	printk(KERN_ALERT "char_module WRITE: offset (%llu)\n", m_offset);
	return u_buffer_len;
}


int char_driver_open (struct inode *pinode, struct file *pfile)
{
	printk(KERN_ALERT "char_module opened: %d\n", ++num_open);
        return 0;
}


int char_driver_close (struct inode *pinode, struct file *pfile)
{
	printk(KERN_ALERT "char_module closed: %d\n", ++num_close);
        return 0;
}

static int char_driver_init(void)
{
	printk(KERN_ALERT "char_module init called\n");
	major = register_chrdev(0, DEVICE_NAME, &m_fops);
	printk(KERN_ALERT "char_module registered with major # %d\n", major);
	return 0;
}

static void char_driver_exit(void)
{
	printk(KERN_ALERT "char_module exit called\n");
	unregister_chrdev(major, DEVICE_NAME);
}

