#if !defined(AFX_HISTOGRAMDLG_H__20D29389_4477_4B3D_AABA_407BAEE4FEB2__INCLUDED_)
#define AFX_HISTOGRAMDLG_H__20D29389_4477_4B3D_AABA_407BAEE4FEB2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// HistogramDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// HistogramDlg dialog

class HistogramDlg : public CDialog
{
// Construction
public:
	HistogramDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(HistogramDlg)
	enum { IDD = IDD_DIALOG1 };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(HistogramDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(HistogramDlg)
	afx_msg void OnPaint();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HISTOGRAMDLG_H__20D29389_4477_4B3D_AABA_407BAEE4FEB2__INCLUDED_)
