#include<linux/module.h>
#include<linux/fs.h>
#include<linux/cdev.h>
#include<linux/device.h>
#include<linux/kdev_t.h>
#include<linux/uaccess.h>
#include<linux/err.h>

#undef pr_fmt
#define pr_fmt(fmt) "%s : " fmt,__func__
#define NO_OF_DEVICES 4

#define MEM_SIZE_MAX_MYCHR1 1024
#define MEM_SIZE_MAX_MYCHR2 512
#define MEM_SIZE_MAX_MYCHR3 1024
#define MEM_SIZE_MAX_MYCHR4 512

#define RDONLY 0X01
#define WRONLY 0x10
#define RDWR 0x11

char device_buffer_mychr1[MEM_SIZE_MAX_MYCHR1];
char device_buffer_mychr2[MEM_SIZE_MAX_MYCHR2];
char device_buffer_mychr3[MEM_SIZE_MAX_MYCHR3];
char device_buffer_mychr4[MEM_SIZE_MAX_MYCHR4];


struct mychr_device_private {
	char *buffer;
	unsigned size;
	char *serial_number;
	int perm;
	struct cdev mychr_cdev;	
};

struct mychr_driver_private{
	int total_devices;
	dev_t device_number;	
	struct class *class_mychr;
	struct device *device_mychr;
	struct mychr_device_private mychrdev_data[NO_OF_DEVICES];
};


struct mychr_driver_private mychrdrv_data={
	.total_devices = NO_OF_DEVICES,
	.mychrdev_data={
		[0] = {
			.buffer = device_buffer_mychr1,
			.size	= MEM_SIZE_MAX_MYCHR1,
			.serial_number = "MYCHA1DEVICE123",
			.perm	= RDONLY
		},
		[1] = {
			.buffer = device_buffer_mychr2,
			.size	= MEM_SIZE_MAX_MYCHR2,
			.serial_number = "MYCHA2DEVICE123",
			.perm	= WRONLY
		},
		[2] = {
			.buffer = device_buffer_mychr3,
			.size	= MEM_SIZE_MAX_MYCHR3,
			.serial_number = "MYCHA3DEVICE123",
			.perm	= RDWR
		},
		[3] = {
			.buffer = device_buffer_mychr4,
			.size	= MEM_SIZE_MAX_MYCHR4,
			.serial_number = "MYCHA4DEVICE123",
			.perm	= RDWR
		}	
	}
};

										
loff_t mychr_lseek(struct file *filp, loff_t offset, int whence)
{
	struct mychr_device_private *mychrdev_data = (struct mychr_device_private*)filp->private_data;
	int max_size = mychrdev_data->size;	
	loff_t temp;
	
	pr_info("lseek requested \n");
	pr_info("Current value of the file position is %lld\n",filp->f_pos);
	
	switch(whence){
		
		case SEEK_SET:
			if((offset > max_size) || ( offset <0 ) ){
				return -EINVAL;
			}
			filp->f_pos = offset;
			break;
		case SEEK_CUR:
			temp=filp->f_pos + offset;	
			if(( temp > max_size) || ( temp < 0 ) ){
				return -EINVAL;
			}
			break;
		case SEEK_END:
                        temp = max_size + offset;
                        if((temp > max_size) || (temp < 0))
                                return -EINVAL;
                        filp->f_pos = temp;
                        break;
		default:	
			return -EINVAL;
	}

	pr_info("New value of the file position = %lld\n",filp->f_pos);

	return filp->f_pos;
}

ssize_t mychr_read(struct file *filp, char __user *buff, size_t count, loff_t *f_pos)
{
	pr_info("Read requested for %zu bytes \n",count);
    struct mychr_device_private *mychrdev_data = (struct mychr_device_private*)filp->private_data;
	int max_size = mychrdev_data->size;	
	
        /* Adjust the 'count' */
        if((*f_pos + count) > max_size)
                count = max_size - *f_pos;

        /*copy to user */
        if(copy_to_user(buff,mychrdev_data->buffer+(*f_pos),count)){
                return -EFAULT;
        }

        /*update the current file postion */
        *f_pos += count;

        pr_info("Number of bytes successfully read = %zu\n",count);
        pr_info("Updated file position = %lld\n",*f_pos);

	return count;

}

ssize_t mychr_write(struct file *filp, const char __user *buff, size_t count, loff_t *f_pos)
{
	
        pr_info("Write requested for %zu bytes\n",count);
        pr_info("Current file position = %lld\n",*f_pos);
		struct mychr_device_private *mychrdev_data = (struct mychr_device_private*)filp->private_data;
		int max_size = mychrdev_data->size;	
	        
        /* Adjust the 'count' */
        if((*f_pos + count) > max_size)
                count = max_size - *f_pos;

        if(!count){
                pr_err("No space left on the device \n");
                return -ENOMEM;
        }

        /*copy from user */
        if(copy_from_user(mychrdev_data->buffer+(*f_pos),buff,count)){
                return -EFAULT;
        }

        /*update the current file postion */
        *f_pos += count;

        pr_info("Number of bytes successfully written = %zu\n",count);
        pr_info("Updated file position = %lld\n",*f_pos);

        /*Return number of bytes which have been successfully written */
        return count;
			
}


int check_permission(int dev_perm, int acc_mode)
{

	if(dev_perm == RDWR)
		return 0;
	
	//ensures readonly access
	if( (dev_perm == RDONLY) && ( (acc_mode & FMODE_READ) && !(acc_mode & FMODE_WRITE) ) )
		return 0;
	
	//ensures writeonly access
	if( (dev_perm == WRONLY) && ( (acc_mode & FMODE_WRITE) && !(acc_mode & FMODE_READ) ) )
		return 0;

	return -EPERM;

}

int mychr_open(struct inode *inode, struct file *filp)
{
	pr_info("open was successful\n");
	int ret;
	int minor_n;
	
	struct mychr_device_private *mychrdev_data;

	/*find out on which device file open was attempted by the user space */
	minor_n = MINOR(inode->i_rdev);
	pr_info("minor access = %d\n",minor_n);

	/*get device's private data structure */
	mychrdev_data = container_of(inode->i_cdev,struct mychr_device_private,mychr_cdev);

	/*to supply device private data to other methods of the driver */
	filp->private_data = mychrdev_data;
		
	/*check permission */
	ret = check_permission(mychrdev_data->perm,filp->f_mode);

	(!ret)?pr_info("open was successful\n"):pr_info("open was unsuccessful\n");

	return ret;
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
	int i;

	/*1. Dynamically allocate a device number */
	ret = alloc_chrdev_region(&mychrdrv_data.device_number,0,NO_OF_DEVICES,"my_char_drv");
	if(ret < 0){
		pr_err("Alloc chrdev failed\n");
		goto out;
	}
	
	
	/*create device class under /sys/class/ */
	mychrdrv_data.class_mychr = class_create(THIS_MODULE,"mychrdrv_class");
	if(IS_ERR(mychrdrv_data.class_mychr)){
		pr_err("Class creation failed\n");
		ret = PTR_ERR(mychrdrv_data.class_mychr);
		goto cdev_del;
	}
	
	for (i=0; i < NO_OF_DEVICES; i++)
	{
		pr_info("Device number <major>:<minor> = %d:%d\n",MAJOR(mychrdrv_data.device_number+i),MINOR(mychrdrv_data.device_number+i));

		/*Initialize the cdev structure with fops*/
		cdev_init(&mychrdrv_data.mychrdev_data[i].mychr_cdev,&mychr_fops);

		/*Register a device (cdev structure) with VFS */
		mychrdrv_data.mychrdev_data[i].mychr_cdev.owner = THIS_MODULE;
		ret = cdev_add(&mychrdrv_data.mychrdev_data[i].mychr_cdev,mychrdrv_data.device_number+i,1);
		if(ret < 0){
			pr_err("Cdev add failed\n");
			goto unreg_chrdev;
		}

		/*Populate the sysfs with device information */
		mychrdrv_data.device_mychr = device_create(mychrdrv_data.class_mychr,NULL,mychrdrv_data.device_number+i,NULL,"mychrdrv-%d",1+i);
		if(IS_ERR(mychrdrv_data.device_mychr)){
			pr_err("Device create failed\n");
			ret = PTR_ERR(mychrdrv_data.device_mychr);
			goto class_del;
		}
	}
	

	pr_info("Module init was successful\n");
	return 0;


cdev_del:
class_del:
	for(;i>=0;i--){
		device_destroy(mychrdrv_data.class_mychr,mychrdrv_data.device_number+i);
		cdev_del(&mychrdrv_data.mychrdev_data[i].mychr_cdev);
	}
	class_destroy(mychrdrv_data.class_mychr);

unreg_chrdev:
	unregister_chrdev_region(mychrdrv_data.device_number,NO_OF_DEVICES);
out:
	pr_info("Module insertion failed\n");
	return ret; 
}



static void __exit mychr_driver_cleanup(void)
{
	int i;
	for(i=0;i<NO_OF_DEVICES;i++){
		device_destroy(mychrdrv_data.class_mychr,mychrdrv_data.device_number+i);
		cdev_del(&mychrdrv_data.mychrdev_data[i].mychr_cdev);
	}
	class_destroy(mychrdrv_data.class_mychr);
	unregister_chrdev_region(mychrdrv_data.device_number,NO_OF_DEVICES);
	pr_info("module unloaded\n");
}


module_init(mychr_driver_init);
module_exit(mychr_driver_cleanup);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Sarath AK");
MODULE_DESCRIPTION("A Simple character driver with multiple device nodes.");
