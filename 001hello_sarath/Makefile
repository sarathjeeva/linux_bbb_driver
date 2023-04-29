obj-m := main.o
ARCH=arm
KERN_DIR=/lib/modules/4.14.108+/build
HOST_KERN=/lib/modules/$(shell uname -r)/build
all:
	make ARCH=$(ARCH) CROSS_COMPILE=arm-linux-gnueabihf- -C $(KERN_DIR) M=$(PWD) modules

clean:
	make ARCH=$(ARCH) CROSS_COMPILE=arm-linux-gnueabihf- -C $(KERN_DIR) M=$(PWD) clean
cp:
	scp main.ko debian@192.168.29.235:/home/debian
host:
	make -C $(HOST_KERN) M=$(PWD) modules
