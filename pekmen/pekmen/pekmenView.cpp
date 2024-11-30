
// pekmenView.cpp : implementation of the CpekmenView class
//

#include "pch.h"
#include "framework.h"
#include <math.h>
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "pekmen.h"
#endif

#include "pekmenDoc.h"
#include "pekmenView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#define PI 3.14
#define toRad PI/180
#define toDegree 180/PI

bool kliknutoDesno = false;
bool kliknutoGore = false;
int kliknuto = 0;
bool dodirujuSe = false;


// CpekmenView

IMPLEMENT_DYNCREATE(CpekmenView, CView)

BEGIN_MESSAGE_MAP(CpekmenView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_KEYDOWN()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

// CpekmenView construction/destruction

CpekmenView::CpekmenView() noexcept
{
	pekUsta = 0;
	pekX = 0;
	pekY = 0;
	pekUsta = 1;
	pekRot = 0;
	pekMirror = false;
	pozadina = new DImage();
	pozadina->Load(CString("blue.png"));
	// TODO: add construction code here

}

CpekmenView::~CpekmenView()
{
	delete pozadina;
}

BOOL CpekmenView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CpekmenView drawing

void CpekmenView::OnDraw(CDC* pDC)
{
	CpekmenDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	CRect rect;
	GetClientRect(&rect);
	CDC* memDC = new CDC();
	memDC->CreateCompatibleDC(pDC);
	CBitmap bm;
	bm.CreateCompatibleBitmap(pDC, rect.Width(), rect.Height());
	memDC->SelectObject(&bm);
	DrawMem(memDC);
	pDC->BitBlt(0, 0, rect.Width(), rect.Height(), memDC, 0, 0, SRCCOPY);
	delete memDC;
	DrawMem(pDC);






	// TODO: add draw code for native data here
}

void CpekmenView::Translate(CDC* pDC, float dX, float dY, float rightMultiply)
{
	XFORM trans = { 1,0,0,1,dX,dY };
	pDC->ModifyWorldTransform(&trans, rightMultiply ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);
}

void CpekmenView::Rotate(CDC* pDC, float angle, float rightMultiply)
{
	XFORM trans = { cos(angle) , sin(angle) , -sin(angle) , cos(angle) ,  0,0 };
	pDC->ModifyWorldTransform(&trans, rightMultiply ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);
}

void CpekmenView::Scale(CDC* pDC, float sX, float sY, float rightMultiply)
{
	XFORM trans = { sX ,0,0,sY, 0,0 };
	pDC->ModifyWorldTransform(&trans, rightMultiply ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);
}

void CpekmenView::Mirror(CDC* pDC, bool mX, bool mY, float rightMultiply)
{
	Scale(pDC, mX ? -1 : 1, mY ? -1 : 1, false);
}

void CpekmenView::DrawPacman(CDC* pDC, CRect rect, float angle)
{
	CBrush* zutaBojica = new CBrush(RGB(255, 255, 0));
	CPen* crnaOlovka = new CPen(PS_SOLID, 1 , RGB(0, 0, 0));
	int centarX = rect.right - rect.right / 2;
	int centarY = rect.bottom - rect.bottom / 2;
	CPoint* centar = new CPoint(centarX, centarY);
	CPoint* gornjaUsna = new CPoint(rect.right, centarY - rect.right / 2 * tan(angle * toRad));
	CPoint* donjaUsna = new CPoint(rect.right, centarY - rect.right / 2 * tan(-angle * toRad));
	CBrush* oldBrush = pDC->SelectObject(zutaBojica);
	CPen* oldPen = pDC->SelectObject(crnaOlovka);
	pDC->BeginPath();
		pDC->MoveTo(centarX, centarY);
		pDC->LineTo(*gornjaUsna);
		pDC->ArcTo(rect, *gornjaUsna, *donjaUsna);
		pDC->LineTo(centarX , centarY);
	pDC->EndPath();
	pDC->StrokeAndFillPath();
	CBrush* crnaBojica = new CBrush(RGB(0, 0, 0));
	pDC->SelectObject(*crnaBojica);
	pDC->Ellipse(centarX+15 , centarY-20 , centarX+25 , centarY-10);
	pDC->SelectObject(oldBrush);
	pDC->SelectObject(oldPen);

}

void CpekmenView::DrawGhost(CDC* pDC, CRect rect) {
	// Dimenzije pravougaonika
	int x = rect.TopLeft().x;   // Gornja leva tačka X
	int y = rect.TopLeft().y;   // Gornja leva tačka Y
	int w = rect.Width();       // Širina pravougaonika
	int h = rect.Height();      // Visina pravougaonika
	int r = w / 2;              // Poluprečnik gornjeg luka
	int waveHeight = h / 6;     // Visina talasa na dnu
	int waveWidth = w / 6;      // Širina svakog talasa

	// Boja duha
	CBrush brush;
	brush.CreateSolidBrush(RGB(255, 0, 0));  // Crvena boja
	CBrush* oldBrush = pDC->SelectObject(&brush);

	// Ivice duha
	CPen pen;
	pen.CreatePen(PS_SOLID, 1, RGB(0, 0, 0));  // Crne ivice
	CPen* oldPen = pDC->SelectObject(&pen);

	// Početak crtanja
	pDC->BeginPath();

	// Crtanje gornjeg dela (polukrug - glava)
	pDC->SetArcDirection(AD_CLOCKWISE);
	pDC->ArcTo(
		CRect(x, y, x + w, y + h - waveHeight), // Pravougaonik za polukrug
		CPoint(x, y + h / 2),                  // Početna tačka luka
		CPoint(x + w, y + h / 2));             // Krajnja tačka luka

	// Crtanje desne ivice prema prvom talasu
	pDC->LineTo(x + w, y + h);

	// Crtanje donjih talasa
	int currentX = x + w;
	for (int i = 0; i < 6; i++) {
		if (i % 2 == 0) { // Talas prema unutra
			pDC->SetArcDirection(AD_CLOCKWISE);
		}
		else {          // Talas prema spolja
			pDC->SetArcDirection(AD_COUNTERCLOCKWISE);
		}
		// Pravougaonik za luk talasa
		pDC->ArcTo(
			CRect(currentX - waveWidth, y + h - waveHeight, currentX, y + h),
			CPoint(currentX, y + h - waveHeight / 2),       // Početak luka
			CPoint(currentX - waveWidth, y + h - waveHeight / 2)); // Kraj luka
		currentX -= waveWidth; // Pomeraj X koordinatu za širinu jednog talasa
	}

	// Crtanje leve ivice prema početnoj tački gornjeg luka
	pDC->LineTo(x, y + h / 2);

	// Zatvaranje puta
	pDC->EndPath();
	pDC->StrokeAndFillPath();

	CBrush* cetkaBela = new CBrush(RGB(255, 255, 255));
	pDC->SelectObject(cetkaBela);
	pDC->Ellipse(x+60,y+25, x+85 , y+50);
	pDC->Ellipse(x + 30, y + 25, x + 55, y + 50);
	CBrush* cetkaPlava = new CBrush(RGB(0, 0, 255));
	pDC->SelectObject(cetkaPlava);
	pDC->Ellipse(x + 67, y + 32, x + 80, y + 45);
	pDC->Ellipse(x + 37, y + 32, x + 50, y + 45);
	// Resetovanje četke i olovke
	pDC->SelectObject(oldBrush);
	pDC->SelectObject(oldPen);
	brush.DeleteObject();
	pen.DeleteObject();
}

void CpekmenView::DrawMem(CDC* pDC)
{
	pDC->SetBkMode(TRANSPARENT);
	int pdcMode = pDC->SetGraphicsMode(GM_ADVANCED);
	XFORM transold;
	CRect rect;
	GetClientRect(&rect);
	pozadina->Draw(pDC, CRect(0, 0, pozadina->Width(), pozadina->Height()), CRect(0, 0 , rect.Width(), rect.Height()));
	pDC->GetWorldTransform(&transold);
	if (kliknuto != 0)
		Translate(pDC, 500 + pekX, 400 + pekY, false); //dodajemo 100 ne znam sto
	else
		Translate(pDC, 400 + pekX, 400 + pekY, false);
	if (kliknutoDesno)
		Translate(pDC, -100, 0, false); // kolki je packman
	if (kliknutoGore)
		Translate(pDC, -100, 100, false); // kolki je packman

	Rotate(pDC, pekRot, false);
	Mirror(pDC, pekMirror, false, false);

	DrawPacman(pDC, CRect(0, 0, 100, 100), pekUsta * 15);
	pDC->SetWorldTransform(&transold);
	DrawGhost(pDC, CRect(100, 120, 196, 216));
	pDC->SetGraphicsMode(pdcMode);
	if (dodirujuSe) 
	{
		CFont font;
		font.CreateFontW(72, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, L"Arial");
		CFont* oldFont = pDC->SelectObject(&font);
		COLORREF staraBoja = pDC->SetTextColor(RGB(255, 255, 255));
		pDC->TextOutW(rect.Width()/2, rect.Height()/2, L"The End");
		pDC->SelectObject(oldFont);
		pDC->SetTextColor(staraBoja);
	}
}


// CpekmenView printing

BOOL CpekmenView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CpekmenView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CpekmenView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// CpekmenView diagnostics

#ifdef _DEBUG
void CpekmenView::AssertValid() const
{
	CView::AssertValid();
}

void CpekmenView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CpekmenDoc* CpekmenView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CpekmenDoc)));
	return (CpekmenDoc*)m_pDocument;
}
#endif //_DEBUG


// CpekmenView message handlers


void CpekmenView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: Add your message handler code here and/or call default
	
	if (nChar == VK_RIGHT) 
	{
		kliknuto++;
		pekRot = 0;
		pekX += 10;
		pekMirror = false;
		kliknutoGore = false;
		kliknutoDesno = true;
	}
	else if (nChar == VK_UP) 
	{
		kliknuto++;
		pekRot = -PI / 2;
		pekY -= 10;
		pekMirror = false;
		kliknutoGore = true;
		kliknutoDesno = false;
	}
	else if (nChar == VK_DOWN) 
	{
		kliknuto++;
		pekRot = PI / 2;
		pekY += 10;
		pekMirror = false;
		kliknutoGore = false;
		kliknutoDesno = false;
	}
	else if (nChar == VK_LEFT) 
	{
		kliknuto++;
		pekRot = 0;
		pekMirror = true;
		pekX -= 10;
		kliknutoGore = false;
		kliknutoDesno = false;
	}
	if (kliknuto % 2 == 0)
	{
		pekUsta = 1;
	}
	else
		pekUsta = 0;
	CRect ghost = CRect(100, 120, 196, 216);
	int ghostX = 148;
	int ghostY = 168;
	float distancaX = abs(400+pekX - ghostX);
	float distancaY = abs(400+pekY - ghostY);
	float d = sqrt(pow(distancaX, 2) + pow(distancaY, 2));
	if (d <= 50 + ghost.Width() / 2)
		dodirujuSe = true;
	else
		dodirujuSe = false;


	Invalidate();

	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}


BOOL CpekmenView::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default
	return TRUE;
	return CView::OnEraseBkgnd(pDC);
}
