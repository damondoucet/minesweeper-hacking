#include "ApiHelpers.h"

DWORD dwFindProcessId(LPTSTR lpProcessName) {
	PROCESSENTRY32 pe32; 

	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

	if (hSnapshot == INVALID_HANDLE_VALUE) {
		_tprintf_s(_T("Unable to enumerable processes. Error code: %d\n"), GetLastError());
		return -1;
	}

	pe32.dwSize = sizeof(PROCESSENTRY32);

	if (!Process32First(hSnapshot, &pe32)) {
		CloseHandle(hSnapshot);
		_tprintf_s(_T("Unable to enumerate processes. Error code: %d\n"), GetLastError());
		return -1;
	}

	do {
		if (_tcsicmp(pe32.szExeFile, lpProcessName) == 0) {
			CloseHandle(hSnapshot);
			return pe32.th32ProcessID;
		}
	} while (Process32Next(hSnapshot, &pe32));

	_tprintf_s(_T("Unable to find process %s. Are you sure it's running?\n"), lpProcessName);
	CloseHandle(hSnapshot);
	return -1;
}

HANDLE hProcOpenProcess(DWORD dwProcessId) {
	return OpenProcess(PROCESS_VM_READ, FALSE, dwProcessId);
}

LPCVOID lpLocateModuleBase(DWORD dwProcessId, LPTSTR lpModuleName) {
	MODULEENTRY32 me32;

	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, dwProcessId);

	if (hSnapshot == INVALID_HANDLE_VALUE) {
		_tprintf_s(_T("Unable to enumerate modules. Error code: %d\n"), GetLastError());
		return NULL;
	}

	me32.dwSize = sizeof(MODULEENTRY32);

	if (!Module32First(hSnapshot, &me32)) {
		CloseHandle(hSnapshot);
		_tprintf_s(_T("Unable to enumerate modules. Error code: %d\n"), GetLastError());
		return NULL;
	}

	do {
		if (_tcscmp(me32.szModule, lpModuleName) == 0) {
			CloseHandle(hSnapshot);
			return me32.modBaseAddr;
		}
	} while (Module32Next(hSnapshot, &me32));

	CloseHandle(hSnapshot);
	_tprintf_s(_T("Unable to find module %s.\n"), lpModuleName);
	return NULL;
}
