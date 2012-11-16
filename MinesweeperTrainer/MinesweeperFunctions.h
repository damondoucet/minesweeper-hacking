#pragma once

#include <Windows.h>
#include <tchar.h>

typedef unsigned long long QWORD;

typedef struct _MINESWEEPER_STRUCTURE {
	QWORD unused;
	DWORD dwMines;
	DWORD dwBoardHeight;
	DWORD dwBoardWidth;
	BYTE unused2[0x40];
	QWORD lpGridArray;
} MINESWEEPER_STRUCTURE, *LPMINESWEEPER_STRUCTURE;

BOOL bReadMinesweeperStructure(HANDLE hProc, LPCVOID dwModuleBase, LPMINESWEEPER_STRUCTURE lpMinesweeperStructure);
BOOL bReadBoard(HANDLE hProc, LPMINESWEEPER_STRUCTURE lpMinesweeperStructure, DWORD **lpBoard);