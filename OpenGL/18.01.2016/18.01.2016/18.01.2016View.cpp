#pragma region fiksno
#include "pch.h"
#include "framework.h"
#ifndef SHARED_HANDLERS
#include "18.01.2016.h"
#endif
#include "18.01.2016Doc.h"
#include "18.01.2016View.h"
#include "GLRenderer.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNCREATE(CMy18012016View, CView)

BEGIN_MESSAGE_MAP(CMy18012016View, CView)
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_DESTROY()
	ON_WM_ERASEBKGND()
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()


CMy18012016View::CMy18012016View() noexcept
{
}
CMy18012016View::~CMy18012016View()
{
}
BOOL CMy18012016View::PreCreateWindow(CREATESTRUCT& cs)
{
	return CView::PreCreateWindow(cs);
}
#pragma endregion

void CMy18012016View::OnDraw(CDC* pDC)
{
	CMy18012016Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	m_glRenderer.DrawScene(pDC);
}

#pragma region ....
BOOL CMy18012016View::OnPreparePrinting(CPrintInfo* pInfo)
{
	return DoPreparePrinting(pInfo);
}
void CMy18012016View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
}
void CMy18012016View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
}

#ifdef _DEBUG
void CMy18012016View::AssertValid() const
{
	CView::AssertValid();
}

void CMy18012016View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CMy18012016Doc* CMy18012016View::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMy18012016Doc)));
	return (CMy18012016Doc*)m_pDocument;
}
#endif //_DEBUG
#pragma endregion
#pragma region handlers
int CMy18012016View::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	CDC* pDC = GetDC();
	m_glRenderer.CreateGLContext(pDC);
	ReleaseDC(pDC);

	return 0;
}
void CMy18012016View::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	CDC* pDC = GetDC();
	m_glRenderer.Reshape(pDC, cx, cy);
	ReleaseDC(pDC);
}
void CMy18012016View::OnDestroy()
{
	CView::OnDestroy();

	CDC* pDC = GetDC();
	m_glRenderer.DestroyScene(pDC);
	ReleaseDC(pDC);
}
BOOL CMy18012016View::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
}
void CMy18012016View::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	CDC* pDC = GetDC();
	m_glRenderer.PrepareScene(pDC);
	ReleaseDC(pDC);
}
void CMy18012016View::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: Add your message handler code here and/or call default
	if (nChar == VK_RIGHT)
		m_glRenderer.alfa -= 5;
	if (nChar == VK_LEFT)
		m_glRenderer.alfa += 5;
	if (nChar == VK_UP)
		m_glRenderer.beta -= 5;
	if (nChar == VK_DOWN)
		m_glRenderer.beta += 5;
	if (nChar == 'Q')
		m_glRenderer.windRotation -= 5.0;
	Invalidate();
	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}
#pragma endregion
