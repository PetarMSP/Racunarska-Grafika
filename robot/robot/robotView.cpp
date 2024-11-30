
// robotView.cpp : implementation of the CrobotView class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "robot.h"
#endif

#include "robotDoc.h"
#include "robotView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define PI 3.14
#define toRad PI/180

// CrobotView

IMPLEMENT_DYNCREATE(CrobotView, CView)

BEGIN_MESSAGE_MAP(CrobotView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_ERASEBKGND()
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()

// CrobotView construction/destruction

CrobotView::CrobotView() noexcept
{
	// TODO: add construction code here
	nadlakticaRot = 0;
	podlakticaRot = 0;
	sakaRot = 0;
	roboRot = 0;
	roboScale = 1;
	glava = new DImage();
	nadkolenica = new DImage();
	nadlaktica = new DImage();
	podkolenica = new DImage();
	podlaktica = new DImage();
	saka = new DImage();
	stopalo = new DImage();
	telo = new DImage();
	pozadina = new DImage();
	glava->Load(CString("glava.png"));
	nadkolenica->Load(CString("nadkolenica.png"));
	nadlaktica->Load(CString("nadlaktica.png"));
	podkolenica->Load(CString("podkolenica.png"));
	podlaktica->Load(CString("podlaktica.png"));
	saka->Load(CString("saka.png"));
	stopalo->Load(CString("stopalo.png"));
	telo->Load(CString("telo.png"));
	pozadina->Load(CString("pozadina.jpg"));
}

CrobotView::~CrobotView()
{
	delete glava;
	delete nadkolenica;
	delete nadlaktica;
	delete podkolenica;
	delete podlaktica;
	delete saka;
	delete stopalo;
	delete telo;
	delete pozadina;
}

BOOL CrobotView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CrobotView drawing

void CrobotView::OnDraw(CDC* pDC)
{
	CrobotDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	// TODO: add draw code for native data here
	CRect rect;
	GetClientRect(&rect);
	CDC* memDC = new CDC();
	memDC->CreateCompatibleDC(pDC);
	CBitmap bm;
	bm.CreateCompatibleBitmap(pDC, rect.Width(), rect.Height());
	CBitmap* oldBitmap = memDC->SelectObject(&bm);
	memDC->SetGraphicsMode(GM_ADVANCED);
	XFORM transOld;
	memDC->GetWorldTransform(&transOld);
	//crtanje u memDC
	DrawRobot(memDC);
	pDC->BitBlt(0, 0, rect.Width(), rect.Height(), memDC, 0, 0, SRCCOPY);
	memDC->SelectObject(oldBitmap);
	memDC->SetWorldTransform(&transOld);
	delete memDC;
}

void CrobotView::DrawBackground(CDC* pDC)
{
	CRect rect;
	GetClientRect(&rect);
	pozadina->Draw(pDC, CRect(0, 0, pozadina->Width(), pozadina->Height()), CRect(0, 0, rect.Width(), rect.Height()));
}

void CrobotView::DrawImgTransparent(CDC* pDC, DImage* pImage)
{
	pImage->DrawTransparent(pDC, pImage);
}

void CrobotView::Translate(CDC* pDC, float dX, float dY, bool rightMultiply)
{
	XFORM trans = { 1,0,0,1,dX,dY };
	pDC->ModifyWorldTransform(&trans, rightMultiply ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);
}

void CrobotView::Rotate(CDC* pDC, float angle, bool rightMultiply)
{
	XFORM trans = { cos(angle) ,sin(angle) , -sin(angle) , cos(angle)};
	pDC->ModifyWorldTransform(&trans, rightMultiply ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);
}

void CrobotView::Scale(CDC* pDC, float sX, float sY, bool rightMultiply)
{
	XFORM trans = { sX , 0 , 0 , sY , 0 , 0 };
	pDC->ModifyWorldTransform(&trans, rightMultiply ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);
}

void CrobotView::Mirror(CDC* pDC, bool mX, bool mY, bool rightMultiply)
{
	Scale(pDC, mX ? -1 : 1, mY ? -1 : 1, rightMultiply);
}

void CrobotView::DrawHalf(CDC* pDC)
{
	XFORM transold;
	pDC->GetWorldTransform(&transold);
	Translate(pDC, 25, 65, false);
	Translate(pDC, -25, -65, false);
	DrawImgTransparent(pDC, telo);

	Translate(pDC, 25 - 35, 65 - 35, false);

	Translate(pDC, 35, 35, false);
	Rotate(pDC, nadlakticaRot, false);
	Translate(pDC, -35, -35, false);
	DrawImgTransparent(pDC, nadlaktica);

	Translate(pDC, 22 - 35, 167 - 35, false);

	Translate(pDC, 30, 33, false);
	Rotate(pDC, podlakticaRot, false);
	Translate(pDC, -30, -33, false);
	DrawImgTransparent(pDC, podlaktica);

	Translate(pDC, 30 - 30, 140 - 33, false);

	Translate(pDC, 25, 3, false);
	Rotate(pDC, sakaRot, false);
	Translate(pDC, -25, -3, false);
	DrawImgTransparent(pDC, saka);

	pDC->SetWorldTransform(&transold);
	Translate(pDC, 61-29, 262-20, false);

	DrawImgTransparent(pDC, nadkolenica);

	Translate(pDC, 30-25, 184-37, false);


	DrawImgTransparent(pDC, podkolenica);

	Translate(pDC, 25 - 20, 248 - 16, false);

	
	DrawImgTransparent(pDC, stopalo);

	pDC->SetWorldTransform(&transold);
}

void CrobotView::DrawHead(CDC* pDC)
{
	glava->DrawTransparent(pDC, glava);
}

void CrobotView::DrawRobot(CDC* pDC)
{
	CRect rect;
	GetClientRect(&rect);
	DrawBackground(pDC);
	Translate(pDC, rect.Width() / 2 - telo->Width(), rect.Height() / 2 - telo->Height(), false);
	DrawHalf(pDC);
	Mirror(pDC, true, false, false);
	Translate(pDC, -2 * telo->Width(), 0, false);
	DrawHalf(pDC);
	Translate(pDC, -rect.Width() / 2 + telo->Width(), -rect.Height() / 2 + telo->Height(), false);
	Translate(pDC, rect.Width() / 2 - glava->Width() / 2, rect.top+glava->Width()/2, false);
	DrawHead(pDC);
	Translate(pDC, -(rect.Width() / 2 - glava->Width() / 2), -(rect.top+glava->Width() / 2), false);
}


// CrobotView printing

BOOL CrobotView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CrobotView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CrobotView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// CrobotView diagnostics

#ifdef _DEBUG
void CrobotView::AssertValid() const
{
	CView::AssertValid();
}

void CrobotView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CrobotDoc* CrobotView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CrobotDoc)));
	return (CrobotDoc*)m_pDocument;
}
#endif //_DEBUG


// CrobotView message handlers


BOOL CrobotView::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default
	return TRUE;
	return CView::OnEraseBkgnd(pDC);
}


void CrobotView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (nChar == 'A')
	{
		if (sakaRot > -10*toRad)
			sakaRot -= 5*toRad;
	}
	else if (nChar == 'S') 
	{
		if (sakaRot < 30*toRad)
			sakaRot += 5*toRad;
	}
	else if (nChar == 'D') 
	{
		if (podlakticaRot > -10 * toRad)
			podlakticaRot -= 5 * toRad;
	}
	else if (nChar == 'F') 
	{
		if (podlakticaRot < 80 * toRad)
			podlakticaRot += 5 * toRad;
	}
	else if (nChar == 'G') 
	{
		if (nadlakticaRot > -10 * toRad)
			nadlakticaRot -= 5 * toRad;
	}
	else if (nChar == 'H')
	{
		if (nadlakticaRot < 90 * toRad)
			nadlakticaRot += 5 * toRad;
	}
	else if (nChar == '1')
	{
		roboRot -= 5*toRad;
	}
	else if (nChar == '2')
	{
		roboRot += PI/12;
	}
	Invalidate();
	// TODO: Add your message handler code here and/or call default

	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}
