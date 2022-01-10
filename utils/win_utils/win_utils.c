#include "win_utils.h"

#include <windows.h>

#include <Assert.h>
#include <Iphlpapi.h>
#include <stdio.h>

#include "convert.h"

void clear_tui(void) { system("cmd /c cls"); }

#ifndef ENABLE_VIRTUAL_TERMINAL_PROCESSING
#define ENABLE_VIRTUAL_TERMINAL_PROCESSING 0x0004
#endif


void win_color_enable(void) {
#if defined WIN32 || defined _WIN32 || defined WIN64 || defined _WIN64
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD dwMode = 0;
    GetConsoleMode(hOut, &dwMode);
    dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    SetConsoleMode(hOut, dwMode);

    // References:
    // SetConsoleMode() and ENABLE_VIRTUAL_TERMINAL_PROCESSING?
    // https://stackoverflow.com/questions/38772468/setconsolemode-and-enable-virtual-terminal-processing

    // Windows console with ANSI colors handling
    // https://superuser.com/questions/413073/windows-console-with-ansi-colors-handling
#endif
}