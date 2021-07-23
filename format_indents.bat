@echo off
set style1="{SpaceBeforeParens: Never, IndentWidth: 4, SortIncludes: true, SortUsingDeclarations: true , UseTab: Never,PointerAlignment: Left, BreakBeforeBraces: Attach, TabWidth: 4,ColumnLimit: 120 }"
set format_tool="C:\Program Files\LLVM\bin\clang-format.exe"

%format_tool%  -verbose -i -style=%style1% C:\projects\code_base\components\gpio\gpio_drv.h
%format_tool%  -verbose -i -style=%style1% C:\projects\code_base\components\gpio\gpio_drv.c

%format_tool%  -verbose -i -style=%style1% C:\projects\code_base\components\uart\uart_drv.h
%format_tool%  -verbose -i -style=%style1% C:\projects\code_base\components\uart\uart_drv.c
%format_tool%  -verbose -i -style=%style1% C:\projects\code_base\launchxl_cc26x2r1_v2\main.c

%format_tool%  -verbose -i -style=%style1% C:\projects\code_base\utils\utils_sys\diag_sys.h
%format_tool%  -verbose -i -style=%style1% C:\projects\code_base\utils\utils_sys\diag_sys.c

%format_tool%  -verbose -i -style=%style1% C:\projects\code_base\utils\debug_info.h
%format_tool%  -verbose -i -style=%style1% C:\projects\code_base\utils\debug_info.c

%format_tool%  -verbose -i -style=%style1% C:\projects\code_base\components\core_drv\core_commands.h
%format_tool%  -verbose -i -style=%style1% C:\projects\code_base\components\core_drv\core_commands.c


%format_tool%  -verbose -i -style=%style1% C:\projects\code_base\components\cli_drv\shell.h
%format_tool%  -verbose -i -style=%style1% C:\projects\code_base\components\cli_drv\shell.c

%format_tool%  -verbose -i -style=%style1% C:\projects\code_base\components\uart\uart_commands.h
%format_tool%  -verbose -i -style=%style1% C:\projects\code_base\components\uart\uart_commands.c
