apns:
	gcc  app_apns.c apns_db_api.c apns_cmd_msg.c -o apns -ffunction-sections -fdata-sections -Wl,-gc-sections -I /mnt/hgfs/share/apnsdemo -I /mnt/hgfs/share/product/include /mnt/hgfs/share/product/lib/libsqlite3.a -ldl -lm -lpthread
	#mips-linux-uclibc-gnu-strip apns
	#cp -f apns /ingenic/Zeratul_Release_20201230/os/rootfs/4.7.2/camera/system/bin/
mcc:
	gcc mccdemo.c apns_cmd_msg.c apns_db_api.c -o mccdemo -ffunction-sections -fdata-sections -Wl,-gc-sections -I /mnt/hgfs/share/apnsdemo -I /mnt/hgfs/share/product/include /mnt/hgfs/share/product/lib/libsqlite3.a -ldl -lm -lpthread
