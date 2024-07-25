// MotionPreviewDialog.cpp : implementation file
//

#include "stdafx.h"
#include "..\KungfuMotion.h"
#include "MotionPreviewDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// MotionPreviewDialog dialog


MotionPreviewDialog::MotionPreviewDialog(CWnd* pParent /*=NULL*/)
	: CDialog(MotionPreviewDialog::IDD, pParent)
{
	m_pTRCBuffer = NULL;

	//{{AFX_DATA_INIT(MotionPreviewDialog)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void MotionPreviewDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(MotionPreviewDialog)
	DDX_Control(pDX, IDC_FRAMESILDER, m_FrameSlider);
	DDX_Control(pDX, IDC_MARKERSIZESILDER, m_MarkerSizeSlider);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(MotionPreviewDialog, CDialog)
	//{{AFX_MSG_MAP(MotionPreviewDialog)
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_PREVIEW_PLAY, OnPreviewPlay)
	ON_BN_CLICKED(IDC_PREVIEW_STOP, OnPreviewStop)
	ON_BN_CLICKED(IDC_PREVIEW_PAUSE, OnPreviewPause)
	ON_WM_HSCROLL()
	ON_WM_KEYDOWN()
	ON_WM_KEYUP()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// MotionPreviewDialog message handlers

int MotionPreviewDialog::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	SetTimer(001, 1, NULL);

	return 0;
}

BOOL MotionPreviewDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here

	m_FrameSlider.SetRange(0, m_pTRCBuffer->numFrames, TRUE);
	m_MarkerSizeSlider.SetPos(0);

	m_MarkerSizeSlider.SetRange(1, 100, FALSE);
	m_MarkerSizeSlider.SetPos(50);

	CButton *button;
	
	// Disable some buttons.
	button = (CButton*)GetDlgItem(IDC_PREVIEW_PAUSE);
	button->EnableWindow(FALSE);
	button = (CButton*)GetDlgItem(IDC_PREVIEW_STOP);
	button->EnableWindow(FALSE);

	// Register window class for motion preview.
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
	wndClass.lpszClassName = "MotionPreviewScreen";

	// Register the window class
	AfxRegisterClass(&wndClass);
	
	CRect glRect, newRect;
	GetDlgItem(IDC_MOTION_PREVIEW_SCREEN_BORDER)->GetWindowRect(&glRect);

	newRect.top = glRect.top - 80;
	newRect.bottom = glRect.bottom - 80;
	newRect.left = glRect.left;
	newRect.right = glRect.right;

	// Create motion preview window.
	m_mpScreen.Create("MotionPreviewScreen", NULL, WS_CHILD | WS_VISIBLE, newRect, this, ID_MOTION_PREVIEW_SCREEN, NULL);
	m_mpScreen.Invalidate(FALSE);
	m_mpScreen.SetPointer(m_pTRCBuffer);
	m_mpScreen.SetPointer(this);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL MotionPreviewDialog::DestroyWindow() 
{
	// TODO: Add your specialized code here and/or call the base class
	m_mpScreen.DestroyWindow();
	KillTimer(001);

	return CDialog::DestroyWindow();
}

void MotionPreviewDialog::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	// Do not call CDialog::OnPaint() for painting messages
}

void MotionPreviewDialog::SetPointer(MotionData *ptr)
{
	m_pTRCBuffer = ptr;
}

void MotionPreviewDialog::OnPreviewPlay() 
{
	// TODO: Add your control notification handler code here
	m_mpScreen.m_IsPlay = true;
	m_mpScreen.m_IsStop = false;
	m_mpScreen.m_IsPaused = false;

	CButton *button;

	button = (CButton*)GetDlgItem(IDC_PREVIEW_PLAY);
	button->EnableWindow(FALSE);
	button = (CButton*)GetDlgItem(IDC_PREVIEW_PAUSE);
	button->EnableWindow(TRUE);
	button = (CButton*)GetDlgItem(IDC_PREVIEW_STOP);
	button->EnableWindow(TRUE);
}

void MotionPreviewDialog::OnPreviewStop() 
{
	// TODO: Add your control notification handler code here
	m_mpScreen.m_IsPlay = false;
	m_mpScreen.m_IsStop = true;
	m_mpScreen.m_IsPaused = false;
	m_mpScreen.m_Frame = 0;

	CButton *button;

	button = (CButton*)GetDlgItem(IDC_PREVIEW_PLAY);
	button->EnableWindow(TRUE);
	button = (CButton*)GetDlgItem(IDC_PREVIEW_PAUSE);
	button->EnableWindow(FALSE);
	button = (CButton*)GetDlgItem(IDC_PREVIEW_STOP);
	button->EnableWindow(FALSE);
}

void MotionPreviewDialog::OnPreviewPause() 
{
	// TODO: Add your control notification handler code here
	m_mpScreen.m_IsPlay = false;
	m_mpScreen.m_IsStop = false;
	m_mpScreen.m_IsPaused = true;

	CButton *button;

	button = (CButton*)GetDlgItem(IDC_PREVIEW_PLAY);
	button->EnableWindow(TRUE);
	button = (CButton*)GetDlgItem(IDC_PREVIEW_PAUSE);
	button->EnableWindow(FALSE);
	button = (CButton*)GetDlgItem(IDC_PREVIEW_STOP);
	button->EnableWindow(TRUE);	
}

void MotionPreviewDialog::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	// TODO: Add your message handler code here and/or call default
	int val;

	if((CSliderCtrl *)pScrollBar == &m_FrameSlider)
	{
		val = m_FrameSlider.GetPos();
		m_mpScreen.m_Frame = val;
	}

	// Change radius of marker balls according to val.
	if((CSliderCtrl *)pScrollBar == &m_MarkerSizeSlider)
	{
		val = m_MarkerSizeSlider.GetPos();
		m_mpScreen.m_MarkerSize = (40.0f * (float)val) / 50.0f;
	}

	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}

void MotionPreviewDialog::UpdateFrameSlider(int val)
{
	m_FrameSlider.SetPos(val);
}

void MotionPreviewDialog::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// TODO: Add your message handler code here and/or call default
	//if(nChar == VK_SPACE)
		//m_mpScreen.m_IsAltDown = true;

	CDialog::OnKeyDown(nChar, nRepCnt, nFlags);
}

void MotionPreviewDialog::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// TODO: Add your message handler code here and/or call default
	//if(nChar == VK_SPACE)
		//m_mpScreen.m_IsAltDown = false;
	
	CDialog::OnKeyUp(nChar, nRepCnt, nFlags);
}

void MotionPreviewDialog::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default

	// Update preview screen.
	m_mpScreen.Invalidate(FALSE);

	// Update frame slider.
	UpdateFrameSlider(m_mpScreen.m_Frame);

	CDialog::OnTimer(nIDEvent);
}
