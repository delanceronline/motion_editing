// KungfuMotion.h : main header file for the KUNGFUMOTION application
//

#if !defined(AFX_KUNGFUMOTION_H__ACB0B6A9_0D93_4F14_9290_5A04CC410FFA__INCLUDED_)
#define AFX_KUNGFUMOTION_H__ACB0B6A9_0D93_4F14_9290_5A04CC410FFA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CKungfuMotionApp:
// See KungfuMotion.cpp for the implementation of this class
//
struct CurveToItem
{
	unsigned int curveIndex;
	HTREEITEM tItem;
};

struct MarkerToItem
{
	unsigned int markerIndex;
	HTREEITEM tItem;
	CurveToItem *cti;
};

struct MotionToItem
{
	unsigned int nMotion;
	HTREEITEM tItem;
};

class CKungfuMotionApp : public CWinApp
{
public:
	CKungfuMotionApp();


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CKungfuMotionApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CKungfuMotionApp)
	afx_msg void OnAppAbout();
	afx_msg void OnFileNewProject();
	afx_msg void OnHelpHelp();
	afx_msg void OnAppAboutKungfuMotion();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_KUNGFUMOTION_H__ACB0B6A9_0D93_4F14_9290_5A04CC410FFA__INCLUDED_)
