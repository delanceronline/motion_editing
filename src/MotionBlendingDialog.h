#if !defined(AFX_MOTIONBLENDINGDIALOG_H__3FB0A51E_F57D_41D8_A515_E3690814564A__INCLUDED_)
#define AFX_MOTIONBLENDINGDIALOG_H__3FB0A51E_F57D_41D8_A515_E3690814564A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MotionBlendingDialog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// MotionBlendingDialog dialog

class MotionBlendingDialog : public CDialog
{
// Construction
public:
	MotionBlendingDialog(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(MotionBlendingDialog)
	enum { IDD = IDD_BLENDING_OPTION_DIALOG };
	CSliderCtrl	m_SFSlider;
	CSliderCtrl	m_EFSlider;
	CSliderCtrl	m_BVSlider;
	CString	m_MotionName;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(MotionBlendingDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
public:
	float m_BlendingValue;

protected:

	// Generated message map functions
	//{{AFX_MSG(MotionBlendingDialog)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MOTIONBLENDINGDIALOG_H__3FB0A51E_F57D_41D8_A515_E3690814564A__INCLUDED_)
