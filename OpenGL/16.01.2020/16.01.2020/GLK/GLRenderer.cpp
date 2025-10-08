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

