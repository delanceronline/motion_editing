#if !defined(AFX_DOUBLESCREENDIALOG_H__BCC20763_D451_4318_B9FC_AC0E1D14E724__INCLUDED_)
#define AFX_DOUBLESCREENDIALOG_H__BCC20763_D451_4318_B9FC_AC0E1D14E724__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Curve Fitting\MotionData.h"
#include "Motion Preview\MotionPreviewScreen.h"
#include "KungfuMotionData.h"
#include "Motion Editing\MotionEditing.h"

// DoubleScreenDialog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// DoubleScreenDialog dialog

class DoubleScreenDialog : public CDialog
{
// Construction
public:
	void SetDlgType(UINT DlgType);
	UINT m_DialogType;
	void SwitchToJointSelection(void);
	unsigned int m_numMotionSelected;
	KungfuMotionData * GetKFMD(unsigned int nMotion);
	unsigned int GetCurrentMotionIndex_Tree();
	void ResetMotionTree(void);
	unsigned int m_numKFMD;
	void ResetMarkerTree(void);
	bool m_IsMotionSelectionMode, m_IsJointSelectionMode;
	void SetKungfuMotionDataHead(KungfuMotionData *ptr, unsigned int numKFMD);
	KungfuMotionData * m_pKFMD_Head;
	MotionEditingInfo * m_pMBInfo;
	void SetMBInfoPointer(MotionEditingInfo *pMBInfo);
	unsigned int GetCurrentMarkerIndex_Tree(void);
	bool * m_pMarkerSelectionArray, * m_pMotionSelectionArray;
	HTREEITEM m_hCurrentItem;
	MarkerToItem * m_mti;
	MotionToItem * m_sti;
	HTREEITEM m_hRoot;
	unsigned int m_S1_StartFrame, m_S1_EndFrame, m_S2_StartFrame, m_S2_EndFrame;
	void SetPointer(KungfuMotionData *ptr1, KungfuMotionData *ptr2);
	KungfuMotionData * m_pMotionData1, * m_pMotionData2;
	DoubleScreenDialog(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(DoubleScreenDialog)
	enum { IDD = IDD_DOUBLESCREEN_DIALOG };
	CStatic	m_SimValSliderStatic;
	CButton	m_OkButton;
	CButton	m_DeselectionAllButton;
	CButton	m_SelectionAllButton;
	CStatic	m_S2_StartFrameStatic;
	CStatic	m_S2_EndFrameStatic;
	CStatic	m_S1_StartFrameStatic;
	CStatic	m_S1_EndFrameStatic;
	CSliderCtrl	m_S1_EndFrameSlider;
	CStatic	m_S2_MarkerSizeStatic;
	CStatic	m_S1_MarkerSizeStatic;
	CEdit	m_MotionName;
	CSliderCtrl	m_S2_EndFrameSlider;
	CSliderCtrl	m_S2_StartFrameSlider;
	CSliderCtrl	m_S1_StartFrameSlider;
	CStatic	m_SimValStatic;
	CSliderCtrl	m_SimValSlider;
	CButton	m_DeselectJointButton;
	CButton	m_SelectJointButton;
	CTreeCtrl	m_MarkerTree;
	CListCtrl	m_SelectedMarkerList;
	CButton	m_SketelonButton;
	CButton	m_JointButton;
	CSliderCtrl	m_R2_Slider;
	CSliderCtrl	m_R1_Slider;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(DoubleScreenDialog)
	public:
	virtual void OnFinalRelease();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	MotionPreviewScreen m_mpScreen1, m_mpScreen2;
	CImageList m_ImageList;
	bool InsertItemToParent(KungfuMotionData *pMB, HTREEITEM hParentItem, unsigned int nMarker_Parent);
	// Generated message map functions
	//{{AFX_MSG(DoubleScreenDialog)
	virtual BOOL OnInitDialog();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnDestroy();
	afx_msg void OnSketelonButton();
	afx_msg void OnJointButton();
	afx_msg void OnSelchangedMarkertreeDoublescreen(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelectionButtonDoublescreen();
	afx_msg void OnItemchangedSelectedmarkerlist(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeselectionButtonDoublescreen();
	virtual void OnOK();
	afx_msg void OnSelectionallButtonDoublescreen();
	afx_msg void OnDeselectionallButtonDoublescreen();
	afx_msg void OnChangeMotionnameTextbox();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DOUBLESCREENDIALOG_H__BCC20763_D451_4318_B9FC_AC0E1D14E724__INCLUDED_)
