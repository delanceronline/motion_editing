// KungfuMotionView.cpp : implementation of the CKungfuMotionView class
//

#include "stdafx.h"
#include "KungfuMotion.h"
#include "KungfuMotionDoc.h"
#include "KungfuMotionView.h"

#include "Motion Preview\MotionPreviewDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CKungfuMotionView

IMPLEMENT_DYNCREATE(CKungfuMotionView, CView)

BEGIN_MESSAGE_MAP(CKungfuMotionView, CView)
	//{{AFX_MSG_MAP(CKungfuMotionView)
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CKungfuMotionView construction/destruction

CKungfuMotionView::CKungfuMotionView()
{
	// TODO: add construction code here
}

CKungfuMotionView::~CKungfuMotionView()
{
}

BOOL CKungfuMotionView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CKungfuMotionView drawing

void CKungfuMotionView::OnDraw(CDC* pDC)
{
	CKungfuMotionDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	// TODO: add draw code for native data here
}

/////////////////////////////////////////////////////////////////////////////
// CKungfuMotionView printing

BOOL CKungfuMotionView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CKungfuMotionView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CKungfuMotionView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CKungfuMotionView diagnostics

#ifdef _DEBUG
void CKungfuMotionView::AssertValid() const
{
	CView::AssertValid();
}

void CKungfuMotionView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CKungfuMotionDoc* CKungfuMotionView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CKungfuMotionDoc)));
	return (CKungfuMotionDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CKungfuMotionView message handlers
