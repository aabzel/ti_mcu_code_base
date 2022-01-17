set com_num=%1
set name=%2
start ""  "C:\Program Files (x86)\teraterm\ttermpro.exe" /LA=E /C=%com_num% /BAUD=460800 /CDATABIT=8 /CPARITY=none /CSTOPBIT=1 /X=20  /Y=10 /W="%name%" /AUTOWINCLOSE=on 


