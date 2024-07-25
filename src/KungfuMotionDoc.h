// KungfuMotionDoc.h : interface of the CKungfuMotionDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_KUNGFUMOTIONDOC_H__2666710D_87F4_4FD8_927A_1D4B4F7B2ABC__INCLUDED_)
#define AFX_KUNGFUMOTIONDOC_H__2666710D_87F4_4FD8_927A_1D4B4F7B2ABC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Curve Fitting\MotionData.h"
#include "Curve Fitting\MotionCurves_UCBS.h"
#include "MarkerView.h"
#include "CurveView.h"
#include "KungfuMotionData.h"

class CKungfuMotionDoc : public CDocument
{
protected: // create from serialization only
	CKungfuMotionDoc();
	DECLARE_DYNCREATE(CKungfuMotionDoc)

// Attributes
public:
	CString m_Message;
	bool m_IsFileLoaded, m_IsMotionSelected, m_IsMarkerSelected, m_ShowCurve;
	MarkerToItem *m_mti;
	unsigned int m_currentMotionData, m_currentMarker, m_currentDOF;
	unsigned int m_numMotionData;
	float m_maxVal, m_minVal;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CKungfuMotionDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	protected:
	virtual BOOL SaveModified();
	//}}AFX_VIRTUAL

// Implementation
public:
	bool DeleteCurrentMotion(void);
	bool DeleteMotionDataNode(KungfuMotionData *pKFMD);
	bool InsertMotionDataNode(KungfuMotionData *md);
	KungfuMotionData *m_pMotionBufferHead, *m_pCurrentMB;
	KungfuMotionData * GetMotionDataBuffer(unsigned int nMotionData);
	KungfuMotionData * GetMotionDataBufferByOrder(unsigned int Index);

	void SetPointer(MarkerView *ptr);
	void SetPointer(CurveView *ptr);
	MarkerView * m_pMarkerView;
	CurveView * m_pCurveView;
	unsigned int m_FileType;
	void ChangeTreeSelection(HTREEITEM hItem);
	virtual ~CKungfuMotionDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
// Generated message map functions
protected:
	//{{AFX_MSG(CKungfuMotionDoc)
	afx_msg void OnFileImport();
	afx_msg void OnFileSaveProject();
	afx_msg void OnFileSaveProjectAs();
	afx_msg void OnFileExport();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_KUNGFUMOTIONDOC_H__2666710D_87F4_4FD8_927A_1D4B4F7B2ABC__INCLUDED_)
