#pragma once
#include <iostream>

inline void PrintHResult(HRESULT hr)
{
    LPSTR message = nullptr;

    FormatMessageA(
        FORMAT_MESSAGE_ALLOCATE_BUFFER |
        FORMAT_MESSAGE_FROM_SYSTEM |
        FORMAT_MESSAGE_IGNORE_INSERTS,
        nullptr,
        hr,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        (LPSTR)&message,
        0,
        nullptr);

    if (message)
    {
        std::cout << message << std::endl;
        LocalFree(message);
    }
}