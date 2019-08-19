// Tool1Dlg.cpp : 実装ファイル
//

#include "stdafx.h"
#include <shlwapi.h>
#include "D3ModelExplorer.h"
#include "Tool1Dlg.h"
#include "FUt.h"
#include ".\tool1dlg.h"

// CTool1Dlg ダイアログ

IMPLEMENT_DYNAMIC(CTool1Dlg, CDialog)
CTool1Dlg::CTool1Dlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTool1Dlg::IDD, pParent)
	, m_strExe(_T(""))
{
}

void CTool1Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_strExe);
	DDX_Control(pDX, IDC_EDIT1, m_wndEdit1);
	DDX_Control(pDX, IDC_RICHED_CREDITS, m_wndRich);
}


BEGIN_MESSAGE_MAP(CTool1Dlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON1, OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON_TEST, OnBnClickedButtonTest)
END_MESSAGE_MAP()


// CTool1Dlg メッセージ ハンドラ

void CTool1Dlg::OnBnClickedButton1()
{
	SelectToolFile(m_wndEdit1, "py", "md5-renderer.py (*.py)|*.py||");
}

static int Runpy(LPCTSTR pszCmd) {
	SHELLEXECUTEINFO sei;
	ZeroMemory(&sei, sizeof(sei));
	sei.cbSize = sizeof(sei);
	sei.fMask = SEE_MASK_NOCLOSEPROCESS;
	sei.hwnd = CWnd::GetActiveWindow()->GetSafeHwnd();
	sei.lpVerb = _T("open");
	sei.lpFile = pszCmd;
	sei.nShow = SW_SHOW;
	if (ShellExecuteEx(&sei)) {
		DWORD errc = 1;
		WaitForSingleObject(sei.hProcess, INFINITE);
		GetExitCodeProcess(sei.hProcess, &errc);
		CloseHandle(sei.hProcess);
		if (errc == 0) {
			// errorleve is 0
			return 0;
		}
		else {
			// errorlevel not 0
			return -2;
		}
	}
	else {
		// shell exec failed
		return -1;
	}
}

void CTool1Dlg::OnBnClickedButtonTest()
{
	if (!UpdateData())
		return;
	if (FileExists(m_strExe)) {
		AfxMessageBox(IDS_PROGRAM_VALID, MB_OK|MB_ICONINFORMATION);
	}
	else {
		AfxMessageBox(IDS_PROGRAM_ABSENT, MB_OK|MB_ICONEXCLAMATION);
		OnBnClickedButton1();
	}

	switch (Runpy( FindGoodFile("pytest-simple.py") )) {
		case 0:
			// py ok
			AfxMessageBox(IDS_PYTHON_VALID, MB_OK|MB_ICONINFORMATION);

			switch (Runpy( FindGoodFile("pytest-PIL.py") )) {
				case 0:
					// PIL ok
					AfxMessageBox(IDS_PIL_PRESENT, MB_OK|MB_ICONINFORMATION);

					switch (Runpy( FindGoodFile("pytest-PyOpenGL.py") )) {
						case 0:
							// PyOpenGL ok
							AfxMessageBox(IDS_PYOPENGL_PRESENT, MB_OK|MB_ICONINFORMATION);
							break;

						case -1:
						case -2:
							// PyOpenGL not found?
							AfxMessageBox(IDS_PYOPENGL_ABSENT, MB_OK|MB_ICONEXCLAMATION);
							break;
					}
					break;

				case -1:
				case -2:
					// PIL not found?
					AfxMessageBox(IDS_PIL_ABSENT, MB_OK|MB_ICONEXCLAMATION);
					break;
			}
			break;

		case -2:
			// python not functional?
			AfxMessageBox(IDS_PYTHON_ABSENT, MB_OK|MB_ICONEXCLAMATION);
			break;
		case -1:
			// python shell exec err
			AfxMessageBox(IDS_PYTHON_ABSENT, MB_OK|MB_ICONEXCLAMATION);
			break;
	}

	// fine
	AfxMessageBox(IDS_CHECK_FINE, MB_OK|MB_ICONINFORMATION);
}

BOOL CTool1Dlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_wndRich.EnableAutoUrlDetect();
	m_wndRich.LoadFromFile( FindGoodFile("Tool1.rtf") );
	m_wndRich.EnableLink4Browse();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}
