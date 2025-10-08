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
void CGLRenderer::DrawCone(double r, double h, int nSeg, double texU, double texV, double texR)
{
	double stepAlpha = 2 * 3.1415 / nSeg;

	glBegin(GL_TRIANGLE_FAN);

	double alpha = 0.0;
	glTexCoord2d(0.75, 0.75);
	glVertex3d(0.0, h, 0.0);

	for (int i = 0; i < nSeg + 1; i++)
	{
		double x1 = r * cos(alpha);
		double z1 = r * sin(alpha);

		double tx1 = x1 / r * texR + texU;
		double ty1 = z1 / r * texR + texV;

		glTexCoord2d(tx1, ty1);
		glVertex3d(x1, 0.0, z1);

		alpha += stepAlpha;
	}

	glEnd();
}

void CGLRenderer::DrawLegSegment(double r, double h, int nSeg)
{
	glPushMatrix();

	DrawSphere(r, 2 * nSeg, 0.25, 0.25, 0.24);
	DrawCone(r, h, nSeg, 0.75, 0.75, 0.25);

	glPopMatrix();
}

void CGLRenderer::DrawLeg()
{
	glPushMatrix();

	DrawLegSegment(1, 10, 5);
	glTranslated(0.0, 10.0, 0.0);
	glRotated(85.0, 1.0, 0.0, 0.0);
	DrawLegSegment(1, 15, 5);

	glPopMatrix();
}

void CGLRenderer::DrawSpiderBody()
{

	// grudni deo
	glPushMatrix();

	glScaled(1.0, 0.5, 1.0);
	DrawSphere(3, 10, 0.25, 0.25, 0.24);

	glPopMatrix();

	// rep
	glPushMatrix();

	glTranslated(6.5, 0.0, 0.0);
	glScaled(1.0, 0.8, 1.0);
	DrawSphere(5, 10, 0.25, 0.25, 0.24);

	glPopMatrix();

	//glava
	glPushMatrix();

	glTranslated(-3.5, 0.0, 0.0);
	glScaled(1.0, 0.5, 1.0);
	DrawSphere(2, 10, 0.75, 0.25, 0.24);

	glPopMatrix();
}

void CGLRenderer::DrawSpider()
{
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, m_texSpider);
	glColor3f(1.0, 1.0, 1.0);

	glPushMatrix();
	DrawSpiderBody();
	glPopMatrix();

	// pipci

	glPushMatrix();

	glRotated(75, 0.0, 1.0, 0.0);
	
	for (int i = 0; i < 4; i++)
	{
		glRotated(-30, 0.0, 1.0, 0.0);
		glPushMatrix();
		glRotatef(45, 1.0, 0.0, 0.0);
		DrawLeg();
		glPopMatrix();
	}

	glPopMatrix();

	glPushMatrix();

	glRotated(180, 0.0, 1.0, 0.0);

	glRotated(75, 0.0, 1.0, 0.0);

	for (int i = 0; i < 4; i++)
	{
		glRotated(-30, 0.0, 1.0, 0.0);
		glPushMatrix();
		glRotatef(45, 1.0, 0.0, 0.0);
		DrawLeg();
		glPopMatrix();
	}

	glPopMatrix();

	glDisable(GL_TEXTURE_2D);
}
void GLRenderer::DrawCylinder(float r, float h, float nr, float nh, bool axes)
{
    float texStepY = 1.0 / nh;
    float texStepX = 1.0 / nr;
    float texY = 0;
    float angStep = (2 * piconst) / nr; //za x i z(i nalazenje normala)
   // float yStep = h / nh;
    //float startY = (((int)nh % 2) == 0) ? (((int)nh / 2) * yStep) : (((int)nh / 2) + 0.5) * yStep;
   // float currY = startY;
    for (int i = 0; i < nh; i++)
    {
        float currAng = 0;
        float texX = 0;
        glBegin(GL_QUAD_STRIP);
        {
            for (int j = 0; j < nr + 1; j++)
            {
                float x = r * cos(currAng);
                float z = -r * sin(currAng);
                float nx = x / r;
                float nz = z / r;
                glNormal3f(nx, 0, nz);
                glTexCoord2f(texX, texY);
                glVertex3f(x, h/2, z);
                glTexCoord2f(texX, texY + texStepY);
                glVertex3f(x, -h/2, z);
                texX += texStepX;
                currAng += angStep;
            }
        }
        glEnd();
        texY += texStepY;
       // currY -= yStep;
    }

    glBegin(GL_TRIANGLE_FAN);
    {
        glNormal3f(0.0, 1.0, 0.0);
        glTexCoord2f(0.5, 0.5);
        glVertex3f(0, h / 2, 0);
        float currAng = 0;
        for (int i = 0; i < nr + 1; i++)
        {
            float x = r * cos(currAng);
            float z = -r * sin(currAng);
            float texX = 0.5 + 0.5 * cos(currAng); //zasto ovo prvo 0.5 + za x i - za y?? i sto uopste koristimo 0.5???
            texY = 0.5 - 0.5 * sin(currAng);
            glTexCoord2f(texX, texY);
            glVertex3f(x, h / 2, z);
            currAng += angStep;
        }
    }
    glEnd();

    glBegin(GL_TRIANGLE_FAN);
    {
        glNormal3f(0.0, -1.0, 0.0);
        glTexCoord2f(0.5, 0.5);
        glVertex3f(0, -h / 2, 0);
        float currAng = 0;
        for (int i = 0; i < nr + 1; i++)
        {
            float x = r * cos(currAng);
            float z = r * sin(currAng);
            float texX = 0.5 + 0.5 * cos(currAng);
            texY = 0.5 - 0.5 * sin(currAng);
            glTexCoord2f(texX, texY);
            glVertex3f(x, -h / 2, z);
            currAng += angStep;
        }
    }
    glEnd();

    if(axes)
    {
        glDisable(GL_LIGHTING);
        glBegin(GL_LINES);
        {
            glColor3f(1.0, 0.0, 0.0);
            glVertex3f(0.0, 0.0, 0.0);
            glVertex3f(r + 1, 0.0, 0.0);

            glColor3f(0.0, 1.0, 0.0);
            glVertex3f(0.0, 0.0, 0.0);
            glVertex3f(0.0, h / 2 + 1, 0.0);

            glColor3f(0.0, 0.0, 1.0);
            glVertex3f(0.0, 0.0, 0.0);
            glVertex3f(0.0, 0.0, r + 1);
        }
        glEnd();
        glEnable(GL_LIGHTING);
    }
}

void GLRenderer::DrawTelescope(float r, float h, float nr, float nh)
{
    for (int i = 0; i < 3; i++)
    {
        glPushMatrix();
        glRotatef(i * 120, 0.0, 1.0, 0.0);
        glRotatef(alpha, 1.0, 0.0, 0.0);
        glTranslatef(0.0, -0.6 * h, 0.0);
        DrawCylinder(0.1 * r, 1.2 * h, nr, nh, false);
        glPopMatrix();
    }

    glPushMatrix();
    glRotatef(-90, 0.0, 0.0, 1.0);
    glRotatef(angleHor, 1.0, 0.0, 0.0);
    glRotatef(angleVer, 0.0, 0.0, 1.0);
    glTranslatef(0.0, (h / 2) - dHolder, 0.0);
    DrawCylinder(r, h, nr, nh, true);
    glPushMatrix();
    glTranslatef(0.0, h / 2 + 0.4 * h - dHidden, 0.0);
    DrawCylinder(0.8 * r, 0.8 * h, nr, nh, false);
    glPopMatrix();
    glTranslatef(0.0, -h / 2 + dVizor, 0.0);
    glRotatef(90, 0.0, 0.0, 1.0);
    glTranslatef(0.0, (0.2 / 2.0) * h, 0.0);
    DrawCylinder(0.1 * r, 0.2 * h, nr, nh, false);
    glTranslatef(0.0, (0.2 / 2.0) * h + (0.1 / 2.0) * r, 0.0);
    glRotatef(-90, 0.0, 0.0, 1.0);
    DrawCylinder(0.1 * r, 0.3 * h, nr, nh, false);
    glPopMatrix();
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
