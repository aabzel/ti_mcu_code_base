rem echo off
echo %0
set workspace_dir=%1
set project_name=%2
set project_dir=%workspace_dir%%project_name%
echo project_dir=%project_dir%
set workspace_dir=%project_dir%\..\

FOR /F "tokens=* delims=" %%a IN ('git branch') DO (
 set branch_var=%%a
)

FOR /F "tokens=*delims=" %%a IN ('git rev-parse --short HEAD') DO (
 set last_hash_var=%%a
)
rem echo branch_var=%branch_var% 
set branch_var=%branch_var:~2%
echo branch_var=%branch_var%
echo last_hash_var=%last_hash_var%

set origin_out_name=%project_dir%\Debug\%project_name%_PATTERN.out
echo origin_out_name=%origin_out_name%
set new_out_name=%project_dir%\Debug\%project_name%_%branch_var%_%last_hash_var%.out
echo new_out_name=%new_out_name%

COPY %origin_out_name% %new_out_name%
DEL %origin_out_name%


set origin_out_name=%project_dir%\Debug\%project_name%_PATTERN.out
echo origin_out_name=%origin_out_name%
set new_out_name=%project_dir%\Debug\%project_name%_%branch_var%_%last_hash_var%.out
echo new_out_name=%new_out_name%

COPY %origin_out_name% %new_out_name%
DEL %origin_out_name%

set origin_map_name=%project_dir%\Debug\%project_name%_PATTERN.map
echo origin_map_name=%origin_map_name%
set new_map_name=%project_dir%\Debug\%project_name%_%branch_var%_%last_hash_var%.map
echo new_map_name=%new_map_name%

COPY %origin_map_name% %new_map_name%
DEL %origin_map_name%

set origin_bin_name=%project_dir%\Debug\%project_name%_PATTERN.bin
echo origin_bin_name=%origin_bin_name%
set new_bin_name=%project_dir%\Debug\%project_name%_%branch_var%_%last_hash_var%.bin
echo new_bin_name=%new_bin_name%

COPY %origin_bin_name% %new_bin_name%
DEL %origin_bin_name%


set origin_hex_name=%project_dir%\Debug\%project_name%_PATTERN.hex
echo origin_hex_name=%origin_hex_name%
set new_hex_name=%project_dir%\Debug\%project_name%_%branch_var%_%last_hash_var%.hex
echo new_hex_name=%new_hex_name%

COPY %origin_hex_name% %new_hex_name%
DEL %origin_hex_name%
