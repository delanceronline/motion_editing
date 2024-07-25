// KungfuMotionView.h : interface of the CKungfuMotionView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_KUNGFUMOTIONVIEW_H__10E0207E_3BFE_4CA2_AB00_D7EF091C2D73__INCLUDED_)
#define AFX_KUNGFUMOTIONVIEW_H__10E0207E_3BFE_4CA2_AB00_D7EF091C2D73__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CKungfuMotionView : public CView
{
protected: // create from serialization only
	CKungfuMotionView();
	DECLARE_DYNCREATE(CKungfuMotionView)

// Attributes
public:
	CKungfuMotionDoc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CKungfuMotionView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CKungfuMotionView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CKungfuMotionView)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in KungfuMotionView.cpp
inline CKungfuMotionDoc* CKungfuMotionView::GetDocument()
   { return (CKungfuMotionDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_KUNGFUMOTIONVIEW_H__10E0207E_3BFE_4CA2_AB00_D7EF091C2D73__INCLUDED_)
