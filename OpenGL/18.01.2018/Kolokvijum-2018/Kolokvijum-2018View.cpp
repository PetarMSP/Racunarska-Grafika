#pragma region fiksno
#include "pch.h"
#include "framework.h"
#ifndef SHARED_HANDLERS
#include "Kolokvijum-2018.h"
#endif
#include "Kolokvijum-2018Doc.h"
#include "Kolokvijum-2018View.h"
#include "GLRenderer.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNCREATE(CKolokvijum2018View, CView)

BEGIN_MESSAGE_MAP(CKolokvijum2018View, CView)
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_DESTROY()
	ON_WM_ERASEBKGND()
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()

CKolokvijum2018View::CKolokvijum2018View() noexcept
{
}
CKolokvijum2018View::~CKolokvijum2018View()
{
}
BOOL CKolokvijum2018View::PreCreateWindow(CREATESTRUCT& cs)
{
	return CView::PreCreateWindow(cs);
}
#pragma endregion

void CKolokvijum2018View::OnDraw(CDC* pDC)
{
	CKolokvijum2018Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	m_glRenderer.DrawScene(pDC);
}

#pragma region ....
BOOL CKolokvijum2018View::OnPreparePrinting(CPrintInfo* pInfo)
{
	return DoPreparePrinting(pInfo);
}
void CKolokvijum2018View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
}
void CKolokvijum2018View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
}
#ifdef _DEBUG
void CKolokvijum2018View::AssertValid() const
{
	CView::AssertValid();
}
void CKolokvijum2018View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
CKolokvijum2018Doc* CKolokvijum2018View::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CKolokvijum2018Doc)));
	return (CKolokvijum2018Doc*)m_pDocument;
}
#endif //_DEBUG
#pragma endregion

#pragma region Handlers
int CKolokvijum2018View::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	CDC* pDC = GetDC();
	m_glRenderer.CreateGLContext(pDC);
	ReleaseDC(pDC);

	return 0;
}
void CKolokvijum2018View::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	CDC* pDC = GetDC();
	m_glRenderer.Reshape(pDC, cx, cy);
	ReleaseDC(pDC);
}
void CKolokvijum2018View::OnDestroy()
{
	CView::OnDestroy();

	CDC* pDC = GetDC();
	m_glRenderer.DestroyScene(pDC);
	ReleaseDC(pDC);
}
BOOL CKolokvijum2018View::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
}
void CKolokvijum2018View::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	CDC* pDC = GetDC();
	m_glRenderer.PrepareScene(pDC);
	ReleaseDC(pDC);
}
void CKolokvijum2018View::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	switch (nChar)
	{
	case 'A': m_glRenderer.lookX -= 5.0; m_glRenderer.lookZ += 5; break;
	case 'S': m_glRenderer.lookY += 5.0; break;
	case 'W': m_glRenderer.lookY -= 5.0; break;
	case 'D': m_glRenderer.lookX += 5.0; m_glRenderer.lookZ -= 5.0; break;

	case VK_RIGHT: m_glRenderer.alfa -= 5.0; break;
	case VK_LEFT: m_glRenderer.alfa += 5.0; break;
	case VK_UP: m_glRenderer.beta -= 5.0; break;
	case VK_DOWN: m_glRenderer.beta += 5.0; break;
	case 'Z': m_glRenderer.distance /=1.1; break;
	case 'X': m_glRenderer.distance *= 1.1; break;
	case 'L': m_glRenderer.light != m_glRenderer.light; break;
	}
	Invalidate();
	CView::OnKeyDown(nChar, nRepCnt, nFlags);

}
#pragma endregion