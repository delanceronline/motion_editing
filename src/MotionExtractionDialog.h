#if !defined(AFX_MOTIONEXTRACTIONDIALOG_H__45C34FE1_C77A_441D_893C_E0205AD07252__INCLUDED_)
#define AFX_MOTIONEXTRACTIONDIALOG_H__45C34FE1_C77A_441D_893C_E0205AD07252__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MotionExtractionDialog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// MotionExtractionDialog dialog

#include "Curve Fitting\MotionData.h"
#include "Motion Preview\MotionPreviewScreen.h"
#include "Motion Editing\MotionEditing.h"

class MotionExtractionDialog : public CDialog
{
// Construction
public:
	void SetPointer(MotionEditingInfo *ptr);
	MotionEditingInfo * m_pMEInfo;
	unsigned int m_EndFrame;
	unsigned int m_StartFrame;
	void SetPointer(MotionData *ptr);
	MotionPreviewScreen m_mpScreen;
	MotionData * m_pMotionData;
	MotionExtractionDialog(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(MotionExtractionDialog)
	enum { IDD = IDD_EXTRACT_DIALOG };
	CButton	m_OkButton;
	CStatic	m_MarkerSizeStatic;
	CSliderCtrl	m_MarkerSizeSlider;
	CStatic	m_StartFrameStatic;
	CSliderCtrl	m_StartFrameSlider;
	CEdit	m_MotionNameTextBox;
	CStatic	m_EndFrameStatic;
	CSliderCtrl	m_EndFrameSlider;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(MotionExtractionDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(MotionExtractionDialog)
	virtual BOOL OnInitDialog();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	virtual void OnOK();
	afx_msg void OnChangeMotionnameExtractionTextbox();
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MOTIONEXTRACTIONDIALOG_H__45C34FE1_C77A_441D_893C_E0205AD07252__INCLUDED_)
