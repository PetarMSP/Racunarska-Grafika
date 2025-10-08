#include "StdAfx.h"
#include "GLRenderer.h"
#include "GL\gl.h"
#include "GL\glu.h"
#include "GL\glaux.h"
#include "GL\glut.h"
#include "DImage.h"

CGLRenderer::CGLRenderer(void)
{
	xEye = sin(ugaoY) * sin(ugaoX) * r;
	yEye = cos(ugaoY) * r;
	zEye = sin(ugaoY) * cos(ugaoX) * r;
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
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glEnable(GL_DEPTH_TEST);
	//ucitamo teksture
	B[0] = LoadTexture("front.jpg");
	B[1] = LoadTexture("left.jpg");
	B[2] = LoadTexture("back.jpg");
	B[3] = LoadTexture("right.jpg");
	B[4] = LoadTexture("top.jpg");
	B[5] = LoadTexture("bot.jpg");
	excavator = LoadTexture("excavator.png");
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
	//FOV 55
	gluPerspective(55, (double)w / (double)h, 0.1, 2000);
	glMatrixMode(GL_MODELVIEW);
	//---------------------------------
	wglMakeCurrent(NULL, NULL);
}
void CGLRenderer::DestroyScene(CDC* pDC)
{
	wglMakeCurrent(pDC->m_hDC, m_hrc);
	wglMakeCurrent(NULL, NULL);
	glDeleteTextures(6,B);
	glDeleteTextures(1,&excavator);
	if (m_hrc)
	{
		wglDeleteContext(m_hrc);
		m_hrc = NULL;
	}
}
UINT CGLRenderer::LoadTexture(char* fileName)
{
	UINT textureID;
	DImage img;
	img.Load(CString(fileName));
	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, img.Width(), img.Height(), GL_BGRA_EXT, GL_UNSIGNED_BYTE, img.GetDIBBits());
	return textureID;
}
#pragma endregion
void CGLRenderer::DrawScene(CDC *pDC)
{
	wglMakeCurrent(pDC->m_hDC, m_hrc);
	//---------------------------------
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	setLight();
	//setujemo pogled
	gluLookAt(xEye,yEye,zEye, 0, 5, 0, 0, 1, 0);
	//crtamo koordinati sistem
	DrawAxis();
	glColor3f(1, 1, 1);
	DrawEnvCube(100);
	DrawExcavator();

	glFlush();
	SwapBuffers(pDC->m_hDC);
	//---------------------------------
	wglMakeCurrent(NULL, NULL);
}
void CGLRenderer::DrawAxis()
{
	glLineWidth(2);
	glBegin(GL_LINES);
	//x-osa
	glColor3f(0, 0, 1);
	glVertex3f(0, 0, 0);
	glVertex3f(10, 0, 0);
	//y-osa
	glColor3f(1, 0, 0);
	glVertex3f(0, 0, 0);
	glVertex3f(0, 10, 0);
	//z-osa
	glColor3f(0, 1, 0);
	glVertex3f(0, 0, 0);
	glVertex3f(0, 0, 10);
	glEnd();
}
//za pozadinu:
void CGLRenderer::DrawEnvCube(double a)
{
	glBindTexture(GL_TEXTURE_2D, B[0]);
	glBegin(GL_QUADS);

	//skroz levo,kad se okrenemo
	glTexCoord2f(0, 1);
	glVertex3f(a / 2, 0, a / 2);
	glTexCoord2f(0, 0);
	glVertex3f(a / 2, a, a / 2);
	glTexCoord2f(1, 0);
	glVertex3f(-a / 2, a, a / 2);
	glTexCoord2f(1, 1);
	glVertex3f(-a / 2, 0, a / 2);
	glEnd();
	//leva 
	glBindTexture(GL_TEXTURE_2D, B[1]);
	glBegin(GL_QUADS);
	glTexCoord2f(0, 1);
	glVertex3f(-a / 2, 0, a / 2);
	glTexCoord2f(0, 0);
	glVertex3f(-a / 2, a, a / 2);
	glTexCoord2f(1, 0);
	glVertex3f(-a / 2, a, -a / 2);
	glTexCoord2f(1, 1);
	glVertex3f(-a / 2, 0, -a / 2);
	glEnd();
	//desno
	glBindTexture(GL_TEXTURE_2D, B[2]);
	glBegin(GL_QUADS);
	glTexCoord2f(0, 1);
	glVertex3f(-a / 2, 0, -a / 2);
	glTexCoord2f(0, 0);
	glVertex3f(-a / 2, a, -a / 2);
	glTexCoord2f(1, 0);
	glVertex3f(a / 2, a, -a / 2);
	glTexCoord2f(1, 1);
	glVertex3f(a / 2, 0, -a / 2);
	glEnd();
	//desna,iza
	glBindTexture(GL_TEXTURE_2D, B[3]);
	glBegin(GL_QUADS);
	glTexCoord2f(0, 1);
	glVertex3f(a / 2, 0, -a / 2);
	glTexCoord2f(0, 0);
	glVertex3f(a / 2, a, -a / 2);
	glTexCoord2f(1, 0);
	glVertex3f(a / 2, a, a / 2);
	glTexCoord2f(1, 1);
	glVertex3f(a / 2, 0, a / 2);
	glEnd();
	//gornja
	glBindTexture(GL_TEXTURE_2D, B[4]);
	glBegin(GL_QUADS);
	glTexCoord2f(0, 1);
	glVertex3f(-a / 2, a, -a / 2);
	glTexCoord2f(0, 0);
	glVertex3f(-a / 2, a, a / 2);
	glTexCoord2f(1, 0);
	glVertex3f(a / 2, a, a / 2);
	glTexCoord2f(1, 1);
	glVertex3f(a / 2, a, -a / 2);
	glEnd();

	//donja tesktura,pod
	glBindTexture(GL_TEXTURE_2D, B[5]);
	glBegin(GL_QUADS);
	glTexCoord2f(0, 1);
	glVertex3f(-a / 2, 0, -a / 2);
	glTexCoord2f(0, 0);
	glVertex3f(a / 2, 0, -a / 2);
	glTexCoord2f(1, 0);
	glVertex3f(a / 2, 0, a / 2);
	glTexCoord2f(1, 1);
	glVertex3f(-a / 2, 0, a / 2);
	glEnd();

}
//funkcije za crtanje bager
void CGLRenderer::DrawPolygon(POINTF* points, POINTF* texCoords, int n)
{
	//po preporuci
	glBegin(GL_TRIANGLE_FAN);
	for (int i = 0; i < n; i++)
	{
		glTexCoord2f(texCoords[i].x, texCoords[i].y); // tacka tesksture
		glVertex2f(points[i].x, points[i].y); // tacka temena
	}
	glEnd();
}
void CGLRenderer::DrawExtrudedPolygon(POINTF* points, POINTF* texCoords, int n, float zh, float r, float g, float b)
{
	glPushMatrix();
	glTranslatef(0, 0, -zh / 2); // udaljeno kao
	DrawPolygon(points, texCoords, n); // sa slike 1, crta taj poligon
	glTranslatef(0, 0, zh); // nakon toga,pozicioniramo za drugu stranu 
	DrawPolygon(points, texCoords, n); // nacrta drugu stranu
	glPopMatrix();
	//uzmemo boju kojom hocemo da crtamo
	glColor3f(r, g, b);
	glBegin(GL_QUAD_STRIP);
	// treba sada obe strane,leva i desna strana da se spoje
	for (int i = 0; i < n; i++)
	{
		glVertex3f(points[i].x, points[i].y, -zh/2);
		glVertex3f(points[i].x, points[i].y, zh/2);
	}

	glVertex3f(points[0].x, points[0].y, -zh / 2);
	glVertex3f(points[0].x, points[0].y, zh / 2);
	glEnd();
	glColor3f(1, 1, 1);
}
void CGLRenderer::DrawBase()
{
	POINTF vertex[8];
	vertex[0] = POINTF({0,0.5});
	vertex[1] = POINTF({0,1.5});
	vertex[2] = POINTF({0.5,2});
	vertex[3] = POINTF({7.5,2});
	vertex[4] = POINTF({8,1.5});
	vertex[5] = POINTF({8,0.5});
	vertex[6] = POINTF({7.5,0});
	vertex[7] = POINTF({0.5,0});
	//gledamo sa slike, teskture 16*16
	//0/16, i 15/16 - predstavlja kocku skroz dole levo,odatle ucitavamo teksturu za Base
	POINTF textures[8];
	//obavezno 16 sa tackom,inace nece da radi
	textures[0] = POINTF({ 0/16.,15 / 16. }); // skroz levo dole
	textures[1] = POINTF({ 0/16.,13 / 16. }); // 2 kocke na gore,sa leve strane dole
	textures[2] = POINTF({ 1 / 16.,12 / 16. }); // gornji levi deo gusenice
	textures[3] = POINTF({ 15 / 16.,12 / 16. }); // skroz desno,desni deo gornji gusenice
	textures[4] = POINTF({ 16 / 16.,13 / 16. }); // pa naredne dve,spusti do donje desne,
	textures[5] = POINTF({ 16 / 16.,15 / 16. });
	textures[6] = POINTF({ 15/ 16.,16 / 16. }); // donji deo desnog kraja gusenice,dole desno skroz kockica
	// i poslednju spoji sa skroz levo,predzadnjom,dole
	textures[7] = POINTF({ 1 / 16,16 / 16 });

	DrawExtrudedPolygon(vertex, textures, 8, 5, 0, 0, 0); // visina prizme 5, 8- tacaka
}
void CGLRenderer::DrawBody()
{
	//podeok je 0.5
	POINTF vertex[5];
	//gledano sa slike 16x16.
	vertex[0] = POINTF({-2,0}); // leva donja
	vertex[1] = POINTF({-2,4}); // leva gornja
	vertex[2] = POINTF({0,4}); // desna donja
	vertex[3] = POINTF({2,2}); // desna gornja
	vertex[4] = POINTF({2,0}); //zadnja tacka

	POINTF textures[5];
	textures[0] = POINTF({8/16.,8/16.}); // donja leva
	//pa ide na gore
	textures[1] = POINTF({8/16.,0/16.}); // gornja leva
	textures[2] = POINTF({12/16.,0/16.}); // gore desno
	//kosa crta
	textures[3] = POINTF({16/16.,4/16.}); // desno
	textures[4] = POINTF({16/16.,8/16.}); // poslednja je dole desno skroz
	DrawExtrudedPolygon(vertex, textures, 5, 4, 0.96, 0.5, 0.12); // visina 4
}
void CGLRenderer::DrawArm(double zh)
{
	POINTF vertex[8];
	//pocinjemo od leve strane
	vertex[0] = POINTF({-1,-0.5});
	vertex[1] = POINTF({-1,0.5}); 
	vertex[2] = POINTF({-0.5,1}); 
	vertex[3] = POINTF({1,1});
	vertex[4] = POINTF({7,0.5}); 
	vertex[5] = POINTF({7,-0.5});
	vertex[6] = POINTF({1,-1}); 
	vertex[7] = POINTF({-0.5,-1});

	POINTF textures[8];
	textures[0] = POINTF({0/16.,11/16.}); // leva skroz,donja
	textures[1] = POINTF({0/16.,9/16.}); // pa gornja 
	textures[2] = POINTF({1/16.,8/16.});
	textures[3] = POINTF({4/16.,8/16.});
	textures[4] = POINTF({16/16.,9/16.});//pa skroz desno
	textures[5] = POINTF({16/16.,11/16.});
	textures[6] = POINTF({4/16.,12/16.}); // vrati na levo dole
	textures[7] = POINTF({1/16.,12/16.}); // donja leva

	DrawExtrudedPolygon(vertex, textures, 8, zh, 0.96, 0.5, 0.12);
}
void CGLRenderer::DrawFork()
{
	POINTF vertex[6];
	vertex[0] = POINTF({-1,-1}); // donja leva tacka
	vertex[1] = POINTF({-1,1.5});
	vertex[2] = POINTF({-0.5,2});
	vertex[3] = POINTF({2.5,2});
	vertex[4] = POINTF({3,1.5});//desno
	vertex[5] = POINTF({3,-1});

	POINTF textures[6];
	textures[0] = POINTF({0/16.,6/16.}); // dole levo
	textures[1] = POINTF({0/16.,1/16.});
	textures[2] = POINTF({1/16.,1/16.});
	textures[3] = POINTF({7/16.,0/16.});
	textures[4] = POINTF({8/16.,1/16.});
	textures[5] = POINTF({8/16.,6/16.});

	DrawExtrudedPolygon(vertex, textures, 6, 1, 0.7, 0.7, 0.7);
}
void CGLRenderer::DrawExcavator()
{
	glBindTexture(GL_TEXTURE_2D, excavator);
	glPushMatrix();
	glTranslatef(-4, 0, 0);
	DrawBase();
	glPopMatrix();

	//primeni se transformacija za telo,odnosno rotacija
	glRotatef(angle1, 0, 1, 0);
	glPushMatrix();
	glTranslatef(0, 2, 0); // podignemo malo
	DrawBody();
	glPopMatrix();

	//duzi deo "ruke"
	glRotatef(90, 0, 0, 1);
	glTranslatef(3, -1, -1.5); // ka desno od kabine
	glRotatef(angle2, 0, 0, 1);
	DrawArm(1);
	
	//nastavlja da crta od centra prvog dela ruke
	glRotatef(-90, 0, 0, 1); // zarotiran za 90 stepeni,ali to i ne mora odmah
	glTranslatef(0, 6.5, 0);  // samo podigne po Y osi
	glRotatef(angle3, 0, 0, 1); // primeni transformaciju za slucaj rotiranja
	DrawArm(1.5); // nacrta ddrugi deo ruke

	//i na kraju viljuska
	glRotatef(-90, 0, 0, 1);
	glTranslatef(0, 6.5, 0);
	glRotatef(angle4, 0, 0, 1);
	DrawFork();
}
void CGLRenderer::setLight()
{
	GLfloat light1_ambient[] = { 0.5,0.5,0.5,1 };
	GLfloat light1_diffuse[] = { 1.0,1.0,1.0,1.0 };
	GLfloat light1_specular[] = { 1.0,1.0,1.0,1.0 };
	GLfloat light1_position[] = { -2.0,-2.0,1.0,1.0 };
	GLfloat light1_direction[] = { -1.0,-1.0,0.0 };

	glLightfv(GL_LIGHT1, GL_AMBIENT, light1_ambient);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, light1_diffuse);
	glLightfv(GL_LIGHT1, GL_SPECULAR, light1_specular);
	glLightfv(GL_LIGHT1, GL_POSITION, light1_position);
	glEnable(GL_LIGHT1);
}
