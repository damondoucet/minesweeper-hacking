#include "MinesweeperFunctions.h"
#include <stdio.h>

#define ModuleOffset 0xAAA38
#define StaticPtrOffset 0x18
#define ListOffset 0x10

BOOL bReadMinesweeperStructure(HANDLE hProc, LPCVOID lpModuleBase, LPMINESWEEPER_STRUCTURE lpMinesweeper) {
	QWORD qwStaticAddr;
	QWORD qwStructureBaseAddr;
	SIZE_T nBytes;

	ReadProcessMemory(hProc, (LPCVOID)((DWORD)lpModuleBase + ModuleOffset), &qwStaticAddr, sizeof(QWORD), &nBytes);

	if (nBytes != sizeof(QWORD)) {
		_tprintf_s(_T("Unable to read static pointer. Error code: %d\n"), GetLastError());
		return FALSE;
	}

	ReadProcessMemory(hProc, (LPCVOID)(qwStaticAddr + StaticPtrOffset), &qwStructureBaseAddr, sizeof(QWORD), &nBytes);

	if (nBytes != sizeof(QWORD)) {
		_tprintf_s(_T("Unable to read base structure address. Error code: %d\n"), GetLastError());
		return FALSE;
	}

	ReadProcessMemory(hProc, (LPCVOID)qwStructureBaseAddr, lpMinesweeper, sizeof(MINESWEEPER_STRUCTURE), &nBytes);

	if (nBytes != sizeof(MINESWEEPER_STRUCTURE)) {
		_tprintf_s(_T("Unable to read minesweeper structure. Error code: %d\n"), GetLastError());
		return FALSE;
	}

	return TRUE;
}

BOOL bReadBoard(HANDLE hProc, LPMINESWEEPER_STRUCTURE lpMinesweeperStructure, DWORD **lpBoard) {
	BYTE *lpCol;

	QWORD lpArrayStructBase = lpMinesweeperStructure->lpGridArray;
	QWORD qwPointerListAddr, *lpPointerList, qwColumnAddr;
	SIZE_T nBytes;

	ReadProcessMemory(hProc, (LPCVOID)(lpArrayStructBase + ListOffset), &qwPointerListAddr, sizeof(QWORD), &nBytes);

	if (nBytes != sizeof(QWORD)) {
		_tprintf_s(_T("Unable to read pointer list base. Error code: %d\n"), GetLastError());
		return FALSE;
	}
	
	lpPointerList = new QWORD[lpMinesweeperStructure->dwBoardWidth];

	ReadProcessMemory(hProc, (LPCVOID)qwPointerListAddr, lpPointerList, sizeof(QWORD) * lpMinesweeperStructure->dwBoardWidth, &nBytes);

	if (nBytes != sizeof(QWORD) * lpMinesweeperStructure->dwBoardWidth) {
		_tprintf_s(_T("Unable to read pointer list. Error code: %d\n"), GetLastError());
		return FALSE;
	}

	lpCol = new BYTE[lpMinesweeperStructure->dwBoardHeight]; //game stores board in columns

	for (unsigned i = 0; i < lpMinesweeperStructure->dwBoardWidth; i++) {
		ReadProcessMemory(hProc, (LPCVOID)(lpPointerList[i] + ListOffset), &qwColumnAddr, sizeof(QWORD), &nBytes);

		if (nBytes != sizeof(QWORD)) {
			_tprintf_s(_T("Unable to read column %d base. Error code: %d\n"), GetLastError());
			delete lpCol;
			delete lpPointerList;
			return FALSE;
		}

		ReadProcessMemory(hProc, (LPCVOID)qwColumnAddr, lpCol, sizeof(BYTE) * lpMinesweeperStructure->dwBoardHeight, &nBytes);

		if (nBytes != sizeof(BYTE) * lpMinesweeperStructure->dwBoardHeight) {
			_tprintf_s(_T("Unable to read column %d. Error code: %d\n"), GetLastError());
			delete lpCol;
			delete lpPointerList;
			return FALSE;
		}

		for (unsigned j = 0; j < lpMinesweeperStructure->dwBoardHeight; j++) {
			lpBoard[j][i] = lpCol[j];
		}
	}

	delete lpPointerList;
	delete lpCol;

	return TRUE;
}