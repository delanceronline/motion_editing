// CurveView.cpp : implementation file
//

#include "stdafx.h"
#include "KungfuMotion.h"
#include "CurveView.h"
#include "KungfuMotionDoc.h"
#include "MainFrm.h"
#include "MarkerView.h"

#include <math.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CurveView

IMPLEMENT_DYNCREATE(CurveView, CView)

CurveView::CurveView()
{
	hRC = NULL;
	m_ViewType = 2;

	m_TimeScale = 0.0f;
	m_AmpScale = 0.0f;
	m_IsActive = true;
	m_IsPaused = false;
	m_IsShowCurve = false;

	m_MaxX = 20.0f;
	m_MaxY = 10.0f;

	m_leftclip = -2.0f;
	m_rightclip = 22.0f;
	m_upclip = 12.0f;
	m_bottomclip = -12.0f;

	m_prevMouseX = 0;
	m_prevMouseY = 0;

	m_DefMaxX = 20.0f;
	m_DefMaxY = 10.0f;

	m_ZoomInFactor = 1.0f;

	m_IsShowCP = false;
	m_IsShowIP = false;
}

CurveView::~CurveView()
{
	ReleaseAll();
}

BEGIN_MESSAGE_MAP(CurveView, CView)
	//{{AFX_MSG_MAP(CurveView)
	ON_COMMAND(ID_VIEW_RESET, OnViewReset)
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_WM_MOUSEMOVE()
	ON_WM_SIZE()
	ON_WM_TIMER()
	ON_COMMAND(ID_VIEW_ZOOM_IN, OnViewZoomIn)
	ON_COMMAND(ID_VIEW_ZOOM_OUT, OnViewZoomOut)
	ON_COMMAND(ID_VIEW_CP, OnViewCp)
	ON_COMMAND(ID_VIEW_PP, OnViewPp)
	ON_COMMAND(ID_VIEW_IP, OnViewIp)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CurveView drawing

void CurveView::OnDraw(CDC* pDC)
{
	// TODO: add draw code here
}

/////////////////////////////////////////////////////////////////////////////
// CurveView diagnostics

#ifdef _DEBUG
void CurveView::AssertValid() const
{
	CView::AssertValid();
}

void CurveView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CurveView message handlers

void CurveView::OnViewReset() 
{
	// TODO: Add your command handler code here
	m_MaxX = m_DefMaxX;
	m_MaxY = m_DefMaxY;

	m_leftclip = -2.0f;
	m_rightclip = 22.0f;
	m_upclip = 12.0f;
	m_bottomclip = -12.0f;

	m_ZoomInFactor = 1;

	SortPeakValue();
	Invalidate(FALSE);
}

int CurveView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO: Add your specialized creation code here
	
	CKungfuMotionDoc *pDoc = (CKungfuMotionDoc *)GetDocument();	
	pDoc->SetPointer(this);

	SetTimer(001, 4000, NULL);

	return 0;
}

void CurveView::ReSizeGLScene(GLsizei width, GLsizei height)
{
	if (height==0)										// Prevent A Divide By Zero By
	{
		height=1;										// Making Height Equal One
	}

	glViewport(0,0,width,height);						// Reset The Current Viewport

	glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix
	glLoadIdentity();									// Reset The Projection Matrix

	// Left, Right, Buttom and Up of viewport.
	gluOrtho2D(m_leftclip, m_rightclip, m_bottomclip, m_upclip);

	glMatrixMode(GL_MODELVIEW);							// Select The Modelview Matrix
	glLoadIdentity();
}

void CurveView::InitGL()
{
	glShadeModel(GL_SMOOTH);							// Enable Smooth Shading
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);				// White Background
	glEnable(GL_COLOR_MATERIAL);						// Enable Coloring Of Material
	
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void CurveView::OnPaint() 
{
	CPaintDC dc(this);

	// TODO: Add your message handler code here


	static	PIXELFORMATDESCRIPTOR pfd=				// pfd Tells Windows How We Want Things To Be
	{
		sizeof(PIXELFORMATDESCRIPTOR),				// Size Of This Pixel Format Descriptor
		1,											// Version Number
		PFD_DRAW_TO_WINDOW |						// Format Must Support Window
		PFD_SUPPORT_OPENGL |						// Format Must Support OpenGL
		PFD_DOUBLEBUFFER,							// Must Support Double Buffering
		PFD_TYPE_RGBA,								// Request An RGBA Format
		16,										// Select Our Color Depth
		0, 0, 0, 0, 0, 0,							// Color Bits Ignored
		0,											// No Alpha Buffer
		0,											// Shift Bit Ignored
		0,											// No Accumulation Buffer
		0, 0, 0, 0,									// Accumulation Bits Ignored
		16,											// 16Bit Z-Buffer (Depth Buffer)  
		0,											// No Stencil Buffer
		0,											// No Auxiliary Buffer
		PFD_MAIN_PLANE,								// Main Drawing Layer
		0,											// Reserved
		0, 0, 0										// Layer Masks Ignored
	};	


	int pixelformat;

	pixelformat = ChoosePixelFormat((HDC)dc, &pfd);

	if(pixelformat == 0)
	{
		MessageBox("Failed to choose pixel format!", "Error!", MB_OK);
	}

	if(!SetPixelFormat((HDC)dc, pixelformat, &pfd))
	{
		MessageBox("Failed to set pixel format!", "Error!", MB_OK);
	}

	if (!(hRC=wglCreateContext((HDC)dc))) // Are We Able To Get A Rendering Context?
	{
		MessageBox("Failed to create rendering context!", "Error!", MB_OK);
	}

	if(!wglMakeCurrent((HDC)dc,hRC))					// Try To Activate The Rendering Context
	{
		MessageBox("Failed to activate rendering context!", "Error!", MB_OK);
	}

	BuildFont();

	RECT rect;
	GetClientRect(&rect);
	ReSizeGLScene(rect.right, rect.bottom);
	InitGL();	
	
	// Draw curve screen.
	DrawScreen();

	// Swap screen buffer.
	SwapBuffers((HDC)dc);

	if (hRC)											// Do We Have A Rendering Context?
	{
		if (!wglMakeCurrent(NULL,NULL))					// Are We Able To Release The DC And RC Contexts?
		{
			MessageBox("Release Of DC And RC Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		}

		if (!wglDeleteContext(hRC))						// Are We Able To Delete The RC?
		{
			MessageBox("Release Rendering Context Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		}
		hRC=NULL;										// Set RC To NULL
	}
	
	glDeleteLists(m_MetricFontList, 96);
	glDeleteLists(m_FontList, 96);

	// Do not call CView::OnPaint() for painting messages
}

void CurveView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint) 
{
	// TODO: Add your specialized code here and/or call the base class	
	SortPeakValue();
}

bool CurveView::InitializeDevices()
{
	return true;
}

bool CurveView::ReleaseDevices()
{
	if (hRC)											// Do We Have A Rendering Context?
	{
		if (!wglMakeCurrent(NULL,NULL))					// Are We Able To Release The DC And RC Contexts?
		{
			MessageBox("Release Of DC And RC Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		}

		if (!wglDeleteContext(hRC))						// Are We Able To Delete The RC?
		{
			MessageBox("Release Rendering Context Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		}
		hRC=NULL;										// Set RC To NULL
	}

	return true;
}

LRESULT CurveView::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CView::WindowProc(message, wParam, lParam);
}

void CurveView::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	CMainFrame *pMainFrame = (CMainFrame *)AfxGetMainWnd();

	CString str;
	str.Format("x: %d y: %d", point.x, point.y);
	pMainFrame->m_wndStatusBar.SetPaneText(0, str, TRUE);

	if(nFlags == MK_MBUTTON)
	{
		HCURSOR hCursor = AfxGetApp()->LoadStandardCursor(IDC_SIZEALL);
		::SetCursor(hCursor);

		RECT rect;
		GetClientRect(&rect);

		m_leftclip += -(m_MaxX / (float)rect.right) * (point.x - m_prevMouseX);
		m_rightclip = m_leftclip + 24.0f;

		m_bottomclip += (m_MaxX / (float)rect.bottom) * (point.y - m_prevMouseY);
		m_upclip = m_bottomclip + 24.0f;

		Invalidate(FALSE);
	}
	
	m_prevMouseX = point.x;
	m_prevMouseY = point.y;

	CView::OnMouseMove(nFlags, point);
}

void CurveView::OnSize(UINT nType, int cx, int cy) 
{
	CView::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	RECT rect;
	GetClientRect(&rect);
	ReSizeGLScene(rect.right, rect.bottom);	
}

void CurveView::PlotCurve()
{
	CKungfuMotionDoc *pDoc = (CKungfuMotionDoc *)GetDocument();	

	unsigned int i, j, numFrames = 0, numDOFs;
	PathPoint pp;
	ControlPoint cp;
	float x, t;
	
	if(pDoc->m_FileType != UNKNOWN_TYPE)
		numFrames = pDoc->m_pCurrentMB->MotionBuffer.numFrames;

	if(pDoc->m_IsMarkerSelected)
	{
		// Display all curves together when marker is selected.
		numDOFs = (pDoc->m_pCurrentMB->MotionBuffer.GetMarker(pDoc->m_currentMarker))->numDOFs;
		for(i = 0; i < numDOFs; i++)
		{
			SetCurveColor(TRC_TYPE, i);

			// Plot PP curves.
			if(pDoc->m_pCurrentMB->m_IsShowPP)
			{
				glBegin(GL_LINE_STRIP);
				for(j = 0; j < numFrames; j++)
				{
					pp = pDoc->m_pCurrentMB->MC_UCBS->GetPathPoint(pDoc->m_currentMarker, j, i);
							
					x = m_AmpScale * pp.GetAmpVal();
					t = m_TimeScale * (float)j;

					glVertex2f(t, x);
				}
				glEnd();
			}

			// Plot CP curves.
			if(pDoc->m_pCurrentMB->m_IsShowCP)
			{
				glPointSize(4.0f);
				glBegin(GL_POINTS);
				for(j = 0; j < numFrames; j++)
				{
					cp = pDoc->m_pCurrentMB->MC_UCBS->GetControlPoint(pDoc->m_currentMarker, j, i);
							
					x = m_AmpScale * cp.GetAmpVal();
					t = m_TimeScale * (float)j;

					glVertex2f(t, x);
				}
				glEnd();
				glPointSize(1.0f);
			}

			// Plot IP curves.
			if(pDoc->m_pCurrentMB->m_IsShowIP)
			{
				float var, t1, t2, length, time, TimeAxisScale, timestep = 0.0f;
				RECT rect;
	
				length = pDoc->m_pCurrentMB->MotionBuffer.numFrames * pDoc->m_pCurrentMB->MotionBuffer.FrameTime;
				var = length / m_MaxX;
				TimeAxisScale = m_MaxX / length;

				t1 = m_leftclip * var;
				t2 = m_rightclip * var;
				
				if(m_leftclip < 0.0f)
					t1 = 0.0f;
				if(m_rightclip < 0.0f)
					t2 = 0.0f;

				if(m_leftclip > m_MaxX)
					t1 = length;
				if(m_rightclip > m_MaxX)
					t2 = length;

				// Find timestep.
				if(t1 > 0.0f || t2 > 0.0f)
				{
					GetClientRect(&rect);
					timestep = (t2 - t1) / (float)rect.right;
				}

				glColor4f(1.0f, 0.0f, 0.0f, 0.4);
				glLineWidth(4.0f);
				glBegin(GL_LINE_STRIP);
				for(time = t1; time <= t2; time += timestep)
				{
					x = m_AmpScale * pDoc->m_pCurrentMB->MC_UCBS->GetCurveVal(pDoc->m_currentMarker, i, time);
					t = TimeAxisScale * (float)time;

					glVertex2f(t, x);
				}
				glEnd();
				glLineWidth(1.0f);
			}
		}// End loop each DOF.

	}// End show marker curves.
	else
	{
		// Display 1 curve for specified DOF.

		//glColor3f(1.0f, 0.0f, 0.0f);
		SetCurveColor(TRC_TYPE, pDoc->m_currentDOF);

		if(pDoc->m_pCurrentMB->m_IsShowPP)
		{
			glBegin(GL_LINE_STRIP);
			for(i = 0; i < numFrames; i++)
			{
				pp = pDoc->m_pCurrentMB->MC_UCBS->GetPathPoint(pDoc->m_currentMarker, i, pDoc->m_currentDOF);
						
				x = m_AmpScale * pp.GetAmpVal();
				t = m_TimeScale * (float)i;

				glVertex2f(t, x);	
			}
			glEnd();
		}

		// Plot CP curve.
		if(pDoc->m_pCurrentMB->m_IsShowCP)
		{
			glPointSize(4.0f);
			glBegin(GL_POINTS);
			for(j = 0; j < numFrames; j++)
			{
				cp = pDoc->m_pCurrentMB->MC_UCBS->GetControlPoint(pDoc->m_currentMarker, j, pDoc->m_currentDOF);
							
				x = m_AmpScale * cp.GetAmpVal();
				t = m_TimeScale * (float)j;

				glVertex2f(t, x);
			}
			glEnd();
			glPointSize(1.0f);
		}

		// Plot IP curve.
		if(pDoc->m_pCurrentMB->m_IsShowIP)
		{
			float var, t1, t2, length, time, TimeAxisScale, timestep = 0.0f;
			RECT rect;
	
			length = pDoc->m_pCurrentMB->MotionBuffer.numFrames * pDoc->m_pCurrentMB->MotionBuffer.FrameTime;
			var = length / m_MaxX;
			TimeAxisScale = m_MaxX / length;

			t1 = m_leftclip * var;
			t2 = m_rightclip * var;
				
			if(m_leftclip < 0.0f)
				t1 = 0.0f;
			if(m_rightclip < 0.0f)
				t2 = 0.0f;

			if(m_leftclip > m_MaxX)
				t1 = length;
			if(m_rightclip > m_MaxX)
				t2 = length;

			// Find timestep.
			if(t1 > 0.0f || t2 > 0.0f)
			{
				GetClientRect(&rect);
				timestep = (t2 - t1) / (float)rect.right;
			}

			glColor4f(1.0f, 0.0f, 0.0f, 0.4);
			glLineWidth(4.0f);
			glBegin(GL_LINE_STRIP);
			for(time = t1; time <= t2; time += timestep)
			{
				x = m_AmpScale * pDoc->m_pCurrentMB->MC_UCBS->GetCurveVal(pDoc->m_currentMarker, pDoc->m_currentDOF, time);
				t = TimeAxisScale * (float)time;

				glVertex2f(t, x);
			}
			glEnd();
			glLineWidth(1.0f);
		}
	}// End show DOF's curve.

	if(pDoc->m_pCurrentMB->m_IsShowPP || pDoc->m_pCurrentMB->m_IsShowIP || pDoc->m_pCurrentMB->m_IsShowCP)
	{
		DrawMetric();
		DisplayAttribute();
	}
}

void CurveView::SetCurveColor(unsigned int FileType, unsigned int DOF)
{
	if(FileType == TRC_TYPE)
	{
		if(DOF == 0)
			glColor3f(1.0f, 0.0f, 0.0f);
		if(DOF == 1)
			glColor3f(1.0f, 0.0f, 1.0f);
		if(DOF == 2)
			glColor3f(0.0f, 0.0f, 1.0f);
		if(DOF == 3)
			glColor3f(0.0f, 0.5f, 1.0f);
		if(DOF == 4)
			glColor3f(1.0f, 0.5f, 0.0f);
		if(DOF == 5)
			glColor3f(0.0f, 0.5f, 0.0f);
	}
}

bool CurveView::BuildFont(void)
{
	CPaintDC dc(this);

	HFONT	font1, font2;										// Windows Font ID

	m_MetricFontList = glGenLists(96);								// Storage For 96 Characters

	font1 = CreateFont(	14,							// Height Of Font
						0,								// Width Of Font
						0,								// Angle Of Escapement
						0,								// Orientation Angle
						FW_REGULAR,						// Font Weight
						FALSE,							// Italic
						FALSE,							// Underline
						FALSE,							// Strikeout
						ANSI_CHARSET,					// Character Set Identifier
						OUT_TT_PRECIS,					// Output Precision
						CLIP_DEFAULT_PRECIS,			// Clipping Precision
						ANTIALIASED_QUALITY,			// Output Quality
						FF_DONTCARE|DEFAULT_PITCH,		// Family And Pitch
						NULL);					// Font Name

	SelectObject((HDC)dc, font1);							// Selects The Font We Want

	wglUseFontBitmaps((HDC)dc, 32, 96, m_MetricFontList);				// Builds 96 Characters Starting At Character 32
	
	m_FontList = glGenLists(96);								// Storage For 96 Characters

	font2 = CreateFont(	18,							// Height Of Font
						0,								// Width Of Font
						0,								// Angle Of Escapement
						0,								// Orientation Angle
						FW_BOLD,						// Font Weight
						FALSE,							// Italic
						FALSE,							// Underline
						FALSE,							// Strikeout
						ANSI_CHARSET,					// Character Set Identifier
						OUT_TT_PRECIS,					// Output Precision
						CLIP_DEFAULT_PRECIS,			// Clipping Precision
						ANTIALIASED_QUALITY,			// Output Quality
						FF_DONTCARE|DEFAULT_PITCH,		// Family And Pitch
						NULL);					// Font Name

	SelectObject((HDC)dc, font2);							// Selects The Font We Want

	wglUseFontBitmaps((HDC)dc, 32, 96, m_FontList);				// Builds 96 Characters Starting At Character 32

	return true;
}

void CurveView::DrawNormalText(const char *fmt, ...)
{
	char		text[256];								// Holds Our String
	va_list		ap;										// Pointer To List Of Arguments

	if (fmt == NULL)									// If There's No Text
		return;											// Do Nothing

	va_start(ap, fmt);									// Parses The String For Variables
	    vsprintf(text, fmt, ap);						// And Converts Symbols To Actual Numbers
	va_end(ap);											// Results Are Stored In Text

	glPushAttrib(GL_LIST_BIT);							// Pushes The Display List Bits
	glListBase(m_FontList - 32);								// Sets The Base Character to 32
	glCallLists(strlen(text), GL_UNSIGNED_BYTE, text);	// Draws The Display List Text
	glPopAttrib();		

}

void CurveView::DrawMetricText(const char *fmt, ...)
{
	char		text[256];								// Holds Our String
	va_list		ap;										// Pointer To List Of Arguments

	if (fmt == NULL)									// If There's No Text
		return;											// Do Nothing

	va_start(ap, fmt);									// Parses The String For Variables
	    vsprintf(text, fmt, ap);						// And Converts Symbols To Actual Numbers
	va_end(ap);											// Results Are Stored In Text

	glPushAttrib(GL_LIST_BIT);							// Pushes The Display List Bits
	glListBase(m_MetricFontList - 32);								// Sets The Base Character to 32
	glCallLists(strlen(text), GL_UNSIGNED_BYTE, text);	// Draws The Display List Text
	glPopAttrib();		
}

void CurveView::DrawMetric()
{
	float i;
	CString str;

	glColor3f(0.0f, 0.0f, 0.0f);
	for(i = -m_MaxY; i <= m_MaxY; i += 1.0f)
	{
		glBegin(GL_LINES);
		{
			glVertex2f(-0.5f, i);
			glVertex2f(0.0f, i);
		}
		glEnd();

		str.Format("%0.3f", i / m_AmpScale);
		glRasterPos2f(-1.5f, i);
		DrawMetricText(str);
	}
	for(i = 0.0f; i <= m_MaxX; i += 4.0f)
	{
		glBegin(GL_LINES);
		{
			glVertex2f(i, 0.0f);
			glVertex2f(i, -0.5f);
		}
		glEnd();

		str.Format("%0.3f", i / m_TimeScale);
		glRasterPos2f(i, -1.0f);
		DrawMetricText(str);
	}
}

void CurveView::DisplayAttribute()
{
	CKungfuMotionDoc *pDoc = (CKungfuMotionDoc *)GetDocument();

	CString str;
	str.Format("%s", pDoc->m_pCurrentMB->MotionBuffer.GetMarkerName(pDoc->m_currentMarker));

	glColor3f(0.0f, 0.5f, 0.8f);
	glRasterPos2f(14.0f, 10.0f);
	DrawNormalText(str);
}

bool CurveView::InitializeAll()
{
	return true;
}

bool CurveView::ReleaseAll()
{
	return true;
}

void CurveView::DrawScreen(void)
{
	CKungfuMotionDoc *pDoc = (CKungfuMotionDoc *)GetDocument();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Clear Screen And Depth Buffer
	glLoadIdentity();									// Reset The Current Modelview Matrix
		
	// Draw grids.
	float i;

	glColor3f(0.85f, 0.85f, 0.85f);
	// Vertical lines.
	for(i = 0.0f; i <= m_MaxX; i += 1.0f)
	{
		glBegin(GL_LINES);
			glVertex2f(i, -m_MaxY);
			glVertex2f(i, m_MaxY);
		glEnd();
	}
	// Horizontal lines.
	for(i = -m_MaxY; i <= m_MaxY; i += 1.0f)
	{
		glBegin(GL_LINES);
			glVertex2f(0.0f, i);
			glVertex2f(m_MaxX, i);
		glEnd();
	}

	// Draw Axis.
	glColor3f(0.0f, 0.0f, 0.0f);
	glBegin(GL_LINES);
		// X AXIS
		glVertex2f(-2.0f, 0.0f);
		glVertex2f(m_MaxX, 0.0f);
		// Y AXIS
		glVertex2f(0.0f, -m_MaxY);
		glVertex2f(0.0f, m_MaxY);
	glEnd();

	if(m_IsShowCurve)
		PlotCurve();
	/*
	// Draw graph.
	if(pDoc->m_ShowCurve)
	{
		PlotCurve();
		DrawMetric();
		DisplayAttribute();
	}
	*/
}

void CurveView::SetMarkerViewPointer(CView *m_pMarkerView_in)
{
	m_pMarkerView = m_pMarkerView_in;
}

BOOL CurveView::DestroyWindow() 
{
	// TODO: Add your specialized code here and/or call the base class
	KillTimer(001);

	return CView::DestroyWindow();
}

void CurveView::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	
	CView::OnTimer(nIDEvent);
}

void CurveView::OnViewZoomIn() 
{
	// TODO: Add your command handler code here
	m_ZoomInFactor += 1.0f;
	m_MaxX = m_DefMaxX * m_ZoomInFactor;
	m_MaxY = m_DefMaxY * m_ZoomInFactor;

	SortPeakValue();
	Invalidate(FALSE);
}

void CurveView::OnViewZoomOut() 
{
	// TODO: Add your command handler code here
	if(m_ZoomInFactor > 1.0f)
	{
		m_ZoomInFactor -= 1.0f;
		m_MaxX = m_DefMaxX * m_ZoomInFactor;
		m_MaxY = m_DefMaxY * m_ZoomInFactor;

		SortPeakValue();
		Invalidate(FALSE);
	}
}

void CurveView::SortPeakValue()
{
	// Following codes are to find out max and min values from curves being shown.
	CKungfuMotionDoc *pDoc = (CKungfuMotionDoc *)GetDocument();

	if(pDoc->m_IsFileLoaded)
	{
		if(pDoc->m_FileType != UNKNOWN_TYPE)
			m_numPoints = pDoc->m_pCurrentMB->MotionBuffer.numFrames;
			
		m_TimeScale = m_MaxX / (float)(m_numPoints);

		if(pDoc->m_IsMarkerSelected)
		{
			// Draw all DOFs' curves.
			unsigned int i, j = 0, numDOFs;
			float *peak, prev, biggest;

			numDOFs = (pDoc->m_pCurrentMB->MotionBuffer.GetMarker(pDoc->m_currentMarker))->numDOFs;

			peak = new float[numDOFs * 2];

			for(i = 0; i < numDOFs; i++)
			{
				peak[j] = (float)fabs((double)(pDoc->m_pCurrentMB->MC_UCBS->GetMaxAmp(pDoc->m_currentMarker, i)));
				peak[j + 1] = (float)fabs((double)(pDoc->m_pCurrentMB->MC_UCBS->GetMinAmp(pDoc->m_currentMarker, i)));
				j += 2;
			}

			// Sort for largest value from peaks.
			for(i = 0; i < numDOFs * 2; i++)
			{
				if(i == 0)
					biggest = peak[0];
				else
				{
					if(peak[i] > biggest)
						biggest = peak[i];
				}

				prev = peak[i];
			}

			m_AmpScale = m_MaxY / biggest;

			delete[] peak;
		}
		else
		{
			// Draw one curve only.
			float pos, neg;

			pos = (float)fabs((double)(pDoc->m_pCurrentMB->MC_UCBS->GetMaxAmp(pDoc->m_currentMarker, pDoc->m_currentDOF)));
			neg = (float)fabs((double)(pDoc->m_pCurrentMB->MC_UCBS->GetMinAmp(pDoc->m_currentMarker, pDoc->m_currentDOF)));

			if(pos > neg)
				m_AmpScale = m_MaxY / pos;
			else
				m_AmpScale = m_MaxY / neg;
		}

	}// End if file loaded.
}

void CurveView::OnViewCp() 
{
	// TODO: Add your command handler code here
	CKungfuMotionDoc *pDoc = (CKungfuMotionDoc *)GetDocument();
	if(pDoc->m_IsFileLoaded)
	{
		if(pDoc->m_pCurrentMB->MotionBuffer.m_IsAllMarkersFitted)
		{
			pDoc->m_pCurrentMB->m_IsShowCP = !pDoc->m_pCurrentMB->m_IsShowCP;
			m_IsShowCP = !m_IsShowCP;
			Invalidate(FALSE);
		}
	}
}

void CurveView::OnViewPp() 
{
	// TODO: Add your command handler code here
	CKungfuMotionDoc *pDoc = (CKungfuMotionDoc *)GetDocument();
	
	if(pDoc->m_IsFileLoaded)
	{
		pDoc->m_pCurrentMB->m_IsShowPP = !pDoc->m_pCurrentMB->m_IsShowPP;
		m_IsShowCurve = !m_IsShowCurve;
		Invalidate(FALSE);
	}
}

void CurveView::OnViewIp() 
{
	// TODO: Add your command handler code here
	CKungfuMotionDoc *pDoc = (CKungfuMotionDoc *)GetDocument();
	if(pDoc->m_IsFileLoaded)
	{
		if(pDoc->m_pCurrentMB->MotionBuffer.m_IsAllMarkersFitted)
		{
			pDoc->m_pCurrentMB->m_IsShowIP = !pDoc->m_pCurrentMB->m_IsShowIP;
			m_IsShowIP = !m_IsShowIP;
			Invalidate(FALSE);
		}
	}
}
