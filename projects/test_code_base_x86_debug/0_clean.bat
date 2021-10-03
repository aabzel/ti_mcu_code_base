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

cd test_code_base_x86_debug

make clean

dir