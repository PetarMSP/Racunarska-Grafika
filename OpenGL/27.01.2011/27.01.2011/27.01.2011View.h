#pragma once
#include "GLRenderer.h"
class CMy27012011View : public CView
{
protected: // create from serialization only
	CMy27012011View() noexcept;
	DECLARE_DYNCREATE(CMy27012011View)

// Attributes
public:
	CMy27012011Doc* GetDocument() const;
	CGLRenderer m_glRenderer;
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
public:
	virtual ~CMy27012011View();
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

#ifndef _DEBUG  // debug version in 27.01.2011View.cpp
inline CMy27012011Doc* CMy27012011View::GetDocument() const
   { return reinterpret_cast<CMy27012011Doc*>(m_pDocument); }
#endif