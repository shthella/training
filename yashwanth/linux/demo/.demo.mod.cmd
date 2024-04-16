savedcmd_/home/admin1/linux/demo/demo.mod := printf '%s\n'   demo.o | awk '!x[$$0]++ { print("/home/admin1/linux/demo/"$$0) }' > /home/admin1/linux/demo/demo.mod
