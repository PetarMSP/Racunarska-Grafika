#include "StdAfx.h"
#include "GLRenderer.h"
#include "GL\gl.h"
#include "GL\glu.h"
#include "GL\glaux.h"
#include "GL\glut.h"
#include "DImage.h"
//#pragma comment(lib, "GL\\glut32.lib")

CGLRenderer::CGLRenderer(void)
{
}

CGLRenderer::~CGLRenderer(void)
{
}
#pragma region osnovne funkcije
bool CGLRenderer::CreateGLContext(CDC* pDC)
{
	PIXELFORMATDESCRIPTOR pfd ;
   	memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));
   	pfd.nSize  = sizeof(PIXELFORMATDESCRIPTOR);
   	pfd.nVersion   = 1; 
   	pfd.dwFlags    = PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW;   
   	pfd.iPixelType = PFD_TYPE_RGBA; 
   	pfd.cColorBits = 32;
   	pfd.cDepthBits = 24; 
   	pfd.iLayerType = PFD_MAIN_PLANE;
	
	int nPixelFormat = ChoosePixelFormat(pDC->m_hDC, &pfd);
	
	if (nPixelFormat == 0) return false; 

	BOOL bResult = SetPixelFormat (pDC->m_hDC, nPixelFormat, &pfd);
  	
	if (!bResult) return false; 

   	m_hrc = wglCreateContext(pDC->m_hDC); 

	if (!m_hrc) return false; 

	return true;	
}
void CGLRenderer::PrepareScene(CDC *pDC)
{
	wglMakeCurrent(pDC->m_hDC, m_hrc);
	//---------------------------------
	glClearColor(0.0, 0.0, 0.0, 1.0); // crna pozadina
	glEnable(GL_DEPTH_TEST);
	//ucitamo teksture
	marble = LoadTexture(CString("Marble.bmp"));
	fabric = LoadTexture(CString("Fabric1.bmp"));
	metal1 = LoadTexture(CString("Metal1.bmp"));
	//---------------------------------
	wglMakeCurrent(NULL, NULL);
}
void CGLRenderer::Reshape(CDC* pDC, int w, int h)
{
	wglMakeCurrent(pDC->m_hDC, m_hrc);
	//---------------------------------
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	//FOV 45
	gluPerspective(45.0, (double)w / (double)h, 0.1, 2000);
	glMatrixMode(GL_MODELVIEW);
	//---------------------------------
	wglMakeCurrent(NULL, NULL);
}
void CGLRenderer::DestroyScene(CDC* pDC)
{
	wglMakeCurrent(pDC->m_hDC, m_hrc);
	// brisemo teksture
	glDeleteTextures(1, &marble);
	glDeleteTextures(1, &fabric);
	glDeleteTextures(1, &metal1);

	wglMakeCurrent(NULL, NULL);
	if (m_hrc)
	{
		wglDeleteContext(m_hrc);
		m_hrc = NULL;
	}
}
UINT CGLRenderer::LoadTexture(CString fileName)
{
	UINT textureID;
	DImage img;
	img.Load(fileName);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
	glGenTextures(1, &textureID);

	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, img.Width(), img.Height(),GL_BGRA_EXT, GL_UNSIGNED_BYTE, img.GetDIBBits());
	return textureID;
}
#pragma endregion
void CGLRenderer::DrawScene(CDC *pDC)
{
	wglMakeCurrent(pDC->m_hDC, m_hrc);
	//---------------------------------
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	glLoadIdentity();
	//postavljamo posmatraca
	gluLookAt(3, 2, 3, 0, 1, 0, 0, 1, 0); // y =1 , gleda u gornju povrsinu

	glRotated(rotation, 0.0, 1.0, 0.0);

	//Postavljamo poziciono svetlo
	//ostale vr. su podrazmevane
	float light_position[] = { 6,4,6,1.0 };
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHTING);
	//nakon podesavanja svetlosti,moze da se pocne crtanje

	Kocka(1.0);
	glTranslated(0.0, 0.5, 0.0);
	Telo(0.2, 0.1, 0.05, 1.0, 16);

	glTranslated(0.0, 1.0, 0.0);

	glBindTexture(GL_TEXTURE_2D, fabric);
	glEnable(GL_TEXTURE_2D);
	Abazur(0.5, 0.25, 0.2, 10);

	glFlush();
	SwapBuffers(pDC->m_hDC);
	//---------------------------------
	wglMakeCurrent(NULL, NULL);
}
void CGLRenderer::Kocka(double size)
{
	glBindTexture(GL_TEXTURE_2D, marble);
	glEnable(GL_TEXTURE_2D);

	double a = size / 2;
	//prednja
	glBegin(GL_QUADS);
	glNormal3f(0.0, 0.0, 1.0); // ka nama
	glTexCoord2f(0, 1);
	glVertex3f(-a,a,a);
	glNormal3f(0.0, 0.0, 1.0); 
	glTexCoord2f(0, 0);
	glVertex3f(-a, -a, a);
	glNormal3f(0.0, 0.0, 1.0); 
	glTexCoord2f(1, 0);
	glVertex3f(a, -a, a);
	glNormal3f(0.0, 0.0, 1.0);
	glTexCoord2f(1, 1);
	glVertex3f(a, a, a);
	glEnd();
	//desna
	glBegin(GL_QUADS);
	glNormal3f(1.0, 0.0, 0.0);
	glTexCoord2f(0, 1);
	glVertex3f(a, a, a);
	glNormal3f(1.0, 0.0, 0.0);
	glTexCoord2f(0, 0);
	glVertex3f(a, -a, a);
	glNormal3f(1.0, 0.0, 0.0);
	glTexCoord2f(1, 0);
	glVertex3f(a, -a, -a);
	glNormal3f(1.0, 0.0, 0.0);
	glTexCoord2f(1, 1);
	glVertex3f(a, a, -a);
	glEnd();
	//zadnja
	glBegin(GL_QUADS);
	glNormal3f(0.0, 0.0, -1.0);
	glTexCoord2f(0, 1);
	glVertex3f(a, a, -a);
	glNormal3f(0.0, 0.0, -1.0);
	glTexCoord2f(0, 0);
	glVertex3f(a, -a, -a);
	glNormal3f(0.0, 0.0, -1.0);
	glTexCoord2f(1, 0);
	glVertex3f(-a, -a, -a);
	glNormal3f(0.0, 0.0, -1.0);
	glTexCoord2f(1, 1);
	glVertex3f(-a, a, -a);
	glEnd();
	//leva
	glBegin(GL_QUADS);
	glNormal3f(-1.0, 0.0, 0.0);
	glTexCoord2f(0, 1);
	glVertex3f(-a, a, -a);
	glNormal3f(-1.0, 0.0, 0.0);
	glTexCoord2f(0, 0);
	glVertex3f(-a, -a, -a);
	glNormal3f(-1.0, 0.0, 0.0);
	glTexCoord2f(1, 0);
	glVertex3f(-a, -a, a);
	glNormal3f(-1.0, 0.0, 0.0);
	glTexCoord2f(1, 1);
	glVertex3f(-a, a, a);
	glEnd();
	//donja
	glBegin(GL_QUADS);
	glNormal3f(0.0, -1.0, 0.0);
	glTexCoord2f(0, 1);
	glVertex3f(a, -a, a);
	glNormal3f(0.0, -1.0, 0.0);
	glTexCoord2f(0, 0);
	glVertex3f(-a, -a, a);
	glNormal3f(0.0, -1.0, 0.0);
	glTexCoord2f(1, 0);
	glVertex3f(-a, -a, -a);
	glNormal3f(0.0, -1.0, 0.0);
	glTexCoord2f(1, 1);
	glVertex3f(a, -a, -a);
	glEnd();
	//gornja
	glBegin(GL_QUADS);
	glNormal3f(0.0, 1.0, 0.0);
	glTexCoord2f(0, 1);
	glVertex3f(a, a, a);
	glNormal3f(0.0, 1.0, 0.0);
	glTexCoord2f(0, 0);
	glVertex3f(a, a, -a);
	glNormal3f(0.0, 1.0, 0.0);
	glTexCoord2f(1, 0);
	glVertex3f(-a, a, -a);
	glNormal3f(0.0, 1.0, 0.0);
	glTexCoord2f(1, 1);
	glVertex3f(-a, a, a);
	glEnd();
	
	glDisable(GL_TEXTURE_2D);
}
void CGLRenderer::Abazur(double r1, double r2, double h, int step)
{
	//broj koraka aproksimacije,za 1
	double alfaStep = (2 * pi) / step; 
	double alfa = 0;
	double texture = 0;
	double textStep = 1.0 / step; // pomeranje za teksturu
	//kao valjak ,samo bez gornjeg i donjeg poklopca
	glBegin(GL_QUAD_STRIP);
	for (int i = 0; i <= step; i++)
	{
		//cvor gore
		glNormal3f(cos(alfa),0,sin(alfa)); // normala
		glTexCoord2f(texture, 1.0);
		glVertex3f(r2 * cos(alfa), h, r2 * sin(alfa));
		//cvor dole
		glNormal3f(cos(alfa), 0, sin(alfa)); // normala
		glTexCoord2f(texture, 0.0);
		glVertex3f(r1 * cos(alfa), 0.0, r1 * sin(alfa));

		alfa += alfaStep;
		texture += textStep;
	}
	glEnd();
}
void CGLRenderer::Telo(double r1, double r2, double r3, double h, int step)
{
	glBindTexture(GL_TEXTURE_2D, metal1);
	glEnable(GL_TEXTURE_2D);
	double hStep = h / 4;
	glPushMatrix();
	//jednostavnije nacrtati iz 4 dela
	//donji deo
	Abazur(r1, r2, hStep, step);
	glTranslated(0.0, hStep, 0.0);
	//pa na taj dodje jos jedan,koji je jos uzi
	Abazur(r2, r3, hStep, step);
	glTranslated(0.0, hStep, 0.0);
	//najuzi deo
	Abazur(r3, r3, hStep, step);
	glTranslated(0.0, hStep, 0.0);
	//i na kraju gornji deo
	Abazur(r3, r2, hStep, step);
	glPopMatrix();
}