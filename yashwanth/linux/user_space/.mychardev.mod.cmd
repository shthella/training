savedcmd_/home/admin1/linux/user_space/mychardev.mod := printf '%s\n'   mychardev.o | awk '!x[$$0]++ { print("/home/admin1/linux/user_space/"$$0) }' > /home/admin1/linux/user_space/mychardev.mod
