
// MainFrm.h : interface of the CMainFrame class
//

#pragma once

#include <stack>
#include "MFCApplication2View.h"
#include "resource.h"

class CMainFrame : public CFrameWndEx
{
	
protected: // create from serialization only
	DECLARE_DYNCREATE(CMainFrame)

// Attributes
public:

// Operations
public:
	CMainFrame() noexcept;

// Overrides
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	void UpdateTurnStatus();
	void OnClose();
	
// Implementation
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // control bar embedded members
	CMenu			  m_wndMenuBar;
	CMFCToolBar       m_wndToolBar;
	CMFCStatusBar     m_wndStatusBar;
	HACCEL			  m_hAccelTable;

// Generated message map functions
protected:
 
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg BOOL PreTranslateMessage(MSG* pMsg);

	std::stack<CPoint> TraverseStack(std::stack<CPoint> list);

	afx_msg void OnAppAbout();
	afx_msg void OnFileNewgame();
	afx_msg void OnFileOpengame();
	afx_msg void OnFileSavegame();
	afx_msg void OnFileSavegameas();
	afx_msg void OnFileLargeboard();
	afx_msg void OnUpdateFileLargeboard(CCmdUI* pCmdUI);
	afx_msg void OnFileSmallboard();
	afx_msg void OnUpdateFileSmallboard(CCmdUI* pCmdUI);
	afx_msg void OnFileOption();
	afx_msg void OnFileExit();
	afx_msg void OnEditUndo();
	afx_msg void OnEditRedo();
	DECLARE_MESSAGE_MAP()

};


