cl.exe /c /EHsc Window.c

rc.exe Window.rc

link.exe Window.obj Window.res USER32.lib GDI32.lib /SUBSYSTEM:WINDOWS
