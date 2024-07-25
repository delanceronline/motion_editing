#if !defined(AFX_MARKERVIEW_H__04F1675C_E56B_4E89_906B_667BBB9FD9CF__INCLUDED_)
#define AFX_MARKERVIEW_H__04F1675C_E56B_4E89_906B_667BBB9FD9CF__INCLUDED_

#include "Motion Preview\MotionPreviewScreen.h"
#include "KungfuMotionData.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MarkerView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// MarkerView view

class MarkerView : public CView
{
protected:
	MarkerView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(MarkerView)
	void OnTreeViewClick(NMHDR* pNMHDR, LRESULT* pResult);
	void OnTreeViewKeyDown(NMHDR* pNMHDR, LRESULT* pResult);
	void OnTreeViewDoubleClick(NMHDR* pNMHDR, LRESULT* pResult);
	void OnTreeItemDrag(NMHDR* pNMHDR, LRESULT* pResult);

// Attributes
public:
	CTreeCtrl m_MarkerTree;
	HTREEITEM m_hRoot;
	unsigned int m_ViewType;
	CView * m_pCurveView;

// Operations
public:
	bool BlendTwoMotions(KungfuMotionData *pKFMD1, KungfuMotionData *pKFMD2);
	KungfuMotionData * GetKungfuMotionData(HTREEITEM hItem);
	HTREEITEM m_hItemDrag;
	HTREEITEM m_hItemDrop;
	CImageList * m_DragImageList;
	bool m_IsDraggingMotion;
	CImageList m_TreeImageList;
	void SetCurveView(CView *m_pCurveView_in);
	bool InsertItemToParent(KungfuMotionData *pMB, HTREEITEM hParentItem, unsigned int nMarker_Parent);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(MarkerView)
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	//}}AFX_VIRTUAL

// Implementation
protected:
	MotionPreviewScreen m_mpScreen;

	virtual ~MarkerView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(MarkerView)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnMotionPreview();
	afx_msg void OnMotionBlending();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnCurveFit();
	afx_msg void OnViewCp();
	afx_msg void OnMotionExaggerating();
	afx_msg void OnMotionReplacing();
	afx_msg void OnMotionExtracting();
	afx_msg void OnMotionWarping();
	afx_msg void OnFileSavesnapshot();
	afx_msg void OnMotionTransition();
	afx_msg void OnMotionFilter();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MARKERVIEW_H__04F1675C_E56B_4E89_906B_667BBB9FD9CF__INCLUDED_)
