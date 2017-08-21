#include <stdio.h>

unsigned int
get_user_length (void)
{
	return (0xffffffff);
}

int
main (void)
{
	signed int length = 0;

	length = get_user_length ();
	
	printf ("length: %d %u (0x%x)\n", length, length, length);
	
	if (length < 12)
		printf ("argument length ok\n");
	else
		printf ("Error: argument length too long\n");
	
	return 0;
}