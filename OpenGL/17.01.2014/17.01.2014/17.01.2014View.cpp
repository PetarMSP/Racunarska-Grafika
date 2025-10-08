#pragma region fiksno
#include "pch.h"
#include "framework.h"
#ifndef SHARED_HANDLERS
#include "17.01.2014.h"
#endif
#include "17.01.2014Doc.h"
#include "17.01.2014View.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNCREATE(CMy17012014View, CView)

BEGIN_MESSAGE_MAP(CMy17012014View, CView)
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_DESTROY()
	ON_WM_ERASEBKGND()
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()

CMy17012014View::CMy17012014View() noexcept
{
}
CMy17012014View::~CMy17012014View()
{
}
BOOL CMy17012014View::PreCreateWindow(CREATESTRUCT& cs)
{
	return CView::PreCreateWindow(cs);
}
#pragma endregion

void CMy17012014View::OnDraw(CDC* pDC)
{
	CMy17012014Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	m_glRenderer.DrawScene(pDC);
}
#pragma region .....
BOOL CMy17012014View::OnPreparePrinting(CPrintInfo* pInfo)
{
	return DoPreparePrinting(pInfo);
}
void CMy17012014View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
}
void CMy17012014View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
}

#ifdef _DEBUG
void CMy17012014View::AssertValid() const
{
	CView::AssertValid();
}

void CMy17012014View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CMy17012014Doc* CMy17012014View::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMy17012014Doc)));
	return (CMy17012014Doc*)m_pDocument;
}
#endif //_DEBUG
#pragma endregion

#pragma region handlers
int CMy17012014View::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	CDC* pDC = GetDC();
	m_glRenderer.CreateGLContext(pDC);
	ReleaseDC(pDC);

	return 0;
}
void CMy17012014View::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	CDC* pDC = GetDC();
	m_glRenderer.Reshape(pDC, cx, cy);
	ReleaseDC(pDC);
}
void CMy17012014View::OnDestroy()
{
	CView::OnDestroy();

	CDC* pDC = GetDC();
	m_glRenderer.DestroyScene(pDC);
	ReleaseDC(pDC);
}
BOOL CMy17012014View::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
}
void CMy17012014View::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	CDC* pDC = GetDC();
	m_glRenderer.PrepareScene(pDC);
	ReleaseDC(pDC);
}
void CMy17012014View::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	switch (nChar)
	{
	case 'E': m_glRenderer.ugaoPosmatraca -= 5.0f;
		break;
	case 'Q': m_glRenderer.ugaoPosmatraca += 5.0f;
		break;
	case VK_NUMPAD1: m_glRenderer.ugao[0] += 5.0f;
		break;
	case VK_NUMPAD2: m_glRenderer.ugao[1] += 5.0f;
		break;
	case VK_NUMPAD3: m_glRenderer.ugao[2] += 5.0f;
		break;
	}
	Invalidate();

	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}
#pragma endregion
