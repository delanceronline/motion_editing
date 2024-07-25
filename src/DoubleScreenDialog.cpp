// DoubleScreenDialog.cpp : implementation file
//

#include "stdafx.h"
#include "KungfuMotion.h"
#include "DoubleScreenDialog.h"
#include "Motion Editing\MotionEditing.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// DoubleScreenDialog dialog


DoubleScreenDialog::DoubleScreenDialog(CWnd* pParent /*=NULL*/)
	: CDialog(DoubleScreenDialog::IDD, pParent)
{
	m_pMotionData1 = NULL;
	m_pMotionData2 = NULL;
	m_hRoot = NULL;
	m_hCurrentItem = NULL;
	m_mti = NULL;
	m_sti = NULL;
	m_pMarkerSelectionArray = NULL;
	m_pMotionSelectionArray = NULL;
	m_pMBInfo = NULL;
	m_pKFMD_Head = NULL;

	m_IsMotionSelectionMode = true;
	m_IsJointSelectionMode = false;

	m_numKFMD = 0;
	m_numMotionSelected = 0;
	//{{AFX_DATA_INIT(DoubleScreenDialog)
	//}}AFX_DATA_INIT
}


void DoubleScreenDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(DoubleScreenDialog)
	DDX_Control(pDX, IDC_SIMVAL_STATIC, m_SimValSliderStatic);
	DDX_Control(pDX, IDOK, m_OkButton);
	DDX_Control(pDX, IDC_DESELECTIONALL_BUTTON_DOUBLESCREEN, m_DeselectionAllButton);
	DDX_Control(pDX, IDC_SELECTIONALL_BUTTON_DOUBLESCREEN, m_SelectionAllButton);
	DDX_Control(pDX, IDC_S2_STARTFRAME_STATIC, m_S2_StartFrameStatic);
	DDX_Control(pDX, IDC_S2_ENDFRAME_STATIC, m_S2_EndFrameStatic);
	DDX_Control(pDX, IDC_S1_STARTFRAME_STATIC, m_S1_StartFrameStatic);
	DDX_Control(pDX, IDC_S1_ENDFRAME_STATIC, m_S1_EndFrameStatic);
	DDX_Control(pDX, IDC_SCREEN1_ENDFRAME_SLIDER, m_S1_EndFrameSlider);
	DDX_Control(pDX, IDC_S2_MARKERSIZE_STATIC, m_S2_MarkerSizeStatic);
	DDX_Control(pDX, IDC_S1_MARKERSIZE_STATIC, m_S1_MarkerSizeStatic);
	DDX_Control(pDX, IDC_MOTIONNAME_TEXTBOX, m_MotionName);
	DDX_Control(pDX, IDC_SCREEN2_ENDFRAME_SLIDER, m_S2_EndFrameSlider);
	DDX_Control(pDX, IDC_SCREEN2_STARTFRAME_SLIDER, m_S2_StartFrameSlider);
	DDX_Control(pDX, IDC_SCREEN1_STARTFRAME_SLIDER, m_S1_StartFrameSlider);
	DDX_Control(pDX, IDC_SimVal, m_SimValStatic);
	DDX_Control(pDX, IDC_SIMVAL_SLIDER, m_SimValSlider);
	DDX_Control(pDX, IDC_DESELECTION_BUTTON_DOUBLESCREEN, m_DeselectJointButton);
	DDX_Control(pDX, IDC_SELECTION_BUTTON_DOUBLESCREEN, m_SelectJointButton);
	DDX_Control(pDX, IDC_MARKERTREE_DOUBLESCREEN, m_MarkerTree);
	DDX_Control(pDX, IDC_SELECTEDMARKERLIST, m_SelectedMarkerList);
	DDX_Control(pDX, IDC_SKETELON_BUTTON, m_SketelonButton);
	DDX_Control(pDX, IDC_JOINT_BUTTON, m_JointButton);
	DDX_Control(pDX, IDC_RADIUS2_SLIDER, m_R2_Slider);
	DDX_Control(pDX, IDC_RADIUS1_SLIDER, m_R1_Slider);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(DoubleScreenDialog, CDialog)
	//{{AFX_MSG_MAP(DoubleScreenDialog)
	ON_WM_HSCROLL()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_SKETELON_BUTTON, OnSketelonButton)
	ON_BN_CLICKED(IDC_JOINT_BUTTON, OnJointButton)
	ON_NOTIFY(TVN_SELCHANGED, IDC_MARKERTREE_DOUBLESCREEN, OnSelchangedMarkertreeDoublescreen)
	ON_BN_CLICKED(IDC_SELECTION_BUTTON_DOUBLESCREEN, OnSelectionButtonDoublescreen)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_SELECTEDMARKERLIST, OnItemchangedSelectedmarkerlist)
	ON_BN_CLICKED(IDC_DESELECTION_BUTTON_DOUBLESCREEN, OnDeselectionButtonDoublescreen)
	ON_BN_CLICKED(IDC_SELECTIONALL_BUTTON_DOUBLESCREEN, OnSelectionallButtonDoublescreen)
	ON_BN_CLICKED(IDC_DESELECTIONALL_BUTTON_DOUBLESCREEN, OnDeselectionallButtonDoublescreen)
	ON_EN_CHANGE(IDC_MOTIONNAME_TEXTBOX, OnChangeMotionnameTextbox)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// DoubleScreenDialog message handlers

void DoubleScreenDialog::SetPointer(KungfuMotionData *ptr1, KungfuMotionData *ptr2)
{
	m_pMotionData1 = ptr1;
	m_pMotionData2 = ptr2;
}

void DoubleScreenDialog::SetKungfuMotionDataHead(KungfuMotionData *ptr, unsigned int numKFMD)
{
	m_pKFMD_Head = ptr;

	m_numKFMD = 0;
	for(ptr = m_pKFMD_Head; ptr != NULL; ptr = ptr->pNext)
	{
		if(ptr->MotionBuffer.m_MotionDataType == BVH_MOTION_TYPE && ptr->MotionBuffer.m_IsAllMarkersFitted)
			m_numKFMD++;
	}
}

void DoubleScreenDialog::SetDlgType(UINT DlgType)
{
	m_DialogType = DlgType;
}

BOOL DoubleScreenDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	unsigned int i;
	CString str;

	CRect glRect, newRect;

	// Register window class for motion preview.
	WNDCLASS wndClass;
	HCURSOR hCursor;
	hCursor = AfxGetApp()->LoadStandardCursor(IDC_CROSS);

	wndClass.style = CS_OWNDC;
	wndClass.lpfnWndProc = AfxWndProc;
	wndClass.cbClsExtra = 0;
	wndClass.cbWndExtra = 0;
	wndClass.hInstance = AfxGetInstanceHandle();
	wndClass.hIcon = NULL;
	wndClass.hCursor = hCursor;
	wndClass.hbrBackground = NULL;
	wndClass.lpszMenuName = NULL;
	wndClass.lpszClassName = "MotionPreviewScreen1";

	// Register Screen1.
	AfxRegisterClass(&wndClass);

	wndClass.style = CS_OWNDC;
	wndClass.lpfnWndProc = AfxWndProc;
	wndClass.cbClsExtra = 0;
	wndClass.cbWndExtra = 0;
	wndClass.hInstance = AfxGetInstanceHandle();
	wndClass.hIcon = NULL;
	wndClass.hCursor = hCursor;
	wndClass.hbrBackground = NULL;
	wndClass.lpszMenuName = NULL;
	wndClass.lpszClassName = "MotionPreviewScreen2";

	// Register Screen2.
	AfxRegisterClass(&wndClass);

	GetDlgItem(IDC_SCREEN1_BORDER)->GetWindowRect(&glRect);
	newRect.top = glRect.top - 80;
	newRect.bottom = glRect.bottom - 80;
	newRect.left = glRect.left;
	newRect.right = glRect.right;

	// Create Screen1.
	m_mpScreen1.Create("MotionPreviewScreen1", NULL, WS_CHILD | WS_VISIBLE, newRect, this, ID_MOTION_PREVIEW_SCREEN, NULL);
	m_mpScreen1.m_MarkerSize = 2.0f;

	GetDlgItem(IDC_SCREEN2_BORDER)->GetWindowRect(&glRect);
	newRect.top = glRect.top - 80;
	newRect.bottom = glRect.bottom - 80;
	newRect.left = glRect.left;
	newRect.right = glRect.right;

	// Create Screen2.
	m_mpScreen2.Create("MotionPreviewScreen2", NULL, WS_CHILD | WS_VISIBLE, newRect, this, ID_MOTION_PREVIEW_SCREEN, NULL);
	m_mpScreen2.m_MarkerSize = 2.0f;

	// Create image list.
	HICON hIcon;
	m_ImageList.Create(16, 16, ILC_COLOR16, 5, 0);
	hIcon = ::LoadIcon(AfxGetResourceHandle(), MAKEINTRESOURCE(IDI_MOTION));
	m_ImageList.Add(hIcon);
	hIcon = ::LoadIcon(AfxGetResourceHandle(), MAKEINTRESOURCE(IDI_JOINT));
	m_ImageList.Add(hIcon);
	hIcon = ::LoadIcon(AfxGetResourceHandle(), MAKEINTRESOURCE(IDI_CURVE));
	m_ImageList.Add(hIcon);
	hIcon = ::LoadIcon(AfxGetResourceHandle(), MAKEINTRESOURCE(IDI_ENDSITE));
	m_ImageList.Add(hIcon);
	hIcon = ::LoadIcon(AfxGetResourceHandle(), MAKEINTRESOURCE(IDI_SELECTED));
	m_ImageList.Add(hIcon);

	m_SelectedMarkerList.SetImageList(&m_ImageList, LVSIL_SMALL);

	// Initialize tree control.
	m_MarkerTree.SetImageList(&m_ImageList, TVSIL_NORMAL);

	// Initialize list control.
	LV_COLUMN lvColumn;
	lvColumn.mask = LVCF_WIDTH | LVCF_TEXT | LVCF_FMT | LVCF_SUBITEM;
	lvColumn.fmt = LVCFMT_CENTER;
	lvColumn.cx = 200;

	// Marker size sliders	
	m_R1_Slider.SetRange(0, 100, FALSE);
	m_R2_Slider.SetRange(0, 100, FALSE);
	m_R1_Slider.SetPos(20);
	m_R2_Slider.SetPos(20);

	m_SimValSlider.SetRange(0, 100, FALSE);
	m_SimValSlider.SetPos(0);

	if(m_DialogType == REPLACING_DLG)
	{
		m_SimValSlider.ShowWindow(SW_HIDE);
		m_SimValSliderStatic.ShowWindow(SW_HIDE);
		m_SimValStatic.ShowWindow(SW_HIDE);
		m_SketelonButton.ShowWindow(SW_HIDE);
	
		m_sti = new MotionToItem[m_numKFMD];
		m_pMotionSelectionArray = new bool[m_numKFMD];
		
		for(i = 0; i < m_numKFMD; i++)
			m_pMotionSelectionArray[i] = false;

		ResetMotionTree();

		// Selection list
		lvColumn.iSubItem = 0;
		lvColumn.pszText = "Selected Motion(s)";
		m_SelectedMarkerList.InsertColumn(0, &lvColumn);

		m_SelectJointButton.EnableWindow(FALSE);
		m_SelectionAllButton.EnableWindow(FALSE);
		m_DeselectJointButton.EnableWindow(FALSE);
		m_DeselectionAllButton.EnableWindow(FALSE);
		m_OkButton.EnableWindow(FALSE);
		m_SimValSlider.EnableWindow(FALSE);

		m_S1_StartFrameSlider.EnableWindow(FALSE);
		m_S2_StartFrameSlider.EnableWindow(FALSE);
		m_S1_EndFrameSlider.EnableWindow(FALSE);
		m_S2_EndFrameSlider.EnableWindow(FALSE);
		m_JointButton.EnableWindow(FALSE);
	}

	if(m_DialogType == TRANSITING_DLG)
	{
		m_SelectionAllButton.ShowWindow(SW_HIDE);
		m_DeselectionAllButton.ShowWindow(SW_HIDE);
		m_SketelonButton.ShowWindow(SW_HIDE);

		m_SimValSliderStatic.SetWindowText("Time:");

		m_sti = new MotionToItem[m_numKFMD];
		m_pMotionSelectionArray = new bool[m_numKFMD];
		
		for(i = 0; i < m_numKFMD; i++)
			m_pMotionSelectionArray[i] = false;

		ResetMotionTree();

		// Selection list
		lvColumn.iSubItem = 0;
		lvColumn.pszText = "Selected Motion(s)";
		m_SelectedMarkerList.InsertColumn(0, &lvColumn);

		m_S1_StartFrameSlider.EnableWindow(FALSE);
		m_S2_StartFrameSlider.EnableWindow(FALSE);
		m_S1_EndFrameSlider.EnableWindow(FALSE);
		m_S2_EndFrameSlider.EnableWindow(FALSE);

		m_SelectJointButton.EnableWindow(FALSE);
		m_DeselectJointButton.EnableWindow(FALSE);
		m_JointButton.EnableWindow(FALSE);
		m_OkButton.EnableWindow(FALSE);
		m_SimValSlider.EnableWindow(FALSE);
	}

	///////////////////////////////////////////////////////////////////////////////////////
	if(m_DialogType == BLENDING_DLG || m_DialogType == EXAGGERATING_DLG)
	{

		// SimVal slider
		if(m_DialogType == BLENDING_DLG)
			m_SimValSliderStatic.SetWindowText("Similarity:");

		if(m_DialogType == EXAGGERATING_DLG)
			m_SimValSliderStatic.SetWindowText("E Factor:");

		if(m_IsMotionSelectionMode)
		{
			m_sti = new MotionToItem[m_numKFMD];
			m_pMotionSelectionArray = new bool[m_numKFMD];
		
			for(i = 0; i < m_numKFMD; i++)
				m_pMotionSelectionArray[i] = false;

			ResetMotionTree();

			// Selection list
			lvColumn.iSubItem = 0;
			lvColumn.pszText = "Selected Motion(s)";
			m_SelectedMarkerList.InsertColumn(0, &lvColumn);

			m_SelectJointButton.EnableWindow(FALSE);
			m_SelectionAllButton.EnableWindow(FALSE);
			m_DeselectJointButton.EnableWindow(FALSE);
			m_DeselectionAllButton.EnableWindow(FALSE);
			m_OkButton.EnableWindow(FALSE);
			m_SimValSlider.EnableWindow(FALSE);

			m_S1_StartFrameSlider.EnableWindow(FALSE);
			m_S2_StartFrameSlider.EnableWindow(FALSE);
			m_S1_EndFrameSlider.EnableWindow(FALSE);
			m_S2_EndFrameSlider.EnableWindow(FALSE);
		}

		if(m_IsJointSelectionMode)
		{
			m_mti = new MarkerToItem[m_pMotionData1->MotionBuffer.numMarkers];
			// Insert markers to tree.
			ResetMarkerTree();

			m_S1_StartFrameSlider.EnableWindow(TRUE);
			m_S2_StartFrameSlider.EnableWindow(TRUE);
			m_S1_EndFrameSlider.EnableWindow(TRUE);
			m_S2_EndFrameSlider.EnableWindow(TRUE);

			// Start Frame Sliders
			m_S1_StartFrameSlider.SetRange(0, m_pMotionData1->MotionBuffer.numFrames, FALSE);
			m_S2_StartFrameSlider.SetRange(0, m_pMotionData2->MotionBuffer.numFrames, FALSE);
			m_S1_StartFrameSlider.SetPos(0);
			m_S2_StartFrameSlider.SetPos(0);
			m_S1_StartFrame = 0;
			m_S2_StartFrame = 0;

			m_S1_StartFrameStatic.SetWindowText("0");
			m_S2_StartFrameStatic.SetWindowText("0");

			// End Frame Sliders.
			m_S1_EndFrameSlider.SetRange(0, m_pMotionData1->MotionBuffer.numFrames, FALSE);
			m_S2_EndFrameSlider.SetRange(0, m_pMotionData2->MotionBuffer.numFrames, FALSE);
			m_S1_EndFrameSlider.SetPos(m_pMotionData1->MotionBuffer.numFrames);
			m_S2_EndFrameSlider.SetPos(m_pMotionData2->MotionBuffer.numFrames);
			m_S1_EndFrame = m_pMotionData1->MotionBuffer.numFrames;
			m_S2_EndFrame = m_pMotionData2->MotionBuffer.numFrames;	

			str.Format("%u", m_pMotionData1->MotionBuffer.numFrames);
			m_S1_EndFrameStatic.SetWindowText(str);
			str.Format("%u", m_pMotionData2->MotionBuffer.numFrames);
			m_S2_EndFrameStatic.SetWindowText(str);

			// Marker Selection Array
			m_pMarkerSelectionArray = new bool[m_pMotionData1->GetNumMarkers()];
			for(i = 0; i < m_pMotionData1->GetNumMarkers(); i++)
				m_pMarkerSelectionArray[i] = false;

			// Similarity values for all separated joints.
			m_pMBInfo->pSimVal = new float[m_pMotionData1->GetNumMarkers()];
			for(i = 0; i < m_pMotionData1->GetNumMarkers(); i++)
				m_pMBInfo->pSimVal[i] = 0.0f;

			m_SimValSlider.EnableWindow(FALSE);

			// Selection list
			lvColumn.iSubItem = 0;
			lvColumn.pszText = "Selected Joint(s)";
			m_SelectedMarkerList.InsertColumn(0, &lvColumn);

			m_mpScreen1.SetPointer(&m_pMotionData1->MotionBuffer);
			m_mpScreen1.SetPointer(this);
			m_mpScreen1.Invalidate(FALSE);

			m_mpScreen2.SetPointer(&m_pMotionData2->MotionBuffer);
			m_mpScreen2.SetPointer(this);
			m_mpScreen2.Invalidate(FALSE);

			// Button
			m_SelectJointButton.EnableWindow(FALSE);
			m_DeselectJointButton.EnableWindow(FALSE);
			m_DeselectionAllButton.EnableWindow(FALSE);
		}

		m_SketelonButton.EnableWindow(FALSE);
		m_JointButton.EnableWindow(FALSE);

		m_OkButton.EnableWindow(FALSE);

		// New motion name
	}
	//////////////////////////////////////////////////////////////////////////////

	m_MotionName.SetLimitText(256);
	m_MotionName.SetWindowText("New Motion1");

	m_OkButton.EnableWindow(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void DoubleScreenDialog::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	// TODO: Add your message handler code here and/or call default
	int val;
	CString str;

	if((CSliderCtrl *)pScrollBar == &m_S1_StartFrameSlider)
	{
		val = m_S1_StartFrameSlider.GetPos();
		m_mpScreen1.m_Frame = val;

		str.Format("%d", val);		
		m_S1_StartFrameStatic.SetWindowText(str);
		m_S1_EndFrameStatic.SetWindowText(str);

		m_S1_EndFrameSlider.SetRange(val, m_pMotionData1->MotionBuffer.numFrames, FALSE);
		m_S1_EndFrameSlider.SetPos(val);

		m_S1_StartFrame = val;

		m_mpScreen1.Invalidate(FALSE);
	}

	if((CSliderCtrl *)pScrollBar == &m_S2_StartFrameSlider)
	{
		val = m_S2_StartFrameSlider.GetPos();
		m_mpScreen2.m_Frame = val;

		str.Format("%d", val);		
		m_S2_StartFrameStatic.SetWindowText(str);
		m_S2_EndFrameStatic.SetWindowText(str);

		m_S2_EndFrameSlider.SetRange(val, m_pMotionData2->MotionBuffer.numFrames, FALSE);
		m_S2_EndFrameSlider.SetPos(val);

		m_S2_StartFrame = val;

		m_mpScreen2.Invalidate(FALSE);
	}

	if((CSliderCtrl *)pScrollBar == &m_S1_EndFrameSlider)
	{
		val = m_S1_EndFrameSlider.GetPos();
		m_mpScreen1.m_Frame = val;

		str.Format("%d", val);		
		m_S1_EndFrameStatic.SetWindowText(str);

		m_S1_EndFrame = val;

		m_mpScreen1.Invalidate(FALSE);
	}

	if((CSliderCtrl *)pScrollBar == &m_S2_EndFrameSlider)
	{
		val = m_S2_EndFrameSlider.GetPos();
		m_mpScreen2.m_Frame = val;

		str.Format("%d", val);		
		m_S2_EndFrameStatic.SetWindowText(str);

		m_S2_EndFrame = val;

		m_mpScreen2.Invalidate(FALSE);
	}

	// Change radius of marker balls according to val.
	if((CSliderCtrl *)pScrollBar == &m_R1_Slider)
	{
		val = m_R1_Slider.GetPos();
		m_mpScreen1.m_MarkerSize = (float)val / 10.0f;

		str.Format("%0.2f", m_mpScreen1.m_MarkerSize);		
		m_S1_MarkerSizeStatic.SetWindowText(str);

		m_mpScreen1.Invalidate(FALSE);
	}
	if((CSliderCtrl *)pScrollBar == &m_R2_Slider)
	{
		val = m_R2_Slider.GetPos();
		m_mpScreen2.m_MarkerSize = (float)val / 10.0f;

		str.Format("%0.2f", m_mpScreen2.m_MarkerSize);		
		m_S2_MarkerSizeStatic.SetWindowText(str);

		m_mpScreen2.Invalidate(FALSE);
	}

	// SimVal slider.
	if((CSliderCtrl *)pScrollBar == &m_SimValSlider)
	{
		val = m_SimValSlider.GetPos();

		if(m_DialogType == TRANSITING_DLG)
		{
			m_pMBInfo->TransitingTime = (float)val * 0.02;
			str.Format("%0.2f", m_pMBInfo->TransitingTime);
			m_OkButton.EnableWindow(TRUE);
		}

		if(m_DialogType == BLENDING_DLG || m_DialogType == EXAGGERATING_DLG)
		{
			int i = m_SelectedMarkerList.GetNextItem(-1, LVNI_SELECTED);
			unsigned int nMarker = m_SelectedMarkerList.GetItemData(i);

			m_pMBInfo->pSimVal[nMarker] = (float)val * 0.01f;
			str.Format("%0.2f", m_pMBInfo->pSimVal[nMarker]);
		}

		m_SimValStatic.SetWindowText(str);
	}

	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}

void DoubleScreenDialog::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	if(m_mti != NULL)
		delete[] m_mti;

	if(m_sti != NULL)
		delete[] m_sti;

	if(m_pMarkerSelectionArray != NULL)
		delete[] m_pMarkerSelectionArray;

	if(m_pMotionSelectionArray != NULL)
		delete[] m_pMotionSelectionArray;
}

void DoubleScreenDialog::OnSketelonButton() 
{
	// TODO: Add your control notification handler code here

	m_SketelonButton.EnableWindow(FALSE);
	m_JointButton.EnableWindow(TRUE);
}

void DoubleScreenDialog::OnJointButton() 
{
	// TODO: Add your control notification handler code here

	m_JointButton.EnableWindow(FALSE);
	m_SketelonButton.EnableWindow(TRUE);

	SwitchToJointSelection();
}

// Recursive function to add child joint under parent joint in tree structure.
bool DoubleScreenDialog::InsertItemToParent(KungfuMotionData *pMB, HTREEITEM hParentItem, unsigned int nMarker_Parent)
{
	HTREEITEM hItem1;
	MotionMarker *pParentMK = pMB->MC_UCBS->GetMarker(nMarker_Parent);

	if(hParentItem == NULL)
	{
		// Add root joint to tree root.
		if(pParentMK->IsEndSite)
			hItem1 = m_MarkerTree.InsertItem(pParentMK->name, 3, 3, m_hRoot, TVI_LAST);
		else
			hItem1 = m_MarkerTree.InsertItem(pParentMK->name, 1, 1, m_hRoot, TVI_LAST);

		m_mti[pParentMK->nPosition].tItem = hItem1;

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
				m_mti[pMK->nPosition].tItem = hItem1;

				InsertItemToParent(pMB, hItem1, pMK->nPosition);
			}

			pMK = pMK->next;
		}
	}

	return true;
}

void DoubleScreenDialog::OnSelchangedMarkertreeDoublescreen(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	m_hCurrentItem = pNMTreeView->itemNew.hItem;

	unsigned int i;

		if(m_IsMotionSelectionMode)
		{
			i = GetCurrentMotionIndex_Tree();

			if(m_pMotionSelectionArray[i] || m_numMotionSelected >= 2)
				m_SelectJointButton.EnableWindow(FALSE);
			else
				m_SelectJointButton.EnableWindow(TRUE);
		}

		if(m_IsJointSelectionMode)
		{
			i = GetCurrentMarkerIndex_Tree();
			MotionMarker *pMK;
			pMK = m_pMotionData1->GetMarker(i);

			if(m_pMarkerSelectionArray[i] || pMK->IsEndSite)
				m_SelectJointButton.EnableWindow(FALSE);
			else
				m_SelectJointButton.EnableWindow(TRUE);
		}

	*pResult = 0;
}

void DoubleScreenDialog::OnSelectionButtonDoublescreen() 
{
	// TODO: Add your control notification handler code here

	LV_ITEM lvItem;
	unsigned int i;

	lvItem.mask = LVIF_IMAGE | LVIF_TEXT | LVIF_STATE | LVIF_PARAM;
	lvItem.state = 0;
	lvItem.stateMask = 0;
	lvItem.iImage = 1;

	lvItem.iItem = 0;
	lvItem.iSubItem = 0;

	if(m_IsMotionSelectionMode)
	{
		bool IsOkay = true;

		if(m_numMotionSelected < 2)
		{
			KungfuMotionData *pKFMD;
			i = GetCurrentMotionIndex_Tree();
			pKFMD = GetKFMD(i);

			// Check this one whether has identical sketelon with primary one.
			if(m_numMotionSelected == 1)
			{
				MotionEditing ME;

				if(!ME.IsSketelonIdentical(&m_pMotionData1->MotionBuffer, &pKFMD->MotionBuffer))
				{
					MessageBox("This motion has the sketelon different from the primary one.", "Error", MB_OK);
					IsOkay = false;
				}
			}

			if(IsOkay)
			{
				m_pMotionSelectionArray[i] = true;

				m_SelectJointButton.EnableWindow(FALSE);

				m_MarkerTree.SetItemImage(m_hCurrentItem, 4, 4);

				lvItem.pszText = pKFMD->name;
				lvItem.lParam = i;
				
				// Set Kungfu Motion Data 1 for first one.
				if(m_numMotionSelected == 0)
					m_pMotionData1 = pKFMD;

				// Set Kungfu Motion Data 2 for second one.
				if(m_numMotionSelected == 1)
					m_pMotionData2 = pKFMD;

				m_numMotionSelected++;
			}
		}

		if(m_numMotionSelected >= 2)
			m_JointButton.EnableWindow(TRUE);
	}

	if(m_IsJointSelectionMode)
	{
		MotionMarker *pMK;

		i = GetCurrentMarkerIndex_Tree();
		
		pMK = m_pMotionData1->GetMarker(i);
		m_pMarkerSelectionArray[i] = true;
		
		if(m_DialogType == REPLACING_DLG)
			m_pMBInfo->pSimVal[i] = 1.0f;

		m_MarkerTree.SetItemImage(m_hCurrentItem, 4, 4);

		lvItem.pszText = pMK->name;
		lvItem.lParam = i;

		m_SelectJointButton.EnableWindow(FALSE);
	}

	m_SelectedMarkerList.InsertItem(&lvItem);
}

unsigned int DoubleScreenDialog::GetCurrentMarkerIndex_Tree()
{
	unsigned int i;

	for(i = 0; i < m_pMotionData1->MotionBuffer.numMarkers; i++)
	{
		if(m_mti[i].tItem == m_hCurrentItem)
			return i;
	}

	return 0;
}

unsigned int DoubleScreenDialog::GetCurrentMotionIndex_Tree()
{
	unsigned int i;

	for(i = 0; i < m_numKFMD; i++)
	{
		if(m_sti[i].tItem == m_hCurrentItem)
			//return m_sti[i].nMotion;
			return i;
	}

	return 0;
}

void DoubleScreenDialog::OnItemchangedSelectedmarkerlist(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here

	if(m_IsMotionSelectionMode)
	{
		if(m_pMotionSelectionArray[pNMListView->lParam])
		{
			m_SelectJointButton.EnableWindow(FALSE);
			m_DeselectJointButton.EnableWindow(TRUE);
		}
	}

	if(m_IsJointSelectionMode)
	{
		if(m_pMarkerSelectionArray[pNMListView->lParam])
		{
			CString str;

			m_DeselectJointButton.EnableWindow(TRUE);
			m_DeselectionAllButton.EnableWindow(TRUE);

			m_SimValSlider.EnableWindow(TRUE);
			m_SimValSlider.SetPos((int)(m_pMBInfo->pSimVal[pNMListView->lParam] * 100.0f));

			str.Format("%0.2f", m_pMBInfo->pSimVal[pNMListView->lParam]);
			m_SimValStatic.SetWindowText(str);
		}
	}

	*pResult = 0;
}

void DoubleScreenDialog::OnDeselectionButtonDoublescreen() 
{
	// TODO: Add your control notification handler code here
	int i = m_SelectedMarkerList.GetNextItem(-1, LVNI_SELECTED);
	unsigned int Index = m_SelectedMarkerList.GetItemData(i);
	m_SelectedMarkerList.DeleteItem(i);
	
	if(m_IsMotionSelectionMode)
	{
		m_pMotionSelectionArray[Index] = false;

		m_MarkerTree.SetItemImage(m_sti[Index].tItem, 0, 0);
		
		if(m_numMotionSelected == 2)
		{
			if(m_pMotionData1 == GetKFMD(Index))
			{
				m_pMotionData1 = m_pMotionData2;
				m_pMotionData2 = NULL;
			}
			if(m_pMotionData2 == GetKFMD(Index))
				m_pMotionData2 = NULL;
		}

		if(m_numMotionSelected == 1)
		{
			m_pMotionData1 = NULL;
			m_pMotionData2 = NULL;
		}

		m_numMotionSelected--;

		if(m_numMotionSelected < 2)
			m_JointButton.EnableWindow(FALSE);

		if(m_SelectedMarkerList.GetSelectedCount() == 0)
			m_DeselectJointButton.EnableWindow(FALSE);
	}

	if(m_IsJointSelectionMode)
	{
		m_pMarkerSelectionArray[Index] = false;
		m_pMBInfo->pSimVal[Index] = 0.0f;

		m_MarkerTree.SetItemImage(m_mti[Index].tItem, 1, 1);

		if(m_SelectedMarkerList.GetSelectedCount() == 0)
		{
			m_DeselectJointButton.EnableWindow(FALSE);
			m_SimValSlider.EnableWindow(FALSE);
		}
	}
}

void DoubleScreenDialog::OnFinalRelease() 
{
	// TODO: Add your specialized code here and/or call the base class


	CDialog::OnFinalRelease();
}

void DoubleScreenDialog::SetMBInfoPointer(MotionEditingInfo *pMBInfo)
{
	m_pMBInfo = pMBInfo;
}

void DoubleScreenDialog::OnOK() 
{
	// TODO: Add extra validation here

	m_pMBInfo->StartFrame_M1 = m_S1_StartFrame;
	m_pMBInfo->StartFrame_M2 = m_S2_StartFrame;
	m_pMBInfo->EndFrame_M1 = m_S1_EndFrame;
	m_pMBInfo->EndFrame_M2 = m_S2_EndFrame;

	m_pMBInfo->StartTime_M1 = m_pMotionData1->MotionBuffer.FrameTime * (float)m_S1_StartFrame;
	m_pMBInfo->StartTime_M2 = m_pMotionData2->MotionBuffer.FrameTime * (float)m_S2_StartFrame;
	m_pMBInfo->EndTime_M1 = m_pMotionData1->MotionBuffer.FrameTime * m_S1_EndFrame;
	m_pMBInfo->EndTime_M2 = m_pMotionData2->MotionBuffer.FrameTime * m_S2_EndFrame;

	if(m_DialogType == TRANSITING_DLG)
		m_pMBInfo->numFrames = (m_S1_EndFrame - m_S1_StartFrame) + (m_S2_EndFrame - m_S2_StartFrame);

	if(m_DialogType == BLENDING_DLG || m_DialogType == EXAGGERATING_DLG  || m_DialogType == REPLACING_DLG)
		m_pMBInfo->numFrames = m_S1_EndFrame - m_S1_StartFrame;

	m_pMBInfo->numMarkers = m_pMotionData1->MotionBuffer.numMarkers;
	m_pMBInfo->FrameTime = m_pMotionData1->MotionBuffer.FrameTime;
	m_pMBInfo->MotionDataType = m_pMotionData1->MotionDataType;
	m_pMBInfo->pMotionData1 = &m_pMotionData1->MotionBuffer;
	m_pMBInfo->pMotionData2 = &m_pMotionData2->MotionBuffer;

	char text[256];
	m_MotionName.GetWindowText(text, 256);
	strcpy(m_pMBInfo->MotionName, text);

	CDialog::OnOK();
}

void DoubleScreenDialog::OnSelectionallButtonDoublescreen() 
{
	// TODO: Add your control notification handler code here
	MotionMarker *pMK;
	LV_ITEM lvItem;
	unsigned int i;
	bool IsMoved = false;

	for(i = 0; i < m_pMotionData1->MotionBuffer.numMarkers; i++)
	{
		pMK = m_pMotionData1->GetMarker(i);

		if(!m_pMarkerSelectionArray[i] && !pMK->IsEndSite)
		{
			m_pMarkerSelectionArray[i] = true;
			m_MarkerTree.SetItemImage(m_mti[i].tItem, 4, 4);

			lvItem.mask = LVIF_IMAGE | LVIF_TEXT | LVIF_STATE | LVIF_PARAM;
			lvItem.state = 0;
			lvItem.stateMask = 0;
			lvItem.iImage = 1;

			lvItem.iItem = 0;
			lvItem.iSubItem = 0;
			lvItem.pszText = pMK->name;
			lvItem.lParam = i;
			m_SelectedMarkerList.InsertItem(&lvItem);

			if(m_DialogType == REPLACING_DLG)
				m_pMBInfo->pSimVal[i] = 1.0f;

			IsMoved = true;
		}
	}

	if(IsMoved)
	{
		m_SelectJointButton.EnableWindow(FALSE);
		m_SelectionAllButton.EnableWindow(FALSE);

		m_DeselectJointButton.EnableWindow(TRUE);
		m_DeselectionAllButton.EnableWindow(TRUE);
	}
}

void DoubleScreenDialog::OnDeselectionallButtonDoublescreen() 
{
	// TODO: Add your control notification handler code here
	unsigned int i;

	m_SelectedMarkerList.DeleteAllItems();

	for(i = 0; i < m_pMotionData1->MotionBuffer.numMarkers; i++)
	{
		m_pMarkerSelectionArray[i] = false;
		m_pMBInfo->pSimVal[i] = 0.0f;

		m_MarkerTree.SetItemImage(m_mti[i].tItem, 1, 1);

		m_SelectJointButton.EnableWindow(TRUE);
		m_DeselectionAllButton.EnableWindow(FALSE);
		m_SelectionAllButton.EnableWindow(TRUE);
		m_SimValSlider.EnableWindow(FALSE);
	}

	m_SelectJointButton.EnableWindow(TRUE);
	m_SelectionAllButton.EnableWindow(TRUE);

	m_DeselectJointButton.EnableWindow(FALSE);
	m_DeselectionAllButton.EnableWindow(FALSE);
}

void DoubleScreenDialog::ResetMarkerTree()
{
	m_MarkerTree.DeleteAllItems();
	m_hRoot = m_MarkerTree.InsertItem(m_pMotionData1->name, 0, 0, TVI_ROOT, TVI_LAST);
	// Run this recursive function to add child item to parent item.
	InsertItemToParent(m_pMotionData1, NULL, 0);

	m_MarkerTree.Expand(m_hRoot, TVE_EXPAND);
	m_MarkerTree.SelectItem(m_MarkerTree.GetChildItem(m_hRoot));
}

void DoubleScreenDialog::ResetMotionTree()
{
	unsigned int i = 0;
	KungfuMotionData *pKFMD = m_pKFMD_Head;

	m_MarkerTree.DeleteAllItems();
	for(pKFMD = m_pKFMD_Head; pKFMD != NULL; pKFMD = pKFMD->pNext)
	{
		if(pKFMD->MotionBuffer.m_MotionDataType == BVH_MOTION_TYPE && pKFMD->MotionBuffer.m_IsAllMarkersFitted)
		{
			m_sti[i].tItem = m_MarkerTree.InsertItem(pKFMD->name, 0, 0, TVI_ROOT, TVI_LAST);
			m_sti[i].nMotion = pKFMD->nKungfuMotionData;
			i++;
		}
	}
}

KungfuMotionData * DoubleScreenDialog::GetKFMD(unsigned int nMotion)
{
	KungfuMotionData *pKFMD;

	for(pKFMD = m_pKFMD_Head; pKFMD != NULL; pKFMD = pKFMD->pNext)
	{
		if(pKFMD->nKungfuMotionData == m_sti[nMotion].nMotion)
			return pKFMD;
	}

	return NULL;
}

void DoubleScreenDialog::SwitchToJointSelection()
{
	if(m_DialogType == TRANSITING_DLG)
	{
		CString str;

		m_S1_StartFrameSlider.EnableWindow(TRUE);
		m_S2_StartFrameSlider.EnableWindow(TRUE);
		m_S1_EndFrameSlider.EnableWindow(TRUE);
		m_S2_EndFrameSlider.EnableWindow(TRUE);
		m_SimValSlider.EnableWindow(TRUE);

		m_SimValSlider.SetRange(1, 100, FALSE);
		m_SimValSlider.SetPos(1);

		// Start Frame Sliders
		m_S1_StartFrameSlider.SetRange(0, m_pMotionData1->MotionBuffer.numFrames, FALSE);
		m_S2_StartFrameSlider.SetRange(0, m_pMotionData2->MotionBuffer.numFrames, FALSE);
		m_S1_StartFrameSlider.SetPos(0);
		m_S2_StartFrameSlider.SetPos(0);
		m_S1_StartFrame = 0;
		m_S2_StartFrame = 0;

		m_S1_StartFrameStatic.SetWindowText("0");
		m_S2_StartFrameStatic.SetWindowText("0");

		// End Frame Sliders.
		m_S1_EndFrameSlider.SetRange(0, m_pMotionData1->MotionBuffer.numFrames, FALSE);
		m_S2_EndFrameSlider.SetRange(0, m_pMotionData2->MotionBuffer.numFrames, FALSE);
		m_S1_EndFrameSlider.SetPos(m_pMotionData1->MotionBuffer.numFrames);
		m_S2_EndFrameSlider.SetPos(m_pMotionData2->MotionBuffer.numFrames);
		m_S1_EndFrame = m_pMotionData1->MotionBuffer.numFrames;
		m_S2_EndFrame = m_pMotionData2->MotionBuffer.numFrames;	

		str.Format("%u", m_pMotionData1->MotionBuffer.numFrames);
		m_S1_EndFrameStatic.SetWindowText(str);
		str.Format("%u", m_pMotionData2->MotionBuffer.numFrames);
		m_S2_EndFrameStatic.SetWindowText(str);

		m_mpScreen1.SetPointer(&m_pMotionData1->MotionBuffer);
		m_mpScreen1.SetPointer(this);
		m_mpScreen1.Invalidate(FALSE);

		m_mpScreen2.SetPointer(&m_pMotionData2->MotionBuffer);
		m_mpScreen2.SetPointer(this);
		m_mpScreen2.Invalidate(FALSE);

		m_MarkerTree.EnableWindow(FALSE);
		m_SelectedMarkerList.EnableWindow(FALSE);
	}

	if(m_DialogType == BLENDING_DLG || m_DialogType == EXAGGERATING_DLG || m_DialogType == REPLACING_DLG)
	{
		CString str;
		unsigned int i;

		m_mti = new MarkerToItem[m_pMotionData1->MotionBuffer.numMarkers];
		// Insert markers to tree.
		ResetMarkerTree();

		m_S1_StartFrameSlider.EnableWindow(TRUE);
		m_S2_StartFrameSlider.EnableWindow(TRUE);
		m_S1_EndFrameSlider.EnableWindow(TRUE);
		m_S2_EndFrameSlider.EnableWindow(TRUE);

		// Start Frame Sliders
		m_S1_StartFrameSlider.SetRange(0, m_pMotionData1->MotionBuffer.numFrames, FALSE);
		m_S2_StartFrameSlider.SetRange(0, m_pMotionData2->MotionBuffer.numFrames, FALSE);
		m_S1_StartFrameSlider.SetPos(0);
		m_S2_StartFrameSlider.SetPos(0);
		m_S1_StartFrame = 0;
		m_S2_StartFrame = 0;

		m_S1_StartFrameStatic.SetWindowText("0");
		m_S2_StartFrameStatic.SetWindowText("0");

		// End Frame Sliders.
		m_S1_EndFrameSlider.SetRange(0, m_pMotionData1->MotionBuffer.numFrames, FALSE);
		m_S2_EndFrameSlider.SetRange(0, m_pMotionData2->MotionBuffer.numFrames, FALSE);
		m_S1_EndFrameSlider.SetPos(m_pMotionData1->MotionBuffer.numFrames);
		m_S2_EndFrameSlider.SetPos(m_pMotionData2->MotionBuffer.numFrames);
		m_S1_EndFrame = m_pMotionData1->MotionBuffer.numFrames;
		m_S2_EndFrame = m_pMotionData2->MotionBuffer.numFrames;	

		str.Format("%u", m_pMotionData1->MotionBuffer.numFrames);
		m_S1_EndFrameStatic.SetWindowText(str);
		str.Format("%u", m_pMotionData2->MotionBuffer.numFrames);
		m_S2_EndFrameStatic.SetWindowText(str);

		// Marker Selection Array
		m_pMarkerSelectionArray = new bool[m_pMotionData1->GetNumMarkers()];
		for(i = 0; i < m_pMotionData1->GetNumMarkers(); i++)
			m_pMarkerSelectionArray[i] = false;

		// Similarity values for all separated joints.
		m_pMBInfo->pSimVal = new float[m_pMotionData1->GetNumMarkers()];
		for(i = 0; i < m_pMotionData1->GetNumMarkers(); i++)
			m_pMBInfo->pSimVal[i] = 0.0f;

		m_SelectedMarkerList.DeleteAllItems();
		m_SelectedMarkerList.DeleteColumn(0);

		LV_COLUMN lvColumn;
		lvColumn.mask = LVCF_WIDTH | LVCF_TEXT | LVCF_FMT | LVCF_SUBITEM;
		lvColumn.fmt = LVCFMT_CENTER;
		lvColumn.cx = 200;
		lvColumn.iSubItem = 0;
		lvColumn.pszText = "Selected Joint(s)";
		m_SelectedMarkerList.InsertColumn(0, &lvColumn);

		m_mpScreen1.SetPointer(&m_pMotionData1->MotionBuffer);
		m_mpScreen1.SetPointer(this);
		m_mpScreen1.Invalidate(FALSE);

		m_mpScreen2.SetPointer(&m_pMotionData2->MotionBuffer);
		m_mpScreen2.SetPointer(this);
		m_mpScreen2.Invalidate(FALSE);

		// Button
		m_SelectJointButton.EnableWindow(TRUE);
		m_SelectionAllButton.EnableWindow(TRUE);
		m_DeselectJointButton.EnableWindow(FALSE);
		m_DeselectionAllButton.EnableWindow(FALSE);
		m_SketelonButton.EnableWindow(FALSE);
		m_SimValSlider.EnableWindow(FALSE);
		m_OkButton.EnableWindow(TRUE);

		m_IsMotionSelectionMode = false;
		m_IsJointSelectionMode = true;
	}

}

void DoubleScreenDialog::OnChangeMotionnameTextbox() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	char text[256];
	CString str;

	m_MotionName.GetWindowText(text, 256);
	str.Format("%s", text);

	if(str.GetLength() > 0)
		m_OkButton.EnableWindow(TRUE);
	else
		m_OkButton.EnableWindow(FALSE);
}
