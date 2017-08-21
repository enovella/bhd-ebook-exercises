#include <sys/ioctl.h>

int
main (void)
{
	unsigned long	ldisc = 0xff000000;	

	ioctl (0, TIOCSETD, &ldisc);

	return 0;
}