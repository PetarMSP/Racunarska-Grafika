#pragma once
#include "DImage.h"
class CGLRenderer
{
public:
	CGLRenderer(void);
	virtual ~CGLRenderer(void);
		
	bool CreateGLContext(CDC* pDC);			// kreira OpenGL Rendering Context
	void PrepareScene(CDC* pDC);			// inicijalizuje scenu,
	void Reshape(CDC* pDC, int w, int h);	// kod koji treba da se izvrsi svaki put kada se promeni velicina prozora ili pogleda i
	void DrawScene(CDC* pDC);				// iscrtava scenu
	void DestroyScene(CDC* pDC);			// dealocira resurse alocirane u drugim funkcijama ove klase,
	void FillVATex(float* buff, float x1, float x2);
	void FillVA(float* buff, float a, float b, float h);
	void DrawPyramid(double a, double b, double h ,float* bufft);
	void DrawBox(double width, double height, double length);
	void DrawPyramid(float* buff, float* bufft);
	void DrawAxis(double length);
	void PrepareTextures();
	void Rotate(double rot1) { rotTower = rot1;}
	void SetMaterial();
	void SetLightModel();
	UINT LoadTexture(char* fileName);
protected:
	HGLRC	 m_hrc; //OpenGL Rendering Context 
	double rotTower;
	//bafer za Normale
	float buff1[48], buff2[48], buff3[48], buff4[48];
	//bafer za Teksturne koordinate
	float bufft1[32], bufft2[32], bufft3[32], bufft4[32];
	UINT T[4]; // za teksture
};
