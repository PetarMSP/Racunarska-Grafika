#pragma region fiksno
#include "pch.h"
#include "framework.h"
#ifndef SHARED_HANDLERS
#include "27.01.2011.h"
#endif
#include "27.01.2011Doc.h"
#include "27.01.2011View.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNCREATE(CMy27012011View, CView)
BEGIN_MESSAGE_MAP(CMy27012011View, CView)
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_DESTROY()
	ON_WM_ERASEBKGND()
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()
CMy27012011View::CMy27012011View() noexcept
{
}
CMy27012011View::~CMy27012011View()
{
}
BOOL CMy27012011View::PreCreateWindow(CREATESTRUCT& cs)
{
	return CView::PreCreateWindow(cs);
}
#pragma endregion
void CMy27012011View::OnDraw(CDC* pDC)
{
	CMy27012011Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	m_glRenderer.DrawScene(pDC);
}
#pragma region .....
BOOL CMy27012011View::OnPreparePrinting(CPrintInfo* pInfo)
{
	return DoPreparePrinting(pInfo);
}
void CMy27012011View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
}
void CMy27012011View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
}
#ifdef _DEBUG
void CMy27012011View::AssertValid() const
{
	CView::AssertValid();
}
void CMy27012011View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
CMy27012011Doc* CMy27012011View::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMy27012011Doc)));
	return (CMy27012011Doc*)m_pDocument;
}
#endif //_DEBUG
#pragma endregion
#pragma region handlers
int CMy27012011View::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	CDC* pDC = GetDC();
	m_glRenderer.CreateGLContext(pDC);
	ReleaseDC(pDC);

	return 0;
}
void CMy27012011View::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	CDC* pDC = GetDC();
	m_glRenderer.Reshape(pDC, cx, cy);
	ReleaseDC(pDC);
}


void CMy27012011View::OnDestroy()
{
	CView::OnDestroy();

	CDC* pDC = GetDC();
	m_glRenderer.DestroyScene(pDC);
	ReleaseDC(pDC);
}
BOOL CMy27012011View::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
}
void CMy27012011View::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	CDC* pDC = GetDC();
	m_glRenderer.PrepareScene(pDC);
	ReleaseDC(pDC);
}
void CMy27012011View::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (nChar == VK_RIGHT)
		m_glRenderer.viewAngle -= 5;
	if (nChar == VK_LEFT)
		m_glRenderer.viewAngle += 5;
	if (nChar == VK_UP)
		m_glRenderer.viewAngleY -= 5;
	if (nChar == VK_DOWN)
		m_glRenderer.viewAngleY += 5;
	if (nChar == 'S')
		m_glRenderer.light = !m_glRenderer.light;
	Invalidate();
	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}
#pragma endregion
