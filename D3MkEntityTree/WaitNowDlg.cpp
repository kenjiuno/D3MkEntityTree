
// +--------------------------------------------------
// |
// | WaitNowDlg.cpp
// |
// | D3MkEntityTree : Copyright (c) 2004, kentaro-k.21
// |

#include "stdafx.h"
#include "D3MkEntityTree.h"
#include "WaitNowDlg.h"


// CWaitNowDlg ダイアログ

IMPLEMENT_DYNAMIC(CWaitNowDlg, CDialog)
CWaitNowDlg::CWaitNowDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CWaitNowDlg::IDD, pParent)
{
}

CWaitNowDlg::~CWaitNowDlg()
{
}

void CWaitNowDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CWaitNowDlg, CDialog)
END_MESSAGE_MAP()


// CWaitNowDlg メッセージ ハンドラ
