#include "stdafx.h"

int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	// !!! REPLACE ClassName AND WindowName WITH YOUR winmine.exe's
	HWND hwnd = FindWindow(_T("É¨À×"), _T("É¨À×"));
	if(hwnd == NULL)
	{
		MessageBox(NULL, _T("Winmine is not found!"), _T("AutoMineSweeper"), MB_ICONSTOP);
		return 1;
	}
	DWORD pid;
	GetWindowThreadProcessId(hwnd, &pid);
	HANDLE handle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
	
	int width, height;
	ReadProcessMemory(handle, (LPVOID)0x1005334, &width, sizeof(int), NULL);
	ReadProcessMemory(handle, (LPVOID)0x1005338, &height, sizeof(int), NULL);
	
	ShowWindow(hwnd, SW_RESTORE);
	SetForegroundWindow(hwnd);
	Sleep(300);
	for(int y = 1; y <= height; y++)
		for(int x = 1; x <= width; x++)
		{
			BYTE data;
			ReadProcessMemory(handle, (LPVOID)(0x1005340 + y * 32 + x), &data, sizeof(BYTE), NULL);
			UINT downMsg = 0, upMsg;
			if(data == 0x0F) // (data & 1<<7) == 0
			{
				downMsg = MOUSEEVENTF_LEFTDOWN;
				upMsg   = MOUSEEVENTF_LEFTUP;
			}
			else if(data == 0x8F)
			{
				downMsg = MOUSEEVENTF_RIGHTDOWN;
				upMsg   = MOUSEEVENTF_RIGHTUP;
			}
			if(downMsg != 0)
			{
				RECT rect;
				GetClientRect(hwnd, &rect);
				POINT curPos = {rect.left + 20 + (x - 1) * 16
    			              , rect.top  + 63 + (y - 1) * 16};
				ClientToScreen(hwnd, &curPos);
				SetCursorPos(curPos.x, curPos.y);
				mouse_event(downMsg, 0, 0, 0, 0);
				mouse_event(upMsg, 0, 0, 0, 0);
				Sleep(20);
			}
		}

	CloseHandle(handle);
	
	return 0;
}
