#include "win_utils.h"

#include <windows.h>

#include <Assert.h>
#include <Iphlpapi.h>
#include <stdio.h>

#include "convert.h"

void clear_tui(void) { system("cmd /c cls"); }

