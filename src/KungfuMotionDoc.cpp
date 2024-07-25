// KungfuMotionDoc.cpp : implementation of the CKungfuMotionDoc class
//

#include "stdafx.h"
#include "KungfuMotion.h"
#include "KungfuMotionDoc.h"
#include "MainFrm.h"
#include "Motion Preview/MotionPreviewDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CKungfuMotionDoc

IMPLEMENT_DYNCREATE(CKungfuMotionDoc, CDocument)

BEGIN_MESSAGE_MAP(CKungfuMotionDoc, CDocument)
	//{{AFX_MSG_MAP(CKungfuMotionDoc)
	ON_COMMAND(ID_FILE_IMPORT, OnFileImport)
	ON_COMMAND(ID_FILE_SAVE_PROJECT, OnFileSaveProject)
	ON_COMMAND(ID_FILE_SAVE_PROJECT_AS, OnFileSaveProjectAs)
	ON_COMMAND(ID_FILE_EXPORT, OnFileExport)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CKungfuMotionDoc construction/destruction

CKungfuMotionDoc::CKungfuMotionDoc()
{
	// TODO: add one-time construction code here
	m_Message = "Message from document!";
	m_IsFileLoaded = false;
	m_ShowCurve = false;
	m_IsMarkerSelected = false;
	m_currentMotionData = 0;
	m_currentMarker = 0;
	m_currentDOF = 0;
	m_numMotionData = 0;
	m_FileType = UNKNOWN_TYPE;
	m_mti = NULL;
	m_pMotionBufferHead = NULL;
	m_pCurrentMB = NULL;
	m_pMarkerView = NULL;
	m_pCurveView = NULL;
}

CKungfuMotionDoc::~CKungfuMotionDoc()
{
	if(m_mti != NULL)
	{
		for(unsigned int i = 0; i < m_pCurrentMB->MotionBuffer.numMarkers; i++)
		{
			if(m_mti[i].cti != NULL)
				delete[] m_mti[i].cti;
		}

		delete[] m_mti;
		m_mti = NULL;
	}

	if(m_pMotionBufferHead != NULL)
	{
		KungfuMotionData *temp;
		KungfuMotionData *pMB = m_pMotionBufferHead;
		while(pMB != NULL)
		{
			temp = pMB->pNext;
			delete pMB;
			pMB = temp;
		}
	}
}

BOOL CKungfuMotionDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CKungfuMotionDoc serialization

void CKungfuMotionDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

/////////////////////////////////////////////////////////////////////////////
// CKungfuMotionDoc diagnostics

#ifdef _DEBUG
void CKungfuMotionDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CKungfuMotionDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CKungfuMotionDoc commands

void CKungfuMotionDoc::OnFileImport() 
{
	// TODO: Add your command handler code here
	static char BASED_CODE szFilter[] = "TRC file (*.trc)|*.trc| BVH file (*.bvh)|*.bvh| All files (*.*)|*.*||";
	CFileDialog fileDialog(TRUE, NULL, NULL, OFN_HIDEREADONLY, szFilter, AfxGetMainWnd());
	
	if(fileDialog.DoModal() == IDOK)
	{
		KungfuMotionData *pMB;
		bool IsImportOk = false;

		if(fileDialog.GetFileExt().CompareNoCase("trc") == 0)
		{
			if(m_pMotionBufferHead == NULL)
			{
				m_pMotionBufferHead = new KungfuMotionData;
				pMB = m_pMotionBufferHead;
				pMB->nKungfuMotionData = 0;
				m_currentMotionData = 0;
				m_numMotionData++;
				m_pCurrentMB = m_pMotionBufferHead;
			}
			else
			{
				unsigned int i = 0;
				pMB = new KungfuMotionData;

				// Link new node to ending of current list.
				InsertMotionDataNode(pMB);
				m_currentMotionData = m_numMotionData - 1;
				m_pCurrentMB = pMB;
			}

			pMB->SetMotionName(fileDialog.GetFileName());

			//////////////////////////////////////////////////////////////////////
			// Load TRC file to motion data buffer.
			if(pMB->Initialize((LPCTSTR)fileDialog.GetPathName(), TRC_MOTION_TYPE))
			{
				// Set file type flag.
				m_FileType = TRC_TYPE;
				// Set file loaded flag.
				m_IsFileLoaded = true;

				IsImportOk = true;
				m_pCurrentMB = pMB;
			}
			else
			{
				DeleteMotionDataNode(pMB);
				m_pCurrentMB = m_pMotionBufferHead;
				m_currentMotionData = 0;
				MessageBox(AfxGetMainWnd()->m_hWnd, "File import error!", "Error", MB_OK);
			}

			//////////////////////////////////////////////////////////////////////
		}

		if(fileDialog.GetFileExt().CompareNoCase("bvh") == 0)
		{
			if(m_pMotionBufferHead == NULL)
			{
				m_pMotionBufferHead = new KungfuMotionData;
				pMB = m_pMotionBufferHead;
				pMB->nKungfuMotionData = 0;
				m_currentMotionData = 0;
				m_numMotionData++;
				m_pCurrentMB = m_pMotionBufferHead;
			}
			else
			{
				unsigned int i = 0;
				pMB = new KungfuMotionData;

				// Link new node to ending of current list.
				InsertMotionDataNode(pMB);
				m_currentMotionData = m_numMotionData - 1;
				m_pCurrentMB = pMB;
				
			}

			pMB->SetMotionName(fileDialog.GetFileName());

			// Load BVH file to motion data buffer.
			if(pMB->Initialize((LPCTSTR)fileDialog.GetPathName(), BVH_MOTION_TYPE))
			{
				// Set file type flag.
				m_FileType = BVH_TYPE;
				// Set file loaded flag.
				m_IsFileLoaded = true;

				IsImportOk = true;
				m_pCurrentMB = pMB;
			}
			else
			{
				DeleteMotionDataNode(pMB);
				m_pCurrentMB = m_pMotionBufferHead;
				m_currentMotionData = 0;
				MessageBox(AfxGetMainWnd()->m_hWnd, "File import error!", "Error", MB_OK);
			}
		}

		if(IsImportOk)
		{
			CString str;
			CMainFrame *pMainFrame = (CMainFrame *)AfxGetMainWnd();
			pMainFrame->m_wndStatusBar.SetPaneText(0, "Successfully imported", TRUE);
		}
		else
			MessageBox(AfxGetMainWnd()->m_hWnd, "Format error!", "Error", MB_OK);
		/*
		// Ask for whether convert path points to control points.
		int result;
		result = MessageBox(NULL, "Do you want to calculate control points for all motion curves?", "Question", MB_YESNO | MB_ICONEXCLAMATION);
		if(result == IDYES)
		{
			CMainFrame *pMainFrame = (CMainFrame *)AfxGetMainWnd();
			CString str;
			unsigned int i, j;
				
			for(i = 0; i < m_pCurrentMB->MotionBuffer.numMarkers; i++)
			{
				j = (unsigned int)((100.0f / (float)(m_pCurrentMB->MotionBuffer.numMarkers)) * (float)i);
				str.Format("Fitting...%d", j);
				pMainFrame->m_wndStatusBar.SetPaneText(0, str, TRUE);
				m_pCurrentMB->MC_UCBS->FitMarkerToUCBS(i);
			}
			m_pCurrentMB->MotionBuffer.m_IsAllMarkersFitted = true;
			pMainFrame->m_wndStatusBar.SetPaneText(0, "Fitting Complete", TRUE);

			// Release coef matrix and inverse matrix of motion buffer.
			m_pCurrentMB->MC_UCBS->ReleaseMatrices();
		}
*/

		SetModifiedFlag(TRUE);
		UpdateAllViews(NULL, 0L, NULL);
	};

	m_pCurveView->Invalidate(FALSE);
}

void CKungfuMotionDoc::ChangeTreeSelection(HTREEITEM hItem)
{
	// Find for selected curve.

	bool IsMatched = false;
	unsigned int i, j, k, numMarkers, numDOFs;

	KungfuMotionData *pMB;

	RECT rect;

	k = 0;
	//for(k = 0; k < m_numMotionData; k++)
	for(pMB = m_pMotionBufferHead; pMB != NULL; pMB = pMB->pNext)
	{
		//pMB = GetMotionDataBuffer(k);
		numMarkers = pMB->MotionBuffer.numMarkers;

		if(pMB->tItem == hItem)
		{
			m_ShowCurve = false;
			m_pCurveView->m_IsShowCurve = false;
			m_pCurveView->GetClientRect(&rect);
			m_pCurveView->InvalidateRect(&rect, FALSE);
			IsMatched = true;
			m_IsMotionSelected = true;
			m_IsMarkerSelected = false;
			m_currentMarker = 0;
			m_currentDOF = 0;
			m_currentMotionData = k;
			m_pCurrentMB = pMB;

			break;
		}

		// Check for whether marker is selected.
		for(i = 0; i < numMarkers; i++)
		{
			if(pMB->mti[i].tItem == hItem)
			{
				m_ShowCurve = true;
				m_pCurveView->m_IsShowCurve = true;
				m_pCurveView->GetClientRect(&rect);
				m_pCurveView->InvalidateRect(&rect, FALSE);
				IsMatched = true;
				m_IsMarkerSelected = true;
				m_IsMotionSelected = false;
				m_currentMarker = i;
				m_currentMotionData = k;

				break;
			}

			numDOFs = pMB->MotionBuffer.GetMarker(i)->numDOFs;
			for(j = 0; j < numDOFs; j++)
			{
				if(pMB->mti[i].cti[j].tItem == hItem)
				{
					m_ShowCurve = true;
					m_pCurveView->m_IsShowCurve = true;
					m_pCurveView->GetClientRect(&rect);
					m_pCurveView->InvalidateRect(&rect, FALSE);

					IsMatched = true;
					m_IsMarkerSelected = false;
					m_IsMotionSelected = false;

					m_currentMarker = i;
					m_currentDOF = j;
					m_currentMotionData = k;

					break;
				}
			}

			if(IsMatched)
				break;
		}
		k++;
	}

	//m_pCurrentMB = GetMotionDataBuffer(m_currentMotionData);
	m_pCurrentMB = GetMotionDataBufferByOrder(m_currentMotionData);
	//m_pCurrentMB = m_pCurrentMB;

	// If neither marker nor DOF was selected, don't show curve.  Otherwise, update CurveView.
	if(!IsMatched)
		m_ShowCurve = false;
	else
		UpdateAllViews(m_pMarkerView, 0L, NULL);
}

// Get pointer of MarkerView.
void CKungfuMotionDoc::SetPointer(MarkerView *ptr)
{
	m_pMarkerView = ptr;
}

// Get pointer of CurveView.
void CKungfuMotionDoc::SetPointer(CurveView *ptr)
{
	m_pCurveView = ptr;
}

BOOL CKungfuMotionDoc::SaveModified() 
{
	// TODO: Add your specialized code here and/or call the base class
	m_pCurveView->m_IsActive = false;
	
	BOOL flag;
	flag = CDocument::SaveModified();

	if(!flag)
		m_pCurveView->Invalidate(FALSE);

	return flag;
}

void CKungfuMotionDoc::OnFileSaveProject() 
{
	// TODO: Add your command handler code here
	
}

void CKungfuMotionDoc::OnFileSaveProjectAs() 
{
	// TODO: Add your command handler code here
	
}

KungfuMotionData *CKungfuMotionDoc::GetMotionDataBuffer(unsigned int nMotionData)
{
	if(m_pMotionBufferHead != NULL)
	{
		KungfuMotionData *pMB = m_pMotionBufferHead;
		unsigned int i = 0;

		while(pMB != NULL)
		{
			if(pMB->nKungfuMotionData == nMotionData)
				return pMB;

			pMB = pMB->pNext;
		}
	}
	else
		return NULL;

	return NULL;
}

KungfuMotionData *CKungfuMotionDoc::GetMotionDataBufferByOrder(unsigned int Index)
{
	if(m_pMotionBufferHead != NULL)
	{
		KungfuMotionData *pMB = m_pMotionBufferHead;
		unsigned int i = 0;

		for(pMB = m_pMotionBufferHead; pMB != NULL; pMB = pMB->pNext)
		{
			if(i == Index)
				return pMB;
			i++;
		}
	}
	else
		return NULL;

	return NULL;
}

bool CKungfuMotionDoc::InsertMotionDataNode(KungfuMotionData *md)
{	
	if(m_pMotionBufferHead == NULL)
	{
		m_pMotionBufferHead = md;
		md->nKungfuMotionData = 0;
		m_numMotionData++;
		return true;
	}

	if(m_pMotionBufferHead != NULL && md != NULL)
	{
		KungfuMotionData *pMB = m_pMotionBufferHead;

		while(pMB->pNext != NULL)
			pMB = pMB->pNext;

		pMB->pNext = md;
		md->nKungfuMotionData = pMB->nKungfuMotionData + 1;
		m_numMotionData++;
	}
	else
		return false;

	return true;
}

bool CKungfuMotionDoc::DeleteMotionDataNode(KungfuMotionData *pKFMD)
{
	if(m_pMotionBufferHead != NULL && pKFMD != NULL)
	{
		KungfuMotionData *prev_pMB = NULL, *pMB = m_pMotionBufferHead;

		for(pMB = m_pMotionBufferHead; pMB != NULL; pMB = pMB->pNext)
		{
			if(pMB->nKungfuMotionData == pKFMD->nKungfuMotionData)
			{
				if(pMB == m_pMotionBufferHead && m_numMotionData == 1)
				{
					m_pMotionBufferHead = NULL;
					m_numMotionData = 0;
					m_IsFileLoaded = false;
				}
				else
				{
					if(prev_pMB == NULL)
						m_pMotionBufferHead = pMB->pNext;
					else
						prev_pMB->pNext = pMB->pNext;

					m_numMotionData--;
				}
				delete pMB;
				return true;
			}
				
			prev_pMB = pMB;
		}
	}
	else
		return false;

	return true;
}

void CKungfuMotionDoc::OnFileExport() 
{
	// TODO: Add your command handler code here
	if(m_pCurrentMB != NULL && m_pCurrentMB->MotionDataType == BVH_MOTION_TYPE)
	{
		static char BASED_CODE szFilter[] = "BVH file (*.bvh)|*.bvh||";
		CFileDialog fileDialog(FALSE, NULL, NULL, OFN_HIDEREADONLY, szFilter, AfxGetMainWnd());

		if(fileDialog.DoModal() == IDOK)
		{
			if(m_pCurrentMB->MotionBuffer.SaveBVHFile(fileDialog.GetPathName()))
				MessageBox(NULL, "File successfully imported!", "Information", MB_OK);
		};
	}
}

bool CKungfuMotionDoc::DeleteCurrentMotion()
{
	if(!DeleteMotionDataNode(m_pCurrentMB))
		return false;
	m_pCurrentMB = m_pMotionBufferHead;
	m_currentMotionData = 0;
	//m_numMotionData--;
	UpdateAllViews(NULL, 0L, NULL);

	return true;
}
