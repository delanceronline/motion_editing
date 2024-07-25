#if !defined(AFX_SNAPSHOTDIALOG_H__C2B23D2B_A979_49AC_9CE0_91DBB76A9B4B__INCLUDED_)
#define AFX_SNAPSHOTDIALOG_H__C2B23D2B_A979_49AC_9CE0_91DBB76A9B4B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SnapShotDialog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// SnapShotDialog dialog

#include "Curve Fitting\MotionData.h"
#include "Motion Preview\MotionPreviewScreen.h"
#include "Motion Editing\MotionEditing.h"
#include "KungfuMotionData.h"
#include <gl\gl.h>
#include <gl\glu.h>

class SnapShotDialog : public CDialog
{
// Construction
public:
	unsigned int m_MaxFrame;
	KungfuMotionData * m_pCurrentKFMD;
	unsigned int m_numKFMD;
	HTREEITEM m_hCurrentItem;
	MotionToItem * m_sti;
	int SaveDIBitmap(const char *filename, BITMAPINFO *info, GLubyte *bits);
	unsigned int GetCurrentMotionIndex_Tree(void);
	void SetPointer(KungfuMotionData *ptr, unsigned int numKFMD);
	void SetPointer(MotionData *ptr);
	SnapShotDialog(CWnd* pParent = NULL);   // standard constructor
	MotionPreviewScreen m_mpScreen;
	MotionData * m_pMotionData;
	KungfuMotionData *m_pKFMD;
	CImageList m_ImageList;
	KungfuMotionData * GetKFMD(unsigned int nMotion);

// Dialog Data
	//{{AFX_DATA(SnapShotDialog)
	enum { IDD = IDD_SNAPSHOT_DIALOG };
	CSliderCtrl	m_MarkerSizeSlider;
	CSliderCtrl	m_FrameSlider;
	CEdit	m_TimeTextBox;
	CButton	m_ShowButton;
	CButton	m_SaveAsButton;
	CTreeCtrl	m_MotionTree;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(SnapShotDialog)
	public:
	virtual BOOL DestroyWindow();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(SnapShotDialog)
	virtual BOOL OnInitDialog();
	afx_msg void OnChangeTimeSnapshotTextbox();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnSelchangedMotiontreeSnapshotTree(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnShowSnapshotButton();
	afx_msg void OnSaveSnapshotButton();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SNAPSHOTDIALOG_H__C2B23D2B_A979_49AC_9CE0_91DBB76A9B4B__INCLUDED_)
