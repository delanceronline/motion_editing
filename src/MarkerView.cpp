// MarkerView.cpp : implementation file
//

#include "stdafx.h"
#include "KungfuMotion.h"
#include "MarkerView.h"
#include "CurveView.h"
#include "MainFrm.h"
#include "Motion Editing\MotionEditing.h"
#include "MotionBlendingDialog.h"
#include "DoubleScreenDialog.h"
#include "MotionExtractionDialog.h"
#include "MotionWarping.h"
#include "SnapShotDialog.h"
#include "FilterDialog.h"
#include "Curve Fitting\MotionCurves_UCBS.h"

#include "KungfuMotionDoc.h"
#include "Motion Preview\MotionPreviewDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// MarkerView

IMPLEMENT_DYNCREATE(MarkerView, CView)

MarkerView::MarkerView()
{
	m_ViewType = 1;
	m_IsDraggingMotion = false;
	m_DragImageList = NULL;
	m_hItemDrop = NULL;
	m_hItemDrag = NULL;
}

MarkerView::~MarkerView()
{
	if(m_DragImageList != NULL)
		delete m_DragImageList;
}


BEGIN_MESSAGE_MAP(MarkerView, CView)
	//{{AFX_MSG_MAP(MarkerView)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_COMMAND(ID_MOTION_PREVIEW, OnMotionPreview)
	ON_COMMAND(ID_MOTION_BLENDING, OnMotionBlending)
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_COMMAND(ID_CURVE_FIT, OnCurveFit)
	ON_COMMAND(ID_VIEW_CP, OnViewCp)
	ON_COMMAND(ID_MOTION_EXAGGERATING, OnMotionExaggerating)
	ON_COMMAND(ID_MOTION_REPLACING, OnMotionReplacing)
	ON_COMMAND(ID_MOTION_EXTRACTING, OnMotionExtracting)
	ON_COMMAND(ID_MOTION_WARPING, OnMotionWarping)
	ON_COMMAND(ID_FILE_SAVESNAPSHOT, OnFileSavesnapshot)
	ON_COMMAND(ID_MOTION_TRANSITION, OnMotionTransition)
	ON_COMMAND(ID_MOTION_FILTER, OnMotionFilter)
	//}}AFX_MSG_MAP
	ON_NOTIFY(TVN_SELCHANGED, IDC_MARKERTREE, OnTreeViewClick)
	ON_NOTIFY(TVN_KEYDOWN, IDC_MARKERTREE, OnTreeViewKeyDown)
	ON_NOTIFY(NM_DBLCLK, IDC_MARKERTREE, OnTreeViewDoubleClick)
	ON_NOTIFY(TVN_BEGINDRAG, IDC_MARKERTREE, OnTreeItemDrag)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// MarkerView drawing

void MarkerView::OnDraw(CDC* pDC)
{
	CKungfuMotionDoc *pDoc = (CKungfuMotionDoc *)GetDocument();

	// TODO: add draw code here
}

/////////////////////////////////////////////////////////////////////////////
// MarkerView diagnostics

#ifdef _DEBUG
void MarkerView::AssertValid() const
{
	CView::AssertValid();
}

void MarkerView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// MarkerView message handlers

int MarkerView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	CKungfuMotionDoc *pDoc = (CKungfuMotionDoc *)GetDocument();
	pDoc->SetPointer(this);

	RECT rect;
	GetClientRect(&rect);
	rect.bottom = 645;

	HICON hIcon;
	m_TreeImageList.Create(16, 16, ILC_COLOR16, 4, 0);
	hIcon = ::LoadIcon(AfxGetResourceHandle(), MAKEINTRESOURCE(IDI_MOTION));
	m_TreeImageList.Add(hIcon);
	hIcon = ::LoadIcon(AfxGetResourceHandle(), MAKEINTRESOURCE(IDI_JOINT_1));
	m_TreeImageList.Add(hIcon);
	hIcon = ::LoadIcon(AfxGetResourceHandle(), MAKEINTRESOURCE(IDI_CURVE));
	m_TreeImageList.Add(hIcon);
	hIcon = ::LoadIcon(AfxGetResourceHandle(), MAKEINTRESOURCE(IDI_ENDSITE));
	m_TreeImageList.Add(hIcon);

	// TODO: Add your specialized creation code here
	m_MarkerTree.Create(WS_VISIBLE | WS_CHILD | TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS | TVS_SHOWSELALWAYS, rect, this, IDC_MARKERTREE);
	m_MarkerTree.SetImageList(&m_TreeImageList, TVSIL_NORMAL);

	return 0;
}

void MarkerView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint) 
{
	// TODO: Add your specialized code here and/or call the base class
	CKungfuMotionDoc *pDoc = (CKungfuMotionDoc *)GetDocument();

	m_MarkerTree.DeleteAllItems();
	
	if(pDoc->m_IsFileLoaded && pDoc->m_FileType != UNKNOWN_TYPE)
	{
		unsigned int i, j, numDOFs;

		// Add all markers into tree.
		HTREEITEM hItem;

		KungfuMotionData *pMB = pDoc->m_pMotionBufferHead;

		while(pMB != NULL)
		{
		
			unsigned int numMarkers = pMB->MotionBuffer.numMarkers;

			if(pMB->MotionDataType == TRC_MOTION_TYPE)
			{
				m_hRoot = m_MarkerTree.InsertItem(pMB->name, 0, 0, TVI_ROOT, TVI_LAST);
				pMB->tItem = m_hRoot;

				for(i = 0; i < numMarkers; i++)
				{
					// Insert markers and curves to tree and store them into cti in Doc.

					hItem = m_MarkerTree.InsertItem(pMB->MotionBuffer.GetMarkerName(i), 1, 1, m_hRoot, TVI_LAST);
					pMB->mti[i].tItem = hItem;
					pMB->mti[i].markerIndex = i;

					numDOFs = (pMB->MotionBuffer.GetMarker(i))->numDOFs;
					for(j = 0; j < numDOFs; j++)
					{
						pMB->mti[i].cti[j].tItem = m_MarkerTree.InsertItem(pMB->MotionBuffer.GetDOFName(i, j), 2, 2, hItem, TVI_LAST);
						pMB->mti[i].cti[j].curveIndex = j;
					}
				}
			}

			if(pMB->MotionDataType == BVH_MOTION_TYPE)
			{
				m_hRoot = m_MarkerTree.InsertItem(pMB->name, 0, 0, TVI_ROOT, TVI_LAST);
				pMB->tItem = m_hRoot;

				// Run this recursive function to add child item to parent item.
				InsertItemToParent(pMB, NULL, 0);
			}

			pMB = pMB->pNext;
		}
	}
}

void MarkerView::OnTreeViewClick(NMHDR* pNMHDR, LRESULT* pResult)
{
	HTREEITEM hItem = m_MarkerTree.GetSelectedItem();

	if (hItem != NULL)
	{
		CKungfuMotionDoc *pDoc = (CKungfuMotionDoc *)GetDocument();

		pDoc->ChangeTreeSelection(hItem);
	}
}

void MarkerView::OnTreeViewKeyDown(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMTVKEYDOWN ptvkd = (LPNMTVKEYDOWN)pNMHDR;
	CKungfuMotionDoc *pDoc = (CKungfuMotionDoc *)GetDocument();

	if(ptvkd->wVKey == VK_F3)
		OnMotionPreview();

	if(ptvkd->wVKey == VK_DELETE)
	{
		pDoc->DeleteCurrentMotion();
	}
}

void MarkerView::OnTreeViewDoubleClick(NMHDR* pNMHDR, LRESULT* pResult)
{
	HTREEITEM hItem = m_MarkerTree.GetSelectedItem();

	if (hItem != NULL)
	{
		CKungfuMotionDoc *pDoc = (CKungfuMotionDoc *)GetDocument();
		pDoc->ChangeTreeSelection(hItem);

		if(pDoc->m_IsMotionSelected)
		{
			MotionPreviewDialog mpDialog;
			mpDialog.SetPointer(&pDoc->m_pCurrentMB->MotionBuffer);
			mpDialog.DoModal();
		}
	}
}

void MarkerView::OnTreeItemDrag(NMHDR* pNMHDR, LRESULT* pResult)
{
	int nImage, nSelectedImage;

	//HTREEITEM hItem = m_MarkerTree.GetSelectedItem();
	UINT nFlags;
	HTREEITEM hItem;
	CPoint pt, pt_Image;

	GetCursorPos(&pt);

	//ScreenToClient(&pt);
	//pt_Image = pt;
	pt_Image = pt;
	ScreenToClient(&pt_Image);

	m_MarkerTree.ScreenToClient(&pt);
	
	hItem = m_MarkerTree.HitTest(pt, &nFlags);

	if(hItem != NULL)
	{
		m_hItemDrag = hItem;
		m_MarkerTree.GetItemImage(hItem, nImage, nSelectedImage);

		if(nImage == 0)
		{
			if(m_DragImageList != NULL)
				delete m_DragImageList;

			m_DragImageList = m_MarkerTree.CreateDragImage(hItem);
			m_DragImageList->DragShowNolock(TRUE);
			m_DragImageList->BeginDrag(0, CPoint(0, 0));
			m_DragImageList->DragMove(pt_Image);
			m_DragImageList->DragEnter(this, pt_Image);
			SetCapture();

			m_IsDraggingMotion = true;
		}
	}
}

BOOL MarkerView::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult) 
{
	// TODO: Add your specialized code here and/or call the base class
	//

	return CView::OnNotify(wParam, lParam, pResult);
}

void MarkerView::OnSize(UINT nType, int cx, int cy) 
{
	CView::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
}

void MarkerView::OnMotionPreview() 
{
	// TODO: Add your command handler code here
	CKungfuMotionDoc *pDoc = (CKungfuMotionDoc *)GetDocument();

	if(pDoc->m_IsFileLoaded)
	{
		MotionPreviewDialog mpDialog;
		
		mpDialog.SetPointer(&pDoc->m_pCurrentMB->MotionBuffer);
		mpDialog.DoModal();
	}
}

// Recursive function to add child joint under parent joint in tree structure.
bool MarkerView::InsertItemToParent(KungfuMotionData *pMB, HTREEITEM hParentItem, unsigned int nMarker_Parent)
{
	CKungfuMotionDoc *pDoc = (CKungfuMotionDoc *)GetDocument();

	unsigned int i;
	HTREEITEM hItem1, hItem2;
	MotionMarker *pParentMK = pMB->MC_UCBS->GetMarker(nMarker_Parent);

	if(hParentItem == NULL)
	{
		// Add root joint to tree root.
		if(pParentMK->IsEndSite)
			hItem1 = m_MarkerTree.InsertItem(pParentMK->name, 3, 3, m_hRoot, TVI_LAST);
		else
			hItem1 = m_MarkerTree.InsertItem(pParentMK->name, 1, 1, m_hRoot, TVI_LAST);

		pMB->mti[pParentMK->nPosition].tItem = hItem1;
		for(i = 0; i < pParentMK->numDOFs; i++)
		{
			hItem2 = m_MarkerTree.InsertItem(pMB->MotionBuffer.GetDOFName(pParentMK->nPosition, i), 2, 2, hItem1, TVI_LAST);
			pMB->mti[pParentMK->nPosition].cti[i].tItem = hItem2;
		}

		InsertItemToParent(pMB, hItem1, pParentMK->nPosition);
	}
	else
	{
		MotionMarker *pMK = pMB->MotionBuffer.pMarkerHead;
		
		while(pMK != NULL)
		{
			if(pMK->parent == pParentMK)
			{
				if(pMK->IsEndSite)
					hItem1 = m_MarkerTree.InsertItem(pMK->name, 3, 3, hParentItem, TVI_LAST);
				else
					hItem1 = m_MarkerTree.InsertItem(pMK->name, 1, 1, hParentItem, TVI_LAST);
				pMB->mti[pMK->nPosition].tItem = hItem1;
				for(i = 0; i < pMK->numDOFs; i++)
				{
					hItem2 = m_MarkerTree.InsertItem(pMB->MotionBuffer.GetDOFName(pMK->nPosition, i), 2, 2, hItem1, TVI_LAST);
					pMB->mti[pMK->nPosition].cti[i].tItem = hItem2;
				}

				InsertItemToParent(pMB, hItem1, pMK->nPosition);
			}

			pMK = pMK->next;
		}
	}

	return true;
}

void MarkerView::SetCurveView(CView *m_pCurveView_in)
{
	m_pCurveView = m_pCurveView_in;
}

void MarkerView::OnMotionBlending() 
{
	// TODO: Add your command handler code here
	CKungfuMotionDoc *pDoc = (CKungfuMotionDoc *)GetDocument();
	MotionEditingInfo MBInfo;

	DoubleScreenDialog DSDlg;
	DSDlg.SetKungfuMotionDataHead(pDoc->m_pMotionBufferHead, pDoc->m_numMotionData);
	DSDlg.SetMBInfoPointer(&MBInfo);
	DSDlg.SetDlgType(BLENDING_DLG);

	if(DSDlg.DoModal() == IDOK)
	{
		KungfuMotionData *pKFMD = new KungfuMotionData;
		MotionEditing ME;

		if(ME.LinearBlendFramesByCurve(MBInfo.pMotionData1, MBInfo.pMotionData2, &MBInfo, &pKFMD->MotionBuffer))
		{
			// Initialize KungfuMotionData structure without loading file.
			//pKFMD->SetMotionName("Newly Blended Motion");
			pKFMD->SetMotionName(MBInfo.MotionName);
			pKFMD->Initialize(pDoc->m_pMotionBufferHead->MotionBuffer.m_MotionDataType);

			pDoc->InsertMotionDataNode(pKFMD);
			pDoc->UpdateAllViews(NULL, 0L, NULL);
		}
		else
		{
			delete pKFMD;
			MessageBox("Error in blending!", "Error!", MB_OK);
		}

		delete[] MBInfo.pSimVal;
	};

}

void MarkerView::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if(m_IsDraggingMotion)
	{
		UINT flags;
		HTREEITEM hitem;
		CPoint pt;
		pt = point;

		ClientToScreen(&pt);
		m_MarkerTree.ScreenToClient(&pt);

		m_DragImageList->DragMove(point);

		if ((hitem = m_MarkerTree.HitTest(pt, &flags)) != NULL)
		{                                                                                               
			m_DragImageList->DragLeave(this);
			m_MarkerTree.SelectDropTarget(hitem);
			m_hItemDrop = hitem;
			m_DragImageList->DragEnter(this, point);
		}
	}

	CView::OnMouseMove(nFlags, point);
}

void MarkerView::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if(m_IsDraggingMotion)
	{
		m_DragImageList->DragLeave(this);
		m_DragImageList->EndDrag();

		m_MarkerTree.SelectDropTarget(NULL);
		ReleaseCapture();

		delete m_DragImageList;
		m_DragImageList = NULL;

		if(m_hItemDrag != NULL && m_hItemDrop != NULL && m_hItemDrag != m_hItemDrop)
		{

			CKungfuMotionDoc *pDoc = (CKungfuMotionDoc *)GetDocument();
			KungfuMotionData *pKFMD1, *pKFMD2;
			MotionData *pMB1 = NULL, *pMB2 = NULL;

			pKFMD1 = GetKungfuMotionData(m_hItemDrop);
			pKFMD2 = GetKungfuMotionData(m_hItemDrag);

			pMB1 = &pKFMD1->MotionBuffer;
			pMB2 = &pKFMD2->MotionBuffer;

			if(pMB1 != NULL && pMB2 != NULL)
			{
				//unsigned int StartFrame, EndFrame;

				//MotionBlendingDialog MBDlg;
				//MBDlg.m_MotionName = "Blended Motion";

				//if(MBDlg.DoModal() == IDOK)
				//{
					/*
					StartFrame = 0;
					if(pMB1->numFrames <= pMB2->numFrames)
						EndFrame = pMB1->numFrames;
					else
						EndFrame = pMB2->numFrames;
						*/

					//if(!BlendTwoMotions(pMB1, pMB2, StartFrame, EndFrame, MBDlg.m_BlendingValue, (LPCTSTR)MBDlg.m_MotionName))
						//MessageBox("Motions can't be blended!\nMake sure their skeletons are the same!", "Error!", MB_OK);
				//}
				if(!BlendTwoMotions(pKFMD1, pKFMD2))
					MessageBox("Motions can't be blended!\nMake sure their skeletons are the same!", "Error!", MB_OK);
			}
		}

		m_IsDraggingMotion = false;
		m_hItemDrag  = NULL;
		m_hItemDrop = NULL;
	}	

	CView::OnLButtonUp(nFlags, point);
}

KungfuMotionData * MarkerView::GetKungfuMotionData(HTREEITEM hItem)
{
	KungfuMotionData *pKFMD;
	CKungfuMotionDoc *pDoc = (CKungfuMotionDoc *)GetDocument();
	
	for(pKFMD = pDoc->m_pMotionBufferHead; pKFMD != NULL; pKFMD = pKFMD->pNext)
	{
		if(pKFMD->tItem == hItem)
			return pKFMD;
	}

	return NULL;
}

bool MarkerView::BlendTwoMotions(KungfuMotionData *pKFMD1, KungfuMotionData *pKFMD2)
{
	CKungfuMotionDoc *pDoc = (CKungfuMotionDoc *)GetDocument();
	MotionEditingInfo MBInfo;
	MotionEditing ME;

	if(ME.IsSketelonIdentical(&pKFMD1->MotionBuffer, &pKFMD2->MotionBuffer))
	{
		DoubleScreenDialog DSDlg;
		DSDlg.SetKungfuMotionDataHead(pDoc->m_pMotionBufferHead, pDoc->m_numMotionData);
		DSDlg.SetPointer(pKFMD1, pKFMD2);
		DSDlg.SetMBInfoPointer(&MBInfo);
		DSDlg.SetDlgType(BLENDING_DLG);
		DSDlg.m_IsMotionSelectionMode = false;
		DSDlg.m_IsJointSelectionMode = true;

		if(DSDlg.DoModal() == IDOK)
		{
			KungfuMotionData *pKFMD = new KungfuMotionData;

			if(ME.LinearBlendFramesByCurve(MBInfo.pMotionData1, MBInfo.pMotionData2, &MBInfo, &pKFMD->MotionBuffer))
			{
				// Initialize KungfuMotionData structure without loading file.
				pKFMD->SetMotionName("Newly Blended Motion");
				pKFMD->Initialize(pKFMD1->MotionBuffer.m_MotionDataType);

				pDoc->InsertMotionDataNode(pKFMD);
				pDoc->UpdateAllViews(NULL, 0L, NULL);
			}
			else
			{
				delete pKFMD;
				MessageBox("Error in blending!", "Error!", MB_OK);
			}

			delete[] MBInfo.pSimVal;
		};

	}
	else
		return false;

	return true;
}

void MarkerView::OnCurveFit() 
{
	// TODO: Add your command handler code here
	CKungfuMotionDoc *pDoc = (CKungfuMotionDoc *)GetDocument();

	if(pDoc->m_IsMotionSelected && pDoc->m_IsFileLoaded)
	{
		if(!pDoc->GetMotionDataBufferByOrder(pDoc->m_currentMotionData)->MotionBuffer.m_IsAllMarkersFitted)
		{
			CMainFrame *pMainFrame = (CMainFrame *)AfxGetMainWnd();
			CString str;
			unsigned int i, j;
				
			for(i = 0; i < pDoc->m_pCurrentMB->MotionBuffer.numMarkers; i++)
			{
				j = (unsigned int)((100.0f / (float)(pDoc->m_pCurrentMB->MotionBuffer.numMarkers)) * (float)i);
				str.Format("Fitting...%d", j);
				pMainFrame->m_wndStatusBar.SetPaneText(0, str, TRUE);
				pDoc->m_pCurrentMB->MC_UCBS->FitMarkerToUCBS(i);
			}
			pDoc->m_pCurrentMB->MotionBuffer.m_IsAllMarkersFitted = true;
			pMainFrame->m_wndStatusBar.SetPaneText(0, "Fitting Complete", TRUE);

			// Release coef matrix and inverse matrix of motion buffer.
			pDoc->m_pCurrentMB->MC_UCBS->ReleaseMatrices();
		}
	}
}

void MarkerView::OnViewCp() 
{
	// TODO: Add your command handler code here
	
}

void MarkerView::OnMotionExaggerating() 
{
	// TODO: Add your command handler code here
	CKungfuMotionDoc *pDoc = (CKungfuMotionDoc *)GetDocument();
	MotionEditingInfo MEInfo;

	DoubleScreenDialog DSDlg;
	DSDlg.SetKungfuMotionDataHead(pDoc->m_pMotionBufferHead, pDoc->m_numMotionData);
	DSDlg.SetMBInfoPointer(&MEInfo);
	DSDlg.SetDlgType(EXAGGERATING_DLG);

	if(DSDlg.DoModal() == IDOK)
	{
		KungfuMotionData *pKFMD = new KungfuMotionData;
		MotionEditing ME;

		if(ME.LinearExaggeratingFramesByCurve(MEInfo.pMotionData1, MEInfo.pMotionData2, &MEInfo, &pKFMD->MotionBuffer))
		{
			// Initialize KungfuMotionData structure without loading file.
			//pKFMD->SetMotionName("Newly Exaggerated Motion");
			pKFMD->SetMotionName(MEInfo.MotionName);
			pKFMD->Initialize(MEInfo.MotionDataType);

			pDoc->InsertMotionDataNode(pKFMD);
			pDoc->UpdateAllViews(NULL, 0L, NULL);
		}
		else
		{
			delete pKFMD;
			MessageBox("Error in exaggerating!", "Error!", MB_OK);
		}

		delete[] MEInfo.pSimVal;
	};	
}

void MarkerView::OnMotionReplacing() 
{
	// TODO: Add your command handler code here
	CKungfuMotionDoc *pDoc = (CKungfuMotionDoc *)GetDocument();
	MotionEditingInfo MEInfo;

	DoubleScreenDialog DSDlg;
	DSDlg.SetMBInfoPointer(&MEInfo);
	DSDlg.SetKungfuMotionDataHead(pDoc->m_pMotionBufferHead, pDoc->m_numMotionData);
	DSDlg.SetDlgType(REPLACING_DLG);

	if(DSDlg.DoModal() == IDOK)
	{
		KungfuMotionData *pKFMD = new KungfuMotionData;
		MotionEditing ME;

		if(ME.LinearBlendFramesByCurve(MEInfo.pMotionData1, MEInfo.pMotionData2, &MEInfo, &pKFMD->MotionBuffer))
		{
			// Initialize KungfuMotionData structure without loading file.
			//pKFMD->SetMotionName("Newly Replaced Motion");
			pKFMD->SetMotionName(MEInfo.MotionName);
			pKFMD->Initialize(MEInfo.MotionDataType);

			pDoc->InsertMotionDataNode(pKFMD);
			pDoc->UpdateAllViews(NULL, 0L, NULL);
		}
		else
		{
			delete pKFMD;
			MessageBox("Error in replacing!", "Error!", MB_OK);
		}

		delete[] MEInfo.pSimVal;
	}
}

void MarkerView::OnMotionExtracting() 
{
	// TODO: Add your command handler code here
	CKungfuMotionDoc *pDoc = (CKungfuMotionDoc *)GetDocument();
	MotionExtractionDialog MEDlg;
	MotionEditingInfo MEInfo;

	if(pDoc->m_pCurrentMB != NULL && pDoc->m_pCurrentMB->MotionBuffer.m_IsAllMarkersFitted)
	{
		MEDlg.SetPointer(&MEInfo);
		MEDlg.SetPointer(&pDoc->m_pCurrentMB->MotionBuffer);
		if(MEDlg.DoModal() == IDOK)
		{
			KungfuMotionData *pKFMD = new KungfuMotionData;
			MotionEditing ME;

			if(ME.ExtractMotionByCurve(&pDoc->m_pCurrentMB->MotionBuffer, &MEInfo, &pKFMD->MotionBuffer))
			{
				// Initialize KungfuMotionData structure without loading file.
				pKFMD->SetMotionName(MEInfo.MotionName);
				pKFMD->Initialize(MEInfo.MotionDataType);

				pDoc->InsertMotionDataNode(pKFMD);
				pDoc->UpdateAllViews(NULL, 0L, NULL);
			}
			else
			{
				delete pKFMD;
				MessageBox("Error in extracting!", "Error!", MB_OK);
			}		
		};
	}
	else
		MessageBox("Please selection a fitted motion to be extracted.", "Error", MB_OK);
}

void MarkerView::OnMotionWarping() 
{
	// TODO: Add your command handler code here
	CKungfuMotionDoc *pDoc = (CKungfuMotionDoc *)GetDocument();
	MotionWarping MEDlg;
	MotionEditingInfo MEInfo;

	if(pDoc->m_pCurrentMB != NULL && pDoc->m_pCurrentMB->MotionBuffer.m_IsAllMarkersFitted)
	{
		MEDlg.SetPointer(&MEInfo);
		MEDlg.SetPointer(&pDoc->m_pCurrentMB->MotionBuffer);

		if(MEDlg.DoModal() == IDOK)
		{
			KungfuMotionData *pKFMD = new KungfuMotionData;
			MotionEditing ME;

			if(ME.WarpMotionByCurve(&pDoc->m_pCurrentMB->MotionBuffer, &MEInfo, &pKFMD->MotionBuffer))
			{
				// Initialize KungfuMotionData structure without loading file.
				pKFMD->SetMotionName(MEInfo.MotionName);
				pKFMD->Initialize(MEInfo.MotionDataType);

				pDoc->InsertMotionDataNode(pKFMD);
				pDoc->UpdateAllViews(NULL, 0L, NULL);
			}
			else
			{
				delete pKFMD;
				MessageBox("Error in extracting!", "Error!", MB_OK);
			}		
		}
	}
	else
		MessageBox("Please selection a fitted motion to be warped.", "Error", MB_OK);
}

void MarkerView::OnFileSavesnapshot() 
{
	// TODO: Add your command handler code here
	CKungfuMotionDoc *pDoc = (CKungfuMotionDoc *)GetDocument();
	SnapShotDialog SSDlg;

	if(pDoc->m_pMotionBufferHead != NULL)
	{
		SSDlg.SetPointer(pDoc->m_pMotionBufferHead, pDoc->m_numMotionData);
		//SSDlg.SetPointer(&pDoc->m_pCurrentMB->MotionBuffer);
		SSDlg.DoModal();
	}
}	

void MarkerView::OnMotionTransition() 
{
	// TODO: Add your command handler code here
	// TODO: Add your command handler code here
	CKungfuMotionDoc *pDoc = (CKungfuMotionDoc *)GetDocument();
	MotionEditingInfo MEInfo;

	DoubleScreenDialog DSDlg;

	DSDlg.SetMBInfoPointer(&MEInfo);
	DSDlg.SetKungfuMotionDataHead(pDoc->m_pMotionBufferHead, pDoc->m_numMotionData);
	DSDlg.SetDlgType(TRANSITING_DLG);

	if(DSDlg.DoModal() == IDOK)
	{
		KungfuMotionData *pKFMD = new KungfuMotionData;
		MotionEditing ME;

		if(ME.TransiteMotionsByCurve(MEInfo.pMotionData1, MEInfo.pMotionData2, &MEInfo, &pKFMD->MotionBuffer))
		{
			// Initialize KungfuMotionData structure without loading file.
			//pKFMD->SetMotionName("Newly Combined Motion");
			pKFMD->SetMotionName(MEInfo.MotionName);
			pKFMD->Initialize(MEInfo.MotionDataType);

			pDoc->InsertMotionDataNode(pKFMD);
			pDoc->UpdateAllViews(NULL, 0L, NULL);
		}
		else
		{
			delete pKFMD;
			MessageBox("Error in combining!", "Error!", MB_OK);
		}

	}	

}

void MarkerView::OnMotionFilter() 
{
	// TODO: Add your command handler code here
	CKungfuMotionDoc *pDoc = (CKungfuMotionDoc *)GetDocument();
	FilterDialog FilterDlg;

	if(pDoc->m_pCurrentMB != NULL)
	{
		FilterDlg.SetPointer(pDoc->m_pCurrentMB, pDoc->m_currentMarker, pDoc->m_currentDOF);
		if(FilterDlg.DoModal() == IDOK)
		{
			pDoc->m_pCurveView->SortPeakValue();
			pDoc->m_pCurveView->Invalidate(FALSE);
		}
	}
}
