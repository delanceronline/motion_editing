// MotionBlendingDialog.cpp : implementation file
//

#include "stdafx.h"
#include "KungfuMotion.h"
#include "MotionBlendingDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// MotionBlendingDialog dialog


MotionBlendingDialog::MotionBlendingDialog(CWnd* pParent /*=NULL*/)
	: CDialog(MotionBlendingDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(MotionBlendingDialog)
	m_MotionName = _T("");
	//}}AFX_DATA_INIT
}


void MotionBlendingDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(MotionBlendingDialog)
	DDX_Control(pDX, IDC_STARTFRAME_SLIDER, m_SFSlider);
	DDX_Control(pDX, IDC_ENDFRAME_SLIDER, m_EFSlider);
	DDX_Control(pDX, IDC_BLENDINGVALUE_SLIDER, m_BVSlider);
	DDX_Text(pDX, IDC_MOTIONNAME_TEXTBOX, m_MotionName);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(MotionBlendingDialog, CDialog)
	//{{AFX_MSG_MAP(MotionBlendingDialog)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// MotionBlendingDialog message handlers

int MotionBlendingDialog::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	
	return 0;
}

BOOL MotionBlendingDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_BVSlider.SetRange(0, 100, FALSE);
	m_BVSlider.SetPos(50);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void MotionBlendingDialog::OnOK() 
{
	// TODO: Add extra validation here
	m_BlendingValue = (float)m_BVSlider.GetPos() / 100.0f;

	CDialog::OnOK();
}
