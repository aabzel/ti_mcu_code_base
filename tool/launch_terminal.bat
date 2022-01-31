set com_num=%1
set BaudRate=%2
set name=%3
start ""  "C:\Program Files (x86)\teraterm\ttermpro.exe" /LA=E /C=%com_num% /BAUD=%BaudRate% /CDATABIT=8 /CPARITY=none /CSTOPBIT=1 /X=20  /Y=10 /W="%name%" /AUTOWINCLOSE=on 


