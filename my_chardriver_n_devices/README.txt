



sarath@Sarath-LinuX:~/workspace/08_LDD/ldd/custom_drivers/my_chardriver_n_devices$ make host
make -C /lib/modules/5.19.1-051901-generic/build/ M=/home/sarath/workspace/08_LDD/ldd/custom_drivers/my_chardriver_n_devices modules
make[1]: Entering directory '/usr/src/linux-headers-5.19.1-051901-generic'
warning: the compiler differs from the one used to build the kernel
  The kernel was built by: gcc (Ubuntu 11.3.0-5ubuntu1) 11.3.0
  You are using:           gcc (Ubuntu 11.3.0-1ubuntu1~22.04) 11.3.0
  CC [M]  /home/sarath/workspace/08_LDD/ldd/custom_drivers/my_chardriver_n_devices/mychrdrv.o
/home/sarath/workspace/08_LDD/ldd/custom_drivers/my_chardriver_n_devices/mychrdrv.c: In function ‘mychr_read’:
/home/sarath/workspace/08_LDD/ldd/custom_drivers/my_chardriver_n_devices/mychrdrv.c:117:5: warning: ISO C90 forbids mixed declarations and code [-Wdeclaration-after-statement]
  117 |     struct mychr_device_private *mychrdev_data = (struct mychr_device_private*)filp->private_data;
      |     ^~~~~~
/home/sarath/workspace/08_LDD/ldd/custom_drivers/my_chardriver_n_devices/mychrdrv.c: In function ‘mychr_write’:
/home/sarath/workspace/08_LDD/ldd/custom_drivers/my_chardriver_n_devices/mychrdrv.c:144:17: warning: ISO C90 forbids mixed declarations and code [-Wdeclaration-after-statement]
  144 |                 struct mychr_device_private *mychrdev_data = (struct mychr_device_private*)filp->private_data;
      |                 ^~~~~~
/home/sarath/workspace/08_LDD/ldd/custom_drivers/my_chardriver_n_devices/mychrdrv.c: In function ‘mychr_open’:
/home/sarath/workspace/08_LDD/ldd/custom_drivers/my_chardriver_n_devices/mychrdrv.c:194:9: warning: ISO C90 forbids mixed declarations and code [-Wdeclaration-after-statement]
  194 |         int ret;
      |         ^~~
  MODPOST /home/sarath/workspace/08_LDD/ldd/custom_drivers/my_chardriver_n_devices/Module.symvers
  CC [M]  /home/sarath/workspace/08_LDD/ldd/custom_drivers/my_chardriver_n_devices/mychrdrv.mod.o
  LD [M]  /home/sarath/workspace/08_LDD/ldd/custom_drivers/my_chardriver_n_devices/mychrdrv.ko
  BTF [M] /home/sarath/workspace/08_LDD/ldd/custom_drivers/my_chardriver_n_devices/mychrdrv.ko
Skipping BTF generation for /home/sarath/workspace/08_LDD/ldd/custom_drivers/my_chardriver_n_devices/mychrdrv.ko due to unavailability of vmlinux
make[1]: Leaving directory '/usr/src/linux-headers-5.19.1-051901-generic'
sarath@Sarath-LinuX:~/workspace/08_LDD/ldd/custom_drivers/my_chardriver_n_devices$ 
sarath@Sarath-LinuX:~/workspace/08_LDD/ldd/custom_drivers/my_chardriver_n_devices$ 





sarath@Sarath-LinuX:~/workspace/08_LDD/ldd/custom_drivers/my_chardriver_n_devices$ sudo insmod mychrdrv.ko 
sarath@Sarath-LinuX:~/workspace/08_LDD/ldd/custom_drivers/my_chardriver_n_devices$ 
sarath@Sarath-LinuX:~/workspace/08_LDD/ldd/custom_drivers/my_chardriver_n_devices$ sudo dmesg 
[ 3649.950725] [UFW BLOCK] IN=wlp0s20f3 OUT= MAC=01:00:5e:00:00:01:8c:a3:99:91:6e:75:08:00 SRC=192.168.29.1 DST=224.0.0.1 LEN=36 TOS=0x00 PREC=0xC0 TTL=1 ID=22748 PROTO=2 
[ 3655.409282] mychr_driver_init : Device number <major>:<minor> = 509:0
[ 3655.409533] mychr_driver_init : Device number <major>:<minor> = 509:1
[ 3655.409706] mychr_driver_init : Device number <major>:<minor> = 509:2
[ 3655.409825] mychr_driver_init : Device number <major>:<minor> = 509:3
[ 3655.409884] mychr_driver_init : Module init was successful
sarath@Sarath-LinuX:~/workspace/08_LDD/ldd/custom_drivers/my_chardriver_n_devices$ 
sarath@Sarath-LinuX:~/workspace/08_LDD/ldd/custom_drivers/my_chardriver_n_devices$ 




sarath@Sarath-LinuX:~/workspace/08_LDD/ldd/custom_drivers/my_chardriver_n_devices$ ls -l /dev/mychrdrv-*
crw------- 1 root root 509, 0 May 12 20:50 /dev/mychrdrv-1
crw------- 1 root root 509, 1 May 12 20:50 /dev/mychrdrv-2
crw------- 1 root root 509, 2 May 12 20:50 /dev/mychrdrv-3
crw------- 1 root root 509, 3 May 12 20:50 /dev/mychrdrv-4
sarath@Sarath-LinuX:~/workspace/08_LDD/ldd/custom_drivers/my_chardriver_n_devices$ 



sarath@Sarath-LinuX:/sys/class/mychrdrv_class$ ls
mychrdrv-1  mychrdrv-2  mychrdrv-3  mychrdrv-4
sarath@Sarath-LinuX:/sys/class/mychrdrv_class$ ls -l
total 0
lrwxrwxrwx 1 root root 0 May 12 20:52 mychrdrv-1 -> ../../devices/virtual/mychrdrv_class/mychrdrv-1
lrwxrwxrwx 1 root root 0 May 12 20:52 mychrdrv-2 -> ../../devices/virtual/mychrdrv_class/mychrdrv-2
lrwxrwxrwx 1 root root 0 May 12 20:52 mychrdrv-3 -> ../../devices/virtual/mychrdrv_class/mychrdrv-3
lrwxrwxrwx 1 root root 0 May 12 20:52 mychrdrv-4 -> ../../devices/virtual/mychrdrv_class/mychrdrv-4
sarath@Sarath-LinuX:/sys/class/mychrdrv_class$ 
sarath@Sarath-LinuX:/sys/class/mychrdrv_class$ 





root@Sarath-LinuX:~# echo "Tata Sons Pvt. Ltd. is the parent company of the Tata Group and holds the bulk of shareholding in the Tata group of companies including their land holdings across India, tea estates and steel plants. It is a privately owned conglomerate of nearly 100 companies encompassing several primary business sectors, including: chemicals, consumer products, energy, engineering, information systems, materials, and services. Its headquarters are in" > /dev/mychrdrv-3 
root@Sarath-LinuX:~# 
root@Sarath-LinuX:~# dmesg 
[ 6077.597467] [UFW BLOCK] IN=wlp0s20f3 OUT= MAC=01:00:5e:00:00:01:8c:a3:99:91:6e:75:08:00 SRC=192.168.29.1 DST=224.0.0.1 LEN=36 TOS=0x00 PREC=0xC0 TTL=1 ID=5873 PROTO=2 
[ 6080.455314] mychr_open : open was successful
[ 6080.455324] mychr_open : minor access = 2
[ 6080.455328] mychr_open : open was successful
[ 6080.455358] mychr_write : Write requested for 440 bytes
[ 6080.455361] mychr_write : Current file position = 0
[ 6080.455364] mychr_write : Number of bytes successfully written = 440
[ 6080.455367] mychr_write : Updated file position = 440
[ 6080.455400] mychr_release : release was successful
root@Sarath-LinuX:~# 
root@Sarath-LinuX:~# 
root@Sarath-LinuX:~# 
root@Sarath-LinuX:~# cat /dev/mychrdrv-2 
cat: /dev/mychrdrv-2: Operation not permitted
root@Sarath-LinuX:~# cat /dev/mychrdrv-3 
Tata Sons Pvt. Ltd. is the parent company of the Tata Group and holds the bulk of shareholding in the Tata group of companies including their land holdings across India, tea estates and steel plants. It is a privately owned conglomerate of nearly 100 companies encompassing several primary business sectors, including: chemicals, consumer products, energy, engineering, information systems, materials, and services. Its headquarters are in
root@Sarath-LinuX:~# dmesg 
[ 6077.597467] [UFW BLOCK] IN=wlp0s20f3 OUT= MAC=01:00:5e:00:00:01:8c:a3:99:91:6e:75:08:00 SRC=192.168.29.1 DST=224.0.0.1 LEN=36 TOS=0x00 PREC=0xC0 TTL=1 ID=5873 PROTO=2 
[ 6080.455314] mychr_open : open was successful
[ 6080.455324] mychr_open : minor access = 2
[ 6080.455328] mychr_open : open was successful
[ 6080.455358] mychr_write : Write requested for 440 bytes
[ 6080.455361] mychr_write : Current file position = 0
[ 6080.455364] mychr_write : Number of bytes successfully written = 440
[ 6080.455367] mychr_write : Updated file position = 440
[ 6080.455400] mychr_release : release was successful
[ 6097.707366] [UFW BLOCK] IN=wlp0s20f3 OUT= MAC=01:00:5e:00:00:01:8c:a3:99:91:6e:75:08:00 SRC=192.168.29.1 DST=224.0.0.1 LEN=36 TOS=0x00 PREC=0xC0 TTL=1 ID=6672 PROTO=2 
[ 6102.419003] mychr_open : open was successful
[ 6102.419009] mychr_open : minor access = 1
[ 6102.419012] mychr_open : open was unsuccessful
[ 6111.068484] mychr_open : open was successful
[ 6111.068490] mychr_open : minor access = 2
[ 6111.068493] mychr_open : open was successful
[ 6111.068504] mychr_read : Read requested for 131072 bytes 
[ 6111.068509] mychr_read : Number of bytes successfully read = 1024
[ 6111.068511] mychr_read : Updated file position = 1024
[ 6111.068526] mychr_read : Read requested for 131072 bytes 
[ 6111.068528] mychr_read : Number of bytes successfully read = 0
[ 6111.068529] mychr_read : Updated file position = 1024
[ 6111.068538] mychr_release : release was successful
[ 6117.717479] [UFW BLOCK] IN=wlp0s20f3 OUT= MAC=01:00:5e:00:00:01:8c:a3:99:91:6e:75:08:00 SRC=192.168.29.1 DST=224.0.0.1 LEN=36 TOS=0x00 PREC=0xC0 TTL=1 ID=7766 PROTO=2 
root@Sarath-LinuX:~# 





