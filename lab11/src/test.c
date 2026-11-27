#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include "whs31_chardev.h"

int main(void)
{
	const char *msg = "Привет от whs31!";
	char buf[128] = {0};
	int size;
	int fd = open("/dev/whs31_chardev", O_RDWR);
	if (fd < 0) { perror("open"); return 1; }

	ioctl(fd, WHS_IOC_CLEAR);
	printf("write: %zd байт\n", write(fd, msg, strlen(msg)));
	ioctl(fd, WHS_IOC_GETSIZE, &size);
	printf("ioctl GETSIZE: %d\n", size);

	lseek(fd, 0, SEEK_SET);
	printf("read: %zd байт -> \"%s\"\n", read(fd, buf, sizeof(buf) - 1), buf);

	lseek(fd, -6, SEEK_END);
	memset(buf, 0, sizeof(buf));
	printf("lseek(-6, SEEK_END) + read: \"%s\"\n", (read(fd, buf, sizeof(buf) - 1), buf));

	close(fd);
	return 0;
}
