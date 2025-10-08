#include "StdAfx.h"
#include "GLRenderer.h"

#include "GL\gl.h"
#include "GL\glu.h"
#include "GL\glaux.h"
#include "GL\glut.h"

#include "DImage.h"

CGLRenderer::CGLRenderer()
{
	ugaoPosmatraca = 0;
	ugao[0] = 0;
	ugao[1] = 0;
	ugao[2] = 0;
}
CGLRenderer::~CGLRenderer()
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
	//podesavamo pozadinu
	glClearColor(0.7, 0.7, 0.7, 1.0);
	glEnable(GL_DEPTH_TEST);

	//glEnable(GL_CULL_FACE);
	//glCullFace(GL_BACK);

	SetWhiteLight();
	LoadTexture();
	//---------------------------------
	wglMakeCurrent(NULL, NULL);
}
void CGLRenderer::LoadTexture()
{
	UINT textureID;

	DImage slika;
	slika.Load(L"OpenGL.bmp");
	glEnable(GL_TEXTURE_2D);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	texture = textureID;
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	gluBuild2DMipmaps(GL_TEXTURE_2D, 4, slika.Width(), slika.Height(), GL_BGRA_EXT, GL_UNSIGNED_BYTE, slika.GetDIBBits());
}
void CGLRenderer::Reshape(CDC* pDC, int w, int h)
{
	wglMakeCurrent(pDC->m_hDC, m_hrc);
	//---------------------------------
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	//FOV 40
	gluPerspective(40.0, (double)w / h, 0.1, 100.0);
	glMatrixMode(GL_MODELVIEW);
	//---------------------------------
	wglMakeCurrent(NULL, NULL);
}
void CGLRenderer::DestroyScene(CDC* pDC)
{
	wglMakeCurrent(pDC->m_hDC, m_hrc);

	glDeleteTextures(1, &texture);

	wglMakeCurrent(NULL, NULL);
	if (m_hrc)
	{
		wglDeleteContext(m_hrc);
		m_hrc = NULL;
	}
}
#pragma endregion
//modifikovano
void CGLRenderer::DrawCube(float a, int i, int j, int k, float texPom)
{
	glBegin(GL_QUADS);

	// donja
	SetMaterial(1.0, 1.0, 0.3);
	//ispod normale
	glNormal3f(0.0, -1.0, 0.0);
	//potrebno je izdvojiti deo teskture
	glTexCoord2f(1.0 - (k + 1) * texPom, 1.0 - i * texPom);
	glVertex3f(a / 2, -a / 2, a / 2);
	glTexCoord2f(1.0 - k * texPom, 1.0 - i * texPom);
	glVertex3f(-a / 2, -a / 2, a / 2);
	glTexCoord2f(1.0 - k * texPom, 1.0 - (i + 1) * texPom);
	glVertex3f(-a / 2, -a / 2, -a / 2);
	glTexCoord2f(1.0 - (k + 1) * texPom, 1.0 - (i + 1) * texPom);
	glVertex3f(a / 2, -a / 2, -a / 2);

	// prednja,koju vidimo
	SetMaterial(1.0, 0.2, 0.2);
	glNormal3f(0.0, 0.0, 1.0); // na desno
	glTexCoord2f((k + 1) * texPom, (j + 1) * texPom);
	glVertex3f(a / 2, -a / 2, a / 2);
	glTexCoord2f((k + 1) * texPom, j * texPom);
	glVertex3f(a / 2, a / 2, a / 2);
	glTexCoord2f(k * texPom, j * texPom);
	glVertex3f(-a / 2, a / 2, a / 2);
	glTexCoord2f(k * texPom, (j + 1) * texPom);
	glVertex3f(-a / 2, -a / 2, a / 2);

	// gornja
	SetMaterial(1.0, 1.0, 1.0);
	glNormal3f(0.0, 1.0, 0.0);
	glTexCoord2f((k + 1) * texPom, 1.0 - i * texPom);
	glVertex3f(a / 2, a / 2, a / 2);
	glTexCoord2f((k + 1) * texPom, 1.0 - (i + 1) * texPom);
	glVertex3f(a / 2, a / 2, -a / 2);
	glTexCoord2f(k * texPom, 1.0 - (i + 1) * texPom);
	glVertex3f(-a / 2, a / 2, -a / 2);
	glTexCoord2f(k * texPom, 1.0 - i * texPom);
	glVertex3f(-a / 2, a / 2, a / 2);

	// zadnja
	SetMaterial(1.0, 0.7, 0.2);
	glNormal3f(0.0, 0.0, -1.0);
	glTexCoord2f(1.0- (k + 1) * texPom, (j + 1) * texPom);
	glVertex3f(a / 2, -a / 2, -a / 2);
	glTexCoord2f(1.0 - k * texPom, (j + 1) * texPom);
	glVertex3f(-a / 2, -a / 2, -a / 2);
	glTexCoord2f(1.0 - k * texPom, j * texPom);
	glVertex3f(-a / 2, a / 2, -a / 2);
	glTexCoord2f(1.0 - (k + 1) * texPom, j * texPom);
	glVertex3f(a / 2, a / 2, -a / 2);

	// leva
	SetMaterial(0.0, 0.7, 0.1);
	glNormal3f(-1.0, 0.0, 0.0);
	glTexCoord2f(1.0 - i * texPom, j * texPom);
	glVertex3f(-a / 2, a / 2, a / 2);
	glTexCoord2f(1.0 - (i + 1) * texPom, j * texPom);
	glVertex3f(-a / 2, a / 2, -a / 2);
	glTexCoord2f(1.0 - (i + 1) * texPom, (j + 1) * texPom);
	glVertex3f(-a / 2, -a / 2, -a / 2);
	glTexCoord2f(1.0 - i * texPom, (j + 1) * texPom);
	glVertex3f(-a / 2, -a / 2, a / 2);

	// desna
	SetMaterial(0.4, 0.2, 1.0);
	glNormal3f(1.0, 0.0, 0.0);
	glTexCoord2f(i * texPom, j * texPom);
	glVertex3f(a / 2, a / 2, a / 2);
	glTexCoord2f(i * texPom, (j + 1) * texPom);
	glVertex3f(a / 2, -a / 2, a / 2);
	glTexCoord2f((i + 1) * texPom, (j + 1) * texPom);
	glVertex3f(a / 2, -a / 2, -a / 2);
	glTexCoord2f((i + 1) * texPom, j * texPom);
	glVertex3f(a / 2, a / 2, -a / 2);

	glEnd();
}
void CGLRenderer::DrawRubikCube(double a, int count)
{
	float polaStranica = count / 2.0;
	float prviPomeraj = polaStranica * a + floor(polaStranica) * a * 0.05 - a / 2;
	//da bi se obezb razmak
	float razmak = 1.05 * a;
	float pomerajTeksture = 1.0 / count;  // isto kao i razmak izmedju kocki,da se napravi razmak
	//broj strana,na koliko se deli delova

	//Kockice crtamo od gore na dole.
	//OpenGL - BELE boje
	//prva kockica koja se crta je Donja Leva kockica BELE OpenGL
	//sa prvom For petljom i unutrasnjom K petljom,crtamo celu gornju stranu
	for (int i = 0; i < count; ++i)
	{
		glPushMatrix();
		glRotatef(ugao[i % count], 1.0, 0.0, 0.0); // sad rotiramo po x
		glTranslatef(-prviPomeraj, prviPomeraj, prviPomeraj);
		//crta na dole,ova for petlja
		for (int j = 0; j < count; ++j)
		{
			glPushMatrix();
			//jedna stranica, od donje leve do gornje desne 
			//sa i = 0 , k od 0 do 2, nacrtamo leve 3 kocke gornje stranice
			for (int k = 0; k < count; ++k)
			{
				DrawCube(a, k, j, i, pomerajTeksture); // posto menjamo translacije po x i z menjamo i koordinate
				glTranslatef(0.0, 0.0, -razmak); // ovo je bilo dole
			}
			glPopMatrix();
			//na dole,po Y osi
			glTranslatef(0.0, -razmak, 0.0);
		}
		glPopMatrix();
		//za sledecu kocku pomeri za razmak po X osi
		glTranslatef(razmak, 0.0, 0.0); // ovo je bilo gore
	}
} 
void CGLRenderer::SetWhiteLight()
{
	glEnable(GL_LIGHTING);
	//globalno
	float global_ambient[] = { 0.5, 0.5, 0.5, 1.0};
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, global_ambient);
	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);

	//tackasti izvor LIGHT0
	float ambient0[] = { 0.1, 0.1, 0.1, 1.0 };
	float diffuse0[] = { 0.8, 0.8, 0.8, 1.0 };
	float specular0[] = { 1.0, 1.0, 1.0, 1.0 };
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient0);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse0);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specular0);
	glEnable(GL_LIGHT0);

	//Usmereni izvor LIGHT1
	float ambient1[] = { 0.4, 0.4, 0.4, 1.0 };
	float diffuse1[] = { 0.8, 0.8, 0.8, 1.0 };
	float specular1[] = { 1.0, 1.0, 1.0, 1.0 };
	glLightfv(GL_LIGHT1, GL_AMBIENT, ambient1);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, diffuse1);
	glLightfv(GL_LIGHT1, GL_SPECULAR, specular1);
	
	//nakon toga,podesavamo poziciju usmerenog izvora svetlosti
	float light1_position[] = { 0.0, 0.0, 0.0 };
	glLightfv(GL_LIGHT1, GL_POSITION, light1_position);

	float light1_direction[] = { 0.0, 0.0, 1.0 };
	glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, light1_direction);
	glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 13.0);
	glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, 2.0);
	glLightf(GL_LIGHT1, GL_CONSTANT_ATTENUATION, 1.0);
	glLightf(GL_LIGHT1, GL_LINEAR_ATTENUATION, 0.01);
	glLightf(GL_LIGHT1, GL_QUADRATIC_ATTENUATION, 0.001);
	glEnable(GL_LIGHT1);
}
void CGLRenderer::SetMaterial(float r, float g, float b)
{
	//ambijentalna boja 20% difuzione broje:
	float ambient[] = { 0.2f * r, 0.2f * g, 0.2f * b, 1.0f };
	float diffuse[] = { r, g, b, 1.0f };
	float specular[] = { r, g, b, 1.0f };
	//nema emisionu komponentu
	float shininess = 64.0f;

	glMaterialfv(GL_FRONT, GL_AMBIENT, ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
	glMaterialf(GL_FRONT, GL_SHININESS, shininess);
}
void CGLRenderer::DrawScene(CDC *pDC)
{
	wglMakeCurrent(pDC->m_hDC, m_hrc);
	//---------------------------------
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	//gluLookAt(20, 20, 20, 0, 0, 0, 0, 1, 0);
	//rotiramo pre crtanja,zahtevamo u 1. zadatku
	glTranslatef(0.0, 0.0, -20);
	glRotatef(45.0, 1.0, 0.0, 0.0);

	float light0_position[] = { 5.0, 20.0, 0.0, 1.0 };
	glLightfv(GL_LIGHT0, GL_POSITION, light0_position);

	//koristimo transformaciju za rotiranje pogleda na scenu po Y osi
	glRotatef(ugaoPosmatraca, 0.0, 1.0, 0.0);
	//nezavisno od preth. transformacija,crtamo kocku
	glPushMatrix();
	DrawRubikCube(2.0, 3);
	glPopMatrix();


	glFlush();
	SwapBuffers(pDC->m_hDC);
	//---------------------------------
	wglMakeCurrent(NULL, NULL);
}
