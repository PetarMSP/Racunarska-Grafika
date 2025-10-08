#include "StdAfx.h"
#include "GLRenderer.h"
#include "GL\gl.h"
#include "GL\glu.h"
#include "GL\glaux.h"
#include "GL\glut.h"
//#pragma comment(lib, "GL\\glut32.lib")

CGLRenderer::CGLRenderer(void)
{
}

CGLRenderer::~CGLRenderer(void)
{
}
#pragma region osnovne
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
	glClearColor(1.0, 1.0, 1.0, 0.0);
	glEnable(GL_DEPTH_TEST);
	SetLightModel();
	PrepareTextures();
	//---------------------------------
	wglMakeCurrent(NULL, NULL);
}
void CGLRenderer::PrepareTextures()
{
	//ucitavamo teksture
	T[0] = LoadTexture("Eiffel-1a.bmp");
	T[1] = LoadTexture("Eiffel-2a.bmp");
	T[2] = LoadTexture("Eiffel-3a.bmp");
	T[3] = LoadTexture("Eiffel-4a.bmp");
}
void CGLRenderer::Reshape(CDC* pDC, int w, int h)
{
	wglMakeCurrent(pDC->m_hDC, m_hrc);
	//---------------------------------
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	//FOV 40
	gluPerspective(40, (double)w / (double)h, 1, 100);
	glMatrixMode(GL_MODELVIEW);
	//---------------------------------
	wglMakeCurrent(NULL, NULL);
}
void CGLRenderer::DestroyScene(CDC* pDC)
{
	wglMakeCurrent(pDC->m_hDC, m_hrc);
	// obavezno brisanje tekstura
	glDeleteTextures(4, T);
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
	gluLookAt(40, 25, 30, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

	float light_position[] = { 100.0, 70.0, 90.0, 1.0 };
	float spot_direction[] = { -1.0, -1.0, -1.0 };
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, spot_direction);
	//---------------------------------
	DrawAxis(100);
	glPushMatrix();
	//malo pomerimo ka nama
	glTranslatef(0,0,10);
	glEnable(GL_TEXTURE_2D);

	//Donji deo tornja
	FillVA(buff1, 4.8, 8, 3.0); // b = 2.4 , h = 1.5 , a = 4
	FillVATex(bufft1, 0.215, 0); // x1  = 0.215 , x2 =0
	//ucitamo prvu teksturu
	glBindTexture(GL_TEXTURE_2D,T[0]);
	//DrawPyramid(buff,bufft);
	glTranslatef(10, 0.0, 0);
	glRotatef(rotTower, 0.0, 1.0, 0.0);
	DrawPyramid(4.8, 8, 3.0, bufft1);

	//terasica donja
	FillVATex(bufft4, 0.0, 0.0);
	glBindTexture(GL_TEXTURE_2D, T[3]);
	glTranslatef(0, 1.5, 0);
	//sada treba naopako da se nacrta,zato je prvo 5.28 pa 4.8 , prva vrednost ako je veca,onda je donji deo gore
	DrawPyramid(5.28, 4.8, 0.5, bufft4);

	//dalje crtamo 2 deo ,isto kao prvi,samo manji za pola
	FillVATex(bufft2, 0.215,0);  // x1  = 0.215 , x2 =0
	glBindTexture(GL_TEXTURE_2D, T[1]);
	glTranslatef(0, 1.5, 0);
	DrawPyramid(2.4, 4.8, 3, bufft2);

	//gornja terasica
	FillVATex(bufft4, 0.0, 0.0);
	glBindTexture(GL_TEXTURE_2D, T[3]);
	glTranslatef(0, 1.5, 0);
	//sada treba naopako da se nacrta,zato je prvo 5.28 pa 4.8 , prva vrednost ako je veca,onda je donji deo gore
	DrawPyramid(2.69, 2.4, 0.5, bufft4);

	// i poslednji deo,onaj duzi
	FillVATex(bufft3, 0.45, 0.05);  // x1  = 0.215 , x2 =0
	glBindTexture(GL_TEXTURE_2D, T[2]);
	glTranslatef(0, 8, 0);
	DrawPyramid(0, 2.16, 16, bufft3); // visina veca

	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
	glFlush();
	//---------------------------------
	SwapBuffers(pDC->m_hDC);
	wglMakeCurrent(NULL, NULL);
}
UINT CGLRenderer::LoadTexture(char* fileName)
{
	UINT texID;
	DImage img;
	img.Load(CString(fileName));

	glPixelStorei(GL_UNPACK_ALIGNMENT,4);
	glGenTextures(1,&texID);

	glBindTexture(GL_TEXTURE_2D,texID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE, GL_MODULATE);

	gluBuild2DMipmaps(GL_TEXTURE_2D,GL_RGBA,img.Width(),img.Height(),
		GL_BGRA_EXT, GL_UNSIGNED_BYTE,img.GetDIBBits());
	return texID;
}

//Parametar x1 definiše s teksturnu koordinatu gornjeg levog temena zarubljene piramide, a parametar x2 donjeg levog temena
void CGLRenderer::FillVATex(float* buff, float x1, float x2)
{
	int count = 0;
	for (int i = 0; i < 4; i++)
	{
		buff[count++] = 0;
		buff[count++] = x1;
		buff[count++] = 1;
		buff[count++] = x2;
		buff[count++] = 1; 
		buff[count++] = 1.0 - x2;
		buff[count++] = 0; 
		buff[count++] = 1.0 - x1;
	}
}
void CGLRenderer::FillVA(float* buff, float a, float b, float h)
{
	//ovu funkciju pisemo na osnovu DrawPyramid.

	//Podesavamo koordinate za teksture
	float x = a / 2.0;
	float y = h / 2.0;
	float z = b / 2.0;
	int count = 0;
	//donja stranica x,delimo sa 2 , jer crtamo iz centra

	//prednja
	//prva tacka za Texture
	buff[count++] = -x;
	buff[count++] = y;
	//a trecu koristimo za Vertex
	buff[count++] = x;

	buff[count++] = -z;
	buff[count++] = -y;
	buff[count++] = z;
	buff[count++] = z;
	buff[count++] = -y;
	buff[count++] = z;
	buff[count++] = x; 
	buff[count++] = y; 
	buff[count++] = x;

	//desna
	buff[count++] = x;
	buff[count++] = y;
	buff[count++] = x;
	buff[count++] = z;
	buff[count++] = -y;
	buff[count++] = z;
	buff[count++] = z; 
	buff[count++] = -y;
	buff[count++] = -z;
	buff[count++] = x; 
	buff[count++] = y;
	buff[count++] = -x;

	//zadnja
	buff[count++] = x;
	buff[count++] = y; 
	buff[count++] = -x;
	buff[count++] = z; 
	buff[count++] = -y;
	buff[count++] = -z;
	buff[count++] = -z; 
	buff[count++] = -y;
	buff[count++] = -z;
	buff[count++] = -x; 
	buff[count++] = y; 
	buff[count++] = -x;

	//leva
	buff[count++] = -x;
	buff[count++] = y;
	buff[count++] = -x;
	buff[count++] = -z; 
	buff[count++] = -y;
	buff[count++] = -z;
	buff[count++] = -z; 
	buff[count++] = -y; 
	buff[count++] = z;
	buff[count++] = -x; 
	buff[count++] = y;
	buff[count++] = x;
}
void CGLRenderer::DrawPyramid(double a, double b, double h, float* bufft)
{
	double x = a / 2.0;
	double y = h / 2.0; // visina
	double z = b / 2.0;
	int countt = 0;
	int count = 0;
	SetMaterial();

	glBegin(GL_QUADS);
	glNormal3f(0.0, 0.0, 1.0);// sa prednje strane crtamo
	glTexCoord2f(bufft[countt++], bufft[countt++]); // donja
	glVertex3d(-x, y, x);
	glTexCoord2f(bufft[countt++], bufft[countt++]); // gornja
	glVertex3d(-z, -y, z);
	glTexCoord2f(bufft[countt++], bufft[countt++]); // gornja
	glVertex3d(z, -y, z); 
	glTexCoord2f(bufft[countt++], bufft[countt++]); // donja
	glVertex3d(x, y, x);

	//desna strana, po x , ide na desno dakle
	glNormal3f(1.0, 0.0, 0.0);
	glTexCoord2f(bufft[countt++], bufft[countt++]);
	glVertex3d(x, y, x);
	glTexCoord2f(bufft[countt++], bufft[countt++]);
	glVertex3d(z, -y, z);
	glTexCoord2f(bufft[countt++], bufft[countt++]);
	glVertex3d(z, -y, -z);
	glTexCoord2f(bufft[countt++], bufft[countt++]);
	glVertex3d(x, y, -x);

	//zadnja
	glNormal3f(0.0, 0.0, -1.0); // -z osa
	glTexCoord2f(bufft[countt++], bufft[countt++]);
	glVertex3d(x, y, -x);
	glTexCoord2f(bufft[countt++], bufft[countt++]);
	glVertex3d(z, -y, -z);
	glTexCoord2f(bufft[countt++], bufft[countt++]);
	glVertex3d(-z, -y, -z);
	glTexCoord2f(bufft[countt++], bufft[countt++]);
	glVertex3d(-x, y, -x);

	//leva
	glNormal3f(-1.0, 0.0, 0.0); // -X osa
	glTexCoord2f(bufft[countt++], bufft[countt++]);
	glVertex3d(-x, y, -x);
	glTexCoord2f(bufft[countt++], bufft[countt++]);
	glVertex3d(-z, -y, -z);
	glTexCoord2f(bufft[countt++], bufft[countt++]);
	glVertex3d(-z, -y, z);
	glTexCoord2f(bufft[countt++], bufft[countt++]);
	glVertex3d(-x, y, x);
	glEnd();
}
void CGLRenderer::DrawAxis(double length)
{
	glBegin(GL_LINES);
	glColor3f(1.0, 0.0, 0.0);
	glVertex3f(0.0, 0.0, 0.0);
	glVertex3f(length, 0.0, 0.0);
	glColor3f(0.0, 1.0, 0.0);
	glVertex3f(0.0, 0.0, 0.0);
	glVertex3f(0.0, length, 0.0);
	glColor3f(0.0, 0.0, 1.0);
	glVertex3f(0.0, 0.0, 0.0);
	glVertex3f(0.0, 0.0, length);
	glEnd();
}
//podesimo materijal i svetlost
void CGLRenderer::SetMaterial()
{
	GLfloat mat_ambient[] = { 0.75, 0.75, 0.75, 1.0 };
	GLfloat mat_diffuse[] = { 0.9, 0.9, 0.7, 1.0 };
	GLfloat mat_specular[] = { 0.1, 0.1, 0.1, 1.0 };
	GLfloat mat_emission[] = { 0.0, 0.0, 0.0, 0.0 };
	GLfloat mat_shininess = 0.3; // malo smo mu dali odsjaj

	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_EMISSION, mat_emission);
	glMaterialf(GL_FRONT, GL_SHININESS, mat_shininess);
}
void CGLRenderer::SetLightModel()
{
	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_FALSE);
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);

	float light_ambient[] = { 0.5, 0.5, 0.5, 1.0 };
	float light_diffuse[] = { 0.2, 0.2, 0.2, 1.0 };
	float light_specular[] = { 0.8, 0.7, 0.7, 1.0 };

	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);

	glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, 1.0);
	glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 45.0);
	glLightf(GL_LIGHT0, GL_SPOT_EXPONENT, 2.0);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
}