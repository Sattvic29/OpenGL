cl.exe /c /EHsc OGL.c

rc.exe OGL.rc

link.exe OGL.obj OGL.res USER32.lib GDI32.lib /SUBSYSTEM:WINDOWS

