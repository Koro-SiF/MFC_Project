
// MFCApplication2View.h : interface of the CMFCApplication2View class
//

#pragma once

#include <stack>
#include "MainFrm.h"
#include "MFCApplication2Doc.h"

class CMFCApplication2View : public CView
{
protected: // create from serialization only
	CMFCApplication2View() noexcept;
	DECLARE_DYNCREATE(CMFCApplication2View)

// Attributes
public:
	CMFCApplication2Doc* GetDocument() const;
	static CMFCApplication2View* GetView();

	int squareSize;
	int collider;
	BOOL isModified = false;
	CRect rect;

	// Saved values
	int m_fileNameSize;
	int m_white;
	int m_firstPlayer = 1;
	int m_boardColour = 0;
	int m_boardSize = 1;
	int board[15][15];
	BOOL m_gameOver = false;
	BOOL m_isSaved = false;
	CString m_filepath;
	std::stack<CPoint> m_moveList;
	int m_moveListSize = 0;
	std::stack<CPoint> m_redoList;
	int m_redoListSize = 0;

// Operations
public:
	void InitialiseBoard(CRect rect);
	BOOL winCheck(int row, int col);
	void SetBoardSize();
	void RedrawBoard();
	void ReColour();

	void SetFirstPlayer(int no);
	void SetBoardColour(int no);
	void SetMoveList(std::stack<CPoint> list);
	void SetRedoList(std::stack<CPoint> list);
	void SetModified(BOOL modified);
	int GetFirstPlayer();
	int GetBoardColour();
	std::stack<CPoint> GetMoveList();
	std::stack<CPoint> GetRedoList();
	BOOL GetModified();

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:

// Implementation
public:
	virtual ~CMFCApplication2View();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	afx_msg void OnPaint();
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in MFCApplication2View.cpp
inline CMFCApplication2Doc* CMFCApplication2View::GetDocument() const
   { return reinterpret_cast<CMFCApplication2Doc*>(m_pDocument); }
#endif

