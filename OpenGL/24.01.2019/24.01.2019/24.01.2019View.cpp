#pragma region fiksno
#include "pch.h"
#include "framework.h"
#ifndef SHARED_HANDLERS
#include "24.01.2019.h"
#endif
#include "24.01.2019Doc.h"
#include "24.01.2019View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNCREATE(CMy24012019View, CView)

BEGIN_MESSAGE_MAP(CMy24012019View, CView)
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_DESTROY()
	ON_WM_ERASEBKGND()
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()

CMy24012019View::CMy24012019View() noexcept
{
}
CMy24012019View::~CMy24012019View()
{
}
BOOL CMy24012019View::PreCreateWindow(CREATESTRUCT& cs)
{
	return CView::PreCreateWindow(cs);
}
#pragma endregion

void CMy24012019View::OnDraw(CDC* pDC)
{
	CMy24012019Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	m_glRenderer.DrawScene(pDC);

}
#pragma region ....
BOOL CMy24012019View::OnPreparePrinting(CPrintInfo* pInfo)
{
	return DoPreparePrinting(pInfo);
}
void CMy24012019View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
}
void CMy24012019View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
}
#ifdef _DEBUG
void CMy24012019View::AssertValid() const
{
	CView::AssertValid();
}
void CMy24012019View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
CMy24012019Doc* CMy24012019View::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMy24012019Doc)));
	return (CMy24012019Doc*)m_pDocument;
}
#endif //_DEBUG
#pragma endregion
#pragma region handlers
int CMy24012019View::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	CDC* pDC = GetDC();
	m_glRenderer.CreateGLContext(pDC);
	ReleaseDC(pDC);

	return 0;
}
void CMy24012019View::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	CDC* pDC = GetDC();
	m_glRenderer.Reshape(pDC, cx, cy);
	ReleaseDC(pDC);
}
void CMy24012019View::OnDestroy()
{
	CView::OnDestroy();

	CDC* pDC = GetDC();
	m_glRenderer.DestroyScene(pDC);
	ReleaseDC(pDC);
}
BOOL CMy24012019View::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
}
void CMy24012019View::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	CDC* pDC = GetDC();
	m_glRenderer.PrepareScene(pDC);
	ReleaseDC(pDC);
}
void CMy24012019View::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	//pogled
	if (nChar == VK_RIGHT) m_glRenderer.ugaoX += 0.1;
	if (nChar == VK_LEFT) m_glRenderer.ugaoX -= 0.1;
	if (nChar == VK_UP) m_glRenderer.ugaoY += 0.1;
	if (nChar == VK_DOWN) m_glRenderer.ugaoY -= 0.1;
	if (nChar == VK_ADD) {
		m_glRenderer.r += 1;
	}
	if (nChar == VK_SUBTRACT) {
		m_glRenderer.r -= 1;
	}
	//nakon pomeranja ,izracuna se vrednost pozicije kamere odakle gledamo
	m_glRenderer.xEye = sin(m_glRenderer.ugaoY) * sin(m_glRenderer.ugaoX) * m_glRenderer.r;
	m_glRenderer.yEye = cos(m_glRenderer.ugaoY) * m_glRenderer.r;
	m_glRenderer.zEye = sin(m_glRenderer.ugaoY) * cos(m_glRenderer.ugaoX) * m_glRenderer.r;
	//rotacije za bager
	if (nChar == '1') m_glRenderer.angle1 += 5;
	if (nChar == '2') m_glRenderer.angle1 -= 5;
	if (nChar == '3') m_glRenderer.angle2 += 5;
	if (nChar == '4') m_glRenderer.angle2 -= 5;
	if (nChar == '5') m_glRenderer.angle3 += 5;
	if (nChar == '6') m_glRenderer.angle3 -= 5;
	if (nChar == '7') m_glRenderer.angle4 += 5;
	if (nChar == '8') m_glRenderer.angle4 -= 5;
	Invalidate();
	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}
#pragma endregion
