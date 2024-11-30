DrawTransparent
  void DImage::DrawTransparent(CDC* pDC, CRect rcImg, CRect rcDC, COLORREF bgColor)
{
    // Kreiramo masku
    CBitmap mask;
    mask.CreateBitmap(Width(), Height(), 1, 1, NULL);

    // Kreiramo kompatibilne DC-ove
    CDC srcDC, maskDC, memDC;
    srcDC.CreateCompatibleDC(pDC);
    maskDC.CreateCompatibleDC(pDC);
    memDC.CreateCompatibleDC(pDC);

    // Selektujemo originalnu bitmapu i masku
    CBitmap* oldSrcBmp = srcDC.SelectObject(m_pBmp); // Originalna slika
    CBitmap* oldMaskBmp = maskDC.SelectObject(&mask); // Maska

    // Postavljamo boju pozadine (za transparentnost)
    COLORREF oldBkColor = srcDC.SetBkColor(bgColor);

    // Kreiramo masku tako što kopiramo piksele pozadine
    maskDC.BitBlt(0, 0, Width(), Height(), &srcDC, 0, 0, SRCCOPY);

    // Vraćamo originalnu boju pozadine
    srcDC.SetBkColor(oldBkColor);

    // Primenjujemo masku: Transparentni deo (bela boja na maski)
    pDC->SetStretchBltMode(HALFTONE);

    // Korak 1: Primena maske sa SRCAND (uklanja transparentne delove)
    pDC->StretchBlt(rcDC.left, rcDC.top, rcDC.Width(), rcDC.Height(),
        &maskDC, rcImg.left, rcImg.top, rcImg.Width(), rcImg.Height(), SRCAND);

    // Korak 2: Crtanje originalne slike sa SRCINVERT (preko transparentnih delova)
    // Umesto SRCPAINT, koristimo SRCINVERT kako bi samo delovi slike koji nisu transparentni bili prikazani
    memDC.SelectObject(m_pBmp);
    pDC->StretchBlt(rcDC.left, rcDC.top, rcDC.Width(), rcDC.Height(),
        &memDC, rcImg.left, rcImg.top, rcImg.Width(), rcImg.Height(), SRCPAINT);

    // Vraćanje originalnih bitmapa
    srcDC.SelectObject(oldSrcBmp);
    maskDC.SelectObject(oldMaskBmp);
}
void DImage::DrawTransparent(CDC* pDC, DImage* pImage)
{
    int w = pImage->Width();
    int h = pImage->Height();
    CRect rect(0, 0, w, h);

    CDC* srcDC = new CDC();
    srcDC->CreateCompatibleDC(pDC);

    CDC* dstDC = new CDC();
    dstDC->CreateCompatibleDC(pDC);

    CBitmap* src = new CBitmap();
    src->CreateCompatibleBitmap(pDC, w, h);

    CBitmap* dst = new CBitmap();
    dst->CreateBitmap(w, h, 1, 1, nullptr);

    CBitmap* srcOld = srcDC->SelectObject(src);
    CBitmap* dstOld = dstDC->SelectObject(dst);

    pImage->Draw(srcDC, rect, rect);

    srcDC->SetBkColor(srcDC->GetPixel(0, 0));

    dstDC->BitBlt(0, 0, w, h, srcDC, 0, 0, SRCCOPY);

    srcDC->SetBkColor(RGB(0, 0, 0));
    srcDC->SetTextColor(RGB(255, 255, 255));
    srcDC->BitBlt(0, 0, w, h, dstDC, 0, 0, SRCAND);

    pDC->BitBlt(0, 0, w, h, dstDC, 0, 0, SRCAND);
    pDC->BitBlt(0, 0, w, h, srcDC, 0, 0, SRCPAINT);


    dstDC->SelectObject(dstOld);
    srcDC->SelectObject(srcOld);

    dstDC->DeleteDC();
    delete dstDC;

    srcDC->DeleteDC();
    delete srcDC;

    delete src;
    delete dst;
}
