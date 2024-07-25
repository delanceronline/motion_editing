#if !defined(AFX_MOTIONPREVIEWDIALOG_H__FA339698_F179_4222_9EE6_4839D082800A__INCLUDED_)
#define AFX_MOTIONPREVIEWDIALOG_H__FA339698_F179_4222_9EE6_4839D082800A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "MotionPreviewScreen.h"
#include "..\Curve Fitting\MotionData.h"

// MotionPreviewDialog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// MotionPreviewDialog dialog

class MotionPreviewDialog : public CDialog
{
// Construction
public:
	void UpdateFrameSlider(int val);
	void SetPointer(MotionData *ptr);
	MotionPreviewDialog(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(MotionPreviewDialog)
	enum { IDD = IDD_PREVIEW_DIALOG };
	CSliderCtrl	m_FrameSlider;
	CSliderCtrl	m_MarkerSizeSlider;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(MotionPreviewDialog)
	public:
	virtual BOOL DestroyWindow();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	MotionPreviewScreen m_mpScreen;
	MotionData *m_pTRCBuffer;
	// Generated message map functions
	//{{AFX_MSG(MotionPreviewDialog)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnPreviewPlay();
	afx_msg void OnPreviewStop();
	afx_msg void OnPreviewPause();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MOTIONPREVIEWDIALOG_H__FA339698_F179_4222_9EE6_4839D082800A__INCLUDED_)
