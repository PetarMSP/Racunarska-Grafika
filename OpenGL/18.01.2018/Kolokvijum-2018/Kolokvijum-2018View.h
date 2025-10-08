#pragma once
#include "GLRenderer.h"

class CKolokvijum2018View : public CView
{
protected: // create from serialization only
	CKolokvijum2018View() noexcept;
	DECLARE_DYNCREATE(CKolokvijum2018View)

// Attributes
public:
	CKolokvijum2018Doc* GetDocument() const;
	CGLRenderer m_glRenderer;

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
	virtual ~CKolokvijum2018View();
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

#ifndef _DEBUG  // debug version in Kolokvijum-2018View.cpp
inline CKolokvijum2018Doc* CKolokvijum2018View::GetDocument() const
   { return reinterpret_cast<CKolokvijum2018Doc*>(m_pDocument); }
#endif

