#pragma once
#include <windows.h>

HANDLE ConnectPipe(LPCSTR szPipeName);
BOOL DisconnectPipe(HANDLE hPipe);
BOOL ReadFromPipe(HANDLE hPipe, LPSTR *lpszMessage);