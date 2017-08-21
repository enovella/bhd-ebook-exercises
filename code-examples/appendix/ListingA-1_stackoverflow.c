#include <string.h>
 
void
overflow (char *arg)
{
	char	buf[12];

	strcpy (buf, arg);
}

int
main (int argc, char *argv[])
{	
	if (argc > 1)
		overflow (argv[1]);

	return 0;
}