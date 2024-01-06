
// MainFrm.cpp : implementation of the CMainFrame class
//

#include "pch.h"
#include "framework.h"
#include "MFCApplication2.h"

#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWndEx)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWndEx)
	ON_WM_CREATE()
    ON_COMMAND(ID_FILE_NEWGAME, &CMainFrame::OnFileNewgame)
    ON_COMMAND(ID_FILE_OPENGAME, &CMainFrame::OnFileOpengame)
    ON_COMMAND(ID_FILE_SAVEGAME, &CMainFrame::OnFileSavegame)
    ON_COMMAND(ID_FILE_SAVEGAME_AS, &CMainFrame::OnFileSavegameas)
    ON_COMMAND(ID_FILE_LARGEBOARD, &CMainFrame::OnFileLargeboard)
    ON_COMMAND(ID_FILE_SMALLBOARD, &CMainFrame::OnFileSmallboard)
    ON_COMMAND(ID_FILE_OPTION, &CMainFrame::OnFileOption)
    ON_COMMAND(ID_APP_EXIT, &CMainFrame::OnFileExit)
    ON_COMMAND(ID_EDIT_UNDO, &CMainFrame::OnEditUndo)
    ON_COMMAND(ID_EDIT_REDO, &CMainFrame::OnEditRedo)
    ON_UPDATE_COMMAND_UI(ID_FILE_LARGEBOARD, &CMainFrame::OnUpdateFileLargeboard)
    ON_UPDATE_COMMAND_UI(ID_FILE_SMALLBOARD, &CMainFrame::OnUpdateFileSmallboard)
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	IDS_TURN,
};

CMainFrame::CMainFrame() noexcept
{
	// TODO: add member initialization code here
	theApp.m_nAppLook = theApp.GetInt(_T("ApplicationLook"));
}

CMainFrame::~CMainFrame()
{
}

BOOL CMainFrame::PreTranslateMessage(MSG* pMsg)
{
	if (m_hAccelTable && ::TranslateAccelerator(*this, m_hAccelTable, pMsg))
	{
		return TRUE;
	}

	return CFrameWnd::PreTranslateMessage(pMsg);
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWndEx::OnCreate(lpCreateStruct) == -1)
		return -1;

    SetWindowText(_T("Gobang"));

	if (!m_wndMenuBar.LoadMenuW(IDR_MENU))
	{
		TRACE0("Failed to create menubar\n");
		return -1;      // fail to create
	}
    SetMenu(&m_wndMenuBar);
    m_wndMenuBar.Detach();

	//m_wndMenuBar.SetPaneStyle(m_wndMenuBar.GetPaneStyle() | CBRS_SIZE_DYNAMIC | CBRS_TOOLTIPS | CBRS_FLYBY);

	// prevent the menu bar from taking the focus on activation
	CMFCPopupMenu::SetForceMenuFocus(FALSE);

	if (!m_wndToolBar.CreateEx(this, WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	if (!m_wndStatusBar.Create(this))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}
	m_wndStatusBar.SetIndicators(indicators, sizeof(indicators)/sizeof(UINT));
    m_wndStatusBar.SetPaneInfo(1, IDS_TURN, SBPS_NORMAL, 100);

	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockPane(&m_wndToolBar);

	CList<UINT, UINT> lstBasicCommands;

	lstBasicCommands.AddTail(ID_FILE_NEW);
	lstBasicCommands.AddTail(ID_FILE_OPEN);
	lstBasicCommands.AddTail(ID_FILE_SAVE);
	lstBasicCommands.AddTail(ID_APP_EXIT);
	lstBasicCommands.AddTail(ID_EDIT_UNDO);
    lstBasicCommands.AddTail(ID_EDIT_REDO);
	lstBasicCommands.AddTail(ID_APP_ABOUT);

	CMFCToolBar::SetBasicCommands(lstBasicCommands);

	m_hAccelTable = ::LoadAccelerators(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_MENU));

	UpdateTurnStatus();

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWndEx::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	cs.style = WS_OVERLAPPED | WS_CAPTION | WS_OVERLAPPEDWINDOW
		 | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_SYSMENU;

	return TRUE;
}

// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWndEx::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWndEx::Dump(dc);
}
#endif //_DEBUG

void CMainFrame::UpdateTurnStatus()
{
	CMFCApplication2View* m_pView = DYNAMIC_DOWNCAST(CMFCApplication2View, GetActiveView());
	if (m_pView != nullptr) {
		CString turn = (m_pView->m_white == 1) ? _T("Next: White") : _T("Next: Black");
		m_wndStatusBar.SetPaneText(1, turn);
	}	
}

void CMainFrame::OnClose()
{
    CMFCApplication2View* m_pView = CMFCApplication2View::GetView();
	if (m_pView->m_isSaved)
	{
		if (MessageBox(_T("You haven't saved, Are you sure you want to close the window?"), _T("Confirmation"), MB_YESNO | MB_ICONQUESTION) == IDYES)
		{
			CFrameWndEx::OnClose();
		}
	}
	else
	{
		CFrameWndEx::OnClose();
	}
}

std::stack<CPoint> CMainFrame::TraverseStack(std::stack<CPoint> list)
{
    std::stack<CPoint> temp;
    while (!list.empty())
    {
        temp.push(list.top());
        list.pop();
    }

    return temp;
}



//--------------------------------------------------------------------
//
//--------------------------------------------------------------------


void CMainFrame::OnFileNewgame()
{
    CMFCApplication2View*  m_pView = CMFCApplication2View::GetView();
    m_pView->InitialiseBoard(m_pView->rect);
    UpdateTurnStatus();
    m_pView->m_moveList = std::stack<CPoint>();
    m_pView->m_redoList = std::stack<CPoint>();
    m_pView->m_isSaved = false;
    m_pView->isModified = true;
    m_pView->m_gameOver = false;
    m_pView->Invalidate();
}

void CMainFrame::OnFileOpengame()
{
    CMFCApplication2View* m_pView = CMFCApplication2View::GetView();
    LPCTSTR pszFilter =
        _T("Game State (*.sav)|*.sav|");

    CFileDialog dlg(TRUE, _T("gobang"), NULL,
        OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, pszFilter);

    if (dlg.DoModal() == IDOK)
    {
        CString filePath = dlg.GetPathName();

        CFile file;
        if (file.Open(filePath, CFile::modeRead))
        {
            CArchive ar(&file, CArchive::load);
            m_pView->isModified = true;
            m_pView->m_isSaved = true;

            for (int i = 0; i < sizeof(m_pView->board) / sizeof(*m_pView->board); i++)
            {
                for (int j = 0; j < sizeof(m_pView->board) / sizeof(*m_pView->board); j++)
                {
                    ar >> m_pView->board[i][j];
                }
            }

            ar >> m_pView->m_white;
            ar >> m_pView->m_firstPlayer;
            ar >> m_pView->m_boardColour;
            ar >> m_pView->m_boardSize;
            ar >> m_pView->m_isSaved;
            ar >> m_pView->m_gameOver;
            ar >> m_pView->m_filepath;

            if (!m_pView->m_moveList.empty())
            {
                m_pView->m_moveListSize = m_pView->m_moveList.size();
                ar >> m_pView->m_moveListSize;
                for (int i = 0; i < m_pView->m_moveListSize; i++)
                {
                    CPoint point;
                    ar >> point;
                    m_pView->m_moveList.push(point);
                }
            }

            if (!m_pView->m_redoList.empty())
            {
                m_pView->m_redoListSize = m_pView->m_redoList.size();
                ar >> m_pView->m_redoListSize;
                for (int i = 0; i < m_pView->m_redoListSize; i++)
                {
                    CPoint point;
                    ar >> point;
                    m_pView->m_redoList.push(point);
                }
            }

            ar.Close();
            file.Close();
        }

        m_pView->Invalidate();
    }
}

void CMainFrame::OnFileSavegame()
{
    CMFCApplication2View* m_pView = CMFCApplication2View::GetView();
    if (!m_pView->m_isSaved) {
        OnFileSavegameas();
        return;
    }       

    LPCTSTR pszFilter =
        _T("Game State (*.sav)|*.sav|");

    CFileDialog dlg(FALSE, _T("gobang"), NULL,
        OFN_OVERWRITEPROMPT, pszFilter);

    CString filePath = dlg.GetPathName();
    CFile file;
    if (file.Open(m_pView->m_filepath, CFile::modeCreate | CFile::modeWrite))
    {
        CArchive ar(&file, CArchive::store);
        m_pView->isModified = false;
        m_pView->m_isSaved = true;

        for (int i = 0; i < sizeof(m_pView->board) / sizeof(*m_pView->board); i++)
        {
            for (int j = 0; j < sizeof(m_pView->board) / sizeof(*m_pView->board); j++)
            {
                ar << m_pView->board[i][j];
            }
        }

        ar << m_pView->m_white;
        ar << m_pView->m_firstPlayer;
        ar << m_pView->m_boardColour;
        ar << m_pView->m_boardSize;
        ar << m_pView->m_isSaved;
        ar << m_pView->m_gameOver;
        ar << m_pView->m_filepath;

        if (!m_pView->m_moveList.empty())
        {
            m_pView->m_moveListSize = m_pView->m_moveList.size();
            ar << m_pView->m_moveListSize;
            std::stack<CPoint> _stack = TraverseStack(m_pView->m_moveList);
            while (!_stack.empty())
            {
                ar << _stack.top();
                _stack.pop();
            }
        }

        if (!m_pView->m_redoList.empty())
        {
            m_pView->m_redoListSize = m_pView->m_redoList.size();
            ar << m_pView->m_redoListSize;
            std::stack<CPoint> _stack = TraverseStack(m_pView->m_redoList);
            while (!_stack.empty())
            {
                ar << _stack.top();
                _stack.pop();
            }
        }
        ar.Close();
        file.Close();
    }
}

void CMainFrame::OnFileSavegameas()
{
    CMFCApplication2View* m_pView = CMFCApplication2View::GetView();
    LPCTSTR pszFilter =
        _T("Game State (*.sav)|*.sav|");

    CFileDialog dlg(FALSE, _T("gobang"), NULL,
        OFN_OVERWRITEPROMPT, pszFilter);

    if (dlg.DoModal() == IDOK)
    {
        CString filePath = dlg.GetPathName();


        CFile file;
        if (file.Open(filePath, CFile::modeCreate | CFile::modeWrite))
        {
            CArchive ar(&file, CArchive::store);
            m_pView->isModified = false;
            m_pView->m_isSaved = true;
            m_pView->m_filepath = filePath;

            for (int i = 0; i < sizeof(m_pView->board) / sizeof(*m_pView->board); i++)
            {
                for (int j = 0; j < sizeof(m_pView->board) / sizeof(*m_pView->board); j++)
                {
                    ar << m_pView->board[i][j];
                }
            }

            ar << m_pView->m_white;
            ar << m_pView->m_firstPlayer;
            ar << m_pView->m_boardColour;
            ar << m_pView->m_boardSize;
            ar << m_pView->m_isSaved;
            ar << m_pView->m_gameOver;
            ar << m_pView->m_filepath;

            if (!m_pView->m_moveList.empty())
            {
                m_pView->m_moveListSize = m_pView->m_moveList.size();
                ar << m_pView->m_moveListSize;
                std::stack<CPoint> _stack = TraverseStack(m_pView->m_moveList);
                while (!_stack.empty())
                {
                    ar << _stack.top();
                    _stack.pop();
                }
            }

            if (!m_pView->m_redoList.empty())
            {
                m_pView->m_redoListSize = m_pView->m_redoList.size();
                ar << m_pView->m_redoListSize;
                std::stack<CPoint> _stack = TraverseStack(m_pView->m_redoList);
                while (!_stack.empty())
                {
                    ar << _stack.top();
                    _stack.pop();
                }
            }
            ar.Close();
            file.Close();
        }
    }
}

void CMainFrame::OnFileLargeboard()
{
    CMFCApplication2View* m_pView = CMFCApplication2View::GetView();
    m_pView->m_boardSize = 1;
    m_pView->isModified = true;
    m_pView->SetBoardSize();
    m_pView->Invalidate();
}

void CMainFrame::OnUpdateFileLargeboard(CCmdUI* pCmdUI)
{
    CMFCApplication2View* m_pView = CMFCApplication2View::GetView();
    pCmdUI->SetCheck(m_pView->m_boardSize ? 1 : 0);
}

void CMainFrame::OnFileSmallboard()
{
    CMFCApplication2View* m_pView = CMFCApplication2View::GetView();
    m_pView->m_boardSize = 0;
    m_pView->isModified = true;
    m_pView->SetBoardSize();
    m_pView->Invalidate();
}

void CMainFrame::OnUpdateFileSmallboard(CCmdUI* pCmdUI)
{
    CMFCApplication2View* m_pView = CMFCApplication2View::GetView();
    pCmdUI->SetCheck(m_pView->m_boardSize ? 0 : 1);
}

void CMainFrame::OnFileOption()
{
    CMFCApplication2View* m_pView = CMFCApplication2View::GetView();
    int m_firstPlayer = m_pView->GetFirstPlayer();
    int m_boardColour = m_pView->GetBoardColour();
    BOOL isModified = m_pView->GetModified();
    Options dlg(m_firstPlayer, m_boardColour);
    if (dlg.DoModal() == IDOK)
    {
        m_pView->SetFirstPlayer(dlg.GetTurn());
        m_pView->SetBoardColour(dlg.GetBoardColour());
        m_pView->SetModified(true);
    }

    UpdateTurnStatus();
    m_pView->Invalidate();
    m_pView->UpdateWindow();
}

void CMainFrame::OnFileExit()
{
    OnClose();
}

void CMainFrame::OnEditUndo()
{
    CMFCApplication2View* m_pView = CMFCApplication2View::GetView();
    if (!m_pView->m_moveList.empty())
    {
        m_pView->board[m_pView->m_moveList.top().x][m_pView->m_moveList.top().y] = 0;
        m_pView->m_redoList.push(m_pView->m_moveList.top());
        m_pView->m_moveList.pop();

        m_pView->m_white *= -1;

        UpdateTurnStatus();
        m_pView->Invalidate();
    }
}


void CMainFrame::OnEditRedo()
{
    CMFCApplication2View* m_pView = CMFCApplication2View::GetView();
    if (!m_pView->m_redoList.empty())
    {
        m_pView->board[m_pView->m_redoList.top().x][m_pView->m_redoList.top().y] = m_pView->m_white;
        m_pView->m_moveList.push(m_pView->m_redoList.top());
        m_pView->m_redoList.pop();

        m_pView->m_white *= -1;

        UpdateTurnStatus();
        m_pView->Invalidate();
    }
}



