#pragma once
class CGLRenderer
{
public:
	CGLRenderer(void);
	virtual ~CGLRenderer(void);

	//osnovne funkcije
	bool CreateGLContext(CDC* pDC);
	void PrepareScene(CDC* pDC);			
	void Reshape(CDC* pDC, int w, int h);	
	void DrawScene(CDC* pDC);				
	void DestroyScene(CDC* pDC);	
	UINT LoadTexture(char* fileName);

	//funkcije za zadatak
	void DrawPolygon(POINTF* points, POINTF* texCoords, int n);
	void DrawExtrudedPolygon(POINTF* points, POINTF* texCoords, int n, float zh, float r, float g, float b);
	void DrawBase();
	void DrawBody();
	void DrawArm(double zh);
	void DrawFork();
	void DrawExcavator();
	void DrawEnvCube(double a);
	void DrawAxis();
	void setLight();

	//Atributi:
public:
	UINT B[6];
	UINT excavator;
	float angle1 = 0; // ugao kabine
	float angle2 = 0; // ugao prvog dela ruke
	float angle3 = 0; // ugao duzeg dela ruke
	float angle4 = 0; // ugao viljuske
	//pogled
	float xEye = 0;
	float yEye = 0;
	float zEye = 0;
	//ugao kojim se pomeramo oko bagera dok ga gledamo
	float ugaoX = 180;
	float ugaoY = 30;
	float r = 20;
protected:
	HGLRC	 m_hrc; //OpenGL Rendering Context 

};
