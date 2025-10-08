#pragma once
#include "GLRenderer.h"
class CMy22012013View : public CView
{
protected: // create from serialization only
	CMy22012013View() noexcept;
	DECLARE_DYNCREATE(CMy22012013View)

public:
	CMy22012013Doc* GetDocument() const;
	CGLRenderer m_glRenderer;
	double rotY;
// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// Implementation
public:
	virtual ~CMy22012013View();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
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

#ifndef _DEBUG  // debug version in 22.01.2013View.cpp
inline CMy22012013Doc* CMy22012013View::GetDocument() const
   { return reinterpret_cast<CMy22012013Doc*>(m_pDocument); }
#endif

