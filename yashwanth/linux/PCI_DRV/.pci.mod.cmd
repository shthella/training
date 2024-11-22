savedcmd_/home/admin1/Documents/linux/pci_drv/pci.mod := printf '%s\n'   pci.o | awk '!x[$$0]++ { print("/home/admin1/Documents/linux/pci_drv/"$$0) }' > /home/admin1/Documents/linux/pci_drv/pci.mod
