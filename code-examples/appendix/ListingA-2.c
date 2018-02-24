#include <stdio.h>

typedef struct pkt {
	char *	value;
} pkt_t;

int
main (void)
{
	pkt_t *	packet	= NULL;

	printf ("%s", packet->value);

	return 0;
}
