cmd_/home/sarath/workspace/08_LDD/ldd/custom_drivers/my_chardriver_n_devices/mychrdrv.mod := printf '%s\n'   mychrdrv.o | awk '!x[$$0]++ { print("/home/sarath/workspace/08_LDD/ldd/custom_drivers/my_chardriver_n_devices/"$$0) }' > /home/sarath/workspace/08_LDD/ldd/custom_drivers/my_chardriver_n_devices/mychrdrv.mod
