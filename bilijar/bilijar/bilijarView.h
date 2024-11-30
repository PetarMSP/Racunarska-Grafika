
// bilijarView.h : interface of the CbilijarView class
//
#include"DImage.h"
#include<math.h>
#pragma once


class CbilijarView : public CView
{
protected: // create from serialization only
	CbilijarView() noexcept;
	DECLARE_DYNCREATE(CbilijarView)

// Attributes
public:
	CbilijarDoc* GetDocument() const;
	DImage* felt2;
	DImage* wood;
	int kuglaX;
	int kuglaY;
	int stapX;
	int duzinaStapa;
	float stapRot;

// Operations
public:

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual void DrawMem(CDC* pDC);
	virtual void DrawStick(CDC* pDC, int w);
	virtual void DrawBall(CDC* pDC, int w);
	virtual void Translate(CDC* pDC, float dX, float dY, bool rightMultiply);
	virtual void Rotate(CDC* pDC, float angle, bool rightMultiply);
	virtual void DrawTable(CDC* pDC, CRect rect);
	virtual void DrawBorder(CDC* pDC, CRect rect, int w);
	virtual void DrawHoles(CDC* pDC, CRect rect, int size);
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// Implementation
public:
	virtual ~CbilijarView();
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

#ifndef _DEBUG  // debug version in bilijarView.cpp
inline CbilijarDoc* CbilijarView::GetDocument() const
   { return reinterpret_cast<CbilijarDoc*>(m_pDocument); }
#endif

