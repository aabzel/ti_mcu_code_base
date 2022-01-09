set python_path="C:\Users\aabdev\.espressif\python_env\idf5.0_py3.9_env\Scripts\python.exe"
set esp_tool=C:\esp\esp-idf\components\esptool_py\esptool\esptool.py
set bin_path=build/hello_world.bin
rem "C:\Users\aabdev\.espressif\python_env\idf5.0_py3.9_env\Scripts\python.exe" C:\esp\esp-idf\tools\idf.py -p COM5  flash
rem "C:\Users\aabdev\.espressif\python_env\idf5.0_py3.9_env\Scripts\python.exe" C:\esp\esp-idf\tools\idf.py -p COM5 -b 921600  flash
%python_path% %esp_tool%  --chip esp32 -p COM5 -b 921600 --before=default_reset --after=hard_reset write_flash --flash_mode dio --flash_freq 40m --flash_size 4MB 0x8000 build/partition_table/partition-table.bin 0x1000 build/bootloader/bootloader.bin 0x10000 %bin_path%

