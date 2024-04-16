savedcmd_/home/admin1/linux/scull1/scull1.mod := printf '%s\n'   scull1.o | awk '!x[$$0]++ { print("/home/admin1/linux/scull1/"$$0) }' > /home/admin1/linux/scull1/scull1.mod
