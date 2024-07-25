#if !defined(AFX_CURVEVIEW_H__D256AFFD_97B1_4097_9526_96F95AE90087__INCLUDED_)
#define AFX_CURVEVIEW_H__D256AFFD_97B1_4097_9526_96F95AE90087__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CurveView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CurveView view

#include <gl\gl.h>			// Header File For The OpenGL32 Library
#include <gl\glu.h>			// Header File For The GLu32 Library
#include <gl\glaux.h>		// Header File For The Glaux Library

class CurveView : public CView
{
protected:
	CurveView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CurveView)

// Attributes
public:
	HGLRC hRC;
	float m_TimeScale, m_AmpScale;
	unsigned int m_numPoints;
	unsigned int m_ViewType;
	bool m_IsActive;
	bool m_IsPaused;
	bool m_IsShowCurve;
	CView * m_pMarkerView;
	PIXELFORMATDESCRIPTOR m_pfd;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CurveView)
	public:
	virtual BOOL DestroyWindow();
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
public:
	bool m_IsShowIP;
	bool m_IsShowCP;
	float m_ZoomInFactor;
	float m_DefMaxY;
	float m_DefMaxX;
	void SortPeakValue(void);
	int m_prevMouseX, m_prevMouseY;
	float m_bottomclip;
	float m_upclip;
	float m_rightclip;
	float m_leftclip;
	float m_MaxY;
	float m_MaxX;
	void SetMarkerViewPointer(CView *m_pMarkerView_in);
	void DrawScreen(void);
	bool ReleaseDevices(void);
	bool InitializeDevices(void);
	void InitGL(void);
	void ReSizeGLScene(GLsizei width, GLsizei height);
	bool InitializeAll(void);
	bool ReleaseAll(void);
	void DisplayAttribute(void);
	void DrawNormalText(const char *fmt, ...);
	void DrawMetric(void);
	unsigned int m_MetricFontList, m_FontList;
	bool BuildFont(void);
	void DrawMetricText(const char *fmt, ...);
	void SetCurveColor(unsigned int FileType, unsigned int DOF);
	void PlotCurve(void);
protected:
	virtual ~CurveView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CurveView)
	afx_msg void OnViewReset();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnViewZoomIn();
	afx_msg void OnViewZoomOut();
	afx_msg void OnViewCp();
	afx_msg void OnViewPp();
	afx_msg void OnViewIp();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CURVEVIEW_H__D256AFFD_97B1_4097_9526_96F95AE90087__INCLUDED_)
