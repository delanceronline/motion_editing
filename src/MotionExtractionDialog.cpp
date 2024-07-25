// MotionExtractionDialog.cpp : implementation file
//

#include "stdafx.h"
#include "KungfuMotion.h"
#include "MotionExtractionDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// MotionExtractionDialog dialog


MotionExtractionDialog::MotionExtractionDialog(CWnd* pParent /*=NULL*/)
	: CDialog(MotionExtractionDialog::IDD, pParent)
{
	m_pMotionData = NULL;
	m_pMEInfo = NULL;
	m_StartFrame = 0;
	m_EndFrame = 0;
	//{{AFX_DATA_INIT(MotionExtractionDialog)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

void MotionExtractionDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(MotionExtractionDialog)
	DDX_Control(pDX, IDOK, m_OkButton);
	DDX_Control(pDX, IDC_MARKERSIZE_EXTRACTION_STATIC, m_MarkerSizeStatic);
	DDX_Control(pDX, IDC_MARKERSIZE_EXTRACTION_SLIDER, m_MarkerSizeSlider);
	DDX_Control(pDX, IDC_STARTFRAME_EXTRACTION_STATIC, m_StartFrameStatic);
	DDX_Control(pDX, IDC_STARTFRAME_EXTRACTION_SLIDER, m_StartFrameSlider);
	DDX_Control(pDX, IDC_MOTIONNAME_EXTRACTION_TEXTBOX, m_MotionNameTextBox);
	DDX_Control(pDX, IDC_ENDFRAME_EXTRACTION_STATIC, m_EndFrameStatic);
	DDX_Control(pDX, IDC_ENDFRAME_EXTRACTION_SLIDER, m_EndFrameSlider);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(MotionExtractionDialog, CDialog)
	//{{AFX_MSG_MAP(MotionExtractionDialog)
	ON_WM_HSCROLL()
	ON_EN_CHANGE(IDC_MOTIONNAME_EXTRACTION_TEXTBOX, OnChangeMotionnameExtractionTextbox)
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// MotionExtractionDialog message handlers

void MotionExtractionDialog::SetPointer(MotionData *ptr)
{
	m_pMotionData = ptr;
}

BOOL MotionExtractionDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	// Register window class for motion preview.
	CString str;

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
	wndClass.lpszClassName = "MotionExtractionScreen";

	// Register the window class
	AfxRegisterClass(&wndClass);
	
	CRect glRect, newRect;
	GetDlgItem(IDC_SCREEN_EXTRACTION_BORDER)->GetWindowRect(&glRect);

	newRect.top = glRect.top - 80;
	newRect.bottom = glRect.bottom - 80;
	newRect.left = glRect.left;
	newRect.right = glRect.right;

	// Create motion preview window.
	m_mpScreen.Create("MotionExtractionScreen", NULL, WS_CHILD | WS_VISIBLE, newRect, this, ID_MOTION_EXTRACTION_SCREEN, NULL);
	m_mpScreen.Invalidate(FALSE);
	m_mpScreen.SetPointer(m_pMotionData);
	m_mpScreen.SetPointer(this);	

	// Start Frame Slider.
	m_StartFrameSlider.SetRange(0, m_pMotionData->numFrames, FALSE);
	m_StartFrameSlider.SetPos(0);
	m_StartFrame = 0;
	m_StartFrameStatic.SetWindowText("0");

	// End Frame Slider.
	m_EndFrameSlider.SetRange(0, m_pMotionData->numFrames, FALSE);
	m_EndFrameSlider.SetPos(m_pMotionData->numFrames);
	m_EndFrame = m_pMotionData->numFrames;
	str.Format("%u", m_pMotionData->numFrames);
	m_EndFrameStatic.SetWindowText(str);
	
	m_MotionNameTextBox.SetLimitText(256);

	m_OkButton.EnableWindow(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void MotionExtractionDialog::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	// TODO: Add your message handler code here and/or call default
	int val;
	CString str;

	if((CSliderCtrl *)pScrollBar == &m_StartFrameSlider)
	{
		val = m_StartFrameSlider.GetPos();
		m_mpScreen.m_Frame = val;

		str.Format("%d", val);		
		m_StartFrameStatic.SetWindowText(str);
		m_EndFrameStatic.SetWindowText(str);

		m_EndFrameSlider.SetRange(val, m_pMotionData->numFrames, FALSE);
		m_EndFrameSlider.SetPos(val);

		m_StartFrame = val;

		m_mpScreen.Invalidate(FALSE);
	}	

	if((CSliderCtrl *)pScrollBar == &m_EndFrameSlider)
	{
		val = m_EndFrameSlider.GetPos();
		m_mpScreen.m_Frame = val;

		str.Format("%d", val);		
		m_EndFrameStatic.SetWindowText(str);

		m_EndFrame = val;

		m_mpScreen.Invalidate(FALSE);
	}

	if((CSliderCtrl *)pScrollBar == &m_MarkerSizeSlider)
	{
		val = m_MarkerSizeSlider.GetPos();
		m_mpScreen.m_MarkerSize = (float)val / 10.0f;

		str.Format("%0.2f", m_mpScreen.m_MarkerSize);		
		m_MarkerSizeStatic.SetWindowText(str);

		m_mpScreen.Invalidate(FALSE);
	}

	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}

void MotionExtractionDialog::OnOK() 
{
	// TODO: Add extra validation here
	m_pMEInfo->StartFrame_M1 = m_StartFrame;
	m_pMEInfo->EndFrame_M1 = m_EndFrame;

	m_pMEInfo->numFrames = m_pMEInfo->EndFrame_M1 - m_pMEInfo->StartFrame_M1;
	m_pMEInfo->numMarkers = m_pMotionData->numMarkers;
	m_pMEInfo->FrameTime = m_pMotionData->FrameTime;
	m_pMEInfo->MotionDataType = m_pMotionData->m_MotionDataType;

	char text[256];
	m_MotionNameTextBox.GetWindowText(text, 256);
	strcpy(m_pMEInfo->MotionName, text);

	CDialog::OnOK();
}

void MotionExtractionDialog::SetPointer(MotionEditingInfo *ptr)
{
	m_pMEInfo = ptr;
}

void MotionExtractionDialog::OnChangeMotionnameExtractionTextbox() 
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

void MotionExtractionDialog::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	
	CDialog::OnTimer(nIDEvent);
}
