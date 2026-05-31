#include <iostream>
#include <winsock2.h>
#include "Header.h"
#include <string>

#pragma comment(lib, "ws2_32.lib")

std::string GetWSErrorMessage(int errorCode)
{
    LPVOID lpMsgBuffer;

    DWORD dwLength = FormatMessageA(
        FORMAT_MESSAGE_ALLOCATE_BUFFER |
        FORMAT_MESSAGE_FROM_SYSTEM |
        FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL,
        errorCode,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        (LPSTR)&lpMsgBuffer,
        0,
        NULL
    );

    std::string message;
    if (dwLength > 0)
    {
        message = static_cast<LPSTR>(lpMsgBuffer);
        if (!message.empty() && message.back() == '\n') {
            message.pop_back();
        }
        if (!message.empty() && message.back() == '\r') {
            message.pop_back();
        }
        LocalFree(lpMsgBuffer);
    }
    else
    {
        message = "Unknown error";
    }

    return message;
}