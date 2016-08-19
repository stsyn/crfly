# Microsoft Developer Studio Project File - Name="lesson1" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=lesson1 - Win32 Release
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "lesson1.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "lesson1.mak" CFG="lesson1 - Win32 Release"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "lesson1 - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "lesson1 - Win32 SPRed" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "lesson1 - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# SUBTRACT MTL /Oicf
# ADD BASE RSC /l 0x1009 /d "NDEBUG"
# ADD RSC /l 0x1009 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 Winmm.lib opengl32.lib glu32.lib glaux.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib SDL2.lib SDL2main.lib SDL2test.lib /nologo /subsystem:windows /machine:I386 /out:"Release/CRFly.exe"

!ELSEIF  "$(CFG)" == "lesson1 - Win32 SPRed"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "lesson1___Win32_SPRed"
# PROP BASE Intermediate_Dir "lesson1___Win32_SPRed"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "lesson1___Win32_SPRed"
# PROP Intermediate_Dir "lesson1___Win32_SPRed"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# SUBTRACT BASE MTL /Oicf
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# SUBTRACT MTL /Oicf
# ADD BASE RSC /l 0x1009 /d "NDEBUG"
# ADD RSC /l 0x1009 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 Winmm.lib opengl32.lib glu32.lib glaux.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib SDL2.lib SDL2main.lib SDL2test.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 Winmm.lib opengl32.lib glu32.lib glaux.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib SDL2.lib SDL2main.lib SDL2test.lib /nologo /subsystem:windows /machine:I386 /out:"Release/SPRed.exe"

!ENDIF 

# Begin Target

# Name "lesson1 - Win32 Release"
# Name "lesson1 - Win32 SPRed"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\Game.cpp

!IF  "$(CFG)" == "lesson1 - Win32 Release"

!ELSEIF  "$(CFG)" == "lesson1 - Win32 SPRed"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Lib.cpp
# End Source File
# Begin Source File

SOURCE=.\Platform.cpp
# End Source File
# Begin Source File

SOURCE=.\SPRed.cpp

!IF  "$(CFG)" == "lesson1 - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "lesson1 - Win32 SPRed"

# PROP BASE Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Updater.cpp

!IF  "$(CFG)" == "lesson1 - Win32 Release"

!ELSEIF  "$(CFG)" == "lesson1 - Win32 SPRed"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\G_Core.h

!IF  "$(CFG)" == "lesson1 - Win32 Release"

!ELSEIF  "$(CFG)" == "lesson1 - Win32 SPRed"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\G_Menu.h

!IF  "$(CFG)" == "lesson1 - Win32 Release"

!ELSEIF  "$(CFG)" == "lesson1 - Win32 SPRed"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\G_OpUnit.h

!IF  "$(CFG)" == "lesson1 - Win32 Release"

!ELSEIF  "$(CFG)" == "lesson1 - Win32 SPRed"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\G_RPG.H

!IF  "$(CFG)" == "lesson1 - Win32 Release"

!ELSEIF  "$(CFG)" == "lesson1 - Win32 SPRed"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\G_System.h

!IF  "$(CFG)" == "lesson1 - Win32 Release"

!ELSEIF  "$(CFG)" == "lesson1 - Win32 SPRed"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Game.h

!IF  "$(CFG)" == "lesson1 - Win32 Release"

!ELSEIF  "$(CFG)" == "lesson1 - Win32 SPRed"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\GameFiles.h

!IF  "$(CFG)" == "lesson1 - Win32 Release"

!ELSEIF  "$(CFG)" == "lesson1 - Win32 SPRed"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Lib.h
# End Source File
# Begin Source File

SOURCE=.\LParticles.h
# End Source File
# Begin Source File

SOURCE=.\LPController.h
# End Source File
# Begin Source File

SOURCE=.\LPObjects.h
# End Source File
# Begin Source File

SOURCE=.\LSprite.h
# End Source File
# Begin Source File

SOURCE=.\Platform.h

!IF  "$(CFG)" == "lesson1 - Win32 Release"

!ELSEIF  "$(CFG)" == "lesson1 - Win32 SPRed"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\SPRedFiles.h

!IF  "$(CFG)" == "lesson1 - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "lesson1 - Win32 SPRed"

# PROP BASE Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Updater.h

!IF  "$(CFG)" == "lesson1 - Win32 Release"

!ELSEIF  "$(CFG)" == "lesson1 - Win32 SPRed"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# Begin Source File

SOURCE=".\CRF(ly) API.txt"
# End Source File
# End Target
# End Project
