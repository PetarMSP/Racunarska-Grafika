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

	UINT CGLRenderer::LoadTexture(char* filename);
	void DrawCube(double a);
	void DrawCone(double r, double h, int n);
	void DrawTube(double r1, double r2, double h, int n);
	void DrawPaddle(double length, double height);
	void setMaterial();
protected:
	HGLRC	 m_hrc; //OpenGL Rendering Context 
public:
	UINT brick, env;
	bool light;
	double windRotation = 0.0;
	double alfa = 0, beta = 0;
};
