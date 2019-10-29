
Fork of https://github.com/REPALphilread/ReP_AL-3D-Lawn-Mower

GNU Public License

Install VSCode

Install Extensions 

    C/C++
	
    Clang-Format
	
    PlatformIO IDE
	
	
Build on Linux and Windows 10

No need for copy/paste libraries - all included via platformio.ini's

Windows 10 extras to do:

     ; activate long 32-bit paths in windows 
     gpedit.msc > Computer Configuration > Administrative Templates > System > Filesystem "Enable Win32 long paths"
	
     ; additinaly you need to run 
     #git config --system core.longpaths true


NodeMCU:

rename config.example.h to config.h and change values
