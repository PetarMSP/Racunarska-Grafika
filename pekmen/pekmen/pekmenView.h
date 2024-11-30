
// pekmenView.h : interface of the CpekmenView class
//
#include "DImage.h"
#pragma once

class CpekmenView : public CView
{
protected: // create from serialization only
	CpekmenView() noexcept;
	DECLARE_DYNCREATE(CpekmenView)

// Attributes
public:
	CpekmenDoc* GetDocument() const;
	int pekX;
	int pekY;
	float pekUsta;
	float pekRot;
	bool pekMirror;
	DImage*pozadina;
// Operations
public:

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual void Translate(CDC* pDC, float dX, float dY, float rightMultiply);
	virtual void Rotate(CDC* pDC, float angle, float rightMultiply);
	virtual void Scale(CDC* pDC, float sX, float sY, float rightMultiply);
	virtual void Mirror(CDC* pDC, bool mX, bool mY, float rightMultiply);
	virtual void DrawPacman(CDC* pDC, CRect rect, float angle);
	virtual void DrawGhost(CDC* pDC, CRect rect);
	virtual void DrawMem(CDC* pDC);

	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// Implementation
public:
	virtual ~CpekmenView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};

#ifndef _DEBUG  // debug version in pekmenView.cpp
inline CpekmenDoc* CpekmenView::GetDocument() const
   { return reinterpret_cast<CpekmenDoc*>(m_pDocument); }
#endif

