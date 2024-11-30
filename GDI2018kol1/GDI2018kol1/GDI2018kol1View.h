
// GDI2018kol1View.h : interface of the CGDI2018kol1View class
//

#pragma once
#include "DImage.h"
#define PI 3.14
#define toRad PI / 180
#define toDeg 180 / PI


class CGDI2018kol1View : public CView
{
protected: // create from serialization only
	CGDI2018kol1View() noexcept;
	DECLARE_DYNCREATE(CGDI2018kol1View)

// Attributes
public:
	CGDI2018kol1Doc* GetDocument() const;

	DImage* p_arm1;
	DImage* p_arm2;
	DImage* p_bager;
	DImage* p_pozadina;
	HENHMETAFILE mf_viljuska;

// Operations
public:

// Overrides
public:

	float a1 = 0.0f, a2 = 0.0f, a3 = 0.0f;
	int posx = 10;

	void Translate(CDC* pDC, float dX, float dY, bool rightMultiply);
	void Rotate(CDC* pDC, float angle, bool rightMultiply);
	void Scale(CDC* pDC, float sX, float sY, bool rightMultiply);
	void DrawBackground(CDC* pDC);
	void DrawImgTransparent(CDC* pDC, DImage* pImage);
	void DrawBody(CDC* pDC);
	void DrawArm1(CDC* pDC);
	void DrawArm2(CDC* pDC);
	void DrawFork(CDC* pDC);
	void DrawExcavator(CDC* pDC);

	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// Implementation
public:
	virtual ~CGDI2018kol1View();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
};

#ifndef _DEBUG  // debug version in GDI2018kol1View.cpp
inline CGDI2018kol1Doc* CGDI2018kol1View::GetDocument() const
   { return reinterpret_cast<CGDI2018kol1Doc*>(m_pDocument); }
#endif

