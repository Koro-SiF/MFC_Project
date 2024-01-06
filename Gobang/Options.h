#pragma once
#include "afxdialogex.h"
#include "framework.h"
#include "resource.h"


// Options dialog

class Options : public CDialogEx
{
	DECLARE_DYNAMIC(Options)

public:
	Options(int firstPlayer, int boardColour, CWnd* pParent = NULL);
	virtual ~Options();

	int m_firstPlayer;
	int m_boardColour;

	int m_radio;

	// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_Options };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual BOOL DestroyWindow();
	afx_msg void OnLbnSelchangeList1();
	afx_msg void OnRadioButtonClicked();

	void LoadList();
	int GetTurn();
	int GetBoardColour();

	CListBox m_listBox;
	CString m_listSelected;
};
