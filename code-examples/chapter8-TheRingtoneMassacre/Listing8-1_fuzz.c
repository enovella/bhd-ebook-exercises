#include <stdio.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>

int 
main (int argc, char *argv[]) 
{
	int				fd			= 0;
	char *			p			= NULL;
	char *			name		= NULL;
	unsigned int	file_size	= 0;
	unsigned int	file_offset	= 0;
	unsigned int	file_value	= 0;

	if (argc < 2) {
		printf ("[-] Error: not enough arguments\n");
		return (1);
	} else {
		file_size	= atol (argv[1]);
		file_offset	= atol (argv[2]);
		file_value	= atol (argv[3]);
		name		= argv[4];
	}

	// open file
	fd = open (name, O_RDWR);
	if (fd < 0) {
		perror ("open");
		exit (1);
	}

	// mmap file
	p = mmap (0, file_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	if ((int) p == -1) {
		perror ("mmap");
		close (fd);
		exit (1);
	}

	// mutate file
	printf ("[+] file offset: 0x%08x (value: 0x%08x)\n", file_offset, file_value);
	fflush (stdout);
	p[file_offset] = file_value;

	close (fd);
	munmap (p, file_size);

	return (0);
}