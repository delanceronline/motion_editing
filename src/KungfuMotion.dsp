# Microsoft Developer Studio Project File - Name="KungfuMotion" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=KungfuMotion - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "KungfuMotion.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "KungfuMotion.mak" CFG="KungfuMotion - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "KungfuMotion - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "KungfuMotion - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "KungfuMotion - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 /nologo /subsystem:windows /machine:I386

!ELSEIF  "$(CFG)" == "KungfuMotion - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 opengl32.lib glu32.lib glaux.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "KungfuMotion - Win32 Release"
# Name "KungfuMotion - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\ChildFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\CurveView.cpp
# End Source File
# Begin Source File

SOURCE=.\DoubleScreenDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\FilterDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\KungfuMotion.cpp
# End Source File
# Begin Source File

SOURCE=.\KungfuMotion.rc
# End Source File
# Begin Source File

SOURCE=.\KungfuMotionData.cpp
# End Source File
# Begin Source File

SOURCE=.\KungfuMotionDoc.cpp
# End Source File
# Begin Source File

SOURCE=.\KungfuMotionView.cpp
# End Source File
# Begin Source File

SOURCE=.\MainFrm.cpp
# End Source File
# Begin Source File

SOURCE=".\Curve Fitting\MarkerSeq.cpp"
# End Source File
# Begin Source File

SOURCE=.\MarkerView.cpp
# End Source File
# Begin Source File

SOURCE=.\MotionBlendingDialog.cpp
# End Source File
# Begin Source File

SOURCE=".\Motion Editing\MotionEditing.cpp"
# End Source File
# Begin Source File

SOURCE=.\MotionExtractionDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\MotionWarping.cpp
# End Source File
# Begin Source File

SOURCE=.\SnapShotDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\ChildFrm.h
# End Source File
# Begin Source File

SOURCE=.\CurveView.h
# End Source File
# Begin Source File

SOURCE=.\DoubleScreenDialog.h
# End Source File
# Begin Source File

SOURCE=.\FilterDialog.h
# End Source File
# Begin Source File

SOURCE=.\KungfuMotion.h
# End Source File
# Begin Source File

SOURCE=.\KungfuMotionData.h
# End Source File
# Begin Source File

SOURCE=.\KungfuMotionDoc.h
# End Source File
# Begin Source File

SOURCE=.\KungfuMotionView.h
# End Source File
# Begin Source File

SOURCE=.\MainFrm.h
# End Source File
# Begin Source File

SOURCE=".\Curve Fitting\MarkerSeq.h"
# End Source File
# Begin Source File

SOURCE=.\MarkerView.h
# End Source File
# Begin Source File

SOURCE=.\MotionBlendingDialog.h
# End Source File
# Begin Source File

SOURCE=".\Motion Editing\MotionEditing.h"
# End Source File
# Begin Source File

SOURCE=.\MotionExtractionDialog.h
# End Source File
# Begin Source File

SOURCE=.\MotionWarping.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\SnapShotDialog.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\ico00001.ico
# End Source File
# Begin Source File

SOURCE=.\RES\ico00002.ico
# End Source File
# Begin Source File

SOURCE=.\RES\ico00003.ico
# End Source File
# Begin Source File

SOURCE=.\RES\ico00004.ico
# End Source File
# Begin Source File

SOURCE=.\RES\ico00005.ico
# End Source File
# Begin Source File

SOURCE=.\RES\ico00006.ico
# End Source File
# Begin Source File

SOURCE=.\res\icon1.ico
# End Source File
# Begin Source File

SOURCE=.\RES\joint.ico
# End Source File
# Begin Source File

SOURCE=".\res\kung fu.ico"
# End Source File
# Begin Source File

SOURCE=.\res\KungfuMotion.ico
# End Source File
# Begin Source File

SOURCE=.\res\KungfuMotion.rc2
# End Source File
# Begin Source File

SOURCE=.\res\KungfuMotionDoc.ico
# End Source File
# Begin Source File

SOURCE=.\RES\motion_s.ico
# End Source File
# Begin Source File

SOURCE=.\res\polyu.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Toolbar.bmp
# End Source File
# Begin Source File

SOURCE=.\res\toolbar1.bmp
# End Source File
# End Group
# Begin Group "Curve Fitting Source"

# PROP Default_Filter "cpp"
# Begin Source File

SOURCE=".\Curve Fitting\ControlPoint.cpp"
# End Source File
# Begin Source File

SOURCE=".\Curve Fitting\ControlPointSet.cpp"
# End Source File
# Begin Source File

SOURCE=".\Curve Fitting\CubicBSplineCurve.cpp"
# End Source File
# Begin Source File

SOURCE=".\Curve Fitting\DOFName.cpp"
# End Source File
# Begin Source File

SOURCE=".\Curve Fitting\Matrix.cpp"
# End Source File
# Begin Source File

SOURCE=".\Curve Fitting\MotionCurves_UCBS.cpp"
# End Source File
# Begin Source File

SOURCE=".\Curve Fitting\MotionData.cpp"
# End Source File
# Begin Source File

SOURCE=".\Curve Fitting\MotionMarker.cpp"
# End Source File
# Begin Source File

SOURCE=".\Curve Fitting\PathPoint.cpp"
# End Source File
# Begin Source File

SOURCE=".\Curve Fitting\PathPointSet.cpp"
# End Source File
# Begin Source File

SOURCE=".\Curve Fitting\Point2D.cpp"
# End Source File
# Begin Source File

SOURCE=".\Curve Fitting\Vector3D.cpp"
# End Source File
# End Group
# Begin Group "Curve Fitting Header"

# PROP Default_Filter "h"
# Begin Source File

SOURCE=".\Curve Fitting\ControlPoint.h"
# End Source File
# Begin Source File

SOURCE=".\Curve Fitting\ControlPointSet.h"
# End Source File
# Begin Source File

SOURCE=".\Curve Fitting\CubicBSplineCurve.h"
# End Source File
# Begin Source File

SOURCE=".\Curve Fitting\DOFName.h"
# End Source File
# Begin Source File

SOURCE=".\Curve Fitting\Matrix.h"
# End Source File
# Begin Source File

SOURCE=".\Curve Fitting\MotionCurves_UCBS.h"
# End Source File
# Begin Source File

SOURCE=".\Curve Fitting\MotionData.h"
# End Source File
# Begin Source File

SOURCE=".\Curve Fitting\MotionMarker.h"
# End Source File
# Begin Source File

SOURCE=".\Curve Fitting\PathPoint.h"
# End Source File
# Begin Source File

SOURCE=".\Curve Fitting\PathPointSet.h"
# End Source File
# Begin Source File

SOURCE=".\Curve Fitting\Point2D.h"
# End Source File
# Begin Source File

SOURCE=".\Curve Fitting\Vector3D.h"
# End Source File
# End Group
# Begin Group "Motion Preview Source"

# PROP Default_Filter "cpp"
# Begin Source File

SOURCE=".\Motion Preview\MotionPreviewDialog.cpp"
# End Source File
# Begin Source File

SOURCE=".\Motion Preview\motionpreviewscreen.cpp"
# End Source File
# End Group
# Begin Group "Motion Preview Header"

# PROP Default_Filter "h"
# Begin Source File

SOURCE=".\Motion Preview\MotionPreviewDialog.h"
# End Source File
# Begin Source File

SOURCE=".\Motion Preview\MotionPreviewScreen.h"
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
