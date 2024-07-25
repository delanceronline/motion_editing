#if !defined(AFX_MOTIONWARPING_H__C5369A73_0F53_41EE_81E6_74A29363A88C__INCLUDED_)
#define AFX_MOTIONWARPING_H__C5369A73_0F53_41EE_81E6_74A29363A88C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MotionWarping.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// MotionWarping dialog

#include "Curve Fitting\MotionData.h"
#include "Motion Preview\MotionPreviewScreen.h"
#include "Motion Editing\MotionEditing.h"

class MotionWarping : public CDialog
{
// Construction
public:
	MotionWarping(CWnd* pParent = NULL);   // standard constructor
	MotionEditingInfo * m_pMEInfo;
	void SetPointer(MotionEditingInfo *ptr);
	MotionPreviewScreen m_mpScreen;
	MotionData * m_pMotionData;
	void SetPointer(MotionData *ptr);

// Dialog Data
	//{{AFX_DATA(MotionWarping)
	enum { IDD = IDD_WARPING_DIALOG };
	CButton	m_OkButton;
	CStatic	m_OriginalDurationStatic;
	CEdit	m_MotionNameTextBox;
	CSliderCtrl	m_MarkerSizeSlider;
	CSliderCtrl	m_FrameSlider;
	CEdit	m_FPSTextBox;
	CEdit	m_DurationTextBox;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(MotionWarping)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(MotionWarping)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnChangeDurationTextbox();
	afx_msg void OnChangeFpsTextbox();
	afx_msg void OnChangeMotionnameWarpingTextbox();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MOTIONWARPING_H__C5369A73_0F53_41EE_81E6_74A29363A88C__INCLUDED_)
