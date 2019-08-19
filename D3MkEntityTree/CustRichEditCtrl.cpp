
// +--------------------------------------------------
// |
// | CustRichEditCtrl.cpp
// |
// | D3MkEntityTree : Copyright (c) 2004, kentaro-k.21
// |

#include "stdafx.h"
#include "CustRichEditCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCustRichEditCtrl

CCustRichEditCtrl::CCustRichEditCtrl()
{

}

CCustRichEditCtrl::~CCustRichEditCtrl()
{

}

BEGIN_MESSAGE_MAP(CCustRichEditCtrl, CRichEditCtrl)
	//{{AFX_MSG_MAP(CCustRichEditCtrl)
	//}}AFX_MSG_MAP
#if (_MFC_VER == 0x0710)
	ON_NOTIFY_REFLECT(EN_LINK, (void (CCmdTarget::* )(NMHDR *,LRESULT *))OnLink)
#else
	ON_NOTIFY_REFLECT(EN_LINK, OnLink)
#endif
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCustRichEditCtrl メッセージ ハンドラ

void CCustRichEditCtrl::OnLink(ENLINK *pNMHDR, LRESULT *pRes)
{
	if (pNMHDR->msg == WM_LBUTTONDOWN) {
		CWaitCursor wc;
		SetSel(pNMHDR->chrg);
		CString strSel = GetSelText();
		ShellExecute(NULL, "open", strSel, NULL, NULL, SW_SHOW);

		*pRes = TRUE;
		return;
	}

	*pRes = FALSE;
}

bool CCustRichEditCtrl::LoadFromResource(LPCTSTR pszType, LPCTSTR pszName)
{
	HRSRC hRes = FindResource(AfxGetInstanceHandle(), pszName, pszType);
	if (hRes) {
		HGLOBAL hMem = LoadResource(AfxGetInstanceHandle(), hRes);
		if (hRes) {
			LPVOID pvData = LockResource(hMem);
			DWORD cbData = SizeofResource(AfxGetInstanceHandle(), hRes);

			MemVuff mv;
			mv.psz = (PBYTE)pvData;
			mv.iCur = 0;
			mv.nLen = cbData;

			EDITSTREAM es;
			es.dwCookie = (DWORD)&mv;
			es.dwError = 0;
			es.pfnCallback = fnEditStreamCallback;

			StreamIn(SF_RTF, es);
			return true;
		}
	}
	return false;
}
