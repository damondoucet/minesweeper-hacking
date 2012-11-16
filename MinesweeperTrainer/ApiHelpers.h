#pragma once

#include <Windows.h>
#include <TlHelp32.h>
#include <tchar.h>

DWORD dwFindProcessId(LPTSTR lpProcessName);
HANDLE hProcOpenProcess(DWORD dwProcessId);
LPCVOID lpLocateModuleBase(DWORD dwProcessId, LPTSTR lpModuleName);