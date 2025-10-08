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
	//bela pozadina
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glEnable(GL_DEPTH_TEST);
	texture = LoadTexture("Kol2011.bmp");
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
	//fov 45
	gluPerspective(45.0, (double)w / h, 0.1, 2000);
	glMatrixMode(GL_MODELVIEW);
	//---------------------------------
	wglMakeCurrent(NULL, NULL);
}
void CGLRenderer::DestroyScene(CDC* pDC)
{
	wglMakeCurrent(pDC->m_hDC, m_hrc);
	// brisanje teksture
	glDeleteTextures(1, &texture);
	wglMakeCurrent(NULL, NULL);
	if (m_hrc)
	{
		wglDeleteContext(m_hrc);
		m_hrc = NULL;
	}
}
UINT CGLRenderer::LoadTexture(char* name)
{
	UINT textureID;
	DImage img;
	img.Load(CString(name));

	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, img.Width(), img.Height(),
		GL_BGRA_EXT, GL_UNSIGNED_BYTE, img.GetDIBBits());

	return textureID;
}
#pragma endregion
void CGLRenderer::DrawScene(CDC *pDC)
{
	wglMakeCurrent(pDC->m_hDC, m_hrc);
	//---------------------------------
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	glLoadIdentity();
	//tackasti izvor,iza posmatraca
	float light_position[] = { 0.5,0.5,1.0, 1.0 };
	if(light)
		glEnable(GL_LIGHT0);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	glEnable(GL_LIGHTING);

	gluLookAt(0, 0, 10, 0, 0, 0, 0, 1, 0);
	//rotiramo pogled
	glRotated(viewAngle, 0, 1.0, 0);
	glRotated(viewAngleY, 1.0, 0, 0);

	glColor3f(1.0, 1.0, 1.0);
	DrawCompoundBody();
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture);
	DrawWings();

	glFlush();
	SwapBuffers(pDC->m_hDC);
	//---------------------------------
	wglMakeCurrent(NULL, NULL);
}
#pragma region Brod
void CGLRenderer::DrawBody(double r, double h1, double h2, int n)
{
	double piramidHeight = (h1 - h2)/2;
	//telo
	DrawCilindar(r, h2, n);
	//posle toga crtamo piramide na vrhu i dnu
	glPushMatrix();
	glTranslated(0.0, h2, 0.0);
	//na vrhu
	DrawCone(r, piramidHeight, n);
	glPopMatrix();

	glPushMatrix();
	glRotated(180.0, 1.0, 0.0, 0.0);
	//na dnu
	DrawCone(r, piramidHeight, n);
	glPopMatrix();
}
void CGLRenderer::DrawCilindar(double r, double h, int step)
{
	double alfa = 0.0;
	double alfaStep = (2*pi) / step; // kao i prethodni
	double texture = 0.0;
	double textStep = 1.0 / step;

	double textStartX = 0.25;
	double textStartY = 0;
	double textEndX = 0.5;
	double textEndY = -0.5;
	double textStepX = (textEndX - textStartX) / step;
	double textStepY = (textEndY - textStartY) / step;
	
	glBegin(GL_QUAD_STRIP);
	for (int i = 0; i <= step; i++)
	{
		//gornji cvor
		glNormal3f(cos(alfa), 0, sin(alfa)); // normala
		glTexCoord2f(textStartX, texture);
		glVertex3f(r * cos(alfa), h, r * sin(alfa));
		//cvor dole
		glNormal3f(cos(alfa), 0, sin(alfa)); // normala
		glTexCoord2f(textEndX, texture);
		glVertex3f(r * cos(alfa), 0.0, r * sin(alfa));

		alfa += alfaStep;
		texture += textStartY;
	}
	glEnd();
}
void CGLRenderer::DrawCone(double r, double h, int step)
{
	double alfa = 0.0;
	double alfaStep = (2 * pi) / step;
	double texture = 0.0;
	double textStep = 1.0 / step;

	double textureStartX = 0.125;
	double textureStartY = -0.25;
	//vrh
	glBegin(GL_TRIANGLE_FAN);
	glNormal3f(0.0, 1.0, 0.0);
	glTexCoord2f(textureStartX, textureStartY);
	glVertex3f(0.0, h, 0.0);

	for (int i = 0; i <= step; i++)
	{
		glNormal3f(cos(alfa), 0.0, sin(alfa));
		glTexCoord2f(textureStartX + 0.125 * cos(alfa), textureStartY + 0.25 * sin(alfa));
		glVertex3f(r * cos(alfa), 0.0, r * sin(alfa));
		
		alfa += alfaStep;
		texture += textStep;
	}
	glEnd();
}
void CGLRenderer::DrawCompoundBody()
{
	//najveci deo
	glEnable(GL_TEXTURE_2D);
	glPushMatrix();
	glRotated(90.0, 1.0, 0.0, 0.0);
	glTranslated(0.0, -1.0, 0.0);
	DrawBody(1.0, 2.5, 2.0, 8);
	glPopMatrix();

	//2 sa strane
	glDisable(GL_TEXTURE_2D);
	glPushMatrix();
	glRotated(-90.0, 0.0, 0.0, 1.0);
	glTranslated(0.0, -1.5, 0.0);
	DrawBody(0.5, 3.5, 3.0, 8);
	glPopMatrix();

	glPushMatrix();
	glRotated(-90.0, 0.0, 0.0, 1.0);
	glTranslated(0.0, -2.5, 0.0);
	DrawBody(0.25, 5.0, 5.0, 8);
	glPopMatrix();

	glEnable(GL_TEXTURE_2D);
}
void CGLRenderer::DrawWing()
{
	glPushMatrix();
	glTranslated(0, 0.75, 0);
	glRotated(-20.0, 1.0, 0, 0);
	glTranslated(0, -0.75, 0);
	
	glBegin(GL_TRIANGLES);
	glTexCoord2d(0, -0.5);
	glVertex3d(-4, 0.75, 0);
	glTexCoord2d(0.5, -0.5);
	glVertex3d(0, 0.75, 0);
	glTexCoord2d(0.5, -1.0);
	glVertex3d(0, 2.75, 0);
	glEnd();

	glBegin(GL_QUADS);
	glTexCoord2d(0.5, -0.5);
	glVertex3d(0, 0.75, 0);
	glTexCoord2d(0.75, -0.5);
	glVertex3d(2, 0.75, 0);
	glTexCoord2d(0.75, -1.0);
	glVertex3d(2, 2.75, 0);
	glTexCoord2d(0.5, -1.0);
	glVertex3d(0, 2.75, 0);
	glEnd();

	glPopMatrix();

	glBegin(GL_QUADS);
	glTexCoord2d(0.75, -0.5);
	glVertex3d(2, 0.75, 0);
	glTexCoord2d(0.5, -0.5);
	glVertex3d(0, 0.75, 0);
	glTexCoord2d(0.5, 0);
	glVertex3d(0, -0.75, 0);
	glTexCoord2d(0.75, 0);
	glVertex3d(2, -0.75, 0);
	glEnd();

	glPushMatrix();
	glScaled(1.0, -1.0, 1.0);
	glTranslated(0, 0.75, 0);
	glRotated(-20.0, 1.0, 0, 0);
	glTranslated(0, -0.75, 0);

	glBegin(GL_TRIANGLES);
	glTexCoord2d(0, -0.5);
	glVertex3d(-4, 0.75, 0);
	glTexCoord2d(0.5, -0.5);
	glVertex3d(0, 0.75, 0);
	glTexCoord2d(0.5, -1.0);
	glVertex3d(0, 2.75, 0);
	glEnd();

	glBegin(GL_QUADS);
	glTexCoord2d(0.5, -0.5);
	glVertex3d(0, 0.75, 0);
	glTexCoord2d(0.75, -0.5);
	glVertex3d(2, 0.75, 0);
	glTexCoord2d(0.75, -1.0);
	glVertex3d(2, 2.75, 0);
	glTexCoord2d(0.5, -1.0);
	glVertex3d(0, 2.75, 0);
	glEnd();

	glPopMatrix();
}
void CGLRenderer::DrawWings()
{
	glPushMatrix();
	glTranslated(2.6, 0.0, 0.0);
	glRotated(90.0, 0.0, 1.0, 0.0);
	glTranslated(-1.0, 0, 0);
	DrawWing();
	glPopMatrix();

	glPushMatrix();
	glRotated(180, 0, 0, 1.0);
	glTranslated(2.6, 0.0, 0.0);
	glRotated(90.0, 0.0, 1.0, 0.0);
	glTranslated(-1.0, 0, 0);
	DrawWing();
	glPopMatrix();
}
#pragma endregion