// SnapShotDialog.cpp : implementation file
//

#include "stdafx.h"
#include "KungfuMotion.h"
#include "SnapShotDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// SnapShotDialog dialog


SnapShotDialog::SnapShotDialog(CWnd* pParent /*=NULL*/)
	: CDialog(SnapShotDialog::IDD, pParent)
{
	m_pKFMD = NULL;
	m_pMotionData = NULL;
	m_pCurrentKFMD = NULL;
	m_sti = NULL;
	m_numKFMD = 0;
	m_MaxFrame = 0;
	m_hCurrentItem = NULL;

	//{{AFX_DATA_INIT(SnapShotDialog)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void SnapShotDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(SnapShotDialog)
	DDX_Control(pDX, IDC_MARKERSIZE_SNAPSHOT_SLIDER, m_MarkerSizeSlider);
	DDX_Control(pDX, IDC_FRAME_SNAPSHOT_SLIDER, m_FrameSlider);
	DDX_Control(pDX, IDC_TIME_SNAPSHOT_TEXTBOX, m_TimeTextBox);
	DDX_Control(pDX, IDC_SHOW_SNAPSHOT_BUTTON, m_ShowButton);
	DDX_Control(pDX, IDC_SAVE_SNAPSHOT_BUTTON, m_SaveAsButton);
	DDX_Control(pDX, IDC_MOTIONTREE_SNAPSHOT_TREE, m_MotionTree);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(SnapShotDialog, CDialog)
	//{{AFX_MSG_MAP(SnapShotDialog)
	ON_EN_CHANGE(IDC_TIME_SNAPSHOT_TEXTBOX, OnChangeTimeSnapshotTextbox)
	ON_WM_HSCROLL()
	ON_NOTIFY(TVN_SELCHANGED, IDC_MOTIONTREE_SNAPSHOT_TREE, OnSelchangedMotiontreeSnapshotTree)
	ON_BN_CLICKED(IDC_SHOW_SNAPSHOT_BUTTON, OnShowSnapshotButton)
	ON_BN_CLICKED(IDC_SAVE_SNAPSHOT_BUTTON, OnSaveSnapshotButton)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// SnapShotDialog message handlers

void SnapShotDialog::SetPointer(MotionData *ptr)
{
	m_pMotionData = ptr;
}

BOOL SnapShotDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
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

	m_MotionTree.SetImageList(&m_ImageList, LVSIL_SMALL);

	m_sti = new MotionToItem[m_numKFMD];

	m_MotionTree.DeleteAllItems();
	KungfuMotionData *pKFMD;
	unsigned int i = 0;
	for(pKFMD = m_pKFMD; pKFMD != NULL; pKFMD = pKFMD->pNext)
	{
		m_sti[i].nMotion = pKFMD->nKungfuMotionData;
		m_sti[i].tItem = m_MotionTree.InsertItem(pKFMD->name, 0, 0, TVI_ROOT, TVI_LAST);
		i++;
	}

	m_pCurrentKFMD = m_pKFMD;
	m_MaxFrame = m_pCurrentKFMD->MotionBuffer.numFrames;

	// TODO: Add extra initialization here
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
	wndClass.lpszClassName = "SnapShotScreen";

	// Register the window class
	AfxRegisterClass(&wndClass);
	
	CRect glRect, newRect;
	GetDlgItem(IDC_SNAPSHOT_BORDER)->GetWindowRect(&glRect);

	newRect.top = glRect.top - 80;
	newRect.bottom = newRect.top + 256;
	//newRect.bottom = glRect.bottom - 80;
	newRect.left = glRect.left;
	newRect.right = glRect.left + 256;
	//newRect.right = glRect.right;

	// Create motion preview window.
	m_mpScreen.Create("SnapShotScreen", NULL, WS_CHILD | WS_VISIBLE, newRect, this, ID_SNAPSHOT_SCREEN, NULL);
	m_mpScreen.SetPointer(&m_pKFMD->MotionBuffer);
	m_mpScreen.SetPointer(this);
	//m_mpScreen.m_time = 0.0f;
	m_mpScreen.m_Frame = 0;
	//m_mpScreen.m_IsByCurve = true;
	m_mpScreen.Invalidate(FALSE);
	
	m_FrameSlider.SetRange(1, m_pKFMD->MotionBuffer.numFrames, FALSE);
	m_FrameSlider.SetPos(1);

	m_MarkerSizeSlider.SetRange(1, 100, FALSE);
	m_MarkerSizeSlider.SetPos(10);

	m_TimeTextBox.SetWindowText("1");

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void SnapShotDialog::OnChangeTimeSnapshotTextbox() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	char text[256];
	float frame;

	m_TimeTextBox.GetWindowText(text, 256);
	frame = atoi(text) - 1;

	if(frame < 0 || frame > m_MaxFrame - 1)
		m_ShowButton.EnableWindow(FALSE);
	else
		m_ShowButton.EnableWindow(TRUE);
}

void SnapShotDialog::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	// TODO: Add your message handler code here and/or call default
	int var;
	CString str;

	if((CSliderCtrl *)pScrollBar == &m_FrameSlider)
	{
		var = m_FrameSlider.GetPos() - 1;
		str.Format("%d", var + 1);
		m_TimeTextBox.SetWindowText(str);

		m_mpScreen.m_Frame = var;
		m_mpScreen.Invalidate(FALSE);
	}	

	if((CSliderCtrl *)pScrollBar == &m_MarkerSizeSlider)
	{
		var = m_MarkerSizeSlider.GetPos() - 1;

		m_mpScreen.m_MarkerSize = (float)var * 0.1f;
		m_mpScreen.Invalidate(FALSE);
	}

	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}

void SnapShotDialog::SetPointer(KungfuMotionData *ptr, unsigned int numKFMD)
{
	m_pKFMD = ptr;
	m_numKFMD = numKFMD;
}

BOOL SnapShotDialog::DestroyWindow() 
{
	// TODO: Add your specialized code here and/or call the base class
	if(m_sti != NULL)
		delete[] m_sti;

	return CDialog::DestroyWindow();
}

void SnapShotDialog::OnSelchangedMotiontreeSnapshotTree(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	unsigned int var;
	CString str;

	m_hCurrentItem = pNMTreeView->itemNew.hItem;
	m_pCurrentKFMD = GetKFMD(GetCurrentMotionIndex_Tree());

	m_MaxFrame = m_pCurrentKFMD->MotionBuffer.numFrames;
	var = m_FrameSlider.GetPos();
	m_FrameSlider.SetRange(1, m_MaxFrame, TRUE);
	if(var > m_MaxFrame)
		var = m_MaxFrame;

	m_FrameSlider.SetPos(var);

	str.Format("%d", var);
	m_TimeTextBox.SetWindowText(str);

	m_mpScreen.ChangePointer(&m_pCurrentKFMD->MotionBuffer);
	m_mpScreen.m_Frame = var - 1;
	m_mpScreen.Invalidate(FALSE);

	*pResult = 0;
}

unsigned int SnapShotDialog::GetCurrentMotionIndex_Tree()
{
	unsigned int i;

	for(i = 0; i < m_numKFMD; i++)
	{
		if(m_sti[i].tItem == m_hCurrentItem)
			return i;
	}

	return 0;
}

void SnapShotDialog::OnShowSnapshotButton() 
{
	// TODO: Add your control notification handler code here
	char text[256];
	float frame;

	m_TimeTextBox.GetWindowText(text, 256);
	frame = atoi(text);

	m_FrameSlider.SetPos(frame);

	//m_mpScreen.m_time = time;
	m_mpScreen.m_Frame = frame - 1;
	m_mpScreen.Invalidate(FALSE);	
}

KungfuMotionData * SnapShotDialog::GetKFMD(unsigned int nMotion)
{
	KungfuMotionData *pKFMD;

	for(pKFMD = m_pKFMD; pKFMD != NULL; pKFMD = pKFMD->pNext)
	{
		if(pKFMD->nKungfuMotionData == m_sti[nMotion].nMotion)
			return pKFMD;
	}

	return NULL;
}

void SnapShotDialog::OnSaveSnapshotButton() 
{
	// TODO: Add your control notification handler code here
	static char BASED_CODE szFilter[] = "BMP file (*.bmp)|*.bmp||";
	CFileDialog fileDialog(FALSE, "bmp", NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter, AfxGetMainWnd());

	//m_mpScreen.GetFrameBuffer();
	
	CClientDC clientDC(this);
	
	int i, horz, vert, offset_x, offset_y;
	CRect glRect, DlgRect;
	GLubyte m_FrameBuffer[65535 * 3 + 1];

	//GetDlgItem(IDC_SNAPSHOT_BORDER)->GetClientRect(&glRect);
	m_mpScreen.GetWindowRect(&glRect);
	GetWindowRect(&DlgRect);

	offset_x = glRect.left - DlgRect.left;
	offset_y = glRect.top - DlgRect.top - 30;

	for(horz = 0; horz < 255; horz++)
	{
		for(vert = 0; vert < 255; vert++)
		{
			i = (256 * 3 * vert) + horz * 3 + 1;
			m_FrameBuffer[i - 1] = clientDC.GetPixel(horz + offset_x, 255 - vert + offset_y);
			m_FrameBuffer[i] = clientDC.GetPixel(horz + offset_x, 255 - vert + offset_y);
			m_FrameBuffer[i + 1] = clientDC.GetPixel(horz + offset_x, 255 - vert + offset_y);
		}
	}

	if(fileDialog.DoModal() == IDOK)
	{
		BITMAPINFO *bmpinfo = new BITMAPINFO;

		bmpinfo->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
		bmpinfo->bmiHeader.biWidth = 256;
		bmpinfo->bmiHeader.biHeight = 256;
		bmpinfo->bmiHeader.biPlanes = 1;
		bmpinfo->bmiHeader.biBitCount = 24;
		bmpinfo->bmiHeader.biCompression = BI_RGB;
		bmpinfo->bmiHeader.biSizeImage = 0;
		bmpinfo->bmiHeader.biXPelsPerMeter = 2952;
		bmpinfo->bmiHeader.biYPelsPerMeter = 2952;
		bmpinfo->bmiHeader.biClrUsed = 0;
		bmpinfo->bmiHeader.biClrImportant = 0;
		
		if(SaveDIBitmap(fileDialog.GetPathName(), bmpinfo, m_FrameBuffer) == 0)
			MessageBox("The file was sucessfully saved.", "Saving Status", MB_ICONINFORMATION | MB_SETFOREGROUND);
		else
			MessageBox("Unable to save the file.", "Saving Status", MB_ICONERROR | MB_OK);

		delete bmpinfo;
	};
}

int SnapShotDialog::SaveDIBitmap(const char *filename, BITMAPINFO *info, GLubyte *bits)
{
    CFile myFile;
    int              size,         //Size of file
                     infosize,     //Size of bitmap info
                     bitsize;      //Size of bitmap pixels
    BITMAPFILEHEADER header;       //File header

	
    if (myFile.Open(filename, CFile::modeCreate | CFile::modeWrite) == 0)
		return (-1);

    //Figure out bitmap size.
    if (info->bmiHeader.biSizeImage == 0)
	bitsize = (info->bmiHeader.biWidth *
        	   info->bmiHeader.biBitCount + 7) / 8 *
		  abs(info->bmiHeader.biHeight);
    else
	bitsize = info->bmiHeader.biSizeImage;

    //Figure out header size.
    infosize = sizeof(BITMAPINFOHEADER);
    switch (info->bmiHeader.biCompression)
	{
	case BI_BITFIELDS :
            infosize += 12;  //Add 3 RGB doubleword masks.
            if (info->bmiHeader.biClrUsed == 0)
	      break;
	case BI_RGB :
            if (info->bmiHeader.biBitCount > 8 &&
        	info->bmiHeader.biClrUsed == 0)
	      break;
	case BI_RLE8 :
	case BI_RLE4 :
            if (info->bmiHeader.biClrUsed == 0)
              infosize += (1 << info->bmiHeader.biBitCount) * 4;
	    else
              infosize += info->bmiHeader.biClrUsed * 4;
	    break;
	}

    size = sizeof(BITMAPFILEHEADER) + infosize + bitsize;

    // Write file header, bitmap information, and bitmap pixel data.
    header.bfType      = 'MB';
    header.bfSize      = size;
    header.bfReserved1 = 0;
    header.bfReserved2 = 0;
    header.bfOffBits   = sizeof(BITMAPFILEHEADER) + infosize;

	myFile.Write(&header, sizeof(BITMAPFILEHEADER));
	myFile.Write(info, infosize);
	myFile.Write(bits, bitsize);

    myFile.Close();
    return (0);
}