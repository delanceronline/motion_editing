#if !defined(AFX_MOTIONPREVIEWSCREEN_H__CBC97FC3_ED27_48BD_8844_06E562634C4E__INCLUDED_)
#define AFX_MOTIONPREVIEWSCREEN_H__CBC97FC3_ED27_48BD_8844_06E562634C4E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <gl\gl.h>			// Header File For The OpenGL32 Library
#include <gl\glu.h>			// Header File For The GLu32 Library
#include <gl\glaux.h>		// Header File For The Glaux Library
#include <gl\glut.h>

#include "..\Curve Fitting\MotionData.h"

// MotionPreviewScreen.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// MotionPreviewScreen window

class MotionPreviewScreen : public CWnd
{
// Construction
public:
	MotionPreviewScreen();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(MotionPreviewScreen)
	public:
	virtual BOOL DestroyWindow();
	//}}AFX_VIRTUAL

// Implementation
public:
	void ChangePointer(MotionData *ptr);
	void GetFrameBuffer(void);
	//GLubyte m_FrameBuffer[196606 * 3 + 1];
	float m_time;
	bool m_IsByCurve;
	float DegToRad(float deg);
	int m_mouseX, m_mouseY;
	float m_step;
	float m_angle, m_till, m_radius, m_tranX, m_tranY, m_tranZ;
	float m_AspectRatio;
	float m_offsetX, m_offsetY;
	void SetPointer(CDialog *ptr);
	void SetPointer(MotionData *ptr);
	MotionData *m_pMotionBuffer;
	LARGE_INTEGER m_timerFrequency;
	LARGE_INTEGER m_globeTime;	// Start time of the animation
	LARGE_INTEGER m_currentTime;	// The current time
	double m_FrameTime;
	double m_TimeElapsed;				// Elapsed time in seconds
	double m_prevTime;				// Previous time in seconds
	float m_AccuTime;
	unsigned int m_Frame;
	float m_MarkerSize;
	bool m_IsPlay, m_IsPaused, m_IsStop;
	bool m_IsAltDown;
	CDialog *m_pDialog;

	int m_nPixelFormat;
	HGLRC m_hRC;
	HDC m_hDC;

	void Draw(void);
	void ReSizeGLScene(GLsizei width, GLsizei height);
	virtual ~MotionPreviewScreen();

	// Generated message map functions
protected:
	//{{AFX_MSG(MotionPreviewScreen)
	afx_msg void OnPaint();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int w, int h);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MOTIONPREVIEWSCREEN_H__CBC97FC3_ED27_48BD_8844_06E562634C4E__INCLUDED_)
