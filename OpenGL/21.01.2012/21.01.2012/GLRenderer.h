#pragma once

class CGLRenderer
{
public:
	CGLRenderer(void);
	virtual ~CGLRenderer(void);
	//osnovne funkcije:
	bool CreateGLContext(CDC* pDC);	
	void PrepareScene(CDC* pDC);
	void Reshape(CDC* pDC, int w, int h);
	void DrawScene(CDC* pDC);
	void DestroyScene(CDC* pDC);
	UINT CGLRenderer::LoadTexture(CString fileName);
	//funkcije za zadatak:

	void Kocka(double size);
	void Abazur(double r1, double r2, double h, int step);
	void Telo(double r1, double r2, double r3, double h, int step);

	//atributi:
public:
	UINT marble, fabric, metal1;
	double pi = 3.1415;
	double toDeg = 180 / pi;
	double toRad = pi / 180;
	double rotation = 0.0;
protected:
	HGLRC	 m_hrc; //OpenGL Rendering Context 

};
