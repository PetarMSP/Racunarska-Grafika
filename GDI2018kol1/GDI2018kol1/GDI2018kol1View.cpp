
// GDI2018kol1View.cpp : implementation of the CGDI2018kol1View class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "GDI2018kol1.h"
#endif

#include "GDI2018kol1Doc.h"
#include "GDI2018kol1View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CGDI2018kol1View

IMPLEMENT_DYNCREATE(CGDI2018kol1View, CView)

BEGIN_MESSAGE_MAP(CGDI2018kol1View, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_ERASEBKGND()
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()

// CGDI2018kol1View construction/destruction

CGDI2018kol1View::CGDI2018kol1View() noexcept
{
	p_arm1 = new DImage();
	p_arm1->Load(CString("arm1.png"));
	p_arm2 = new DImage();
	p_arm2->Load(CString("arm2.png"));
	p_bager = new DImage();
	p_bager->Load(CString("bager.png"));
	p_pozadina = new DImage();
	p_pozadina->Load(CString("pozadina.png"));

	mf_viljuska = GetEnhMetaFile(CString("viljuska.emf"));

}

CGDI2018kol1View::~CGDI2018kol1View()
{
	delete p_arm1;
	delete p_arm2;
	delete p_bager;
	delete p_pozadina;

	DeleteEnhMetaFile(mf_viljuska);
}

BOOL CGDI2018kol1View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CGDI2018kol1View drawing

void CGDI2018kol1View::Translate(CDC* pDC, float dX, float dY, bool rightMultiply)
{
	XFORM trans = { 1,0,0,1,dX,dY };
	pDC->ModifyWorldTransform(&trans, rightMultiply ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);
}

void CGDI2018kol1View::Rotate(CDC* pDC, float angle, bool rightMultiply)
{
	XFORM trans = { cos(angle * toRad),sin(angle * toRad),-sin(angle * toRad),cos(angle * toRad),0,0 };
	pDC->ModifyWorldTransform(&trans, rightMultiply ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);
}

void CGDI2018kol1View::Scale(CDC* pDC, float sX, float sY, bool rightMultiply)
{
	XFORM trans = { sX,0,0,sY,0,0 };
	pDC->ModifyWorldTransform(&trans, rightMultiply ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);
}

void CGDI2018kol1View::DrawBackground(CDC* pDC)
{
	CRect rect;
	GetClientRect(&rect);

	int x, y, w, h;

	// uzmi za primer recimo kl.prozor 500 slika 300

	x = (rect.Width() - p_pozadina->Width()) / 2;
	w = x + p_pozadina->Width();
	y = rect.Height() - p_pozadina->Height();
	h = y + p_pozadina->Height();

	// prvi CRect - oblast slike koju zelimo da nacrtamo -> izvor slike
	// drugi CRect - gde na ekranu zelimo da prikazemo sliku
	p_pozadina->Draw(pDC, CRect(0, 0, p_pozadina->Width(), p_pozadina->Height()), CRect(x, y, w, h));
}

void CGDI2018kol1View::DrawImgTransparent(CDC* pDC, DImage* pImage)
{
	pImage->DrawTransparent(pDC, pImage);
}

void CGDI2018kol1View::DrawBody(CDC* pDC)
{
	Translate(pDC, -p_bager->Width(), -p_bager->Height(), false);
	DrawImgTransparent(pDC, p_bager);

	Translate(pDC, 5, 105, false);
}

void CGDI2018kol1View::DrawArm1(CDC* pDC)
{
	Translate(pDC, 58, 61, false);
	Rotate(pDC, a1 - 90, false);
	Translate(pDC, -58, -61, false);

	DrawImgTransparent(pDC, p_arm1);

	// sada je -x po y osi a -y po x osi
	Translate(pDC, 309 - 36, 61 - 41, false);
}

void CGDI2018kol1View::DrawArm2(CDC* pDC)
{
	Translate(pDC, 36, 40, false);
	Rotate(pDC, a2 - 90, false);
	Translate(pDC, -36, -40, false);

	DrawImgTransparent(pDC, p_arm2);

	Translate(pDC, 272, 40 ,false);
}
void CGDI2018kol1View::DrawFork(CDC* pDC)
{
	ENHMETAHEADER header;
	GetEnhMetaFileHeader(mf_viljuska, sizeof(ENHMETAHEADER), &header);
	int w = header.rclBounds.right - header.rclBounds.left;
	int h = header.rclBounds.bottom - header.rclBounds.top;
	Scale(pDC, 2.5, 2.5, false);
	Translate(pDC, -14, -20, false);
	Translate(pDC, 14, 20, false);
	Rotate(pDC, a3 - PI / 2, false);
	Translate(pDC, -14, -20, false);
;	PlayEnhMetaFile(pDC->m_hDC, mf_viljuska, CRect(0, 0, w, h));
}
void CGDI2018kol1View::DrawExcavator(CDC* pDC)
{
	CRect rect;
	GetClientRect(&rect);
	Translate(pDC, rect.Width(), rect.Height(), false);

	DrawBody(pDC);
	DrawArm1(pDC);
	DrawArm2(pDC);
	DrawFork(pDC);
}

void CGDI2018kol1View::OnDraw(CDC* pDC)
{
	CGDI2018kol1Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	CRect rect;
	GetClientRect(&rect);

	CDC* memDC = new CDC();
	memDC->CreateCompatibleDC(pDC);
	CBitmap bmp;
	bmp.CreateCompatibleBitmap(pDC, rect.Width(), rect.Height());
	memDC->SelectObject(&bmp);

	XFORM transOld;
	int gm = memDC->SetGraphicsMode(GM_ADVANCED);
	memDC->GetWorldTransform(&transOld);

	DrawBackground(memDC);
	Translate(memDC, -posx, 0, false);
	DrawExcavator(memDC);

	memDC->SetWorldTransform(&transOld);
	memDC->SetGraphicsMode(gm);

	pDC->BitBlt(0, 0, rect.Width(), rect.Height(), memDC, 0, 0, SRCCOPY);

	// TODO: add draw code for native data here
}


// CGDI2018kol1View printing

BOOL CGDI2018kol1View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CGDI2018kol1View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CGDI2018kol1View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// CGDI2018kol1View diagnostics

#ifdef _DEBUG
void CGDI2018kol1View::AssertValid() const
{
	CView::AssertValid();
}

void CGDI2018kol1View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CGDI2018kol1Doc* CGDI2018kol1View::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CGDI2018kol1Doc)));
	return (CGDI2018kol1Doc*)m_pDocument;
}
#endif //_DEBUG


// CGDI2018kol1View message handlers


BOOL CGDI2018kol1View::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;

	return CView::OnEraseBkgnd(pDC);
}


void CGDI2018kol1View::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (nChar == 'Q')
	{
		a1 -= 10.0f;
		Invalidate();
	}
	if (nChar == 'W')
	{
		a1 += 10.0f;
		Invalidate();
	}
	if (nChar == 'E')
	{
		a2 -= 10.0f;
		Invalidate();
	}
	if (nChar == 'R')
	{
		a2 += 10.0f;
		Invalidate();
	}
	if (nChar == 'T')
	{
		a3 -= 10.0f;
		Invalidate();
	}
	if (nChar == 'Y')
	{
		a3 += 10.0f;
		Invalidate();
	}
	if (nChar == VK_RIGHT)
	{
		posx -= 10;
		if (posx == 0)
			posx = 0;
		Invalidate();
	}
	if (nChar == VK_LEFT)
	{
		CRect rect;
		GetClientRect(&rect);

		posx += 10;
		if (posx == rect.Width())
			posx = rect.Width();
		Invalidate();
	}

	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}
