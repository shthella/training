savedcmd_/home/admin1/linux/scull2/scull2.mod := printf '%s\n'   scull2.o | awk '!x[$$0]++ { print("/home/admin1/linux/scull2/"$$0) }' > /home/admin1/linux/scull2/scull2.mod
