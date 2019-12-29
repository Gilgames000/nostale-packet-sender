#include <windows.h>
#include <stdio.h>
#include "pipe_client.h"

HANDLE ConnectPipe(LPCSTR szPipeName)
{
    HANDLE hPipe;

    for (;;)
    {
        printf("[INFO]: Trying to connect to the pipe server at '%s'... ", szPipeName);
        hPipe = CreateFileA(
            szPipeName,
            GENERIC_READ,
            0,    // no sharing
            NULL, // default security attributes
            OPEN_EXISTING,
            0,     // default attributes
            NULL); // no template file

        if (hPipe != INVALID_HANDLE_VALUE)
        {
            printf("SUCCESS\n");
            break;
        }
        printf("FAILURE\n");

        printf("[INFO]: The pipe server may be busy or offline; waiting for a server...\n");
        while (!WaitNamedPipeA(szPipeName, NMPWAIT_WAIT_FOREVER))
            Sleep(5 * 1000);

        printf("[INFO]: Pipe server found\n");
    }

    return hPipe;
}

BOOL DisconnectPipe(HANDLE hPipe)
{
    printf("[INFO]: Pipe server disconnected\n");
    return CloseHandle(hPipe);
}

BOOL ReadFromPipe(HANDLE hPipe, LPSTR *lpszMessage)
{
    BOOL bSuccess;
    DWORD cbRead;
    DWORD dwBufSize;

    dwBufSize = 512;
    *lpszMessage = malloc(dwBufSize);

    bSuccess = ReadFile(
        hPipe,                        // pipe handle
        *lpszMessage,                 // buffer to receive reply
        dwBufSize * sizeof(char) - 1, // bytes to read
        &cbRead,                      // number of bytes read
        NULL);                        // not overlapped

    (*lpszMessage)[cbRead] = '\0';

#ifdef NT_SND_LOG
    if (bSuccess)
        printf("[PIPE]: %s\n", *lpszMessage);
#endif

    return bSuccess;
}