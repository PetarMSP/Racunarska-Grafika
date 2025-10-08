#include "StdAfx.h"
#include "GLRenderer.h"
#include "GL\gl.h"
#include "GL\glu.h"
#include "GL\glaux.h"
#include "GL\glut.h"
//#pragma comment(lib, "GL\\glut32.lib")
CGLRenderer::CGLRenderer(void)
{
	alfa = 0;
	beta = 0;
	distance = 10;
	light = false;
	lookX = 0;
	lookY = 0;
	lookZ = 0;
}
CGLRenderer::~CGLRenderer(void)
{
}
#pragma region fiksne funkcije
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
	glClearColor(1.0,1.0,1.0,1.0);
	glEnable(GL_DEPTH_TEST);
	PrepareTextures();
	//---------------------------------
	wglMakeCurrent(NULL, NULL);
}
void CGLRenderer::PrepareTextures()  // takozvana PrepareScene
{
	//ucitavanje tekstura
	ship = LoadTexture("ShipT1.png");
	textureSpace[0] = LoadTexture("front.jpg");
	textureSpace[1] = LoadTexture("left.jpg");
	textureSpace[2] = LoadTexture("right.jpg");
	textureSpace[3] = LoadTexture("back.jpg");
	textureSpace[4] = LoadTexture("top.jpg");
	textureSpace[5] = LoadTexture("bot.jpg");

	glEnable(GL_TEXTURE_2D);
	wglMakeCurrent(NULL,NULL);
}
void CGLRenderer::Reshape(CDC* pDC, int w, int h)
{
	wglMakeCurrent(pDC->m_hDC, m_hrc);
	//---------------------------------
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(50.0, (double)w / (double)h, 1, 100);
	glMatrixMode(GL_MODELVIEW);
	//---------------------------------
	wglMakeCurrent(NULL, NULL);
}
void CGLRenderer::DestroyScene(CDC* pDC)
{
	wglMakeCurrent(pDC->m_hDC, m_hrc);
	// obavezno brisanje ucitanih tekstura
	glDeleteTextures(1,&ship);
	glDeleteTextures(6,textureSpace);
	wglMakeCurrent(NULL, NULL);
	if (m_hrc)
	{
		wglDeleteContext(m_hrc);
		m_hrc = NULL;
	}
}
#pragma endregion
void CGLRenderer::SetLook(double x,double y,double z)
{
	lookX = x;
	lookY = y;
	lookZ = z;
}
void CGLRenderer::DrawScene(CDC *pDC)
{
	wglMakeCurrent(pDC->m_hDC, m_hrc);
	//---------------------------------
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	gluLookAt(5,5,5,lookX,lookY,lookZ,0,1,0);

	glDisable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);
	glEnable(GL_TEXTURE_2D);

	glPushMatrix();
	glRotated(beta,1.0,0.0,0.0);
	glRotated(alfa,0.0,1.0,0.0);
	DrawSpaceCube(50.0);
	glPopMatrix();

	glEnable(GL_DEPTH_TEST);
	//glTranslatef(0,0,-distance);
	glRotated(beta,1.0,0.0,0.0);
	glRotated(alfa,0.0,1.0,0.0);

	GLfloat light_position[] = {0.0,0.0,1.0,0.0};
	glLightfv(GL_LIGHT0,GL_POSITION,light_position);
	if (light)
	{
		glEnable(GL_LIGHT0);
		glEnable(GL_LIGHTING);
	}
	//axis:
	glLineWidth(2.0);
	glBegin(GL_LINES);
	glColor3f(1,0,0);
	glVertex3f(0,0,0);
	glVertex3f(10,0,0);
	glColor3f(0, 1, 0);
	glVertex3f(0, 0, 0);
	glVertex3f(0, 10, 0);
	glColor3f(0, 0, 1);
	glVertex3f(0, 0, 0);
	glVertex3f(0, 0, 10);
	glEnd();

	glRotated(90,1,0,0);
	glTranslated(-5.8/2.0,0,0);
	DrawShip();

	//---------------------------------
	glFlush();
	SwapBuffers(pDC->m_hDC);
	wglMakeCurrent(NULL, NULL);
}
UINT CGLRenderer::LoadTexture(char* fileName)
{
	UINT textureID;
	DImage image;
	image.Load(CString(fileName));

	glPixelStorei(GL_UNPACK_ALIGNMENT,4);
	glGenTextures(1,&textureID);
	glBindTexture(GL_TEXTURE_2D,textureID);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);

	//glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	//glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);
	gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, image.Width(), image.Height(),GL_BGRA_EXT,GL_UNSIGNED_BYTE,image.GetDIBBits());
	return textureID;
}
void CGLRenderer::DrawTriangle(float d1,float d2,float rep)
{
	double a1 = atan2(d2,d1);
	double d3 = sqrt(d1*d1+d2*d2);
	double y = d1 * cos(a1)/d3;
	double x = d1 * sin(a1) / d3;
	glBegin(GL_TRIANGLES);
	glColor3f(1.0,1.0,1.0);
	glNormal3f(0,0,1.0);
	glTexCoord2f(0.5*rep,0.0);
	glVertex3f(0.0,0.0,0.0);
	glTexCoord2f((0.5+x) * rep, y*rep);
	glVertex3f(d1, 0.0, 0.0);
	glTexCoord2f(0.5 * rep, rep);
	glVertex3f(d1, d2, 0.0);
	glEnd();
}
void CGLRenderer::DrawShip()
{
	//katete : 5.8 i 2.15
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D,ship);
	double a1 = atan2(2.15,5.8);
	//gornji levi:
	glPushMatrix();
	//hipotenuza nagnuta u odnosu na X osu
	glRotatef(-4.75,0,1,0);
	glRotatef(13,1,0,0);
	//bez -a1*180/3.14,crta suportno ugao
	glRotatef(-a1 * 180 / 3.14, 0, 0, 1);
	DrawTriangle(5.8,2.15,3);
	glPopMatrix();
	//gornji desno:
	glPushMatrix();
	glScalef(1,-1,1);
	glRotatef(-4.75,0,1,0);
	glRotatef(13,1,0,0);
	glRotatef(-a1*180/3.14,0,0,1);
	DrawTriangle(5.8, 2.15, 3.0);
	glPopMatrix();
	glPushMatrix();
	glRotatef(180,1,0,0);
	//levo
	glPushMatrix();
	glRotatef(-4.75,0,1,0);
	glRotatef(13,1,0,0);
	glRotatef(-a1*180/3.14,0,0,1);
	DrawTriangle(5.8,2.15,3.0);
	glPopMatrix();
	//desni:
	glPushMatrix();
	glScalef(1, -1, 1);
	glRotatef(-4.75, 0, 1, 0);
	glRotatef(13, 1, 0, 0);
	glRotatef(-a1*180/3.14, 0, 0, 1);
	DrawTriangle(5.8, 2.15, 3.0);
	glPopMatrix();
	glPopMatrix();
}
void CGLRenderer::DrawSpaceCube(double a)
{
	//zadnja desna
	glBindTexture(GL_TEXTURE_2D,textureSpace[0]);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0,1.0);
	glVertex3d(-a/2,a/2,-a/2);
	glTexCoord2f(0.0,0.0);
	glVertex3d(-a/2,-a/2,-a/2);
	glTexCoord2f(1.0, 0.0);
	glVertex3d(a/2,-a/2,-a/2);
	glTexCoord2f(1.0,1.0);
	glVertex3d(a/2,a/2,-a/2);
	glEnd();
	//zadnja leva
	
	glBindTexture(GL_TEXTURE_2D, textureSpace[1]);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0, 1.0);
	glVertex3d(-a / 2, a / 2, a / 2);
	glTexCoord2f(0.0, 0.0);
	glVertex3d(-a / 2, -a / 2, a / 2);
	glTexCoord2f(1.0, 0.0);
	glVertex3d(-a / 2, -a / 2, -a / 2);
	glTexCoord2f(1.0, 1.0);
	glVertex3d(-a / 2, a / 2, -a / 2);
	glEnd();
	//desna strana
	glBindTexture(GL_TEXTURE_2D, textureSpace[2]);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0, 1.0);
	glVertex3d(a/2, a / 2, -a / 2);
	glTexCoord2f(0.0, 0.0);
	glVertex3d(a / 2, -a / 2, -a / 2);
	glTexCoord2f(1.0, 0.0);
	glVertex3d(a / 2, -a / 2,a / 2);
	glTexCoord2f(1.0, 1.0);
	glVertex3d(a / 2, a / 2, a / 2);
	glEnd();

	//back,nama prednja leva
	glBindTexture(GL_TEXTURE_2D, textureSpace[3]);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0, 1.0);
	glVertex3d(a / 2, a / 2, a / 2);
	glTexCoord2f(0.0, 0.0);
	glVertex3d(a / 2, -a / 2, a / 2);
	glTexCoord2f(1.0, 0.0);
	glVertex3d(-a / 2, -a / 2, a / 2);
	glTexCoord2f(1.0, 1.0);
	glVertex3d(-a / 2, a / 2, a / 2);
	glEnd();

	//Gornja
	glBindTexture(GL_TEXTURE_2D, textureSpace[4]);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0, 1.0);
	glVertex3d(-a / 2, a / 2, a / 2);
	glTexCoord2f(0.0, 0.0);
	glVertex3d(-a / 2, a / 2,- a / 2);
	glTexCoord2f(1.0, 0.0);
	glVertex3d(a / 2, a / 2, -a / 2);
	glTexCoord2f(1.0, 1.0);
	glVertex3d(a / 2, a / 2, a / 2);
	glEnd();

	//donja
	glBindTexture(GL_TEXTURE_2D, textureSpace[1]);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0, 1.0);
	glVertex3d(-a / 2, -a / 2, a / 2);
	glTexCoord2f(0.0, 0.0);
	glVertex3d(a / 2, -a / 2, a / 2);
	glTexCoord2f(1.0, 0.0);
	glVertex3d(a / 2, -a / 2, -a / 2);
	glTexCoord2f(1.0, 1.0);
	glVertex3d(-a / 2, -a / 2, -a / 2);
	glEnd();
	glBindTexture(GL_TEXTURE_2D,0);

}