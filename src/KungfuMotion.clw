; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=FilterDialog
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "KungfuMotion.h"
LastPage=0

ClassCount=18
Class1=CKungfuMotionApp
Class2=CKungfuMotionDoc
Class3=CKungfuMotionView
Class4=CMainFrame
Class7=MarkerView

ResourceCount=22
Resource1=IDD_WARPING_DIALOG
Resource2=IDD_WARPING_DIALOG (English (U.S.))
Class5=CChildFrame
Class6=CAboutDlg
Class8=CurveView
Resource3=IDD_SNAPSHOT_DIALOG (English (U.S.))
Resource4=IDD_SNAPSHOT_DIALOG
Resource5=IDR_MAINFRAME1 (English (U.S.))
Resource6=IDR_KUNGFUTYPE (English (U.S.))
Resource7=IDR_MAINFRAME
Class9=OpenGLWindow
Class10=MarkerTree
Resource8=IDD_EXTRACT_DIALOG (English (U.S.))
Class11=MotionPreviewDialog
Class12=MotionPreviewScreen
Resource9=IDD_DOUBLESCREEN_DIALOG
Class13=MotionBlendingDialog
Resource10=IDD_BLENDING_OPTION_DIALOG
Class14=DoubleScreenDialog
Resource11=IDR_KUNGFUTYPE
Class15=MotionExtractionDialog
Resource12=IDD_ABOUTBOX
Class16=MotionWarping
Resource13=IDD_PREVIEW_DIALOG
Class17=SnapShotDialog
Resource14=IDD_EXTRACT_DIALOG
Class18=FilterDialog
Resource15=IDD_FILTER_DIALOG
Resource16=IDR_MAINFRAME (Chinese (Hong Kong))
Resource17=IDR_MAINFRAME (English (U.S.))
Resource18=IDD_ABOUTBOX (English (U.S.))
Resource19=IDD_PREVIEW_DIALOG (English (U.S.))
Resource20=IDD_BLENDING_OPTION_DIALOG (English (U.S.))
Resource21=IDD_DOUBLESCREEN_DIALOG (English (U.S.))
Resource22=IDD_FILTER_DIALOG (English (U.S.))

[CLS:CKungfuMotionApp]
Type=0
HeaderFile=KungfuMotion.h
ImplementationFile=KungfuMotion.cpp
Filter=N
BaseClass=CWinApp
VirtualFilter=AC
LastObject=CKungfuMotionApp

[CLS:CKungfuMotionDoc]
Type=0
HeaderFile=KungfuMotionDoc.h
ImplementationFile=KungfuMotionDoc.cpp
Filter=N
BaseClass=CDocument
VirtualFilter=DC
LastObject=CKungfuMotionDoc

[CLS:CKungfuMotionView]
Type=0
HeaderFile=KungfuMotionView.h
ImplementationFile=KungfuMotionView.cpp
Filter=C
LastObject=ID_MOTION_FILTER
BaseClass=CView
VirtualFilter=VWC


[CLS:CMainFrame]
Type=0
HeaderFile=MainFrm.h
ImplementationFile=MainFrm.cpp
Filter=T
LastObject=CMainFrame
BaseClass=CMDIFrameWnd
VirtualFilter=fWC


[CLS:CChildFrame]
Type=0
HeaderFile=ChildFrm.h
ImplementationFile=ChildFrm.cpp
Filter=M
BaseClass=CMDIChildWnd
VirtualFilter=mfWC
LastObject=CChildFrame


[CLS:CAboutDlg]
Type=0
HeaderFile=KungfuMotion.cpp
ImplementationFile=KungfuMotion.cpp
Filter=D

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[MNU:IDR_MAINFRAME]
Type=1
Class=CMainFrame
Command1=ID_FILE_NEW_PROJECT
Command2=ID_FILE_OPEN_PROJECT
Command3=ID_FILE_PRINT_SETUP
Command4=ID_FILE_MRU_FILE1
Command5=ID_APP_EXIT
Command6=ID_VIEW_TOOLBAR
Command7=ID_VIEW_STATUS_BAR
Command8=ID_HELP_HELP
Command9=ID_APP_ABOUT_KUNGFU_MOTION
CommandCount=9

[MNU:IDR_KUNGFUTYPE]
Type=1
Class=CKungfuMotionView
Command1=ID_FILE_NEW_PROJECT
Command2=ID_FILE_OPEN_PROJECT
Command3=ID_FILE_CLOSE_PROJECT
Command4=ID_FILE_SAVE_PROJECT
Command5=ID_FILE_SAVE_PROJECT_AS
Command6=ID_FILE_IMPORT
Command7=ID_FILE_EXPORT
Command8=ID_FILE_PRINT
Command9=ID_FILE_PRINT_PREVIEW
Command10=ID_FILE_PRINT_SETUP
Command11=ID_FILE_SAVESNAPSHOT
Command12=ID_FILE_MRU_FILE1
Command13=ID_APP_EXIT
Command14=ID_EDIT_UNDO
Command15=ID_EDIT_REDO
Command16=ID_EDIT_CUT
Command17=ID_EDIT_COPY
Command18=ID_EDIT_PASTE
Command19=ID_VIEW_MOVE
Command20=ID_VIEW_ZOOM_IN
Command21=ID_VIEW_ZOOM_OUT
Command22=ID_VIEW_RESET
Command23=ID_VIEW_TOOLBAR
Command24=ID_VIEW_STATUS_BAR
Command25=ID_VIEW_CP
Command26=ID_VIEW_PP
Command27=ID_VIEW_IP
Command28=ID_CURVE_FIT
Command29=ID_CURVE_SELECT_CURVE
Command30=ID_CURVE_SELECT_POINT
Command31=ID_CURVE_INSERT_POINT
Command32=ID_CURVE_DELETE_POINT
Command33=ID_CURVE_MERGE_POINT
Command34=ID_CURVE_SPLIT_POINT
Command35=ID_CURVE_FLATTEN
Command36=ID_CURVE_SHARPEN
Command37=ID_CURVE_SINGLE_TANGENT
Command38=ID_CURVE_DOUBLE_TANGENT
Command39=ID_MOTION_PREVIEW
Command40=ID_MOTION_BLENDING
Command41=ID_MOTION_TRANSITION
Command42=ID_MOTION_EXAGGERATING
Command43=ID_MOTION_EXTRACTING
Command44=ID_MOTION_REPLACING
Command45=ID_MOTION_WARPING
Command46=ID_MOTION_FILTER
Command47=ID_HELP_HELP
Command48=ID_APP_ABOUT_KUNGFU_MOTION
CommandCount=48

[ACL:IDR_MAINFRAME]
Type=1
Class=CMainFrame
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_SAVE
Command4=ID_FILE_PRINT
Command5=ID_EDIT_UNDO
Command6=ID_EDIT_CUT
Command7=ID_EDIT_COPY
Command8=ID_EDIT_PASTE
Command9=ID_EDIT_UNDO
Command10=ID_EDIT_CUT
Command11=ID_EDIT_COPY
Command12=ID_EDIT_PASTE
Command13=ID_NEXT_PANE
Command14=ID_PREV_PANE
CommandCount=14

[CLS:MarkerView]
Type=0
HeaderFile=MarkerView.h
ImplementationFile=MarkerView.cpp
BaseClass=CView
Filter=C
LastObject=ID_MOTION_TRANSITION
VirtualFilter=VWC

[CLS:CurveView]
Type=0
HeaderFile=CurveView.h
ImplementationFile=CurveView.cpp
BaseClass=CView
Filter=C
LastObject=ID_VIEW_IP
VirtualFilter=VWC

[MNU:IDR_MAINFRAME (English (U.S.))]
Type=1
Class=CMainFrame
Command1=ID_FILE_NEW_PROJECT
Command2=ID_FILE_OPEN_PROJECT
Command3=ID_FILE_PRINT_SETUP
Command4=ID_FILE_MRU_FILE1
Command5=ID_APP_EXIT
Command6=ID_VIEW_TOOLBAR
Command7=ID_VIEW_STATUS_BAR
Command8=ID_HELP_HELP
Command9=ID_APP_ABOUT_KUNGFU_MOTION
CommandCount=9

[MNU:IDR_KUNGFUTYPE (English (U.S.))]
Type=1
Class=CKungfuMotionView
Command1=ID_FILE_NEW_PROJECT
Command2=ID_FILE_OPEN_PROJECT
Command3=ID_FILE_CLOSE_PROJECT
Command4=ID_FILE_SAVE_PROJECT
Command5=ID_FILE_SAVE_PROJECT_AS
Command6=ID_FILE_IMPORT
Command7=ID_FILE_EXPORT
Command8=ID_FILE_PRINT
Command9=ID_FILE_PRINT_PREVIEW
Command10=ID_FILE_PRINT_SETUP
Command11=ID_FILE_SAVESNAPSHOT
Command12=ID_FILE_MRU_FILE1
Command13=ID_APP_EXIT
Command14=ID_EDIT_UNDO
Command15=ID_EDIT_REDO
Command16=ID_EDIT_CUT
Command17=ID_EDIT_COPY
Command18=ID_EDIT_PASTE
Command19=ID_VIEW_MOVE
Command20=ID_VIEW_ZOOM_IN
Command21=ID_VIEW_ZOOM_OUT
Command22=ID_VIEW_RESET
Command23=ID_VIEW_TOOLBAR
Command24=ID_VIEW_STATUS_BAR
Command25=ID_VIEW_CP
Command26=ID_VIEW_PP
Command27=ID_VIEW_IP
Command28=ID_CURVE_FIT
Command29=ID_CURVE_SELECT_CURVE
Command30=ID_CURVE_SELECT_POINT
Command31=ID_CURVE_INSERT_POINT
Command32=ID_CURVE_DELETE_POINT
Command33=ID_CURVE_MERGE_POINT
Command34=ID_CURVE_SPLIT_POINT
Command35=ID_CURVE_FLATTEN
Command36=ID_CURVE_SHARPEN
Command37=ID_CURVE_SINGLE_TANGENT
Command38=ID_CURVE_DOUBLE_TANGENT
Command39=ID_MOTION_PREVIEW
Command40=ID_MOTION_BLENDING
Command41=ID_MOTION_TRANSITION
Command42=ID_MOTION_EXAGGERATING
Command43=ID_MOTION_EXTRACTING
Command44=ID_MOTION_REPLACING
Command45=ID_MOTION_WARPING
Command46=ID_MOTION_FILTER
Command47=ID_HELP_HELP
Command48=ID_APP_ABOUT_KUNGFU_MOTION
CommandCount=48

[ACL:IDR_MAINFRAME (English (U.S.))]
Type=1
Class=?
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_SAVE
Command4=ID_FILE_PRINT
Command5=ID_EDIT_UNDO
Command6=ID_EDIT_CUT
Command7=ID_EDIT_COPY
Command8=ID_EDIT_PASTE
Command9=ID_EDIT_UNDO
Command10=ID_EDIT_CUT
Command11=ID_EDIT_COPY
Command12=ID_EDIT_PASTE
Command13=ID_NEXT_PANE
Command14=ID_PREV_PANE
CommandCount=14

[DLG:IDD_ABOUTBOX (English (U.S.))]
Type=1
Class=CAboutDlg
ControlCount=3
Control1=IDC_STATIC,static,1342308480
Control2=IDC_STATIC,static,1342308352
Control3=IDOK,button,1342373889

[TB:IDR_MAINFRAME1 (English (U.S.))]
Type=1
Class=?
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_SAVE
Command4=ID_EDIT_CUT
Command5=ID_EDIT_COPY
Command6=ID_EDIT_PASTE
Command7=ID_FILE_PRINT
Command8=ID_APP_ABOUT
CommandCount=8

[TB:IDR_MAINFRAME]
Type=1
Class=CMainFrame
Command1=ID_TOOL_SELECT_CURVE
Command2=ID_TOOL_SELECT_POINT
Command3=ID_TOOL_INSERT_POINT
Command4=ID_TOOL_DELETE_POINT
Command5=ID_TOOL_MERGE_POINT
Command6=ID_TOOL_SPLIT_POINT
Command7=ID_TOOL_FLATTEN
Command8=ID_TOOL_SHARPEN
Command9=ID_TOOL_SINGLE_TANGENT
Command10=ID_TOOL_DOUBLE_TANGENT
Command11=ID_VIEW_MOVE
Command12=ID_VIEW_ZOOM_IN
Command13=ID_VIEW_ZOOM_OUT
Command14=ID_VIEW_RESET
CommandCount=14

[TB:IDR_MAINFRAME (Chinese (Hong Kong))]
Type=1
Class=CMainFrame
Command1=ID_CURVE_FIT
Command2=ID_TOOL_SELECT_CURVE
Command3=ID_TOOL_SELECT_POINT
Command4=ID_TOOL_INSERT_POINT
Command5=ID_TOOL_DELETE_POINT
Command6=ID_TOOL_MERGE_POINT
Command7=ID_TOOL_SPLIT_POINT
Command8=ID_TOOL_FLATTEN
Command9=ID_TOOL_SHARPEN
Command10=ID_TOOL_SINGLE_TANGENT
Command11=ID_TOOL_DOUBLE_TANGENT
Command12=ID_VIEW_MOVE
Command13=ID_VIEW_ZOOM_IN
Command14=ID_VIEW_ZOOM_OUT
Command15=ID_VIEW_RESET
CommandCount=15

[CLS:OpenGLWindow]
Type=0
HeaderFile=OpenGLWindow.h
ImplementationFile=OpenGLWindow.cpp
BaseClass=CWnd
Filter=W
VirtualFilter=WC
LastObject=ID_APP_ABOUT

[CLS:MarkerTree]
Type=0
HeaderFile=MarkerTree.h
ImplementationFile=MarkerTree.cpp
BaseClass=CTreeCtrl
Filter=W

[DLG:IDD_PREVIEW_DIALOG]
Type=1
Class=MotionPreviewDialog
ControlCount=7
Control1=IDCANCEL,button,1342242816
Control2=IDC_FRAMESILDER,msctls_trackbar32,1342242840
Control3=IDC_PREVIEW_PLAY,button,1342242816
Control4=IDC_PREVIEW_STOP,button,1342242816
Control5=IDC_PREVIEW_PAUSE,button,1342242816
Control6=IDC_MOTION_PREVIEW_SCREEN_BORDER,button,1073741831
Control7=IDC_MARKERSIZESILDER,msctls_trackbar32,1342242840

[CLS:MotionPreviewDialog]
Type=0
HeaderFile=motion preview\motionpreviewdialog.h
ImplementationFile=motion preview\motionpreviewdialog.cpp
BaseClass=CDialog
LastObject=ID_APP_ABOUT
Filter=D
VirtualFilter=dWC

[CLS:MotionPreviewScreen]
Type=0
HeaderFile=motion preview\motionpreviewscreen.h
ImplementationFile=motion preview\motionpreviewscreen.cpp
BaseClass=CWnd
Filter=W
VirtualFilter=WC
LastObject=MotionPreviewScreen

[DLG:IDD_BLENDING_OPTION_DIALOG]
Type=1
Class=MotionBlendingDialog
ControlCount=10
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_BLENDINGVALUE_SLIDER,msctls_trackbar32,1342242840
Control4=IDC_STARTFRAME_SLIDER,msctls_trackbar32,1342242840
Control5=IDC_ENDFRAME_SLIDER,msctls_trackbar32,1342242840
Control6=IDC_STATIC,static,1342308352
Control7=IDC_STATIC,static,1342308352
Control8=IDC_STATIC,static,1342308352
Control9=IDC_MOTIONNAME_TEXTBOX,edit,1350631552
Control10=IDC_STATIC,static,1342308352

[CLS:MotionBlendingDialog]
Type=0
HeaderFile=MotionBlendingDialog.h
ImplementationFile=MotionBlendingDialog.cpp
BaseClass=CDialog
Filter=D
LastObject=IDOK
VirtualFilter=dWC

[DLG:IDD_DOUBLESCREEN_DIALOG]
Type=1
Class=DoubleScreenDialog
ControlCount=34
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_SCREEN1_BORDER,button,1073741831
Control4=IDC_SCREEN2_STARTFRAME_SLIDER,msctls_trackbar32,1350631448
Control5=IDC_SCREEN1_STARTFRAME_SLIDER,msctls_trackbar32,1350631448
Control6=IDC_RADIUS1_SLIDER,msctls_trackbar32,1342242840
Control7=IDC_RADIUS2_SLIDER,msctls_trackbar32,1342242840
Control8=IDC_STATIC,static,1342308352
Control9=IDC_STATIC,static,1342308352
Control10=IDC_SCREEN1_ENDFRAME_SLIDER,msctls_trackbar32,1350631448
Control11=IDC_STATIC,static,1342308352
Control12=IDC_STATIC,static,1342308352
Control13=IDC_SCREEN2_ENDFRAME_SLIDER,msctls_trackbar32,1350631448
Control14=IDC_MARKERTREE_DOUBLESCREEN,SysTreeView32,1350631463
Control15=IDC_SELECTEDMARKERLIST,SysListView32,1350666253
Control16=IDC_SELECTION_BUTTON_DOUBLESCREEN,button,1342242816
Control17=IDC_SIMVAL_SLIDER,msctls_trackbar32,1342242840
Control18=IDC_SIMVAL_STATIC,static,1342308352
Control19=IDC_SCREEN2_BORDER,button,1073741831
Control20=IDC_SKETELON_BUTTON,button,1342242816
Control21=IDC_JOINT_BUTTON,button,1342242816
Control22=IDC_DESELECTION_BUTTON_DOUBLESCREEN,button,1342242816
Control23=IDC_SimVal,static,1342308353
Control24=IDC_S1_STARTFRAME_STATIC,static,1342308352
Control25=IDC_S1_ENDFRAME_STATIC,static,1342308352
Control26=IDC_S2_STARTFRAME_STATIC,static,1342308352
Control27=IDC_S2_ENDFRAME_STATIC,static,1342308352
Control28=IDC_S1_MARKERSIZE_STATIC,static,1342308352
Control29=IDC_S2_MARKERSIZE_STATIC,static,1342308352
Control30=IDC_STATIC,static,1342308352
Control31=IDC_STATIC,static,1342308352
Control32=IDC_MOTIONNAME_TEXTBOX,edit,1350631552
Control33=IDC_SELECTIONALL_BUTTON_DOUBLESCREEN,button,1342242816
Control34=IDC_DESELECTIONALL_BUTTON_DOUBLESCREEN,button,1342242816

[CLS:DoubleScreenDialog]
Type=0
HeaderFile=DoubleScreenDialog.h
ImplementationFile=DoubleScreenDialog.cpp
BaseClass=CDialog
Filter=D
LastObject=DoubleScreenDialog
VirtualFilter=dWC

[DLG:IDD_EXTRACT_DIALOG]
Type=1
Class=MotionExtractionDialog
ControlCount=14
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_SCREEN_EXTRACTION_BORDER,button,1073741831
Control4=IDC_STARTFRAME_EXTRACTION_SLIDER,msctls_trackbar32,1342242840
Control5=IDC_ENDFRAME_EXTRACTION_SLIDER,msctls_trackbar32,1342242840
Control6=IDC_STARTFRAME_EXTRACTION_STATIC,static,1342308352
Control7=IDC_ENDFRAME_EXTRACTION_STATIC,static,1342308352
Control8=IDC_STATIC,static,1342308352
Control9=IDC_STATIC,static,1342308352
Control10=IDC_MOTIONNAME_EXTRACTION_TEXTBOX,edit,1350631552
Control11=IDC_STATIC,static,1342308352
Control12=IDC_MARKERSIZE_EXTRACTION_SLIDER,msctls_trackbar32,1342242840
Control13=IDC_STATIC,static,1342308352
Control14=IDC_MARKERSIZE_EXTRACTION_STATIC,static,1342308352

[CLS:MotionExtractionDialog]
Type=0
HeaderFile=MotionExtractionDialog.h
ImplementationFile=MotionExtractionDialog.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=ID_APP_ABOUT

[DLG:IDD_WARPING_DIALOG]
Type=1
Class=MotionWarping
ControlCount=15
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_WARPING_BORDER,button,1073741831
Control4=IDC_FRAME_WARPING_SLIDER,msctls_trackbar32,1342242840
Control5=IDC_STATIC,static,1342308352
Control6=IDC_STATIC,static,1342308352
Control7=IDC_DURATION_TEXTBOX,edit,1350631552
Control8=IDC_FPS_TEXTBOX,edit,1350631552
Control9=IDC_STATIC,static,1342308352
Control10=IDC_MARKERSIZE_WARPING_SLIDER,msctls_trackbar32,1342242840
Control11=IDC_STATIC,static,1342308352
Control12=IDC_STATIC,static,1342308352
Control13=IDC_STATIC,static,1342308352
Control14=IDC_MOTIONNAME_WARPING_TEXTBOX,edit,1350631552
Control15=IDC_ORIGINALDURATION_WARPING_STATIC,static,1342308352

[CLS:MotionWarping]
Type=0
HeaderFile=MotionWarping.h
ImplementationFile=MotionWarping.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=IDC_MOTIONNAME_WARPING_TEXTBOX

[DLG:IDD_SNAPSHOT_DIALOG]
Type=1
Class=SnapShotDialog
ControlCount=10
Control1=IDOK,button,1342242817
Control2=IDC_SNAPSHOT_BORDER,button,1073741831
Control3=IDC_SAVE_SNAPSHOT_BUTTON,button,1342242816
Control4=IDC_TIME_SNAPSHOT_TEXTBOX,edit,1350631552
Control5=IDC_STATIC,static,1342308352
Control6=IDC_MOTIONTREE_SNAPSHOT_TREE,SysTreeView32,1350631424
Control7=IDC_SHOW_SNAPSHOT_BUTTON,button,1342242816
Control8=IDC_FRAME_SNAPSHOT_SLIDER,msctls_trackbar32,1342242840
Control9=IDC_MARKERSIZE_SNAPSHOT_SLIDER,msctls_trackbar32,1342242840
Control10=IDC_STATIC,static,1342308352

[CLS:SnapShotDialog]
Type=0
HeaderFile=SnapShotDialog.h
ImplementationFile=SnapShotDialog.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=ID_APP_ABOUT

[DLG:IDD_FILTER_DIALOG]
Type=1
Class=FilterDialog
ControlCount=11
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_IGNORESIGN_FILTER_CHECK,button,1342242819
Control4=IDC_STATIC,static,1342308352
Control5=IDC_TOLERANCE_FILTER_EDIT,edit,1350631552
Control6=IDC_STATIC,static,1342308352
Control7=IDC_STATIC,static,1342308352
Control8=IDC_MOTIONNAME_FILTER_STATIC,static,1342308352
Control9=IDC_CURVENAME_FILTER_STATIC,static,1342308352
Control10=IDC_STATIC,static,1342308352
Control11=IDC_MARKERNAMESTATIC,static,1342308352

[CLS:FilterDialog]
Type=0
HeaderFile=FilterDialog.h
ImplementationFile=FilterDialog.cpp
BaseClass=CDialog
Filter=D
LastObject=IDC_IGNORESIGN_FILTER_CHECK
VirtualFilter=dWC

[DLG:IDD_PREVIEW_DIALOG (English (U.S.))]
Type=1
Class=?
ControlCount=7
Control1=IDCANCEL,button,1342242816
Control2=IDC_FRAMESILDER,msctls_trackbar32,1342242840
Control3=IDC_PREVIEW_PLAY,button,1342242816
Control4=IDC_PREVIEW_STOP,button,1342242816
Control5=IDC_PREVIEW_PAUSE,button,1342242816
Control6=IDC_MOTION_PREVIEW_SCREEN_BORDER,button,1073741831
Control7=IDC_MARKERSIZESILDER,msctls_trackbar32,1342242840

[DLG:IDD_BLENDING_OPTION_DIALOG (English (U.S.))]
Type=1
Class=?
ControlCount=10
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_BLENDINGVALUE_SLIDER,msctls_trackbar32,1342242840
Control4=IDC_STARTFRAME_SLIDER,msctls_trackbar32,1342242840
Control5=IDC_ENDFRAME_SLIDER,msctls_trackbar32,1342242840
Control6=IDC_STATIC,static,1342308352
Control7=IDC_STATIC,static,1342308352
Control8=IDC_STATIC,static,1342308352
Control9=IDC_MOTIONNAME_TEXTBOX,edit,1350631552
Control10=IDC_STATIC,static,1342308352

[DLG:IDD_DOUBLESCREEN_DIALOG (English (U.S.))]
Type=1
Class=?
ControlCount=34
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_SCREEN1_BORDER,button,1073741831
Control4=IDC_SCREEN2_STARTFRAME_SLIDER,msctls_trackbar32,1350631448
Control5=IDC_SCREEN1_STARTFRAME_SLIDER,msctls_trackbar32,1350631448
Control6=IDC_RADIUS1_SLIDER,msctls_trackbar32,1342242840
Control7=IDC_RADIUS2_SLIDER,msctls_trackbar32,1342242840
Control8=IDC_STATIC,static,1342308352
Control9=IDC_STATIC,static,1342308352
Control10=IDC_SCREEN1_ENDFRAME_SLIDER,msctls_trackbar32,1350631448
Control11=IDC_STATIC,static,1342308352
Control12=IDC_STATIC,static,1342308352
Control13=IDC_SCREEN2_ENDFRAME_SLIDER,msctls_trackbar32,1350631448
Control14=IDC_MARKERTREE_DOUBLESCREEN,SysTreeView32,1350631463
Control15=IDC_SELECTEDMARKERLIST,SysListView32,1350666253
Control16=IDC_SELECTION_BUTTON_DOUBLESCREEN,button,1342242816
Control17=IDC_SIMVAL_SLIDER,msctls_trackbar32,1342242840
Control18=IDC_SIMVAL_STATIC,static,1342308352
Control19=IDC_SCREEN2_BORDER,button,1073741831
Control20=IDC_SKETELON_BUTTON,button,1342242816
Control21=IDC_JOINT_BUTTON,button,1342242816
Control22=IDC_DESELECTION_BUTTON_DOUBLESCREEN,button,1342242816
Control23=IDC_SimVal,static,1342308353
Control24=IDC_S1_STARTFRAME_STATIC,static,1342308352
Control25=IDC_S1_ENDFRAME_STATIC,static,1342308352
Control26=IDC_S2_STARTFRAME_STATIC,static,1342308352
Control27=IDC_S2_ENDFRAME_STATIC,static,1342308352
Control28=IDC_S1_MARKERSIZE_STATIC,static,1342308352
Control29=IDC_S2_MARKERSIZE_STATIC,static,1342308352
Control30=IDC_STATIC,static,1342308352
Control31=IDC_STATIC,static,1342308352
Control32=IDC_MOTIONNAME_TEXTBOX,edit,1350631552
Control33=IDC_SELECTIONALL_BUTTON_DOUBLESCREEN,button,1342242816
Control34=IDC_DESELECTIONALL_BUTTON_DOUBLESCREEN,button,1342242816

[DLG:IDD_EXTRACT_DIALOG (English (U.S.))]
Type=1
Class=?
ControlCount=14
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_SCREEN_EXTRACTION_BORDER,button,1073741831
Control4=IDC_STARTFRAME_EXTRACTION_SLIDER,msctls_trackbar32,1342242840
Control5=IDC_ENDFRAME_EXTRACTION_SLIDER,msctls_trackbar32,1342242840
Control6=IDC_STARTFRAME_EXTRACTION_STATIC,static,1342308352
Control7=IDC_ENDFRAME_EXTRACTION_STATIC,static,1342308352
Control8=IDC_STATIC,static,1342308352
Control9=IDC_STATIC,static,1342308352
Control10=IDC_MOTIONNAME_EXTRACTION_TEXTBOX,edit,1350631552
Control11=IDC_STATIC,static,1342308352
Control12=IDC_MARKERSIZE_EXTRACTION_SLIDER,msctls_trackbar32,1342242840
Control13=IDC_STATIC,static,1342308352
Control14=IDC_MARKERSIZE_EXTRACTION_STATIC,static,1342308352

[DLG:IDD_WARPING_DIALOG (English (U.S.))]
Type=1
Class=?
ControlCount=15
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_WARPING_BORDER,button,1073741831
Control4=IDC_FRAME_WARPING_SLIDER,msctls_trackbar32,1342242840
Control5=IDC_STATIC,static,1342308352
Control6=IDC_STATIC,static,1342308352
Control7=IDC_DURATION_TEXTBOX,edit,1350631552
Control8=IDC_FPS_TEXTBOX,edit,1350631552
Control9=IDC_STATIC,static,1342308352
Control10=IDC_MARKERSIZE_WARPING_SLIDER,msctls_trackbar32,1342242840
Control11=IDC_STATIC,static,1342308352
Control12=IDC_STATIC,static,1342308352
Control13=IDC_STATIC,static,1342308352
Control14=IDC_MOTIONNAME_WARPING_TEXTBOX,edit,1350631552
Control15=IDC_ORIGINALDURATION_WARPING_STATIC,static,1342308352

[DLG:IDD_SNAPSHOT_DIALOG (English (U.S.))]
Type=1
Class=?
ControlCount=10
Control1=IDOK,button,1342242817
Control2=IDC_SNAPSHOT_BORDER,button,1073741831
Control3=IDC_SAVE_SNAPSHOT_BUTTON,button,1342242816
Control4=IDC_TIME_SNAPSHOT_TEXTBOX,edit,1350631552
Control5=IDC_STATIC,static,1342308352
Control6=IDC_MOTIONTREE_SNAPSHOT_TREE,SysTreeView32,1350631424
Control7=IDC_SHOW_SNAPSHOT_BUTTON,button,1342242816
Control8=IDC_FRAME_SNAPSHOT_SLIDER,msctls_trackbar32,1342242840
Control9=IDC_MARKERSIZE_SNAPSHOT_SLIDER,msctls_trackbar32,1342242840
Control10=IDC_STATIC,static,1342308352

[DLG:IDD_FILTER_DIALOG (English (U.S.))]
Type=1
Class=?
ControlCount=11
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_IGNORESIGN_FILTER_CHECK,button,1342242819
Control4=IDC_STATIC,static,1342308352
Control5=IDC_TOLERANCE_FILTER_EDIT,edit,1350631552
Control6=IDC_STATIC,static,1342308352
Control7=IDC_STATIC,static,1342308352
Control8=IDC_MOTIONNAME_FILTER_STATIC,static,1342308352
Control9=IDC_CURVENAME_FILTER_STATIC,static,1342308352
Control10=IDC_STATIC,static,1342308352
Control11=IDC_MARKERNAMESTATIC,static,1342308352

