cmd_/home/sarath/workspace/08_LDD/ldd/custom_drivers/my_chardriver/modules.order := {   echo /home/sarath/workspace/08_LDD/ldd/custom_drivers/my_chardriver/mychrdrv.ko; :; } | awk '!x[$$0]++' - > /home/sarath/workspace/08_LDD/ldd/custom_drivers/my_chardriver/modules.order