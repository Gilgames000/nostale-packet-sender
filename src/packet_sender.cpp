#include <windows.h>
#include <stdio.h>

extern "C"
{
#include "memscan.h"
#include "pipe_client.h"
#include "packet_sender.h"
#include "nostale_string.h"

    const BYTE SEND_PATTERN[] = {0x53, 0x56, 0x8B, 0xF2, 0x8B, 0xD8, 0xEB, 0x04};
    const BYTE PACKET_THIS_PATTERN[] = {0xA1, 0x00, 0x00, 0x00, 0x00, 0x8B, 0x00, 0xE8, 0x00, 0x00, 0x00, 0x00, 0xA1, 0x00, 0x00, 0x00, 0x00, 0x8B, 0x00, 0x33, 0xD2, 0x89, 0x10};
    LPCSTR SEND_MASK = "xxxxxxxx";
    LPCSTR PACKET_THIS_MASK = "x????xxx????x????xxxxxx";

    LPVOID lpvSendAddy;
    LPVOID lpvPacketThis;

    LPCSTR PIPE_NAME = "\\\\.\\pipe\\nt_snd_42";
    HANDLE hPipe;

    void SendPacket(LPCSTR szPacket)
    {
        NostaleStringA str(szPacket);
        char *packet = str.get();

        _asm {
            MOV EAX, DWORD PTR DS : [lpvPacketThis]
            MOV EAX, DWORD PTR DS : [EAX]
            MOV EAX, DWORD PTR DS : [EAX]
            MOV EDX, packet
            CALL lpvSendAddy
        }
    }

    DWORD WINAPI PacketSender(LPVOID lpQueue)
    {
        LPSTR szPacket;

        for (;;)
        {
            BOOL ok = ReadFromPipe(hPipe, &szPacket);
            if (!ok)
            {
                printf("[INFO]: Failed to read from the pipe; aborting");
                break;
            }

            SendPacket(szPacket);
            // free(szPacket)? The client could be freeing the packets by itself
            // and by freeing it here we may incur in a situation where the send
            // function being executed asynchronously has yet to send our packet
            // when it's already gone. On the other hand, if the client doesn't
            // clean up by itself, we'd be leaking memory with every sent packet.
        }

        return TRUE;
    }

    BOOL StartSender()
    {
#ifdef NT_SND_DEBUG
        FILE *pDummy;

        AllocConsole();
        freopen_s(&pDummy, "CONIN$", "r", stdin);
        freopen_s(&pDummy, "CONOUT$", "w", stdout);
        freopen_s(&pDummy, "CONOUT$", "w", stderr);
#endif

        if (!FindAddresses())
            return FALSE;

        hPipe = ConnectPipe(PIPE_NAME);

        printf("[INFO]: Starting sender thread... ");
        CreateThread(NULL, 0, PacketSender, NULL, 0, NULL);
        printf("DONE\n");

        return TRUE;
    }

    BOOL StopSender()
    {
#ifdef NT_SND_DEBUG
        FreeConsole();
#endif
        return TRUE;
    }

    BOOL FindAddresses()
    {
        printf("[INFO]: Scanning process memory to find send address... ");
        lpvSendAddy = FindPattern(SEND_PATTERN, SEND_MASK);
        printf("%s\n", lpvSendAddy ? "SUCCESS" : "FAILURE");

        printf("[INFO]: Scanning process memory to find packet this address... ");
        DWORD pThisPacket = (DWORD_PTR)FindPattern(PACKET_THIS_PATTERN, PACKET_THIS_MASK) + 0x1;
        lpvPacketThis = (LPVOID) * (DWORD *)pThisPacket;
        printf("%s\n", lpvPacketThis ? "SUCCESS" : "FAILURE");

        return lpvSendAddy && lpvPacketThis;
    }
}