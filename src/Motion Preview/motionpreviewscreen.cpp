// MotionPreviewScreen.cpp : implementation file
//

#include "stdafx.h"
#include "..\KungfuMotion.h"
#include "..\Curve Fitting\PathPoint.h"
#include "MotionPreviewScreen.h"
#include "MotionPreviewDialog.h"
#include "..\Curve Fitting\MotionMarker.h"
#include <math.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// MotionPreviewScreen

MotionPreviewScreen::MotionPreviewScreen()
{
	m_step = 0.0f;
	m_offsetX = 0.0f;
	m_offsetY = 0.0f;
	m_tranX = 0.0f;
	m_tranY = 0.0f;
	m_angle = 0.0f;
	m_till = 0.0f;
	m_radius = 5000.0f;
	m_AccuTime = 0.0f;
	m_MarkerSize = 1.8f;
	m_Frame = 0;
	m_pMotionBuffer = NULL;
	m_mouseX = 0;
	m_mouseY = 0;
	m_AspectRatio = 0.0f;
	m_time = 0.0f;

	m_IsPlay = false;
	m_IsPaused = false;
	m_IsStop = true;
	m_IsAltDown = false;
	m_IsByCurve = false;
}

MotionPreviewScreen::~MotionPreviewScreen()
{
}


BEGIN_MESSAGE_MAP(MotionPreviewScreen, CWnd)
	//{{AFX_MSG_MAP(MotionPreviewScreen)
	ON_WM_PAINT()
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_MOUSEMOVE()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// MotionPreviewScreen message handlers

void MotionPreviewScreen::OnPaint() 
{
	CPaintDC dc(this);

	QueryPerformanceCounter(&m_currentTime);
	m_TimeElapsed = (double)(m_currentTime.QuadPart - m_globeTime.QuadPart) / (double)m_timerFrequency.QuadPart;
	if(m_prevTime == 0.0f)
		m_FrameTime = 0.0f;
	else
		m_FrameTime = (double)(m_TimeElapsed - m_prevTime);
	m_prevTime = m_TimeElapsed;

	//m_FrameTime = 0.02f;

	PIXELFORMATDESCRIPTOR pfd = {
		sizeof(PIXELFORMATDESCRIPTOR),	// Size of this structure
		1,								// Version of this structure	
		PFD_DRAW_TO_WINDOW |			// Draw to Window (not to bitmap)
		PFD_SUPPORT_OPENGL |			// Support OpenGL calls in window
		PFD_DOUBLEBUFFER,				// Double buffered mode
		PFD_TYPE_RGBA,					// RGBA Color mode
		16,							// Want 32 bit color 
		0,0,0,0,0,0,					// Not used to select mode
		0,0,							// Not used to select mode
		0,0,0,0,0,						// Not used to select mode
		16,								// Size of depth buffer
		1,								// Not used to select mode
		0,								// Not used to select mode
		0,	            				// Not used to select mode
		0,								// Not used to select mode
		0,0,0 };

	//m_nPixelFormat = ChoosePixelFormat(m_hDC, &pfd);
	m_nPixelFormat = ChoosePixelFormat((HDC)dc, &pfd);

	// Set the Pixel Format
	SetPixelFormat((HDC)dc, m_nPixelFormat, NULL);

	// Create the rendering context and make it current
	m_hRC = wglCreateContext((HDC)dc);

	if(m_hRC != NULL)
		wglMakeCurrent((HDC)dc, m_hRC);

	RECT rect;
	GetClientRect(&rect);
	ReSizeGLScene(rect.right, rect.bottom);

	glShadeModel(GL_SMOOTH);							// Enable Smooth Shading
	glClearColor(0.9f, 0.9f, 0.9f, 1.0f);				// Black Background
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	glEnable(GL_LIGHTING);
	GLfloat lightPos[] = {0.0f, 0.0f, 0.0f, 1.0f};
	GLfloat ambientLight[] = {0.9f, 0.7f, 0.7f, 1.0f};
	GLfloat diffuseLight[] = {0.9f, 0.7f, 0.7f, 1.0f};
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
	glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
	glEnable(GL_LIGHT0);
	glEnable(GL_COLOR_MATERIAL);						// Enable Coloring Of Material
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if(m_pMotionBuffer != NULL)
	{
		unsigned int marker;
/*
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(45.0, m_AspectRatio, 1.0, (float)fabs((double)m_radius * 10.0));
*/
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		//glColor3f(1.0f, 0.0f, 0.0f);
		//glutSolidSphere(m_MarkerSize, 10, 10);

		//glColor3f(0.8f, 0.7f, 0.3f);
		glColor3f(0.1f, 0.1f, 0.1f);
		
		float x, y, z;
		PathPoint pp;

		if(m_pMotionBuffer->m_MotionDataType == TRC_MOTION_TYPE)
		{

			if(m_AccuTime >= 0.01f && m_IsPlay)
			{
				// Advance frame number or reset it to first one.
				if(m_Frame < m_pMotionBuffer->numFrames)
					m_Frame++;
				else
					m_Frame = 0;

				m_AccuTime = 0.0f;
			}

			for(marker = 0; marker < m_pMotionBuffer->numMarkers; marker++)
			{
				// TRC motion data access.
				if(!m_IsByCurve || !m_pMotionBuffer->m_IsAllMarkersFitted)
				{
					m_pMotionBuffer->GetPointX(marker, m_Frame, &pp);
					x = pp.GetAmpVal();
					m_pMotionBuffer->GetPointY(marker, m_Frame, &pp);
					y = pp.GetAmpVal();
					m_pMotionBuffer->GetPointZ(marker, m_Frame, &pp);
					z = pp.GetAmpVal();
				}
				else
				{
					x = m_pMotionBuffer->GetCurveVal(marker, 0, m_time);
					y = m_pMotionBuffer->GetCurveVal(marker, 1, m_time);
					z = m_pMotionBuffer->GetCurveVal(marker, 2, m_time);
				}

				glTranslatef(m_offsetX, m_offsetY, z);

				m_tranX = m_radius * (float)sin((double)m_angle);
				m_tranZ = m_radius * (float)cos((double)m_angle);
				m_tranY = m_radius * (float)sin((double)m_till);
				gluLookAt(m_tranX, m_tranY, m_tranZ, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);

				glTranslatef(x, y, z);
				glutSolidSphere(m_MarkerSize, 10, 10);

				glLoadIdentity();
			}// End loop through each marker.

		}// End if TRC_MOTION_TYPE.

		if(m_pMotionBuffer->m_MotionDataType == BVH_MOTION_TYPE)
		{
			PathPoint pp;
			float sx, cx, sy, cy, sz, cz;
			Matrix mat1(4, 1), mat2(4, 1), matRot(4, 4), matTran(4, 4), temp_mat(4, 4);
			Matrix Rx(4, 4), Ry(4, 4), Rz(4, 4);

			MotionMarker *pMK;
			pMK = m_pMotionBuffer->pMarkerHead;

			if(m_AccuTime >= 0.01f && m_IsPlay)
			{
				// Advance frame number or reset it to first one.
				if(m_Frame < m_pMotionBuffer->numFrames)
					m_Frame++;
				else
					m_Frame = 0;

				m_AccuTime = 0.0f;
			}

			// Loop through each marker.
			while(pMK != NULL)
			{
				float tx, ty, tz;

				if(pMK->parent == NULL)
				{
					// Root joint
					if(!m_IsByCurve || !m_pMotionBuffer->m_IsAllMarkersFitted)
					{
						pp = m_pMotionBuffer->GetPathPoint(pMK->nPosition, m_Frame, 4);
						sx = sinf(DegToRad(pp.GetAmpVal()));
						cx = cosf(DegToRad(pp.GetAmpVal()));
						pp = m_pMotionBuffer->GetPathPoint(pMK->nPosition, m_Frame, 5);
						sy = sinf(DegToRad(pp.GetAmpVal()));
						cy = cosf(DegToRad(pp.GetAmpVal()));
						pp = m_pMotionBuffer->GetPathPoint(pMK->nPosition, m_Frame, 3);
						sz = sinf(DegToRad(pp.GetAmpVal()));
						cz = cosf(DegToRad(pp.GetAmpVal()));

						pp = m_pMotionBuffer->GetPathPoint(pMK->nPosition, m_Frame, 0);
						tx = pp.GetAmpVal();
						pp = m_pMotionBuffer->GetPathPoint(pMK->nPosition, m_Frame, 1);
						ty = pp.GetAmpVal();
						pp = m_pMotionBuffer->GetPathPoint(pMK->nPosition, m_Frame, 2);
						tz = pp.GetAmpVal();
					}
					else
					{
						sx = sinf(DegToRad(m_pMotionBuffer->GetCurveVal(pMK->nPosition, 4, m_time)));
						cx = cosf(DegToRad(m_pMotionBuffer->GetCurveVal(pMK->nPosition, 4, m_time)));
						sy = sinf(DegToRad(m_pMotionBuffer->GetCurveVal(pMK->nPosition, 5, m_time)));
						cy = cosf(DegToRad(m_pMotionBuffer->GetCurveVal(pMK->nPosition, 5, m_time)));
						sz = sinf(DegToRad(m_pMotionBuffer->GetCurveVal(pMK->nPosition, 3, m_time)));
						cz = cosf(DegToRad(m_pMotionBuffer->GetCurveVal(pMK->nPosition, 3, m_time)));

						tx = m_pMotionBuffer->GetCurveVal(pMK->nPosition, 0, m_time);
						ty = m_pMotionBuffer->GetCurveVal(pMK->nPosition, 1, m_time);
						tz = m_pMotionBuffer->GetCurveVal(pMK->nPosition, 2, m_time);
					}

					Rx.SetToIdentity();
					Rx.SetVal(1, 1, cx);
					Rx.SetVal(1, 2, -sx);
					Rx.SetVal(2, 1, sx);
					Rx.SetVal(2, 2, cx);

					Ry.SetToIdentity();
					Ry.SetVal(0, 0, cy);
					Ry.SetVal(0, 2, sy);
					Ry.SetVal(2, 0, -sy);
					Ry.SetVal(2, 2, cy);
					
					Rz.SetToIdentity();
					Rz.SetVal(0, 0, cz);
					Rz.SetVal(0, 1, -sz);
					Rz.SetVal(1, 0, sz);
					Rz.SetVal(1, 1, cz);


					Rz.Product(&Rx, &temp_mat);
					temp_mat.Product(&Ry, &matRot);

					matTran.SetToIdentity();
					matTran.SetVal(0, 3, pMK->offset.x + tx);
					matTran.SetVal(1, 3, pMK->offset.y + ty);
					matTran.SetVal(2, 3, pMK->offset.z + tz);

					matTran.Product(&matRot, &pMK->TranMat);

					mat1.SetVal(0, 0, 0.0f);
					mat1.SetVal(1, 0, 0.0f);
					mat1.SetVal(2, 0, 0.0f);
					mat1.SetVal(3, 0, 1.0f);
					mat2.SetVal(0, 0, 0.0f);
					mat2.SetVal(1, 0, 0.0f);
					mat2.SetVal(2, 0, 0.0f);
					mat2.SetVal(3, 0, 1.0f);

					pMK->TranMat.Product(&mat1, &mat2);

					x = mat2.GetVal(0, 0);
					y = mat2.GetVal(1, 0);
					z = mat2.GetVal(2, 0);

					pMK->CurrentPosition = Vector3D(x, y, z);
				}
				else
				{
					// Child joint

					if(!m_IsByCurve || !m_pMotionBuffer->m_IsAllMarkersFitted)
					{
						pp = m_pMotionBuffer->GetPathPoint(pMK->nPosition, m_Frame, 1);
						sx = sinf(DegToRad(pp.GetAmpVal()));
						cx = cosf(DegToRad(pp.GetAmpVal()));
						pp = m_pMotionBuffer->GetPathPoint(pMK->nPosition, m_Frame, 2);
						sy = sinf(DegToRad(pp.GetAmpVal()));
						cy = cosf(DegToRad(pp.GetAmpVal()));
						pp = m_pMotionBuffer->GetPathPoint(pMK->nPosition, m_Frame, 0);
						sz = sinf(DegToRad(pp.GetAmpVal()));
						cz = cosf(DegToRad(pp.GetAmpVal()));
					}
					else
					{
						sx = sinf(DegToRad(m_pMotionBuffer->GetCurveVal(pMK->nPosition, 1, m_time)));
						cx = cosf(DegToRad(m_pMotionBuffer->GetCurveVal(pMK->nPosition, 1, m_time)));
						sy = sinf(DegToRad(m_pMotionBuffer->GetCurveVal(pMK->nPosition, 2, m_time)));
						cy = cosf(DegToRad(m_pMotionBuffer->GetCurveVal(pMK->nPosition, 2, m_time)));
						sz = sinf(DegToRad(m_pMotionBuffer->GetCurveVal(pMK->nPosition, 0, m_time)));
						cz = cosf(DegToRad(m_pMotionBuffer->GetCurveVal(pMK->nPosition, 0, m_time)));
					}

					Matrix currMat(4, 4);

					currMat.SetVal(3, 3, 1.0f);
					
					Rx.SetToIdentity();
					Rx.SetVal(1, 1, cx);
					Rx.SetVal(1, 2, -sx);
					Rx.SetVal(2, 1, sx);
					Rx.SetVal(2, 2, cx);

					Ry.SetToIdentity();
					Ry.SetVal(0, 0, cy);
					Ry.SetVal(0, 2, sy);
					Ry.SetVal(2, 0, -sy);
					Ry.SetVal(2, 2, cy);
					
					Rz.SetToIdentity();
					Rz.SetVal(0, 0, cz);
					Rz.SetVal(0, 1, -sz);
					Rz.SetVal(1, 0, sz);
					Rz.SetVal(1, 1, cz);


					Rz.Product(&Rx, &temp_mat);
					temp_mat.Product(&Ry, &matRot);


					matTran.SetToIdentity();
					matTran.SetVal(0, 3, pMK->offset.x);
					matTran.SetVal(1, 3, pMK->offset.y);
					matTran.SetVal(2, 3, pMK->offset.z);

					matTran.Product(&matRot, &currMat);


					pMK->parent->TranMat.Product(&currMat, &pMK->TranMat);

					mat1.SetVal(0, 0, 0.0f);
					mat1.SetVal(1, 0, 0.0f);
					mat1.SetVal(2, 0, 0.0f);
					mat1.SetVal(3, 0, 1.0f);
					mat2.SetVal(0, 0, 0.0f);
					mat2.SetVal(1, 0, 0.0f);
					mat2.SetVal(2, 0, 0.0f);
					mat2.SetVal(3, 0, 1.0f);

					pMK->TranMat.Product(&mat1, &mat2);

					x = mat2.GetVal(0, 0);
					y = mat2.GetVal(1, 0);
					z = mat2.GetVal(2, 0);

					pMK->CurrentPosition = Vector3D(x, y, z);


				}

				glTranslatef(m_offsetX, m_offsetY, 0.0f);
				m_tranX = m_radius * (float)sin((double)m_angle);
				m_tranZ = m_radius * (float)cos((double)m_angle);
				m_tranY = m_radius * (float)sin((double)m_till);
				gluLookAt(m_tranX, m_tranY, m_tranZ, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);


				if(pMK->parent != NULL)
				{
					glBegin(GL_LINES);
						glVertex3f(x, y, z);
						glVertex3f(pMK->parent->CurrentPosition.x, pMK->parent->CurrentPosition.y, pMK->parent->CurrentPosition.z);
					glEnd();
				}


				glTranslatef(x, y, z);
				glutSolidSphere(m_MarkerSize, 10, 10);
				
				pMK = pMK->next;

				glLoadIdentity();
			}// End loop through each marker.
		}// End if BVH_MOTION_TYPE.

	}// End if motion data buffer is not null.

	m_AccuTime += (float)m_FrameTime;

	SwapBuffers((HDC)dc);

	if(m_hRC)
	{	
		wglMakeCurrent(NULL, NULL);
		wglDeleteContext(m_hRC);
		m_hRC = NULL;
	}

	// Do not call CWnd::OnPaint() for painting messages
}

int MotionPreviewScreen::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here

	// Get current time and timer's frequency.
	QueryPerformanceCounter(&m_globeTime);
	QueryPerformanceFrequency(&m_timerFrequency);
	m_prevTime = 0.0f;

	//SetTimer(001, 1, NULL);
	//pFrameBuffer = unsigned char[65535 * 3 + 1];

	return 0;
}

BOOL MotionPreviewScreen::DestroyWindow() 
{
	// TODO: Add your specialized code here and/or call the base class
	//KillTimer(001);
	//delete[] pFrameBuffer;

	return CWnd::DestroyWindow();
}

void MotionPreviewScreen::ReSizeGLScene(GLsizei width, GLsizei height)
{
	if (height==0)										// Prevent A Divide By Zero By
	{
		height=1;										// Making Height Equal One
	}

	glViewport(0,0,width,height);						// Reset The Current Viewport

	glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix
	glLoadIdentity();									// Reset The Projection Matrix

    m_AspectRatio = (float)width/(float)height;
    //gluPerspective(45.0, m_AspectRatio, 1.0, (float)fabs((double)m_radius * 4.0));
	gluPerspective(45.0, m_AspectRatio, 1.0, 10000.0f);

	glMatrixMode(GL_MODELVIEW);							// Select The Modelview Matrix
	glLoadIdentity();
}

void MotionPreviewScreen::Draw()
{
}

void MotionPreviewScreen::OnSize(UINT nType, int w, int h) 
{
	CWnd::OnSize(nType, w, h);
	
	// TODO: Add your message handler code here
	// Prevent a divide by zero
	if(h == 0)
		h = 1;

	// Set Viewport to window dimensions
    glViewport(0, 0, w, h);

	// Reset coordinate system
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

    m_AspectRatio = (float)w/(float)h;
    //gluPerspective(45.0, m_AspectRatio, 1.0, (float)fabs((double)m_radius * 4.0));
	gluPerspective(45.0, m_AspectRatio, 1.0, 100000.0f);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();	
}

void MotionPreviewScreen::SetPointer(MotionData *ptr)
{
	m_pMotionBuffer = ptr;

	if(m_pMotionBuffer->m_MotionDataType == TRC_MOTION_TYPE)
	{
		m_step = 20.0f;
		m_radius = 5000.0f;
	}

	if(m_pMotionBuffer->m_MotionDataType == BVH_MOTION_TYPE)
	{
		m_step = 4.0f;
		m_radius = 250.0f;
	}
}

void MotionPreviewScreen::SetPointer(CDialog *ptr)
{
	m_pDialog = ptr;
}

void MotionPreviewScreen::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	UINT result = MK_LBUTTON | MK_MBUTTON;

	if(nFlags == MK_LBUTTON)
	{
		HCURSOR hCursor = AfxGetApp()->LoadStandardCursor(IDC_SIZEWE);
		::SetCursor(hCursor);

		m_angle += -(float)(point.x - m_mouseX) * 0.04f;
	}

	if(nFlags == result)
	{
		HCURSOR hCursor = AfxGetApp()->LoadStandardCursor(IDC_SIZENS);
		::SetCursor(hCursor);

		m_radius += (float)(point.y - m_mouseY) * m_step;
	}

	if(nFlags == MK_MBUTTON)
	{
		HCURSOR hCursor = AfxGetApp()->LoadStandardCursor(IDC_SIZEALL);
		::SetCursor(hCursor);

		m_offsetX += (float)(point.x - m_mouseX) * m_step;
		m_offsetY += -(float)(point.y - m_mouseY) * m_step;
	}

	if(nFlags == MK_RBUTTON)
	{
		HCURSOR hCursor = AfxGetApp()->LoadStandardCursor(IDC_SIZENS);
		::SetCursor(hCursor);

		m_till += (float)(point.y - m_mouseY) * 0.04f;
	}

	m_mouseX = point.x;
	m_mouseY = point.y;

	Invalidate(FALSE);

	CWnd::OnMouseMove(nFlags, point);
}

float MotionPreviewScreen::DegToRad(float deg)
{
	return deg * 0.0174533f;
}

void MotionPreviewScreen::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default

	CWnd::OnTimer(nIDEvent);
}

void MotionPreviewScreen::GetFrameBuffer()
{
	/*
	GLint result;

	glReadBuffer(GL_FRONT);
	glReadPixels(0, 0, 256, 256, GL_RGB, GL_UNSIGNED_BYTE, m_FrameBuffer);

	glGetIntegerv(GL_INDEX_MODE, &result);
	if(result == GL_INVALID_ENUM  || result == GL_INVALID_VALUE || result == GL_INVALID_OPERATION)
		result = 0;
		*/
}

void MotionPreviewScreen::ChangePointer(MotionData *ptr)
{
	m_pMotionBuffer = ptr;
}
