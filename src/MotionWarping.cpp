// MotionWarping.cpp : implementation file
//

#include "stdafx.h"
#include "KungfuMotion.h"
#include "MotionWarping.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// MotionWarping dialog


MotionWarping::MotionWarping(CWnd* pParent /*=NULL*/)
	: CDialog(MotionWarping::IDD, pParent)
{
	m_pMotionData = NULL;
	m_pMEInfo = NULL;
	//{{AFX_DATA_INIT(MotionWarping)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void MotionWarping::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(MotionWarping)
	DDX_Control(pDX, IDOK, m_OkButton);
	DDX_Control(pDX, IDC_ORIGINALDURATION_WARPING_STATIC, m_OriginalDurationStatic);
	DDX_Control(pDX, IDC_MOTIONNAME_WARPING_TEXTBOX, m_MotionNameTextBox);
	DDX_Control(pDX, IDC_MARKERSIZE_WARPING_SLIDER, m_MarkerSizeSlider);
	DDX_Control(pDX, IDC_FRAME_WARPING_SLIDER, m_FrameSlider);
	DDX_Control(pDX, IDC_FPS_TEXTBOX, m_FPSTextBox);
	DDX_Control(pDX, IDC_DURATION_TEXTBOX, m_DurationTextBox);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(MotionWarping, CDialog)
	//{{AFX_MSG_MAP(MotionWarping)
	ON_WM_HSCROLL()
	ON_EN_CHANGE(IDC_DURATION_TEXTBOX, OnChangeDurationTextbox)
	ON_EN_CHANGE(IDC_FPS_TEXTBOX, OnChangeFpsTextbox)
	ON_EN_CHANGE(IDC_MOTIONNAME_WARPING_TEXTBOX, OnChangeMotionnameWarpingTextbox)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// MotionWarping message handlers

void MotionWarping::SetPointer(MotionData *ptr)
{
	m_pMotionData = ptr;
}

void MotionWarping::SetPointer(MotionEditingInfo *ptr)
{
	m_pMEInfo = ptr;
}

BOOL MotionWarping::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	WNDCLASS wndClass;
	HCURSOR hCursor;
	hCursor = AfxGetApp()->LoadStandardCursor(IDC_CROSS);

	wndClass.style = CS_OWNDC;
	wndClass.lpfnWndProc = AfxWndProc;
	wndClass.cbClsExtra = 0;
	wndClass.cbWndExtra = 0;
	wndClass.hInstance = AfxGetInstanceHandle();
	wndClass.hIcon = NULL;
	wndClass.hCursor = hCursor;
	wndClass.hbrBackground = NULL;
	wndClass.lpszMenuName = NULL;
	wndClass.lpszClassName = "MotionWarpingScreen";

	// Register the window class
	AfxRegisterClass(&wndClass);
	
	CRect glRect, newRect;
	GetDlgItem(IDC_WARPING_BORDER)->GetWindowRect(&glRect);

	newRect.top = glRect.top - 80;
	newRect.bottom = glRect.bottom - 80;
	newRect.left = glRect.left;
	newRect.right = glRect.right;

	// Create motion preview window.
	m_mpScreen.Create("MotionWarpingScreen", NULL, WS_CHILD | WS_VISIBLE, newRect, this, ID_MOTION_WARPING_SCREEN, NULL);
	m_mpScreen.Invalidate(FALSE);
	m_mpScreen.SetPointer(m_pMotionData);
	m_mpScreen.SetPointer(this);	
	
	// Frame Slider.
	m_FrameSlider.SetRange(0, m_pMotionData->numFrames, FALSE);
	m_FrameSlider.SetPos(0);

	CString str;
	float length = (float)m_pMotionData->numFrames * m_pMotionData->FrameTime;

	str.Format("Original: %0.3fs at %u FPS", length, (unsigned int)(1.0f / m_pMotionData->FrameTime));
	m_OriginalDurationStatic.SetWindowText(str);

	m_FPSTextBox.SetWindowText("30");

	m_MotionNameTextBox.SetLimitText(256);
	m_MotionNameTextBox.SetWindowText("Warped motion");

	m_DurationTextBox.SetLimitText(256);

	m_OkButton.EnableWindow(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void MotionWarping::OnOK() 
{
	// TODO: Add extra validation here
	char text[256];
	int FPS;

	m_MotionNameTextBox.GetWindowText(text, 256);
	strcpy(m_pMEInfo->MotionName, text);

	m_FPSTextBox.GetWindowText(text, 256);
	FPS = atoi(text);
	m_pMEInfo->FrameTime = 1.0f / FPS;

	m_DurationTextBox.GetWindowText(text, 256);
	m_pMEInfo->Duration = (float)atof(text);

	m_pMEInfo->MotionDataType = m_pMotionData->m_MotionDataType;
	m_pMEInfo->numMarkers = m_pMotionData->numMarkers;
	m_pMEInfo->StartTime_M1 = 0.0f;
	m_pMEInfo->EndTime_M1 = m_pMotionData->FrameTime * (float)m_pMotionData->numFrames;

	m_pMEInfo->numFrames = (unsigned int)(m_pMEInfo->Duration * (float)FPS);

	CDialog::OnOK();
}

void MotionWarping::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	// TODO: Add your message handler code here and/or call default
	int val;

	if((CSliderCtrl *)pScrollBar == &m_FrameSlider)
	{
		val = m_FrameSlider.GetPos();
		m_mpScreen.m_Frame = val;
		m_mpScreen.Invalidate(FALSE);
	}

	if((CSliderCtrl *)pScrollBar == &m_MarkerSizeSlider)
	{
		val = m_MarkerSizeSlider.GetPos();
		m_mpScreen.m_MarkerSize = (float)val / 10.0f;
		m_mpScreen.Invalidate(FALSE);
	}	

	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}

void MotionWarping::OnChangeDurationTextbox() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	char text[256];

	m_DurationTextBox.GetWindowText(text, 256);
	m_pMEInfo->Duration = (float)atof(text);
	if(m_pMEInfo->Duration <= 0.0f)
		m_OkButton.EnableWindow(FALSE);
	else
		m_OkButton.EnableWindow(TRUE);
}

void MotionWarping::OnChangeFpsTextbox() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	char text[256];
	int FPS;

	m_FPSTextBox.GetWindowText(text, 256);
	FPS = atoi(text);
	if(FPS <= 0)
		m_OkButton.EnableWindow(FALSE);
	else
	{
		m_pMEInfo->FrameTime = 1.0f / FPS;
		m_OkButton.EnableWindow(TRUE);	
	}
}

void MotionWarping::OnChangeMotionnameWarpingTextbox() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	char text[256];
	CString str;

	m_MotionNameTextBox.GetWindowText(text, 256);
	str.Format("%s", text);

	if(str.GetLength() > 0)
		m_OkButton.EnableWindow(TRUE);
	else
		m_OkButton.EnableWindow(FALSE);
}
