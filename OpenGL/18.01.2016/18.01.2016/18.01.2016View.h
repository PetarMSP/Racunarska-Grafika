#pragma once
#include "GLRenderer.h"

class CMy18012016View : public CView
{
protected: // create from serialization only
	CMy18012016View() noexcept;
	DECLARE_DYNCREATE(CMy18012016View)

public:
	CMy18012016Doc* GetDocument() const;
	CGLRenderer m_glRenderer;

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
public:
	virtual ~CMy18012016View();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	virtual void OnInitialUpdate();
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
};

#ifndef _DEBUG  // debug version in 18.01.2016View.cpp
inline CMy18012016Doc* CMy18012016View::GetDocument() const
   { return reinterpret_cast<CMy18012016Doc*>(m_pDocument); }
#endif

