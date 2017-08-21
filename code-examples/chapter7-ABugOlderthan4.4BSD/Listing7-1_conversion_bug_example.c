typedef char *	caddr_t;

// output the bit pattern
void
bitpattern (int a)
{
	int				m		= 0;
	int				b		= 0;
	int				cnt		= 0;
	int				nbits	= 0;
	unsigned int	mask	= 0;

	nbits = 8 * sizeof (int);
	m = 0x1 << (nbits - 1);

	mask = m;
	for (cnt = 1; cnt <= nbits; cnt++) {
		b = (a & mask) ? 1 : 0;
		printf ("%x", b);
		if (cnt % 4 == 0)
			printf (" ");
		mask >>= 1;
	}
	printf ("\n");
}

int
main ()
{
	caddr_t	data	= "\xff\xff\xff\xff";
	int		t		= 0;

	t = *(int *)data;

	printf ("Bit pattern of t: ");
	bitpattern (t);

	printf ("t = %d (0x%08x)\n", t, t);

	return 0;
}