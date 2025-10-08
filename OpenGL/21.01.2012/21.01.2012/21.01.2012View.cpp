#pragma region fiksno
#include "pch.h"
#include "framework.h"
#ifndef SHARED_HANDLERS
#include "21.01.2012.h"
#endif
#include "21.01.2012Doc.h"
#include "21.01.2012View.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNCREATE(CMy21012012View, CView)

BEGIN_MESSAGE_MAP(CMy21012012View, CView)
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_DESTROY()
	ON_WM_ERASEBKGND()
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()

CMy21012012View::CMy21012012View() noexcept
{
}
CMy21012012View::~CMy21012012View()
{
}
BOOL CMy21012012View::PreCreateWindow(CREATESTRUCT& cs)
{
	return CView::PreCreateWindow(cs);
}
#pragma endregion

void CMy21012012View::OnDraw(CDC* pDC)
{
	CMy21012012Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	m_glRenderer.DrawScene(pDC);
}
#pragma region ....
BOOL CMy21012012View::OnPreparePrinting(CPrintInfo* pInfo)
{
	return DoPreparePrinting(pInfo);
}
void CMy21012012View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
}
void CMy21012012View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
}
#ifdef _DEBUG
void CMy21012012View::AssertValid() const
{
	CView::AssertValid();
}

void CMy21012012View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CMy21012012Doc* CMy21012012View::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMy21012012Doc)));
	return (CMy21012012Doc*)m_pDocument;
}
#endif //_DEBUG
#pragma endregion
#pragma region handlers
int CMy21012012View::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	CDC* pDC = GetDC();
	m_glRenderer.CreateGLContext(pDC);
	ReleaseDC(pDC);

	return 0;
}
void CMy21012012View::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	CDC* pDC = GetDC();
	m_glRenderer.Reshape(pDC, cx, cy);
	ReleaseDC(pDC);
}
void CMy21012012View::OnDestroy()
{
	CView::OnDestroy();

	CDC* pDC = GetDC();
	m_glRenderer.DestroyScene(pDC);
	ReleaseDC(pDC);
}
BOOL CMy21012012View::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
}
void CMy21012012View::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	CDC* pDC = GetDC();
	m_glRenderer.PrepareScene(pDC);
	ReleaseDC(pDC);
}
void CMy21012012View::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: Add your message handler code here and/or call default
	if (nChar == VK_RIGHT)
		m_glRenderer.rotation -= 5.0;
	if (nChar == VK_LEFT)
		m_glRenderer.rotation += 5.0;
	Invalidate();
	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}
#pragma endregion
