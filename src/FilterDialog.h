#if !defined(AFX_FILTERDIALOG_H__D3270A6A_E93E_4FBC_A982_153A272F4C39__INCLUDED_)
#define AFX_FILTERDIALOG_H__D3270A6A_E93E_4FBC_A982_153A272F4C39__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FilterDialog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// FilterDialog dialog

#include "Motion Editing\MotionEditing.h"
#include "KungfuMotionData.h"

class FilterDialog : public CDialog
{
// Construction
public:
	float m_tol;
	bool m_IsIgnoreSign;
	KungfuMotionData * m_pKFMD;
	MotionMarker *m_pMK;
	unsigned int m_nDOF, m_nMarker;
	void SetPointer(KungfuMotionData *pKFMD, unsigned int nMarker, unsigned int nDOF);
	FilterDialog(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(FilterDialog)
	enum { IDD = IDD_FILTER_DIALOG };
	CStatic	m_MarkerNameStatic;
	CStatic	m_MotionNameStatic;
	CStatic	m_CurveNameStatic;
	CButton	m_OkButton;
	CEdit	m_ToleranceTextBox;
	CButton	m_IgnoreSignCheck;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(FilterDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(FilterDialog)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnChangeToleranceFilterEdit();
	afx_msg void OnIgnoresignFilterCheck();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FILTERDIALOG_H__D3270A6A_E93E_4FBC_A982_153A272F4C39__INCLUDED_)
