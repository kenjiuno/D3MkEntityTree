// D3ModelExplorer.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "D3ModelExplorer.h"
#include "MainFrm.h"

#include "D3ModelExplorerDoc.h"
#include "D3ModelExplorerView.h"
#include "OSP.h"

using namespace OSP;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CString g_strCnf;
CString g_strExpDir;

// CD3ModelExplorerApp

BEGIN_MESSAGE_MAP(CD3ModelExplorerApp, CWinApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
END_MESSAGE_MAP()


// CD3ModelExplorerApp construction

CD3ModelExplorerApp::CD3ModelExplorerApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}


// The one and only CD3ModelExplorerApp object

CD3ModelExplorerApp theApp;

// CD3ModelExplorerApp initialization

BOOL CD3ModelExplorerApp::InitInstance()
{
	CWinApp::InitInstance();

	if (!AfxInitRichEdit())
		return false;

	g_strCnf = OSP_JoinPath(OSP_GetDir(OSP_GetModuleFileName(NULL)), "D3ModelExplorer.ini");

	g_strExpDir = OSP_JoinPath(OSP_GetDir(OSP_GetModuleFileName(NULL)), "temp");
	mkdir(g_strExpDir);

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	// of your final executable, you should remove from the following
	// the specific initialization routines you do not need
	// Change the registry key under which our settings are stored
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization
	SetRegistryKey(_T("Local AppWizard-Generated Applications"));
	LoadStdProfileSettings(0);  // Load standard INI file options (including MRU)
	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views
	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CD3ModelExplorerDoc),
		RUNTIME_CLASS(CMainFrame),       // main SDI frame window
		RUNTIME_CLASS(CD3ModelExplorerView));
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);
	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);
	// Dispatch commands specified on the command line.  Will return FALSE if
	// app was launched with /RegServer, /Register, /Unregserver or /Unregister.
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;
	// The one and only window has been initialized, so show and update it
	m_pMainWnd->ShowWindow(SW_SHOW);
	m_pMainWnd->UpdateWindow();
	// call DragAcceptFiles only if there's a suffix
	//  In an SDI app, this should occur after ProcessShellCommand
	return TRUE;
}



// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()

// App command to run the dialog
void CD3ModelExplorerApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}


// CD3ModelExplorerApp message handlers

static const DWORD nDefOpenFlags = 0
	|OFN_ENABLESIZING
	|OFN_EXPLORER
	|OFN_FILEMUSTEXIST
	|OFN_HIDEREADONLY
	|OFN_LONGNAMES
	|OFN_PATHMUSTEXIST
	;

void SelectToolFile(CEdit &wndEdit, LPCTSTR pszExt, LPCTSTR pszFilter)
{
	CString strFileName;

	wndEdit.GetWindowText(strFileName);

	CFileDialog wndDlg(
		true,
		pszExt,
		strFileName,
		nDefOpenFlags,
		pszFilter,
		CWnd::GetActiveWindow()
		);
	if (wndDlg.DoModal() != IDOK) return;

	strFileName = wndDlg.GetPathName();

	wndEdit.SetWindowText(strFileName);
}

CString FindGoodFile(CString strPath)
{
	TCHAR tc[2048 +1] = {_T("")};
	LPTSTR psz = NULL;
	if (SearchPath(NULL, strPath, NULL, 2048, tc, &psz) != 0) {
		tc[2048] = 0;
		return tc;
	}
	return strPath;
}
