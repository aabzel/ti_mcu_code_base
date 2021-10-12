@echo off
del /S *.o
If exist "*.exe" (
    Echo File exist
    del *.exe
)
cd ..
del *.o
If exist "*.exe" (
    Echo File exist
    del *.exe
)

cd fw_loader_debug

make clean

dir