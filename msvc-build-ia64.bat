@ECHO OFF
ECHO building with MSVC - 64 Bit
call vcvarsall.bat ia64
cd deps\PDCurses-3.4\win32
nmake -f vcwin32.mak WIDE=Y
cd ..\..\..
