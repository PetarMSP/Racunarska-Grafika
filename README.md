DrawTransparent

void DImage::DrawTransparent(CDC* pDC, CRect rcImg, CRect rcDC, COLORREF bgColor) {
    CBitmap mask;
    mask.CreateBitmap(Width(), Height(), 1, 1, NULL);//monohorna maska crno-bela

    CDC srcDC, memDC, maskDC;
    srcDC.CreateCompatibleDC(pDC);
    memDC.CreateCompatibleDC(pDC);
    maskDC.CreateCompatibleDC(pDC);

    CBitmap* oldSrcBmp = srcDC.SelectObject(m_pBmp);
    CBitmap* oldMaskBmp = maskDC.SelectObject(&mask);

    COLORREF oldBkcolor = srcDC.SetBkColor(bgColor);//treasparenta boja za bele piskele maske

    maskDC.BitBlt(0, 0, Width(), Height(), &srcDC, 0, 0, SRCCOPY);//svi pikseli slike orginalne koji imaju bgcolor su beli u maski a ostali crni

    srcDC.SetBkColor(oldBkcolor);//vrecamo prvobitnu strau u srcDC prozor
    //primena maske na celom DC (pDC glavni)
    pDC->SetStretchBltMode(HALFTONE);
    //ciljni pDC ima probusenu rupu u obliku slike jer AND uklanja tamo ge je belo na maski to jest pozadinu
    pDC->StretchBlt(rcDC.left, rcDC.top, rcDC.Width(), rcDC.Height(), &maskDC, 
          rcImg.left, rcImg.top,rcImg.Width(), rcImg.Height(), SRCAND);

    memDC.SelectObject(m_pBmp);
    pDC->StretchBlt(rcDC.left, rcDC.top, rcDC.Width(), rcDC.Height(),&memDC, 
          rcImg.left, rcImg.top, rcImg.Width(), rcImg.Height(), SRCPAINT);

    srcDC.SelectObject(oldSrcBmp);
    maskDC.SelectObject(oldMaskBmp);
}   
// TODO: add draw code for native data here
//Anti-Fliker (od ovog komentara do kod i od kraj koda do kraj flikera)
CRect rect;
GetClientRect(&rect);

CDC* pMemDC = new CDC();
pMemDC->CreateCompatibleDC(pDC);
CBitmap bmp;
bmp.CreateCompatibleBitmap(pDC, rect.Width(), rect.Height());
pMemDC->SelectObject(&bmp);
//Kod
int pdcMode = pDC->SetGraphicsMode(GM_ADVANCED);
XFORM transold;
CRect rect;
GetClientRect(&rect);
pozadina->Draw(pDC, CRect(0, 0, pozadina->Width(), pozadina->Height()), CRect(0, 0 , rect.Width(), rect.Height()));
pDC->GetWorldTransform(&transold);
//neki..kod
pDC->SetWorldTransform(&transold);
pDC->SetGraphicsMode(pdcMode);

CFont font;
font.CreateFontW(72, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, L"Arial");
CFont* oldFont = pDC->SelectObject(&font);
COLORREF staraBoja = pDC->SetTextColor(RGB(255, 255, 255));
pDC->TextOutW(rect.Width()/2, rect.Height()/2, L"The End");
pDC->SelectObject(oldFont);
pDC->SetTextColor(staraBoja);
//kraj koda
pDC->BitBlt(0, 0, rect.Width(), rect.Height(),pMemDC, 0, 0, SRCCOPY);
pMemDC->DeleteDC();
delete pMemDC;
//kraj flikera
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
}t;
}
