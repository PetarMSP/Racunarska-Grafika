#pragma once
class CGLRenderer
{
public:
	float ugaoPosmatraca; 
	UINT texture;
	float ugao[3];

public:
	CGLRenderer(void);
	virtual ~CGLRenderer(void);
	//osnovne funkcije
	bool CreateGLContext(CDC* pDC);
	void PrepareScene(CDC* pDC);	
	void Reshape(CDC* pDC, int w, int h);
	void DrawScene(CDC* pDC);
	void DestroyScene(CDC* pDC);
	void LoadTexture();

	//funkcije za crtanje scene
	void DrawCube(float a, int i, int j, int k, float texPom);
	void DrawRubikCube(double a, int count);
	void SetWhiteLight();
	void SetMaterial(float r, float g, float b);
protected:
	HGLRC	 m_hrc; //OpenGL Rendering Context 

};
