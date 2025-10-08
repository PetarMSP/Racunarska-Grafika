#pragma once
#include "GLRenderer.h"

class CMy24012019View : public CView
{
protected: // create from serialization only
	CMy24012019View() noexcept;
	DECLARE_DYNCREATE(CMy24012019View)

// Attributes
public:
	CMy24012019Doc* GetDocument() const;
	CGLRenderer m_glRenderer;
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// Implementation
public:
	virtual ~CMy24012019View();
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
#ifndef _DEBUG  // debug version in 24.01.2019View.cpp
inline CMy24012019Doc* CMy24012019View::GetDocument() const
   { return reinterpret_cast<CMy24012019Doc*>(m_pDocument); }
#endif