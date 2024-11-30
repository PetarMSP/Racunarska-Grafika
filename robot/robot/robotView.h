
// robotView.h : interface of the CrobotView class
//

#pragma once
#include"DImage.h"

class CrobotView : public CView
{
protected: // create from serialization only
	CrobotView() noexcept;
	DECLARE_DYNCREATE(CrobotView)

// Attributes
public:
	CrobotDoc* GetDocument() const;
	float nadlakticaRot;
	float podlakticaRot;
	float sakaRot;
	float roboRot;
	float roboScale;
	DImage* glava;
	DImage* nadkolenica;
	DImage* nadlaktica;
	DImage* podkolenica;
	DImage* podlaktica;
	DImage* saka;
	DImage* stopalo;
	DImage* telo;
	DImage* pozadina;

// Operations
public:

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual void DrawBackground(CDC* pDC);
	virtual void DrawImgTransparent(CDC* pDC, DImage* pImage);
	virtual void Translate(CDC* pDC, float dX, float dY, bool rightMultiply);
	virtual void Rotate(CDC* pDC, float angle, bool rightMultiply);
	virtual void Scale(CDC* pDC, float sX, float sY, bool rightMultiply);
	virtual void Mirror(CDC* pDC, bool mX, bool mY, bool rightMultiply);
	virtual void DrawHalf(CDC* pDC);
	virtual void DrawHead(CDC* pDC);
	virtual void DrawRobot(CDC* pDC);	
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// Implementation
public:
	virtual ~CrobotView();
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

#ifndef _DEBUG  // debug version in robotView.cpp
inline CrobotDoc* CrobotView::GetDocument() const
   { return reinterpret_cast<CrobotDoc*>(m_pDocument); }
#endif

