
// MFCApplication2View.cpp : implementation of the CMFCApplication2View class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "MFCApplication2.h"
#endif

#include "MFCApplication2Doc.h"
#include "MFCApplication2View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMFCApplication2View

IMPLEMENT_DYNCREATE(CMFCApplication2View, CView)

BEGIN_MESSAGE_MAP(CMFCApplication2View, CView)
	ON_WM_PAINT()
	ON_WM_CONTEXTMENU()
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()

// CMFCApplication2View construction/destruction

// View implementation file
CMFCApplication2View* CMFCApplication2View::GetView()
{
	CFrameWndEx* pFrame = (CFrameWndEx*)(AfxGetApp()->m_pMainWnd);

	CView* pView = pFrame->GetActiveView();

	if (!pView)
		return NULL;

	// Fail if view is of wrong kind
	// (this could occur with splitter windows, or additional
	// views on a single document
	if (!pView->IsKindOf(RUNTIME_CLASS(CMFCApplication2View)))
		return NULL;

	return (CMFCApplication2View*)pView;
}

CMFCApplication2View::CMFCApplication2View() noexcept
{

}

CMFCApplication2View::~CMFCApplication2View()
{
}

BOOL CMFCApplication2View::PreCreateWindow(CREATESTRUCT& cs)
{

	m_white = m_firstPlayer;
	InitialiseBoard(rect);

	return CView::PreCreateWindow(cs);
}

// CMFCApplication2View drawing

void CMFCApplication2View::OnDraw(CDC* /*pDC*/)
{
	CMFCApplication2Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here
}

void CMFCApplication2View::OnPaint()
{
	CPaintDC dc(this);
	ReColour();
	GetClientRect(rect);
	//ReColour();

	for (int row = 1; row <= 14; row++)
	{
		for (int col = 1; col <= 14; col++)
		{
			int x = col * squareSize;
			int y = row * squareSize;

			dc.MoveTo(x, y);
			dc.LineTo(x + squareSize, y);
			dc.LineTo(x + squareSize, y + squareSize);
			dc.LineTo(x, y + squareSize);
			dc.LineTo(x, y);

		}
	}

	RedrawBoard();
}

void CMFCApplication2View::InitialiseBoard(CRect rect)
{

	for (int i = 0; i < (sizeof(board) / sizeof(*board)); i++)
	{
		for (int j = 0; j < (sizeof(board) / sizeof(*board)); j++)
		{
			board[i][j] = 0;
		}
	}

	SetBoardSize();
}

void CMFCApplication2View::SetBoardSize()
{
	if (m_boardSize)
	{
		squareSize = 50;
		collider = 20;
	}
	else
	{
		squareSize = 40;
		collider = 15;
	}
}

void CMFCApplication2View::RedrawBoard()
{
	CDC* pDC = GetDC();

	// Redraws the pieces.
	for (int row = 0; row < (sizeof(board) / sizeof(*board)); row++)
	{
		for (int col = 0; col < (sizeof(board) / sizeof(*board)); col++)
		{
			CBrush brush;
			brush.CreateSolidBrush((board[row][col] == 1) ? RGB(255, 255, 255) : RGB(0, 0, 0));
			pDC->SelectObject(brush);

			int x = (col + 1) * squareSize;
			int y = (row + 1) * squareSize;

			if (board[row][col] != 0)
			{
				pDC->Ellipse(CRect(x - collider, y - collider, x + collider, y + collider));
			}
		}
	}
	ReleaseDC(pDC);
}

void CMFCApplication2View::ReColour()
{
	CDC* pDC = GetDC();
	CBrush brush;

	switch (m_boardColour)
	{
	case 0:
		brush.CreateSolidBrush(RGB(153, 153, 153)); // Gray
		break;
	case 1:
		brush.CreateSolidBrush(RGB(103, 215, 103)); // Green
		break;
	case 2:
		brush.CreateSolidBrush(RGB(245, 87, 98)); // Red
		break;
	case 3:
		brush.CreateSolidBrush(RGB(255, 255, 255)); // White
		break;
	case 4:
		brush.CreateSolidBrush(RGB(255, 255, 127)); // Yellow
		break;
	}

	CRect rectOffset(rect.left-50, rect.top - 50, rect.right + 50, rect.bottom + 50);
	pDC->FillRect(rectOffset, &brush);
	ReleaseDC(pDC);
}

void CMFCApplication2View::SetFirstPlayer(int no)
{
	m_firstPlayer = no;
}

void CMFCApplication2View::SetBoardColour(int no)
{
	m_boardColour = no;
}

void CMFCApplication2View::SetMoveList(std::stack<CPoint> list)
{
	m_moveList = list;
}

void CMFCApplication2View::SetRedoList(std::stack<CPoint> list)
{
	m_redoList = list;
}

void CMFCApplication2View::SetModified(BOOL modified)
{
	isModified = modified;
}

int CMFCApplication2View::GetFirstPlayer()
{
	return m_firstPlayer;
}

int CMFCApplication2View::GetBoardColour()
{
	return m_boardColour;
}

std::stack<CPoint> CMFCApplication2View::GetMoveList()
{
	return m_moveList;
}

std::stack<CPoint> CMFCApplication2View::GetRedoList()
{
	return m_redoList;
}

BOOL CMFCApplication2View::GetModified()
{
	return isModified;
}

void CMFCApplication2View::OnLButtonDown(UINT nFlags, CPoint point)
{
	if (m_gameOver)
	{
		(m_white == 1) ? MessageBox(_T("black wins")) : MessageBox(_T("white wins"));
		return;
	}
	CDC* pDC = GetDC();
	CBrush brush;
	brush.CreateSolidBrush((m_white == 1) ? RGB(255, 255, 255) : RGB(0, 0, 0));
	pDC->SelectObject(brush);

	for (int row = 1; row <= 15; row++)
	{
		for (int col = 1; col <= 15; col++)
		{
			int x = col * squareSize;
			int y = row * squareSize;

			if (point.x >= x - collider &&
				point.x <= x + collider &&
				point.y >= y - collider &&
				point.y <= y + collider &&
				board[row - 1][col - 1] == 0)
			{
				pDC->Ellipse(CRect(x - collider, y - collider, x + collider, y + collider));
				if (m_white == 1) board[row - 1][col - 1] = 1;
				else board[row - 1][col - 1] = -1;

				if (board[row - 1][col - 1] != 0)
					if (winCheck(row, col)) (m_white == 1) ? MessageBox(_T("white wins")) : MessageBox(_T("black wins"));

				m_white *= -1;

				CPoint point = { row - 1, col - 1 };
				m_moveList.push(point);
			}

		}
	}
	isModified = true;
	RedrawBoard();
	CMainFrame* m_pMainFrame = (CMainFrame*)AfxGetMainWnd();
	m_pMainFrame->UpdateTurnStatus();
	ReleaseDC(pDC);

	CView::OnLButtonDown(nFlags, point);
}

// CMFCApplication2View diagnostics

#ifdef _DEBUG
void CMFCApplication2View::AssertValid() const
{
	CView::AssertValid();
}

void CMFCApplication2View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CMFCApplication2Doc* CMFCApplication2View::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMFCApplication2Doc)));
	return (CMFCApplication2Doc*)m_pDocument;
}
#endif //_DEBUG

BOOL CMFCApplication2View::winCheck(int row, int col)
{
	int sum = 0;
	// Origin point: board[row - 1][col - 1]
	int control = board[row - 1][col - 1];
	int deltaY = (sizeof(board) / sizeof(*board)) - (col - 1);
	int deltaX = (sizeof(board) / sizeof(*board)) - (row - deltaY);

	// Horizontal check
	for (int i = 0; i < (sizeof(board) / sizeof(*board)); i++)
	{
		if (board[row - 1][i] == control)
		{
			sum++;
			if (sum >= 5)
			{
				m_gameOver = true;
				return true;
			}
		}
		else
		{
			sum = 0;
		}
	}

	// Vertical check
	for (int i = 0; i < (sizeof(board) / sizeof(*board)); i++)
	{
		if (board[i][col - 1] == control)
		{
			sum++;
			if (sum >= 5) 
			{
				m_gameOver = true;
				return true;
			}
		}
		else
		{
			sum = 0;
		}
	}

	// Diagonal TL-BR check
	int divider = row - col;
	if (divider > 0)
		for (int i = 0; (row - 1) - (col - 1) + i < sizeof(board) / sizeof(*board); i++)
			if (board[(row - 1) - (col - 1) + i][i] == control)
			{
				sum++;
				if (sum >= 5) 
				{
					m_gameOver = true;
					return true;
				}
			}
			else
				sum = 0;
	else if (divider < 0)
		for (int i = 0; (col - 1) - (row - 1) + i < sizeof(board) / sizeof(*board); i++)
			if (board[i][(col - 1) - (row - 1) + i] == control)
			{
				sum++;
				if (sum >= 5) 
				{
					m_gameOver = true;
					return true;
				}
			}
			else
				sum = 0;
	else
		for (int i = 0; i < sizeof(board) / sizeof(*board); i++)
			if (board[i][i] == control)
			{
				sum++;
				if (sum >= 5)
				{
					m_gameOver = true;
					return true;
				}
			}
			else
				sum = 0;

	// Diagonal TR-BL check
	if ((row)+(col) < (sizeof(board) / sizeof(*board)) + 1)
		for (int i = 0; i < col + row - 1; i++)
			if (board[i][(col - 1 + row - 1) - i] == control)
			{
				sum++;
				if (sum >= 5)
				{
					m_gameOver = true;
					return true;
				}
			}
			else
				sum = 0;
	else
		for (int i = 0; i <= deltaX; i++)
			if (board[row - 1 - deltaY + i][col - 1 + deltaY - i] == control)
			{
				sum++;
				if (sum >= 5) 
				{
					m_gameOver = true;
					return true;
				}
			}
			else
			{
				sum = 0;
			}


	return false;
}
// CMFCApplication2View message handlers
