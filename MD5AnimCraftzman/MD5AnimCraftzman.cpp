// MD5AnimCraftzman.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "MD5AnimCraftzman.h"
#include "MainFrm.h"

#include "MD5AnimCraftzmanDoc.h"
#include "MD5AnimCraftzmanView.h"
#include "ChildFrm.h"
#include "OSP.h"
#include ".\md5animcraftzman.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace OSP;

// CMD5AnimCraftzmanApp

BEGIN_MESSAGE_MAP(CMD5AnimCraftzmanApp, CWinApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
	ON_COMMAND(ID_OPTS_CONF, OnOptsConf)
	ON_COMMAND(ID_OPTS_UPDATE, OnOptsUpdate)
END_MESSAGE_MAP()


// CMD5AnimCraftzmanApp construction

CMD5AnimCraftzmanApp::CMD5AnimCraftzmanApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}


// The one and only CMD5AnimCraftzmanApp object

CMD5AnimCraftzmanApp theApp;

// CMD5AnimCraftzmanApp initialization

BOOL CMD5AnimCraftzmanApp::InitInstance()
{
	CWinApp::InitInstance();

	m_strCnf = OSP_JoinPath(OSP_GetDir(OSP_GetModuleFileName(NULL)), "D3AnimCraftzman.ini");

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	// of your final executable, you should remove from the following
	// the specific initialization routines you do not need
	// Change the registry key under which our settings are stored
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization
	SetRegistryKey(_T("Local AppWizard-Generated Applications"));
	LoadStdProfileSettings(16);  // Load standard INI file options (including MRU)
	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views
	CMultiDocTemplate* pDocTemplate;
	pDocTemplate = new CMultiDocTemplate(IDR_MD5AnimCraftzmaTYPE,
		RUNTIME_CLASS(CMD5AnimCraftzmanDoc),
		RUNTIME_CLASS(CChildFrame), // custom MDI child frame
		RUNTIME_CLASS(CMD5AnimCraftzmanView));
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);
	// create main MDI Frame window
	CMainFrame* pMainFrame = new CMainFrame;
	if (!pMainFrame || !pMainFrame->LoadFrame(IDR_MAINFRAME))
		return FALSE;
	m_pMainWnd = pMainFrame;
	// call DragAcceptFiles only if there's a suffix
	//  In an MDI app, this should occur immediately after setting m_pMainWnd
	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	m_wndCnf.Revert(true);

	// Dispatch commands specified on the command line.  Will return FALSE if
	// app was launched with /RegServer, /Register, /Unregserver or /Unregister.
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;
	// The main window has been initialized, so show and update it
	pMainFrame->ShowWindow(m_nCmdShow);
	pMainFrame->UpdateWindow();
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
void CMD5AnimCraftzmanApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}


// CMD5AnimCraftzmanApp message handlers

void CMD5AnimCraftzmanApp::OnOptsConf()
{
	m_wndCnf.DoModal();
}

void CMD5AnimCraftzmanApp::OnOptsUpdate()
{
	if (m_wndCnf.m_strDoom3.IsEmpty()) {
		if (m_wndCnf.DoModal() != IDOK) return;
		if (m_wndCnf.m_strDoom3.IsEmpty()) return;

		int r = AfxMessageBox("Update now?", 0 |MB_ICONEXCLAMATION |MB_YESNO);
		if (r != IDYES) return;
	}

	CWaitCursor wc;

	m_com.Run(m_wndCnf.m_strDoom3, "", false, false, false, false, false, croParseMaterial, false, Renderer::rrtInvis, gsDoom3);
}

CMD5AnimCraftzmanApp *GetApp()
{
	return &theApp;
}

CMyDoom3Context &GetCom()
{
	return theApp.GetCom();
}

int CMD5AnimCraftzmanApp::ExitInstance()
{
	m_wndCnf.Revert(false);

	return CWinApp::ExitInstance();
}

BOOL CMD5AnimCraftzmanApp::OnIdle(LONG lCount)
{
	int x = CWinApp::OnIdle(lCount) ? 1 : 0;

	for (size_t i = 0; i < m_vec.size(); i++) x += m_vec[i]->OnIdle(lCount) ? 1 : 0;

	return x != 0;
}
