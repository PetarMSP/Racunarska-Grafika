#pragma once
#include "DImage.h"
class CGLRenderer
{
public:
	CGLRenderer(void);
	virtual ~CGLRenderer(void);

	UINT ship;
	UINT textureSpace[6];
	double beta, alfa,distance;
	double lookX, lookY, lookZ; // provera za Space
	bool light; // za button S

	//fiksne funkcije
	bool CreateGLContext(CDC* pDC);
	void PrepareScene(CDC* pDC);
	void Reshape(CDC* pDC, int w, int h);
	void DrawScene(CDC* pDC);
	void DestroyScene(CDC* pDC);

	//Funkcije za zadatak:
	UINT LoadTexture(char* fileName);
	void PrepareTextures();
	void SetLook(double x, double y, double z);
	//Zeznuta:
	void DrawTriangle(float d1,float d2,float rep);
	void DrawShip();
	void DrawSpaceCube(double a);
protected:
	HGLRC	 m_hrc; 
};
