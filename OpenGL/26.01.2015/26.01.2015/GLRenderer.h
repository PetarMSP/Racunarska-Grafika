#pragma once
#include "DImage.h"
class CGLRenderer
{
public:
	CGLRenderer(void);
	virtual ~CGLRenderer(void);
	double rot1, rot2, dist;
	double rotMesecY;
	double rotMesecX;
	bool lighting;
	UINT T[6];
	UINT M[6];
	UINT S[6];

	//fiksne funkcije
	bool CreateGLContext(CDC* pDC);
	void PrepareScene(CDC* pDC);
	void Reshape(CDC* pDC, int w, int h);
	void DrawScene(CDC* pDC);
	void DestroyScene(CDC* pDC);

	//Funkcije za zadatak:
	void DrawAxis();
	UINT LoadTexture(char* fileName);
	void PrepareTextures();
	void DrawPatch(double R, int n);
	void DrawEarth(double R, int tes);
	void DrawMoon(double R, int tes);
	void DrawSpace(double R, int tes);
protected:
	HGLRC	 m_hrc; 
};
