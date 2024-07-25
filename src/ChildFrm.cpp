// ChildFrm.cpp : implementation of the CChildFrame class
//

#include "stdafx.h"
#include "KungfuMotion.h"

#include "ChildFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CChildFrame

IMPLEMENT_DYNCREATE(CChildFrame, CMDIChildWnd)

BEGIN_MESSAGE_MAP(CChildFrame, CMDIChildWnd)
	//{{AFX_MSG_MAP(CChildFrame)
	ON_WM_CREATE()
	ON_WM_MOUSEMOVE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CChildFrame construction/destruction

CChildFrame::CChildFrame()
{
	// TODO: add member initialization code here
	
}

CChildFrame::~CChildFrame()
{
}

BOOL CChildFrame::OnCreateClient( LPCREATESTRUCT /*lpcs*/,
	CCreateContext* pContext)
{
	if(!m_wndSplitter.CreateStatic(this, 1, 2))
		return FALSE;

	RECT rect;
	GetClientRect(&rect);

	int MarkerViewWidth, CurveViewWidth;
	MarkerViewWidth = (int)((float)rect.right * 0.3f);
	CurveViewWidth = (int)((float)rect.right * 0.7f);

	if(!m_wndSplitter.CreateView(0, 0, RUNTIME_CLASS(MarkerView), CSize(MarkerViewWidth, 645), pContext) ||
		!m_wndSplitter.CreateView(0, 1, RUNTIME_CLASS(CurveView), CSize(CurveViewWidth, 645), pContext))
	{
		m_wndSplitter.DestroyWindow();
		return FALSE;
	}

	return TRUE;
}

BOOL CChildFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	if( !CMDIChildWnd::PreCreateWindow(cs) )
		return FALSE;

	cs.style = WS_OVERLAPPEDWINDOW | WS_VISIBLE | FWS_ADDTOTITLE;
	//cs.style = WS_CHILD | WS_VISIBLE | WS_CAPTION | WS_SYSMENU
		//| FWS_ADDTOTITLE;

	return TRUE;
}

void CChildFrame::ActivateFrame(int nCmdShow)
{
	// TODO: Modify this function to change how the frame is activated.

	nCmdShow = SW_SHOWMAXIMIZED;
	CMDIChildWnd::ActivateFrame(nCmdShow);
}


/////////////////////////////////////////////////////////////////////////////
// CChildFrame diagnostics

#ifdef _DEBUG
void CChildFrame::AssertValid() const
{
	CMDIChildWnd::AssertValid();
}

void CChildFrame::Dump(CDumpContext& dc) const
{
	CMDIChildWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CChildFrame message handlers

int CChildFrame::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CMDIChildWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here

	//Set icon for this document.
	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_FIGHT));
	SetIcon(hIcon, TRUE);
	
	return 0;
}

void CChildFrame::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	/*
	RECT rect;
	GetClientRect(&rect);

	int MarkerViewWidth, CurveViewWidth;
	MarkerViewWidth = (int)((float)rect.right * 0.3f);
	CurveViewWidth = (int)((float)rect.right * 0.7f);
	
	m_wndSplitter.SetColumnInfo(0, MarkerViewWidth, MarkerViewWidth);
	m_wndSplitter.SetColumnInfo(1, CurveViewWidth, CurveViewWidth);
	*/

	CMDIChildWnd::OnMouseMove(nFlags, point);
}
