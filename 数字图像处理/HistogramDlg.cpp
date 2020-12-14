// HistogramDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Êý×ÖÍ¼Ïñ´¦Àí.h"
#include "HistogramDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// HistogramDlg dialog


HistogramDlg::HistogramDlg(CWnd* pParent /*=NULL*/)
	: CDialog(HistogramDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(HistogramDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void HistogramDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(HistogramDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(HistogramDlg, CDialog)
	//{{AFX_MSG_MAP(HistogramDlg)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// HistogramDlg message handlers
extern DWORD H[256];
void Histogram();
void HistogramDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	int i;
	DWORD max;

	dc.Rectangle(20,20,277,221);

	max = 0;
	for(i=0;i<256;i++)
	{
		if(H[i]>max)
			max = H[i];
	}

	for(i = 0;i<256;i++)
	{
		dc.MoveTo(i+20,220);
		dc.LineTo(i+20,220 - (int)(H[i] *200/max));
	}
	// Do not call CDialog::OnPaint() for painting messages
}

BOOL HistogramDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	Histogram();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
