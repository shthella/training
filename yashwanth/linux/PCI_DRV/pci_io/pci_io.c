#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/io.h>
#include <fcntl.h>

int main(int argc, char **argv) {
	int fd;
	char pci_config[128];
	unsigned char out;
	unsigned int io_addr;
	
	if (argc != 3 && argc != 4) {
		printf("Usage: %s <devfile> <r/w> <value>\n", argv[0]);
		return 1;
	}

	sprintf(pci_config, "/sys/bus/pci/devices/0000:%s/config", argv[1]);
	fd = open(pci_config, O_RDONLY);
	if (fd < 0) {
		perror("open pci config");
		return 1;
	}

	if (4 != pread(fd, &io_addr, 4,0x10)) {
		perror("read BAR0 addr");
		close(fd);
		return 1;
	}

	close(fd);

	if (io_addr == 0) {
		printf("BAR0 is not used at device %s\n", argv[1]);
		return 1;
	}
	if ((io_addr & 1) == 0) {
		printf("BAR0 of %s is memory space and not io ports!\n", argv[1]);
		return 1;
	}
	
	io_addr &= ~(1);

	printf("Accessing io ports at 0x%x\n", io_addr);

	if (ioperm(io_addr, 2, 1) != 0) {
		perror("ioperm");
		return 1;
	}

	switch (argv[2][0]) {
		case 'r':
			printf("inputs: 0x%x\n", inb(io_addr + 1));
			break;
		case 'w':
			out = (unsigned char) strtol(argv[2], NULL, 0);
			outb(out, io_addr);
			break;
		default:
			break;
	}

	if (ioperm(io_addr, 2, 0) != 0) {
		perror("ioperm");
		return 1;
	}

	return 0;
}
