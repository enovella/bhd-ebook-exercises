#include <windows.h>
#include <stdio.h>
 
int
main (int argc, char *argv[])
{
	unsigned int	method	= 0;
	unsigned int	code	= 0;
	
	if (argc != 2) {
		fprintf (stderr, "Usage: %s <IOCTL code>\n", argv[0]);
		return 1;
	}
	
	code = strtoul (argv[1], (char **) NULL, 16);
	method = code & 3;
	
	switch (method) {
		case 0:
			printf ("METHOD_BUFFERED\n");
			break;
		case 1: 
			printf ("METHOD_IN_DIRECT\n");
			break;
		case 2:
			printf ("METHOD_OUT_DIRECT\n");
			break;
		case 3:
			printf ("METHOD_NEITHER\n");
			break;
		default:
			fprintf (stderr, "ERROR: invalid IOCTL data transfer method\n");
			break;
	}
	
	return 0;
}