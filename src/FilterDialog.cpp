// FilterDialog.cpp : implementation file
//

#include "stdafx.h"
#include "KungfuMotion.h"
#include "FilterDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// FilterDialog dialog


FilterDialog::FilterDialog(CWnd* pParent /*=NULL*/)
	: CDialog(FilterDialog::IDD, pParent)
{
	m_pKFMD = NULL;
	m_pMK = NULL;
	m_nDOF = 0;
	m_nMarker = 0;
	m_tol = 0.0f;
	m_IsIgnoreSign = false;
	//{{AFX_DATA_INIT(FilterDialog)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void FilterDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(FilterDialog)
	DDX_Control(pDX, IDC_MARKERNAMESTATIC, m_MarkerNameStatic);
	DDX_Control(pDX, IDC_MOTIONNAME_FILTER_STATIC, m_MotionNameStatic);
	DDX_Control(pDX, IDC_CURVENAME_FILTER_STATIC, m_CurveNameStatic);
	DDX_Control(pDX, IDOK, m_OkButton);
	DDX_Control(pDX, IDC_TOLERANCE_FILTER_EDIT, m_ToleranceTextBox);
	DDX_Control(pDX, IDC_IGNORESIGN_FILTER_CHECK, m_IgnoreSignCheck);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(FilterDialog, CDialog)
	//{{AFX_MSG_MAP(FilterDialog)
	ON_EN_CHANGE(IDC_TOLERANCE_FILTER_EDIT, OnChangeToleranceFilterEdit)
	ON_BN_CLICKED(IDC_IGNORESIGN_FILTER_CHECK, OnIgnoresignFilterCheck)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// FilterDialog message handlers

void FilterDialog::SetPointer(KungfuMotionData *pKFMD, unsigned int nMarker, unsigned int nDOF)
{
	m_pKFMD = pKFMD;
	m_nDOF = nDOF;
	m_nMarker = nMarker;
}

BOOL FilterDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if(m_pKFMD != NULL)
	{
		CString str;
		float var;

		m_pMK = m_pKFMD->GetMarker(m_nMarker);

		m_MotionNameStatic.SetWindowText(m_pKFMD->name);
		m_MarkerNameStatic.SetWindowText(m_pMK->name);
		m_CurveNameStatic.SetWindowText(m_pMK->CurveName[m_nDOF].name);
	
		var = (float)(fabs(m_pKFMD->MotionBuffer.GetMaxAmp(m_nMarker, m_nDOF)) + fabs(m_pKFMD->MotionBuffer.GetMinAmp(m_nMarker, m_nDOF)));
		m_tol = var * 0.25f;
		str.Format("%0.3f", m_tol);
		m_ToleranceTextBox.SetWindowText(str);
	}
	else
		return FALSE;

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void FilterDialog::OnOK() 
{
	// TODO: Add extra validation here
	MotionEditing ME;

	ME.FlipFilter(m_pKFMD->MotionBuffer.GetPathPointSet(m_nMarker, m_nDOF), m_tol, m_IsIgnoreSign);
	m_pKFMD->MotionBuffer.RenewMaxAndMinPPAmp(m_nMarker, m_nDOF);

	CDialog::OnOK();
}

void FilterDialog::OnChangeToleranceFilterEdit() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	char text[256];

	m_ToleranceTextBox.GetWindowText(text, 256);
	
	if(atof(text) <= 0.0)
		m_OkButton.EnableWindow(FALSE);
	else
		m_OkButton.EnableWindow(TRUE);	
}

void FilterDialog::OnIgnoresignFilterCheck() 
{
	// TODO: Add your control notification handler code here
	if(m_IgnoreSignCheck.GetState() == 0)
		m_IsIgnoreSign = false;
	else
		m_IsIgnoreSign = true;
}
