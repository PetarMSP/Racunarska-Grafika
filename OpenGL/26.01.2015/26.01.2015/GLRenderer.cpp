#include "StdAfx.h"
#include "GLRenderer.h"
#include "GL\gl.h"
#include "GL\glu.h"
#include "GL\glaux.h"
#include "GL\glut.h"
//#pragma comment(lib, "GL\\glut32.lib")
CGLRenderer::CGLRenderer(void)
{
	//rotacija pogleda
	rot1 = 0; // po Y
	rot2 = 0; // po X
	//rotacija zemlje oko ose:
	//rotacija meseca oko ose:
	rotMesecX = 0;
	rotMesecY = 0;
	dist = 30;  // u zadatku trazi 50
	lighting = true;
}
CGLRenderer::~CGLRenderer(void)
{
}
#pragma region fiskne funkcije
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
	glClearColor(0.8,0.8,0.8,1.0);
	glEnable(GL_DEPTH_TEST);
	//SetLight();
	PrepareTextures();
	//---------------------------------
	wglMakeCurrent(NULL, NULL);
}
void CGLRenderer::PrepareTextures()  // takozvana PrepareScene
{
	//ucitavanje tekstura

	T[0] = LoadTexture("TSC0.jpg");
	T[1] = LoadTexture("TSC1.jpg");
	T[2] = LoadTexture("TSC2.jpg");
	T[3] = LoadTexture("TSC3.jpg");
	T[4] = LoadTexture("TSC4.jpg");
	T[5] = LoadTexture("TSC5.jpg");

	S[0] = LoadTexture("S0.jpg");
	S[1] = LoadTexture("S1.jpg");
	S[2] = LoadTexture("S2.jpg");
	S[3] = LoadTexture("S3.jpg");
	S[4] = LoadTexture("S4.jpg");
	S[5] = LoadTexture("S5.jpg");

	M[0] = LoadTexture("M0.jpg");
	M[1] = LoadTexture("M1.jpg");
	M[2] = LoadTexture("M2.jpg");
	M[3] = LoadTexture("M3.jpg");
	M[4] = LoadTexture("M4.jpg");
	M[5] = LoadTexture("M5.jpg");

	glEnable(GL_TEXTURE_2D);
}
void CGLRenderer::Reshape(CDC* pDC, int w, int h)
{
	wglMakeCurrent(pDC->m_hDC, m_hrc);
	//---------------------------------
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	//FOV 40 podeseno sa ovim 40
	gluPerspective(40.0, (double)w / (double)h, 1, 100);
	glMatrixMode(GL_MODELVIEW);
	//---------------------------------
	wglMakeCurrent(NULL, NULL);
}
void CGLRenderer::DestroyScene(CDC* pDC)
{
	wglMakeCurrent(pDC->m_hDC, m_hrc);
	// obavezno brisanje ucitanih tekstura
	glDeleteTextures(6, T);
	glDeleteTextures(6, S);
	glDeleteTextures(6, M);
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
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);

	glPushMatrix();
	glRotated(rot2, 1, 0, 0);
	glRotated(rot1, 0, 1, 0);
	DrawSpace(75, 20);
	glPopMatrix();
	// u centru nacrta zemlju,da bi 
	glEnable(GL_DEPTH_TEST); //da ne bi bile prozirne
	// 8. deo zad. Direkcioni izvor svetlosti
	//amb, dif, spe za light0	
	if (lighting)
	{
		glEnable(GL_LIGHT0);
		glEnable(GL_LIGHTING);
	}
	else
	{
		glDisable(GL_LIGHTING);
		glDisable(GL_LIGHT0);
	}
	
	GLfloat amb[] = { 1, 1, 1, 1.0 };
	GLfloat dif[] = { 1, 1, 1, 1 };
	GLfloat spe[] = { 1, 1, 1, 1 };
	GLfloat pos[] = { 0,0,1,0 }; // u pravcu pozitibne Z-ose
	glLightfv(GL_LIGHT0, GL_POSITION, pos);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, dif);
	glLightfv(GL_LIGHT0, GL_AMBIENT, amb);
	glLightfv(GL_LIGHT0, GL_SPECULAR, spe);



	glTranslated(0, 0, -dist); // udaljimo zemlju po Z osi,da bude manja,sa Q je priblizimo,odnosno sa E udaljimo
	glRotated(rot1+180, 0, 1, 0); // gore i dole rotacija zemlje
	glRotated(rot2+180 , 1, 0, 0); // na levo i desno rotacija

	// +180 stepeni stalvjamo jer crta zemlju naopako,pa da bi izgledala ok

	DrawEarth(3, 20);

	glTranslated(-15, 0, 0);
	glRotated(rotMesecX, 1, 0, 0);
	glRotated(rotMesecY,0,1,0);
	DrawMoon(1, 20);
	//---------------------------------
	
	//---------------------------------
	glFlush();
	SwapBuffers(pDC->m_hDC);
	wglMakeCurrent(NULL, NULL);
}
UINT CGLRenderer::LoadTexture(char* fileName)
{
	UINT textureID;
	DImage img;
	img.Load(CString(fileName));

	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	//da se poonavlja
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, img.Width(), img.Height(), GL_BGRA_EXT, GL_UNSIGNED_BYTE, img.GetDIBBits());
	return textureID;
}
void CGLRenderer::DrawPatch(double R, int n)
{
	double x = -1;
	double y = -1;
	double step = (double)2.0 / n;
	//za svaki podeok:
	for (int i = 0; i < n; i++)
	{
		glBegin(GL_QUAD_STRIP);
		for (int j =0 ; j <= n; j++)
		{
			//prelazak u polarne,formula sa blanketa
			double fi = atan(x);
			double teta = atan(y*cos(fi));

			//prelazak u dekartove:
			double x1 = cos(teta)*sin(fi);
			double z1 = cos(teta) * cos(fi);
			double y1 = sin(teta);
			//generise teksturne koordinate i normale
			glNormal3f(x1,y1,z1);
			glTexCoord2d((double)j/n,(double)i/n);
			glVertex3d(R*x1,R*y1,R*z1);

			//fi se ne menja
			teta = atan((y+step)*cos(fi));
			x1 = cos(teta) * sin(fi);
			z1 = cos(teta) * cos(fi);
			y1 = sin(teta);
			glNormal3d(x1, y1, z1);
			glTexCoord2d((double)(j) / n, (double)(i + 1) / n);
			glVertex3d(R * x1, R * y1, R * z1);

			x += step;
		}
		x = -1.0;
		y += step;
		glEnd();
	}
}
void CGLRenderer::DrawMoon(double R, int tes)
{
	glPushMatrix();
	glBindTexture(GL_TEXTURE_2D, M[0]);
	DrawPatch(R, tes);
	glRotated(90, 0, 1, 0);
	glBindTexture(GL_TEXTURE_2D, M[1]);
	DrawPatch(R, tes);
	glRotated(90, 0, 1, 0);
	glBindTexture(GL_TEXTURE_2D, M[2]);
	DrawPatch(R, tes);
	glRotated(90, 0, 1, 0);
	glBindTexture(GL_TEXTURE_2D, M[3]);
	DrawPatch(R, tes);
	glPopMatrix();

	glPushMatrix();
	glRotated(90, 1, 0, 0);
	glBindTexture(GL_TEXTURE_2D, M[4]);
	DrawPatch(R, tes);
	glRotated(180, 1, 0, 0);
	glBindTexture(GL_TEXTURE_2D, M[5]);
	DrawPatch(R, tes);
	glPopMatrix();
}
void CGLRenderer::DrawSpace(double R, int tes)
{
	glPushMatrix();
	glBindTexture(GL_TEXTURE_2D, S[0]);
	DrawPatch(R, tes);
	glRotated(90, 0, 1, 0);
	glBindTexture(GL_TEXTURE_2D, S[1]);
	DrawPatch(R, tes);
	glRotated(90, 0, 1, 0);
	glBindTexture(GL_TEXTURE_2D, S[2]);
	DrawPatch(R, tes);
	glRotated(90, 0, 1, 0);
	glBindTexture(GL_TEXTURE_2D, S[3]);
	DrawPatch(R, tes);
	glPopMatrix();

	glPushMatrix();
	glRotated(90, 1, 0, 0);
	glBindTexture(GL_TEXTURE_2D, S[4]);
	DrawPatch(R, tes);
	glRotated(180, 1, 0, 0);
	glBindTexture(GL_TEXTURE_2D, S[5]);
	DrawPatch(R, tes);
	glPopMatrix();

}
void CGLRenderer::DrawEarth(double R, int tes)
{
	// prvo matricu pushamo
	// pratimo redosled kako je zadatao,prvo prve 4 na desno
	glPushMatrix();
	glBindTexture(GL_TEXTURE_2D,T[0]);
	DrawPatch(R,tes);
	glRotated(90,0,1,0);
	glBindTexture(GL_TEXTURE_2D, T[1]);
	DrawPatch(R, tes);
	glRotated(90, 0, 1, 0);
	glBindTexture(GL_TEXTURE_2D, T[2]);
	DrawPatch(R, tes);
	glRotated(90, 0, 1, 0);
	glBindTexture(GL_TEXTURE_2D, T[3]);
	DrawPatch(R, tes);
	glPopMatrix();
	//za preostale 2 radimo sl:
	glPushMatrix();
	glRotated(90, 1, 0, 0);
	glBindTexture(GL_TEXTURE_2D, T[4]);
	DrawPatch(R, tes);
	glRotated(180, 1, 0, 0);
	glBindTexture(GL_TEXTURE_2D, T[5]);
	DrawPatch(R, tes);
	glPopMatrix();
}

void CGLRenderer::DrawAxis()
{
	glBegin(GL_LINES);
	glColor3f(0.0, 0.0, 0.0);
	glVertex3d(0, 0, 0);
	glVertex3d(100, 0, 0);
	glVertex3d(0, 0, 0);
	glVertex3d(0, 100, 0);
	glVertex3d(0, 0, 0);
	glVertex3d(0, 0, 100);

	glEnd();
}