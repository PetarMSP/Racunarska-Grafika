
// bilijarView.cpp : implementation of the CbilijarView class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "bilijar.h"
#endif

#include "bilijarDoc.h"
#include "bilijarView.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define PI 3.14
#define toRad PI/180
#define toDegree 180/PI

bool kolizija = false;
int vratiX = 0;
int vratiY = 0;
bool kuglaXChanged = false;
bool kuglaYChanged = false;

// CbilijarView

IMPLEMENT_DYNCREATE(CbilijarView, CView)

BEGIN_MESSAGE_MAP(CbilijarView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_ERASEBKGND()
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()

// CbilijarView construction/destruction

CbilijarView::CbilijarView() noexcept
{
	// TODO: add construction code here
	felt2 = new DImage();
	felt2->Load(CString("felt2.jpg"));
	wood = new DImage();
	wood->Load(CString("wood.jpg"));
	kuglaX = 600;
	kuglaY = 400;
	stapRot = 0;
	stapX = 0;
	duzinaStapa = 500;

}

CbilijarView::~CbilijarView()
{
	delete felt2;
	delete wood;
}

BOOL CbilijarView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CbilijarView drawing

void CbilijarView::OnDraw(CDC* pDC)
{
	CbilijarDoc* pDoc = GetDocument();
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
	// TODO: add draw code for native data here
}

void CbilijarView::DrawMem(CDC* pDC)
{
	pDC->SetGraphicsMode(GM_ADVANCED);
	XFORM transold;
	pDC->GetWorldTransform(&transold);
	CRect rect;
	GetClientRect(&rect);
	DrawBorder(pDC, CRect(0, 0, rect.Width(), rect.Height()), rect.Width());
	DrawTable(pDC, CRect(rect.left + 50, rect.top + 50, rect.right - 50, rect.bottom - 50));
	DrawHoles(pDC, CRect(rect.left + 50, rect.top + 50, rect.right - 50, rect.bottom - 50), 40);
	Translate(pDC, kuglaX,kuglaY, false);
	DrawBall(pDC, 30);
	Rotate(pDC, stapRot, false);
	Translate(pDC, -kuglaX, -kuglaY, false);
	Translate(pDC, stapX, 390, false);
	DrawStick(pDC, duzinaStapa);
	Translate(pDC, -stapX, -390, false);
	pDC->SetWorldTransform(&transold);

}

void CbilijarView::DrawStick(CDC* pDC, int w)
{

	CPen braonOlovka(PS_SOLID, 1, RGB(64, 32, 0));
	CPen belaOlovka(PS_SOLID, 1, RGB(255, 255, 255));
	CBrush braonBojica(RGB(64, 32, 0));
	CBrush okerBojica(RGB(255, 190, 128));

	CPen* staraOlovka = pDC->SelectObject(&braonOlovka);
	CBrush* staraBojica = pDC->SelectObject(&braonBojica);
	int startX = 0;
	int startY = 0;
	int wBraon = w / 50; // 400
	int wOker = w / 100;
	int presekBoja = w / 1.0 / 3.0;
	pDC->MoveTo(startX,startY);
	pDC->BeginPath();
	pDC->LineTo(CPoint(startX + presekBoja, startY));
	pDC->LineTo(CPoint(startX + presekBoja, startY + wBraon));
	pDC->LineTo(CPoint(startX, startY + wBraon));
	pDC->SetArcDirection(AD_CLOCKWISE);
	CRect rect(startX-5, startY , startX+5,startY+wBraon);
	pDC->ArcTo(rect, CPoint(startX, startY + wBraon), CPoint(startX, startY));
	pDC->EndPath();
	pDC->StrokeAndFillPath();

	pDC->MoveTo(startX + presekBoja, startY);
	pDC->SelectObject(okerBojica);
	pDC->BeginPath();
	pDC->LineTo(startX + presekBoja + w-(startX + presekBoja), startY + wOker);
	pDC->LineTo(startX + presekBoja + w - (startX + presekBoja), startY + wOker + 4);
	pDC->LineTo(startX + presekBoja , startY + wBraon );


	pDC->EndPath();
	pDC->StrokeAndFillPath();

	pDC->SelectObject(staraOlovka);
	pDC->SelectObject(staraBojica);

}

void CbilijarView::DrawBall(CDC* pDC, int w)
{
	CPen crvenaOlovka(PS_SOLID, 1, RGB(77, 10, 6));
	CBrush crvenaBojica(RGB(128, 0, 0));
	CBrush belaBojica(RGB(255, 255, 255));
	CPen* staraOlovka = pDC->SelectObject(&crvenaOlovka);
	CBrush* staraBojica = pDC->SelectObject(&crvenaBojica);

	pDC->Ellipse(CRect(0 - w/2 , 0 - w/2 , 0 + w/2 , 0+w/2));
	pDC->SelectObject(belaBojica);
	pDC->Ellipse(CRect(5 - w/2/3 , -5-w/2/3 , 5 + w /2/3, -5 + w /2/3));


	pDC->SelectObject(staraOlovka);
	pDC->SelectObject(staraBojica);
}

void CbilijarView::Translate(CDC* pDC, float dX, float dY, bool rightMultiply)
{
	XFORM trans = { 1,0,0,1,dX,dY };
	pDC->ModifyWorldTransform(&trans, rightMultiply ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);
}

void CbilijarView::Rotate(CDC* pDC, float angle, bool rightMultiply)
{
	XFORM trans = { cos(angle) , sin(angle) , -sin(angle) , cos(angle) };
	pDC->ModifyWorldTransform(&trans, rightMultiply ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);
}

void CbilijarView::DrawTable(CDC* pDC, CRect rect)
{
	int w = felt2->Width();
	int h = felt2->Height();
	for (int i = 0; i < rect.Width() / w ; i++) 
	{
		for (int j = 0; j < rect.Height() / h; j++) 
		{
			felt2->Draw(pDC, CRect(0, 0, w, h), CRect(rect.left + i * w, rect.top + j * h, rect.left + i * w + w, rect.top + j * h + h));
		}
	}
}

void CbilijarView::DrawBorder(CDC* pDC, CRect rect, int w)
{
	wood->Draw(pDC, CRect(0, 0, wood->Width(), wood->Height()) , CRect(0,0,w,w));
}

void CbilijarView::DrawHoles(CDC* pDC, CRect rect, int size)
{
	CPen crnaOlovka(PS_SOLID, 1, RGB(0, 0, 0));
	CBrush crnaBojica(RGB(0, 0, 0));
	CPen* staraOlovka = pDC->SelectObject(&crnaOlovka);
	CBrush* staraBojica = pDC->SelectObject(&crnaBojica);

	pDC->Ellipse(CRect(CPoint(rect.left - size / 2, rect.top - size / 2), CPoint(rect.left + size / 2, rect.top + size / 2)));
	pDC->Ellipse(CRect(CPoint(rect.Width()/2 - size / 2, rect.top - size / 2), CPoint(rect.Width()/2 + size / 2, rect.top + size / 2)));
	pDC->Ellipse(CRect(CPoint(rect.right - 40 - size / 2, rect.top - size / 2), CPoint(rect.right -40 + size / 2, rect.top + size / 2)));
	pDC->Ellipse(CRect(CPoint(rect.left - size / 2, rect.bottom - size / 2), CPoint(rect.left + size / 2, rect.bottom + size / 2)));
	pDC->Ellipse(CRect(CPoint(rect.Width()/2 - size / 2, rect.bottom - size / 2), CPoint(rect.Width()/2 + size / 2, rect.bottom + size / 2)));
	pDC->Ellipse(CRect(CPoint(rect.right - 40 - size / 2, rect.bottom - size / 2), CPoint(rect.right - 40 + size / 2, rect.bottom + size / 2)));

	pDC->SelectObject(staraOlovka);
	pDC->SelectObject(staraBojica);
}


// CbilijarView printing

BOOL CbilijarView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CbilijarView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CbilijarView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// CbilijarView diagnostics

#ifdef _DEBUG
void CbilijarView::AssertValid() const
{
	CView::AssertValid();
}

void CbilijarView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CbilijarDoc* CbilijarView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CbilijarDoc)));
	return (CbilijarDoc*)m_pDocument;
}
#endif //_DEBUG


// CbilijarView message handlers


BOOL CbilijarView::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default
	return TRUE;
	return CView::OnEraseBkgnd(pDC);
}


void CbilijarView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: Add your message handler code here and/or call default
	if (nChar == VK_LEFT && !kolizija) 
	{
		stapRot += 10*toRad;
	}
	else if(nChar == VK_RIGHT && !kolizija){
		stapRot -= 10*toRad;
		if (stapRot < 0)
			stapRot += 2 * PI;
	}
	else if(nChar == VK_UP)
	{
		if (kolizija) 
		{
			if (stapRot >= 0 && stapRot < PI / 2) //na desno i gore desno 
			{
				if (fabs(stapRot - PI / 2) < 0.01) 
				{
					 kuglaY += 10;
					 kuglaXChanged = true;
					 kuglaYChanged = false;
					 vratiX = 0;
					 vratiY = 10;
				}
				else if (fabs(stapRot) < 0.01)
				{
					kuglaX += 10;
					 kuglaXChanged = true;
					 kuglaYChanged = false;
					 vratiX = 10;
					 vratiY = 0;
				}
				else 
				{
					kuglaX += 10;
					kuglaY += 10;
					 kuglaXChanged = true;
					 kuglaYChanged = true;
					 vratiX = 10;
					 vratiY = 10;
				}
				

			}
			else if (stapRot >= PI / 2 && stapRot < PI) 
			{
				if (fabs(stapRot - PI / 2) < 0.01)
				{
					kuglaY += 10;
					 kuglaXChanged = false;
					 kuglaYChanged = true;
					 vratiX = 0;
					 vratiY = 10;
				}
				else if (fabs(stapRot - PI) < 0.01)
				{
					kuglaX -= 10;
					 kuglaXChanged = true;
					 kuglaYChanged = false;
					 vratiX = -10;
					 vratiY = 0;
				}
				else
				{
					 kuglaX -= 10;
					 kuglaY += 10;
					 kuglaXChanged = true;
					 kuglaYChanged = true;
					 vratiX = -10;
					 vratiY = 10;
				}
			}

			else if (stapRot >= PI  && stapRot < 3*PI/2)
			{
				if (fabs(stapRot - PI) < 0.01)
				{
					kuglaY -= 10;
					 kuglaXChanged = false;
					 kuglaYChanged = true;
					 vratiX =0;
					 vratiY = -10;
				}
				else if (fabs(stapRot - 3*PI/2) < 0.01)
				{
					kuglaX -= 10;
					 kuglaXChanged = true;
					 kuglaYChanged = false;
					 vratiX = -10;
					 vratiY = 0;
				}
				else
				{
					kuglaX -= 10;
					kuglaY -= 10;
					 kuglaXChanged = true;
					 kuglaYChanged = true;
					 vratiX = -10;
					 vratiY = -10;
				}
			}
			else if (stapRot >= 3*PI/2 && stapRot <2*PI)
			{
				if (fabs(stapRot - 3*PI/2) < 0.01)
				{
					kuglaY -= 10;
					 kuglaXChanged = false;
					 kuglaYChanged = true;
					 vratiX = 0;
					 vratiY = -10;
				}
				else if (fabs(stapRot - 2*PI) < 0.01)
				{
					kuglaX += 10;
					 kuglaXChanged = true;
					 kuglaYChanged = false;
					 vratiX = 10;
					 vratiY = 0;
				}
				else
				{
					kuglaX += 10;
					kuglaY -= 10;
					 kuglaXChanged = true;
					 kuglaYChanged = true;
					 vratiX = 10;
					 vratiY = -10;
				}
			}
			
		}
		else {
			stapX += 10;
			if(stapX + duzinaStapa + 15 >= kuglaX) //15 poluprecnik kruga
			{
				kolizija = true;
			}
		}
	}
	else if (nChar == VK_DOWN) 
	{
		if (kolizija)
		{
			if (kuglaXChanged)
				kuglaX -= vratiX;
			if (kuglaYChanged)
				kuglaY -= vratiY;
			if (stapX + duzinaStapa + 15 >= kuglaX) //15 poluprecnik kruga
			{
				stapX -= 10;
				kolizija = false;
			}
		}
		else {
			stapX -= 10;
		}
	}
	Invalidate();
	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}
