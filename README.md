class CGLRenderer
{
public:
	CGLRenderer(void);
	virtual ~CGLRenderer(void);

	// promenljive

	double m_alpha = 0.0;
	double m_beta = 0.0;
	double m_dist = 20.0;

	UINT m_texEnv[6];
	UINT m_texSpider;

	// metode
	void DrawAxes();
	UINT LoadTexture(char* fileName);
	void DrawEnvCube(double a);
--------------------------------------
void CGLRenderer::DrawAxes()
{
	glLineWidth(2.0);

	glBegin(GL_LINES);

	glColor3d(0.0, 0.0, 1.0);
	glVertex3d(0.0, 0.0, 0.0);
	glVertex3d(50.0, 0.0, 0.0);

	glColor3d(1.0, 0.0, 0.0);
	glVertex3d(0.0, 0.0, 0.0);
	glVertex3d(0.0, 50.0, 0.0);

	glColor3d(0.0, 1.0, 0.0);
	glVertex3d(0.0, 0.0, 0.0);
	glVertex3d(0.0, 0.0, 50.0);

	glEnd();
}

void CGLRenderer::PrepareScene(CDC *pDC)
{
	wglMakeCurrent(pDC->m_hDC, m_hrc);
	
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glEnable(GL_DEPTH_TEST);

	m_texEnv[0] = LoadTexture("assets/front.jpg");
	m_texEnv[1] = LoadTexture("assets/back.jpg");
	m_texEnv[2] = LoadTexture("assets/left.jpg");
	m_texEnv[3] = LoadTexture("assets/right.jpg");
	m_texEnv[4] = LoadTexture("assets/top.jpg");
	m_texEnv[5] = LoadTexture("assets/bot.jpg");

	m_texSpider = LoadTexture("assets/spider.png");

	wglMakeCurrent(NULL, NULL);
}

void CGLRenderer::DrawScene(CDC *pDC)
{
	wglMakeCurrent(pDC->m_hDC, m_hrc);
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	glTranslated(0.0, 0.0, -m_dist);
	glRotated(-m_alpha, 1.0, 0.0, 0.0);
	glRotated(-m_beta, 0.0, 1.0, 0.0);

	DrawAxes();

	//DrawSphere(5, 32, 0.25, 0.25, 0.24);
	//DrawCone(2, 5, 32, 0.75, 0.75, 0.25);

	DrawSpider();

	DrawEnvCube(100);

	glFlush();
	SwapBuffers(pDC->m_hDC);

	wglMakeCurrent(NULL, NULL);
}

UINT CGLRenderer::LoadTexture(char* fileName)
{
	UINT texID;
	DImage img;
	img.Load(CString(fileName));

	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
	glGenTextures(1, &texID);
	glBindTexture(GL_TEXTURE_2D, texID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, img.Width(), img.Height(), GL_BGRA_EXT, GL_UNSIGNED_BYTE, img.GetDIBBits());

	return texID;
}

void CGLRenderer::Reshape(CDC *pDC, int w, int h)
{
	wglMakeCurrent(pDC->m_hDC, m_hrc);
	
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(50, (double)w / (double)h, 0.1, 2000);
	glMatrixMode(GL_MODELVIEW);

	wglMakeCurrent(NULL, NULL);
}

void CGLRenderer::DestroyScene(CDC *pDC)
{
	wglMakeCurrent(pDC->m_hDC, m_hrc);
	
	glDeleteTextures(6, m_texEnv);
	glDeleteTextures(1, &m_texSpider);

	wglMakeCurrent(NULL,NULL); 
	if(m_hrc) 
	{
		wglDeleteContext(m_hrc);
		m_hrc = NULL;
	}
}

void CGLRenderer::DrawEnvCube(double a)
{
	glEnable(GL_TEXTURE_2D);

	// front
	glBindTexture(GL_TEXTURE_2D, m_texEnv[0]);
	glBegin(GL_QUADS);
	glColor3d(1.0, 1.0, 1.0);

	glTexCoord2d(0, 1);
	glVertex3d(-a / 2, -a / 2, -a / 2);
	glTexCoord2d(0, 0);
	glVertex3d(-a / 2, a / 2, -a / 2);
	glTexCoord2d(1, 0);
	glVertex3d(a / 2, a / 2, -a / 2);
	glTexCoord2d(1, 1);
	glVertex3d(a / 2, -a / 2, -a / 2);

	glEnd();

	// back
	glBindTexture(GL_TEXTURE_2D, m_texEnv[1]);
	glBegin(GL_QUADS);
	glColor3d(1.0, 1.0, 1.0);

	glTexCoord2d(0, 1);
	glVertex3d(a / 2, -a / 2, a / 2);
	glTexCoord2d(0, 0);
	glVertex3d(a / 2, a / 2, a / 2);
	glTexCoord2d(1, 0);
	glVertex3d(-a / 2, a / 2, a / 2);
	glTexCoord2d(1, 1);
	glVertex3d(-a / 2, -a / 2, a / 2);

	glEnd();


	// left
	glBindTexture(GL_TEXTURE_2D, m_texEnv[2]);
	glBegin(GL_QUADS);
	glColor3d(1.0, 1.0, 1.0);

	glTexCoord2d(0, 1);
	glVertex3d(-a / 2, -a / 2, a / 2);
	glTexCoord2d(0, 0);
	glVertex3d(-a / 2, a / 2, a / 2);
	glTexCoord2d(1, 0);
	glVertex3d(-a / 2, a / 2, -a / 2);
	glTexCoord2d(1, 1);
	glVertex3d(-a / 2, -a / 2, -a / 2);

	glEnd();

	// right
	glBindTexture(GL_TEXTURE_2D, m_texEnv[3]);
	glBegin(GL_QUADS);
	glColor3d(1.0, 1.0, 1.0);

	glTexCoord2d(0, 1);
	glVertex3d(a / 2, -a / 2, -a / 2);
	glTexCoord2d(0, 0);
	glVertex3d(a / 2, a / 2, -a / 2);
	glTexCoord2d(1, 0);
	glVertex3d(a / 2, a / 2, a / 2);
	glTexCoord2d(1, 1);
	glVertex3d(a / 2, -a / 2, a / 2);

	glEnd();


	// top
	glBindTexture(GL_TEXTURE_2D, m_texEnv[4]);
	glBegin(GL_QUADS);
	glColor3d(1.0, 1.0, 1.0);

	glTexCoord2d(0, 1);
	glVertex3d(-a / 2, a / 2, -a / 2);
	glTexCoord2d(0, 0);
	glVertex3d(-a / 2, a / 2, a / 2);
	glTexCoord2d(1, 0);
	glVertex3d(a / 2, a / 2, a / 2);
	glTexCoord2d(1, 1);
	glVertex3d(a / 2, a / 2, -a / 2);

	glEnd();

	//bot
	glBindTexture(GL_TEXTURE_2D, m_texEnv[5]);
	glBegin(GL_QUADS);
	glColor3d(1.0, 1.0, 1.0);

	glTexCoord2d(0, 1);
	glVertex3d(-a / 2, -a / 2, a / 2);
	glTexCoord2d(0, 0);
	glVertex3d(-a / 2, -a / 2, -a / 2);
	glTexCoord2d(1, 0);
	glVertex3d(a / 2, -a / 2, -a / 2);
	glTexCoord2d(1, 1);
	glVertex3d(a / 2, -a / 2, a / 2);

	glEnd();



	glDisable(GL_TEXTURE_2D);
}
void CGLRenderer::DrawSphere(double r, int nSeg, double texU, double texV, double texR)
{
	double stepAlpha = 3.1415 / nSeg;
	double stepBeta = 2 * 3.1415 / nSeg;


	double alpha, beta;

	alpha = -3.1415 / 2;

	for (int i = 0; i < nSeg; i++)
	{
		beta = 0.0;

		glBegin(GL_QUAD_STRIP);
	

		for (int j = 0; j < nSeg + 1; j++)
		{
			double x1 = r * cos(alpha) * cos(beta);
			double y1 = r * sin(alpha);
			double z1 = r * cos(alpha) * sin(beta);

			double tx1 = x1 / r * texR + texU;
			double ty1 = z1 / r * texR + texV;

			glTexCoord2d(tx1, ty1);
			glVertex3d(x1, y1, z1);


			double x2 = r * cos(alpha + stepAlpha) * cos(beta);
			double y2 = r * sin(alpha + stepAlpha);
			double z2 = r * cos(alpha + stepAlpha) * sin(beta);

			double tx2 = x2 / r * texR + texU;
			double ty2 = z2 / r * texR + texV;

			glTexCoord2d(tx2, ty2);
			glVertex3d(x2, y2, z2);

			beta += stepBeta;
		}
		glEnd();

		alpha += stepAlpha;
	}
}
-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

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
