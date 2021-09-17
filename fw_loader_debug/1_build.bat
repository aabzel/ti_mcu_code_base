@echo off
If exist "*.exe" (
    Echo *.exe exist
    del *.exe
)

make 
rem sleep 30

