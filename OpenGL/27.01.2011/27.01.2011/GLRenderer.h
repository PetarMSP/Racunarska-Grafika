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
	UINT LoadTexture(char *name);

	void DrawCilindar(double r, double h, int step);
	void DrawCone(double r, double h, int step);
	void DrawBody(double r, double h1, double h2, int n);
	void DrawCompoundBody();
	void DrawWing();
	void DrawWings();
	//atrubuti:
public:
	double viewAngle =0;
	double viewAngleY = 0;
	double pi = 3.1415;
	double toRad = pi / 180;
	double toDeg = 180 / pi;
	bool light=true;
	UINT texture;
protected:
	HGLRC	 m_hrc;

};
