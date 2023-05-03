#include<linux/module.h>
#include<linux/fs.h>
#include<linux/cdev.h>
#include<linux/device.h>
#include<linux/kdev_t.h>
#include<linux/uaccess.h>


#undef pr_fmt
#define pr_fmt(fmt) "%s : " fmt,__func__

#define DEV_MEM_SIZE 512

/* pseudo device's memory */
char device_buffer[DEV_MEM_SIZE];

/* This holds the device number */
dev_t device_number;

/* Cdev variable */
struct cdev mychr_cdev;

/*holds the class pointer */
struct class *class_mychr;

struct device *device_mychr;


loff_t mychr_lseek(struct file *filp, loff_t offset, int whence)
{
	pr_info("lseek opeation");
	return 0;
}

ssize_t mychr_read(struct file *filp, char __user *buff, size_t count, loff_t *f_pos)
{
	pr_info("Read requested for %zu bytes \n",count);
	return count;
}

ssize_t mychr_write(struct file *filp, const char __user *buff, size_t count, loff_t *f_pos)
{
	pr_info("Write requested for %zu bytes\n",count);
	return count;
}

int mychr_open(struct inode *inode, struct file *filp)
{
	pr_info("open was successful\n");
	return 0;
}

int mychr_release(struct inode *inode, struct file *flip)
{
	pr_info("release was successful\n");
	return 0;
}


/* file operations of the driver */
struct file_operations mychr_fops=
{
	.open = mychr_open,
	.release = mychr_release,
	.read = mychr_read,
	.write = mychr_write,
	.owner = THIS_MODULE
};


static int __init mychr_driver_init(void)
{
	int ret;

	/*1. Dynamically allocate a device number */
	ret = alloc_chrdev_region(&device_number,0,1,"my_char_drv");
	if(ret < 0){
		pr_err("Alloc chrdev failed\n");
		goto out;
	}

	pr_info("Device number <major>:<minor> = %d:%d\n",MAJOR(device_number),MINOR(device_number));

	/*2. Initialize the cdev structure with fops*/
	cdev_init(&mychr_cdev,&mychr_fops);

	/* 3. Register a device (cdev structure) with VFS */
	mychr_cdev.owner = THIS_MODULE;
	ret = cdev_add(&mychr_cdev,device_number,1);
	if(ret < 0){
		pr_err("Cdev add failed\n");
		goto unreg_chrdev;
	}

	/*4. create device class under /sys/class/ */
	class_mychr = class_create(THIS_MODULE,"mychrdrv_class");
	if(IS_ERR(class_mychr)){
		pr_err("Class creation failed\n");
		ret = PTR_ERR(class_mychr);
		goto cdev_del;
	}

	/*5.  populate the sysfs with device information */
	device_mychr = device_create(class_mychr,NULL,device_number,NULL,"mychrdrv");
	if(IS_ERR(device_mychr)){
		pr_err("Device create failed\n");
		ret = PTR_ERR(device_mychr);
		goto class_del;
	}

	pr_info("Module init was successful\n");

	return 0;

class_del:
	class_destroy(class_mychr);
cdev_del:
	cdev_del(&mychr_cdev);	
unreg_chrdev:
	unregister_chrdev_region(device_number,1);
out:
	pr_info("Module insertion failed\n");
	return ret;
}



static void __exit mychr_driver_cleanup(void)
{
	device_destroy(class_mychr,device_number);
	class_destroy(class_mychr);
	cdev_del(&mychr_cdev);
	unregister_chrdev_region(device_number,1);
	pr_info("module unloaded\n");
}


module_init(mychr_driver_init);
module_exit(mychr_driver_cleanup);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Sarath AK");
MODULE_DESCRIPTION("A Simple character driver.");
