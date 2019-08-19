// DoneDlg.cpp : �����t�@�C��
//

#include "stdafx.h"
#include "D3MkEntityTree.h"
#include "DoneDlg.h"

// CDoneDlg �_�C�A���O

CDoneDlg::CDoneDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDoneDlg::IDD, pParent)
	, m_strHref(_T(""))
{

}

CDoneDlg::~CDoneDlg()
{

}

void CDoneDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_HREF, m_wndLink);
	DDX_Text(pDX, IDC_STATIC_HREF, m_strHref);

	m_wndLink.SetURL(m_strHref);
}

BEGIN_MESSAGE_MAP(CDoneDlg, CDialog)
END_MESSAGE_MAP()
