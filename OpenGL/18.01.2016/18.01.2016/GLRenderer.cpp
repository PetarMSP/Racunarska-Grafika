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
	light = true;
}

CGLRenderer::~CGLRenderer(void)
{
}
#pragma region osnovno
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
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glEnable(GL_DEPTH_TEST);
	//teksture
	env = LoadTexture("env.png");
	brick = LoadTexture("brick.png");
	glEnable(GL_TEXTURE_2D);
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
	//FOV 40
	gluPerspective(40.0, (double)w / (double)h, 0.1, 2000);
	glMatrixMode(GL_MODELVIEW);
	//---------------------------------
	wglMakeCurrent(NULL, NULL);
}
UINT CGLRenderer::LoadTexture(char* filename)
{
	UINT textureID;
	DImage img;
	img.Load(CString(filename));
	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, img.Width(), img.Height(), GL_RGBA, GL_UNSIGNED_BYTE, img.GetDIBBits());
	return textureID;
}
void CGLRenderer::DestroyScene(CDC* pDC)
{
	wglMakeCurrent(pDC->m_hDC, m_hrc);
	// brisanje tekstura
	glDeleteTextures(1, &brick);
	glDeleteTextures(1, &env);

	wglMakeCurrent(NULL, NULL);
	if (m_hrc)
	{
		wglDeleteContext(m_hrc);
		m_hrc = NULL;
	}
}
#pragma endregion
void CGLRenderer::DrawScene(CDC *pDC)
{
	wglMakeCurrent(pDC->m_hDC, m_hrc);
	//---------------------------------
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	glLoadIdentity();
	gluLookAt(0,0,10,0,0,0,0,1,0);
	glDisable(GL_LIGHTING);
	glEnable(GL_TEXTURE_2D);
	/*
	glPushMatrix();
	glBegin(GL_LINES);
	//X-osa
	glColor3f(1.0,0.0,0.0);
	glVertex3f(0, 0, 0);
	glVertex3f(100, 0, 0);
	//Y-osa
	glColor3f(0.0, 1.0, 0.0);
	glVertex3f(0, 0, 0);
	glVertex3f(0, 100, 0);
	//Z-osa
	glColor3f(0.0, 0.0, 1.0);
	glVertex3f(0, 0, 0);
	glVertex3f(0, 0, 100);
	glEnd();
	glPopMatrix();*/

	//pogled oko centra
	glPushMatrix();
	glRotated(beta, 1.0, 0.0, 0.0);
	glRotated(alfa, 0.0, 1.0, 0.0);

	DrawCube(100);
	if (light)
	{
		glEnable(GL_LIGHTING);
		glEnable(GL_LIGHT0);
	}
	glTranslated(0.0, -10, -10);
	GLfloat position[] = { 0.0, 0.0, 1.0, 0.0 };
	float ambient[] = { 0.2, 0.2, 0.2, 1.0 };
	float diffuse[] = { 0.5, 0.2, 1.0,1.0 };
	float specular[] = { 1.0, 1.0, 1.0, 1.0 };
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
	glLightfv(GL_LIGHT0, GL_POSITION, position);

	DrawTube(3.5, 2.5, 10.0, 32); // 32 segm omotac valjka, visina 10

	glColor3f(1.0, 1.0, 1.0);
	glTranslated(0.0, 10.0, 0.0);
	DrawCone(2.8, 2, 32);
	glTranslated(0.0, 0.0, 3.5);
	glRotated(windRotation, 0.0, 0.0, 1.0);
	for (int i = 0; i < 4; i++)
	{
		DrawPaddle(8, 1.5);
		glRotated(90.0, 0.0, 0.0, 1.0);
	}

	glPopMatrix();

	glFlush();
	SwapBuffers(pDC->m_hDC);
	//---------------------------------
	wglMakeCurrent(NULL, NULL);
}
void CGLRenderer::DrawCube(double a)
{
	//enable teksture
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, env);

	//crtamo quad
	glBegin(GL_QUADS);
	//zadnja, 
	glTexCoord2f(0.25, 1.0);
	glVertex3d(-a / 2, a / 2, -a / 2);
	glTexCoord2f(0.25, 0.67);
	glVertex3d(-a / 2, -a / 2, -a / 2);
	glTexCoord2f(0.5, 0.67); 
	glVertex3d(a / 2, -a / 2, -a / 2);
	glTexCoord2f(0.5, 1.0);
	glVertex3d(a / 2, a / 2, -a / 2);
	//leva
	glTexCoord2f(0.0, 0.33);
	glVertex3d(-a / 2, a / 2, a / 2);
	glTexCoord2f(0.25, 0.33);
	glVertex3d(-a / 2, -a / 2, a / 2);
	glTexCoord2f(0.25, 0.67);
	glVertex3d(-a / 2, -a / 2, -a / 2);
	glTexCoord2f(0.0, 0.67); 
	glVertex3d(-a / 2, a / 2, -a / 2);
	
	//desna
	glTexCoord2f(0.75, 0.67);
	glVertex3d(a / 2, a / 2, -a / 2);
	glTexCoord2f(0.5, 0.67);
	glVertex3d(a / 2, -a / 2, -a / 2);
	glTexCoord2f(0.5, 0.33);
	glVertex3d(a / 2, -a / 2, a / 2);
	glTexCoord2f(0.75, 0.33);
	glVertex3d(a / 2, a / 2, a / 2);
	
	//prednja
	glTexCoord2f(0.5, 0.0); 
	glVertex3d(a / 2, a / 2, a / 2);
	glTexCoord2f(0.5, 0.33); 
	glVertex3d(a / 2, -a / 2, a / 2);
	glTexCoord2f(0.25, 0.33);
	glVertex3d(-a / 2, -a / 2, a / 2);
	glTexCoord2f(0.25, 0.0); 
	glVertex3d(-a / 2, a / 2, a / 2);

	//donja
	glTexCoord2f(0.25, 0.67);
	glVertex3d(-a / 2, -a / 2, -a / 2);
	glTexCoord2f(0.25, 0.33);
	glVertex3d(-a / 2, -a / 2, a / 2);
	glTexCoord2f(0.5, 0.33);
	glVertex3d(a / 2, -a / 2, a / 2);
	glTexCoord2f(0.5, 0.67); 
	glVertex3d(a / 2, -a / 2, -a / 2);
	
	//gornja
	glTexCoord2f(0.75, 0.33); 
	glVertex3d(a / 2, a / 2, a / 2);
	glTexCoord2f(1.0, 0.33);
	glVertex3d(-a / 2, a / 2, a / 2);
	glTexCoord2f(1.0, 0.67); 
	glVertex3d(-a / 2, a / 2, -a / 2);
	glTexCoord2f(0.75, 0.67);
	glVertex3d(a / 2, a / 2, -a / 2);
	
	glEnd();
}
void CGLRenderer::DrawCone(double r, double h, int n)
{
	glDisable(GL_TEXTURE_2D);

	double alfaStep = (2 * 3.14) / n;
	double alfa = 0.0;

	glBegin(GL_TRIANGLE_FAN);
	//vrh
	glNormal3f(0.0, 1.0, 0.0);
	glVertex3f(0.0, h, 0.0);
	for (int i = 0; i <= n; i++)
	{
		glNormal3f(cos(alfa), 0.0, sin(alfa));
		glVertex3f(r*cos(alfa), 0.0, r*sin(alfa));
		alfa += alfaStep;
	}
	glEnd();
}
void CGLRenderer::DrawTube(double r1, double r2, double h, int n)
{
	setMaterial();
	//valjak
	double alfastep = (2 * 3.14) / n;
	double alfa = 0.0;
	double textstep = 1.0 / n;
	double textpos = 0.0;

	//gornji poklopac manji precnik r2
	glBegin(GL_POLYGON);
	while (alfa <= 2*3.14) {
		alfa = alfa + alfastep;
		glVertex3f(r2* cos(alfa), h, r2 * sin(alfa));
	}
	glEnd();
	alfa = 0.0;
	//donji poklopac veci precnik r1
	glBegin(GL_POLYGON);
	while (alfa <= 2 * 3.14) {
		alfa = alfa + alfastep;
		glVertex3f(r1 *cos(alfa), 0.0, r1 * sin(alfa));
	}
	glEnd();

	//OMOTAC
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, brick);
	glBegin(GL_QUAD_STRIP);
	for (int i = 0; i <= n; i++)
	{
		glNormal3f(cos(alfa), 0.0, sin(alfa));
		glTexCoord2f(textpos, 1.0);
		glVertex3f(r2*cos(alfa), h, r2*sin(alfa));

		glNormal3f(cos(alfa), 0.0, sin(alfa));
		glTexCoord2f(textpos, 0.0);
		glVertex3f(r1*cos(alfa), 0.0, r1*sin(alfa));
		textpos += textstep;
		alfa += alfastep;
	}
	glEnd();
}
void CGLRenderer::DrawPaddle(double length, double widht)
{
	//krilo vetrenjace
	setMaterial();
	glDisable(GL_TEXTURE_2D);
	double w8 = widht / 8;
	double l8 = length / 8;
	glBegin(GL_QUADS);
	//mali deo
	glVertex3d(0.0, 0.0, 0.0);
	glVertex3d(0.0, -w8, 0.0);
	glVertex3d(l8, -w8, 0.0);
	glVertex3d(l8, 0.0, 0.0);
	//kosi deo
	glVertex3d(l8, 0.0, 0.0);
	glVertex3d(l8, -w8, 0.0);
	glVertex3d(2 * l8, -widht, 0.0);
	glVertex3d(2 * l8, 0.0, 0.0);
	//veliki deo,duzi
	glVertex3d(2 * l8, 0.0, 0.0);
	glVertex3d(2 * l8, -widht, 0.0);
	glVertex3d(length, -widht, 0.0);
	glVertex3d(length, 0.0, 0.0);
	glEnd();
}
void CGLRenderer::setMaterial()
{
	//svetlo sive
	GLfloat mat_ambient[] = { 0.6, 0.6, 0.6, 1.0 };
	GLfloat mat_diffuse[] = { 0.1, 0.1, 0.7, 1.0 };
	GLfloat mat_specular[] = { 0.1, 1.0, 0.0, 1.0 };
	GLfloat mat_emission[] = { 0.5, 0.5, 0.5, 0.0 };
	GLfloat mat_shininess = 0.5; // malo smo mu dali odsjaj

	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_EMISSION, mat_emission);
	glMaterialf(GL_FRONT, GL_SHININESS, mat_shininess);
}