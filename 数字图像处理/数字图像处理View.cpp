// 数字图像处理View.cpp : implementation of the CMyView class
//

#include "stdafx.h"
#include "数字图像处理.h"

#include "数字图像处理Doc.h"
#include "数字图像处理View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMyView

IMPLEMENT_DYNCREATE(CMyView, CScrollView)

BEGIN_MESSAGE_MAP(CMyView, CScrollView)
	//{{AFX_MSG_MAP(CMyView)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyView construction/destruction

CMyView::CMyView()
{
	// TODO: add construction code here

}

CMyView::~CMyView()
{
}

BOOL CMyView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CScrollView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CMyView drawing
extern BITMAPINFO *lpBitsInfo;
void CMyView::OnDraw(CDC* pDC)
{
	CMyDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	// TODO: add draw code for native data here

	if(NULL == lpBitsInfo)
		return;

	LPVOID lpBits = (LPVOID)&lpBitsInfo->bmiColors[lpBitsInfo->bmiHeader.biClrUsed];
	StretchDIBits(
		pDC->GetSafeHdc(),
		0,0,
		lpBitsInfo->bmiHeader.biWidth,
		lpBitsInfo->bmiHeader.biHeight,
		0,0,
		lpBitsInfo->bmiHeader.biWidth,
		lpBitsInfo->bmiHeader.biHeight,
		lpBits,
		lpBitsInfo,
		DIB_RGB_COLORS,
		SRCCOPY);
}

void CMyView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	CSize sizeTotal;
	// TODO: calculate the total size of this view
	sizeTotal.cx = sizeTotal.cy = 100;
	SetScrollSizes(MM_TEXT, sizeTotal);
}

/////////////////////////////////////////////////////////////////////////////
// CMyView diagnostics

#ifdef _DEBUG
void CMyView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CMyView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

CMyDoc* CMyView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMyDoc)));
	return (CMyDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMyView message handlers
