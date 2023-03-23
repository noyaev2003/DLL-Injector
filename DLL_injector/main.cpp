#include <windows.h>
#include <iostream>
#include <tchar.h>
#include <string>

#define LOAD_LIBRARY_VERSION "LoadLibraryA"


int main(int argc, char* argv[])
{

	HANDLE chosenProcess = NULL;
	int chosenProcessId = 0;
	HMODULE kernel32Handle = NULL;
	FARPROC loadLibraryAddress = NULL;
	int injectedDllPathLength = 0;
	BOOL writeProcessMemorySuccess = FALSE;
	LPVOID remoteProcessAllocatedMemory = NULL;
	HANDLE remoteThread = NULL;

	if (3 > argc)
	{
		std::cout << "Enter PID to inject and the absolute path to the desired dll" << std::endl;
		exit(-1);
	}


	chosenProcessId = std::stoi(argv[1]);
	if (0 == chosenProcessId)
	{
		std::cout << "You have entered an invalid PID number or _tstoi failed" << std::endl;
		exit(-2);
	}

	std::cout << "Chosen PID is: " << chosenProcessId << std::endl;

	chosenProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, chosenProcessId);

	if (NULL == chosenProcess)
	{
		std::cout << "OpenProcess failed." << std::endl;
		exit(-3);
	}
	std::cout << "[+] Opened process handle succesfully" << std::endl;


	kernel32Handle = GetModuleHandle(TEXT("kernel32.dll"));
	if (NULL == kernel32Handle)
	{
		std::cout << "GetModuleHandle failed." << std::endl;
		exit(-4);
	}
	std::cout << "[+] Got kernel32.dll module handle succesfully" << std::endl;


	loadLibraryAddress = GetProcAddress(kernel32Handle, LOAD_LIBRARY_VERSION);
	if (NULL == loadLibraryAddress)
	{
		std::cout << "GetProcAddress failed." << std::endl;
		exit(-5);
	}
	std::cout << "[+] LoadLibrary Address is at: 0x" << std::hex << std::uppercase << loadLibraryAddress << std::endl;


	injectedDllPathLength = strnlen(argv[2], MAX_PATH) * sizeof(argv[2][0]) + sizeof(argv[2][0]);

	remoteProcessAllocatedMemory = VirtualAllocEx(chosenProcess, NULL, injectedDllPathLength, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
	if (NULL == remoteProcessAllocatedMemory)
	{
		std::cout << "VirtualAllocEx failed." << std::endl;
		exit(-6);
	}
	std::cout << "[+] Allocated memory for dll path at: 0x" << std::hex << std::uppercase << remoteProcessAllocatedMemory << std::endl;


	writeProcessMemorySuccess = WriteProcessMemory(chosenProcess, remoteProcessAllocatedMemory, argv[2], injectedDllPathLength, NULL);
	if (!writeProcessMemorySuccess)
	{
		std::cout << "WriteProccessMemory failed" << std::endl;
		exit(-7);
	}


	remoteThread = CreateRemoteThread(chosenProcess, NULL, 0, (LPTHREAD_START_ROUTINE)loadLibraryAddress, remoteProcessAllocatedMemory, 0, NULL);
	if (NULL == remoteThread)
	{
		std::cout << "CreateRemoteThread failed." << std::endl;
		exit(-8);
	}
	std::cout << "[+] Created remote thread succesfully" << std::endl;

	CloseHandle(chosenProcess);
	CloseHandle(kernel32Handle);
	

	return 0;
}