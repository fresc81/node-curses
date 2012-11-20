@ECHO OFF
ECHO building with MSVC - 32 Bit
call vcvarsall.bat x86
cd deps\PDCurses-3.4\win32
nmake -f vcwin32.mak WIDE=Y
cd ..\..\..
