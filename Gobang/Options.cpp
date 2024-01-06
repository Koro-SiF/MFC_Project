#include "pch.h"
#include "Options.h"

// Options dialog

IMPLEMENT_DYNAMIC(Options, CDialogEx)

Options::Options(int firstPlayer, int boardColour, CWnd* pParent /*= NULL*/)
	: CDialogEx(IDD_OPTIONS, pParent)
{
	m_firstPlayer = firstPlayer;
	m_boardColour = boardColour;
}

Options::~Options()
{
}

void Options::DoDataExchange(CDataExchange* pDX)
{
	DDX_Radio(pDX, IDC_RADIO1, m_radio);
	//DDX_LBIndex(pDX, IDC_BOARDCOLOUR, m_boardColour);
	DDX_Control(pDX, IDC_LIST1, m_listBox);
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(Options, CDialogEx)
	ON_LBN_SELCHANGE(IDC_LIST1, &Options::OnLbnSelchangeList1)
	ON_BN_CLICKED(IDC_RADIO1, &Options::OnRadioButtonClicked)
END_MESSAGE_MAP()


// Options message handlers


BOOL Options::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	SetWindowText(_T("Options"));
	LoadList();
	UpdateData();
	return TRUE;
}

void Options::LoadList()
{
	int index = m_listBox.AddString(_T("Green"));
	m_listBox.SetItemData(index, 0);
	index = m_listBox.AddString(_T("Gray"));
	m_listBox.SetItemData(index, 1);
	index = m_listBox.AddString(_T("Red"));
	m_listBox.SetItemData(index, 2);
	index = m_listBox.AddString(_T("White"));
	m_listBox.SetItemData(index, 3);
	index = m_listBox.AddString(_T("Yellow"));
	m_listBox.SetItemData(index, 4);
}


BOOL Options::DestroyWindow()
{
	return CDialogEx::DestroyWindow();
}


void Options::OnLbnSelchangeList1()
{
	m_listBox.GetText(m_listBox.GetCurSel(), m_listSelected);
	m_boardColour = m_listBox.GetCurSel();
	UpdateData();
}

void Options::OnRadioButtonClicked()
{
	UpdateData();
	m_firstPlayer = m_radio;

}

int Options::GetTurn()
{
	return m_firstPlayer;
}

int Options::GetBoardColour()
{
	return m_boardColour;
}