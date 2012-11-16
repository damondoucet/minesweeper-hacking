#include <stdio.h>
#include "ApiHelpers.h"
#include "MinesweeperFunctions.h"

DWORD **AllocateBoard(DWORD dwHeight, DWORD dwWidth) {
	DWORD **lpBoard = new DWORD*[dwHeight];
	
	for (unsigned i = 0; i < dwHeight; i++)
		lpBoard[i] = new DWORD[dwWidth];

	return lpBoard;
}

void FreeBoard(DWORD **lpBoard, DWORD dwHeight) {
	for (unsigned i = 0; i < dwHeight; i++)
		delete lpBoard[i];

	delete lpBoard;
}

int main() {
	DWORD dwProcessId = dwFindProcessId(_T("minesweeper.exe"));

	if (dwProcessId == -1) {
		getchar();
		return 1;
	}

	HANDLE hProc = hProcOpenProcess(dwProcessId);

	if (hProc == INVALID_HANDLE_VALUE) {
		_tprintf_s(_T("Unable to open process. Error code: %d\n"), GetLastError());
		getchar();
		return 1;
	}

	LPCVOID lpModuleBaseAddr = lpLocateModuleBase(dwProcessId, _T("minesweeper.exe"));

	if (lpModuleBaseAddr == NULL) {
		getchar();
		return 1;
	}

	MINESWEEPER_STRUCTURE ms;
	if (!bReadMinesweeperStructure(hProc, lpModuleBaseAddr, &ms)) {
		getchar();
		return 1;
	}

	_tprintf_s(_T("Mines: %d\nHeight: %d\nWidth: %d\nArray Ptr: %x\n"), 
		ms.dwMines,
		ms.dwBoardHeight,
		ms.dwBoardWidth,
		ms.lpGridArray);

	DWORD **lpBoard = AllocateBoard(ms.dwBoardHeight, ms.dwBoardWidth);

	if (!bReadBoard(hProc, &ms, lpBoard)) {
		getchar();
		FreeBoard(lpBoard, ms.dwBoardWidth);
		return 1;
	}

	for (unsigned i = 0; i < ms.dwBoardHeight; i++)
		for (unsigned j = 0; j < ms.dwBoardWidth; j++)
			_tprintf_s(_T("%d%c"), lpBoard[i][j], j == ms.dwBoardWidth - 1 ? '\n' : ' ');

	getchar();
	
	FreeBoard(lpBoard, ms.dwBoardHeight);

	return 0;
}