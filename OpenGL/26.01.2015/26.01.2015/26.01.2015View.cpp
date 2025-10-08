#pragma region   fiksno
#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "26.01.2015.h"
#endif
#include "26.01.2015Doc.h"
#include "26.01.2015View.h"
#include "GLRenderer.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNCREATE(CMy26012015View, CView)

BEGIN_MESSAGE_MAP(CMy26012015View, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_DESTROY()
	ON_WM_ERASEBKGND()
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()

CMy26012015View::CMy26012015View() noexcept
{
}
CMy26012015View::~CMy26012015View()
{
}
BOOL CMy26012015View::PreCreateWindow(CREATESTRUCT& cs)
{
	return CView::PreCreateWindow(cs);
}
#pragma endregion

void CMy26012015View::OnDraw(CDC* pDC)
{
	CMy26012015Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	m_glRenderer.DrawScene(pDC);
}
#pragma region ....
BOOL CMy26012015View::OnPreparePrinting(CPrintInfo* pInfo)
{
	return DoPreparePrinting(pInfo);
}
void CMy26012015View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
}
void CMy26012015View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
}

#ifdef _DEBUG
void CMy26012015View::AssertValid() const
{
	CView::AssertValid();
}
void CMy26012015View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
CMy26012015Doc* CMy26012015View::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMy26012015Doc)));
	return (CMy26012015Doc*)m_pDocument;
}
#endif //_DEBUG
#pragma endregion
#pragma region Handlers
int CMy26012015View::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	CDC* pDC = GetDC();
	m_glRenderer.CreateGLContext(pDC);
	ReleaseDC(pDC);

	return 0;
}
void CMy26012015View::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	CDC* pDC = GetDC();
	m_glRenderer.Reshape(pDC, cx, cy);
	ReleaseDC(pDC);
}
void CMy26012015View::OnDestroy()
{
	CView::OnDestroy();

	CDC* pDC = GetDC();
	m_glRenderer.DestroyScene(pDC);
	ReleaseDC(pDC);
}
BOOL CMy26012015View::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
}
void CMy26012015View::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	CDC* pDC = GetDC();
	m_glRenderer.PrepareScene(pDC);
	ReleaseDC(pDC);
}
void CMy26012015View::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	switch (nChar)
	{
	case VK_LEFT:m_glRenderer.rot1 -= 5.0; break;
	case VK_RIGHT:m_glRenderer.rot1 += 5.0; break;
	case VK_UP:m_glRenderer.rot2 += 5.0; break;
	case VK_DOWN:m_glRenderer.rot2 -= 5.0; break;
	case 'S': m_glRenderer.lighting != m_glRenderer.lighting; break;
	//rotiranje zemlje oko njene ose:
	case 'M':m_glRenderer.dist -= 5.0; break; //minus
	case 'P':m_glRenderer.dist += 5.0; break; //plus
	//rotiranje meseca oko njegove ose po Y
	case 'Z':m_glRenderer.rotMesecY += 5; break;
	case 'X':m_glRenderer.rotMesecY -= 5; break;
	case 'C':m_glRenderer.rotMesecX += 5; break;
	case 'V':m_glRenderer.rotMesecX -= 5; break;
	
	}
	CView::OnKeyDown(nChar, nRepCnt, nFlags);
	Invalidate();
}
#pragma endregion