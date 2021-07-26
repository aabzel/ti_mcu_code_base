@echo off
set work_space_dir=C:\projects\code_base
set style1="{SpaceBeforeParens: Never, IndentWidth: 4, SortIncludes: true, SortUsingDeclarations: true , UseTab: Never,PointerAlignment: Left, BreakBeforeBraces: Attach, TabWidth: 4,ColumnLimit: 120 }"
set format_tool="C:\Program Files\LLVM\bin\clang-format.exe"

%format_tool%  -verbose -i -style=%style1% %work_space_dir%\components\cli_drv\cli_manager.c
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\components\gpio\gpio_drv.h
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\components\gpio\gpio_drv.c

%format_tool%  -verbose -i -style=%style1% %work_space_dir%\components\uart\uart_drv.h
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\components\uart\uart_drv.c
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\launchxl_cc26x2r1_v2\main.c

%format_tool%  -verbose -i -style=%style1% %work_space_dir%\utils\utils_sys\diag_sys.h
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\utils\utils_sys\diag_sys.c

%format_tool%  -verbose -i -style=%style1% %work_space_dir%\utils\debug_info.h
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\utils\debug_info.c

%format_tool%  -verbose -i -style=%style1% %work_space_dir%\components\core_drv\core_commands.h
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\components\core_drv\core_commands.c

%format_tool%  -verbose -i -style=%style1% %work_space_dir%\components\cli_drv\shell.h
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\components\cli_drv\shell.c

%format_tool%  -verbose -i -style=%style1% %work_space_dir%\components\uart\uart_commands.h
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\components\uart\uart_commands.c
