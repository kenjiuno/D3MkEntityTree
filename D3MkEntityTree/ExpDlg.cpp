
// +--------------------------------------------------
// |
// | ExpDlg.cpp
// |
// | D3MkEntityTree : Copyright (c) 2004, kentaro-k.21
// |

#include "stdafx.h"
#include "D3MkEntityTree.h"
#include "ExpDlg.h"
#include "OSP.h"
#include "Context.h"
#include "ProfStr.h"
#include "SelEntityDefDlg.h"
#include "Sol.h"
#include "AboutDlg.h"
#include "ProfStr1.h"
#include "DoneDlg.h"

using namespace OSP;

namespace
{
const DWORD nSelDirFlags = 0
	|OFN_ENABLESIZING
	|OFN_EXPLORER
	|OFN_HIDEREADONLY
	|OFN_LONGNAMES
	|OFN_PATHMUSTEXIST
	;
};

IMPLEMENT_DYNAMIC(CExpDlg, CDialog)
CExpDlg::CExpDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CExpDlg::IDD, pParent)
	, m_strDoom3(_T(""))
	, m_strNewFolder(_T(""))
	, m_fExpose(FALSE)
	, m_fGamma(FALSE)
	, m_fMakeAvi(FALSE)
	, m_fARB_fragment_program(true)
	, m_fARB_texture_env_dot3(true)
	, m_fMake_entityDef(FALSE)
	, m_fMake_Mapobjs(FALSE)
	, m_fMkHHP(FALSE)
	, m_fLow(FALSE)
	, m_fEntityModel(FALSE)
	, m_iGameSel(0)
{
#ifdef _DEBUG
	m_strDoom3 = 
		"C:\\HDA5\\Doom 3";
	m_strNewFolder =
		"C:\\HDA5\\Doom_3.ex";
#endif

	m_strCnf = OSP_JoinPath(OSP_GetDir(OSP_GetModuleFileName(NULL)), "D3MkEntityTree.ini");

	Revert(true);
}

void CExpDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_DOOM3, m_strDoom3);
	DDX_Text(pDX, IDC_EDIT_NEWFOLDER, m_strNewFolder);
	//	DDX_Check(pDX, IDC_CHECK_EXPOSE, m_fExpose);
	DDX_Check(pDX, IDC_CHECK_GAMMA, m_fGamma);
	DDX_Check(pDX, IDC_CHECK_MAKE_AVI, m_fMakeAvi);
	DDX_Check(pDX, IDC_CHECK_GL01, m_fARB_fragment_program);
	DDX_Check(pDX, IDC_CHECK_GL02, m_fARB_texture_env_dot3);
	DDX_Control(pDX, IDC_STATIC_SEL_ENTITYDEF, m_wndSelEntityDef);
	DDX_Control(pDX, IDC_STATIC_SEL_MAPOBJS, m_wndSelMapobjs);
	DDX_Check(pDX, IDC_CHECK_MAKE_ENTITYDEF, m_fMake_entityDef);
	DDX_Check(pDX, IDC_CHECK_MAKE_MAPOBJS, m_fMake_Mapobjs);
	DDX_Check(pDX, IDC_CHECK_MK_HHP, m_fMkHHP);
	DDX_Check(pDX, IDC_CHECK_LOW, m_fLow);
	DDX_Check(pDX, IDC_CHECK_ENTITY_MODEL, m_fEntityModel);
	DDX_Control(pDX, IDC_STATIC_SEL_RENDERER_TAR, m_wndSelRendererTar);
	DDX_CBIndex(pDX, IDC_COMBO_GAMESEL, m_iGameSel);
}

BEGIN_MESSAGE_MAP(CExpDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_DOOM3, OnBnClickedButtonDoom3)
	ON_BN_CLICKED(IDC_BUTTON_NEWFOLDER, OnBnClickedButtonNewfolder)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_WM_DESTROY()
	ON_WM_CLOSE()
	ON_REGISTERED_MESSAGE(WM_XHYPERLINK_CLICKED, OnXHyperLink)
	ON_STN_CLICKED(IDC_STATIC_SEL_ENTITYDEF, OnStnClickedStaticSelEntitydef)
	ON_STN_CLICKED(IDC_STATIC_SEL_MAPOBJS, OnStnClickedStaticSelMapobjs)
	ON_BN_CLICKED(IDC_BUTTON1, OnBnClickedButton1)
	ON_STN_CLICKED(IDC_STATIC_SEL_RENDERER_TAR, OnStnClickedStaticSelRendererTar)
END_MESSAGE_MAP()

void CExpDlg::OnBnClickedButtonDoom3()
{
	if (!UpdateData()) return;

	CFileDialog wndDlg(
		true,
		"exe",
		OSP_JoinPath(m_strDoom3, "(DIR)"),
		nSelDirFlags,
		"DOOM3.exe (*.exe)|*.exe||",
		this
		);
	if (wndDlg.DoModal() != IDOK) return;

	m_strDoom3 = OSP_GetDir(wndDlg.GetPathName());
	UpdateData(false);
}

void CExpDlg::OnBnClickedButtonNewfolder()
{
	if (!UpdateData()) return;

	CFileDialog wndDlg(
		false,
		"",
		OSP_JoinPath(m_strNewFolder, "(DIR)"),
		nSelDirFlags,
		"Select new folder (*.*)|*.*||",
		this
		);
	if (wndDlg.DoModal() != IDOK) return;

	m_strNewFolder = OSP_GetDir(wndDlg.GetPathName());
	UpdateData(false);
}

void CExpDlg::OnBnClickedOk()
{
	if (!UpdateData()) return;

	{
		int cro = 0
			|(m_fMake_entityDef ? (croMake_entityDef) : 0)
			|(m_fMake_Mapobjs ? (croMake_Mapobjects) : 0)
			|(m_fMkHHP ? (croMake_hhp) : 0)
			|(m_fEntityModel ? (croMake_Mapentity) : 0)
			;

		bool fExpose = (GetKeyState(VK_NONCONVERT) & 0x8000) != 0; // only my DEBUG purpose

		CDoom3Context c(m_cwk);
		c.Run(m_strDoom3, m_strNewFolder, fExpose, m_fGamma, m_fMakeAvi, m_fARB_fragment_program, m_fARB_texture_env_dot3, (ContextRunOpts)cro, m_fLow, m_wndSelTar.m_iSel, (GameSel)m_iGameSel);

		printf("Release\n");
	}
	printf("Release ok\n\n");

	CString strHref = OSP_JoinPath(m_strNewFolder, "index.html");

	DWORD x = GetFileAttributes(strHref);
	if (x != 0xffffffff && !(x & FILE_ATTRIBUTE_DIRECTORY)) {
		CDoneDlg wndDlg(this);
		wndDlg.m_strHref = strHref;
		wndDlg.DoModal();
	}
	else {
		AfxMessageBox("Export finished", 0 |MB_ICONINFORMATION);
	}
}

void CExpDlg::OnDestroy()
{
	if (UpdateData()) {
		Revert(false);
	}

	CDialog::OnDestroy();
}

void CExpDlg::OnClose()
{
	CDialog::OnClose();
}

BOOL CExpDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	LOGFONT lf;
	ZeroMemory(&lf, sizeof(lf));
	::GetObject(::GetStockObject(DEFAULT_GUI_FONT), sizeof(LOGFONT), &lf);
	lf.lfWeight = FW_BOLD;
	m_font.CreateFontIndirect(&lf);

	{
		CWnd *p = GetWindow(GW_CHILD);
		for (; p != NULL; p = p->GetNextWindow()) {
			TCHAR tc[MAX_PATH +1] = {""};
			::GetClassName(*p, tc, MAX_PATH);
			if (_tcsicmp(tc, "STATIC") != 0)
				continue;
			if (!(p->GetDlgCtrlID() < 0))
				continue;
			if (!(p->GetStyle() & SS_SIMPLE))
				continue;
			p->SetFont(&m_font);
		}
	}

#define SET_UP_XHL(X) \
	X.EnableURL(false); \
	X.EnableTooltip(false); \
	X.SetNotifyParent(true); \

	SET_UP_XHL(m_wndSelEntityDef)
	SET_UP_XHL(m_wndSelMapobjs)
	SET_UP_XHL(m_wndSelRendererTar)

	return TRUE;
}

LRESULT CExpDlg::OnXHyperLink(WPARAM wp, LPARAM)
{
	SendMessage(WM_COMMAND, MAKEWPARAM(wp, STN_CLICKED));
	return 0;
}

void CExpDlg::OnStnClickedStaticSelEntitydef()
{
	if (!UpdateData()) return;

	CSelEntityDefDlg wndDlg(this);
	wndDlg.m_strDoom3 = m_strDoom3;
	wndDlg.m_fUse_entityDef = true;
	wndDlg.m_names.swap(m_cwk.entityDefExport);
	wndDlg.m_fSelAll = wndDlg.m_names.empty();
	wndDlg.m_fOnly = m_cwk.fOnly_entityDef;
	wndDlg.m_gs = (GameSel)m_iGameSel;

	int r = wndDlg.DoModal();
	wndDlg.m_names.swap(m_cwk.entityDefExport);
	m_cwk.fOnly_entityDef = wndDlg.m_fOnly ? true : false;
	if (r != IDOK) return;
}

void CExpDlg::OnStnClickedStaticSelMapobjs()
{
	if (!UpdateData()) return;

	CSelEntityDefDlg wndDlg(this);
	wndDlg.m_strDoom3 = m_strDoom3;
	wndDlg.m_fUse_entityDef = false;
	wndDlg.m_names.swap(m_cwk.mapobjsExport);
	wndDlg.m_fSelAll = wndDlg.m_names.empty();
	wndDlg.m_fOnly = m_cwk.fOnly_mapobjs;
	wndDlg.m_gs = (GameSel)m_iGameSel;

	int r = wndDlg.DoModal();
	wndDlg.m_names.swap(m_cwk.mapobjsExport);
	m_cwk.fOnly_mapobjs = wndDlg.m_fOnly ? true : false;
	if (r != IDOK) return;
}

void CExpDlg::Revert(bool fRevert)
{
	LPCSTR pszApp = "D3MkEntityTree";

	if (fRevert) {
		ProfStr::LoadProfileStringFrom(pszApp, "Doom3", m_strDoom3, "", m_strCnf);
		ProfStr::LoadProfileStringFrom(pszApp, "NewFolder", m_strNewFolder, "", m_strCnf);
	//	m_fExpose = ProfStr::GetProfileIntFrom(pszApp, "Expose", 0, m_strCnf) ? true : false;
		m_fGamma = ProfStr::GetProfileIntFrom(pszApp, "Gamma", 0, m_strCnf) ? true : false;
		m_fMakeAvi = ProfStr::GetProfileIntFrom(pszApp, "MakeAvi", 0, m_strCnf) ? true : false;
		m_fARB_fragment_program = ProfStr::GetProfileIntFrom(pszApp, "UseARB_fragment_program", 1, m_strCnf) ? true : false;
		m_fARB_texture_env_dot3 = ProfStr::GetProfileIntFrom(pszApp, "UseARB_texture_env_dot3", 1, m_strCnf) ? true : false;
		m_fMake_entityDef = ProfStr::GetProfileIntFrom(pszApp, "Make_entityDef", 1, m_strCnf) ? true : false;
		m_fMake_Mapobjs = ProfStr::GetProfileIntFrom(pszApp, "Make_Mapobjs", 1, m_strCnf) ? true : false;
		m_cwk.fOnly_entityDef = ProfStr::GetProfileIntFrom(pszApp, "Only_entityDef", 0, m_strCnf) ? true : false;
		m_cwk.fOnly_mapobjs = ProfStr::GetProfileIntFrom(pszApp, "Only_mapobjs", 0, m_strCnf) ? true : false;
		ProfStr2::ParseProfileSectionFrom("entityDefExport", m_cwk.entityDefExport, m_strCnf);
		ProfStr2::ParseProfileSectionFrom("mapobjsExport", m_cwk.mapobjsExport, m_strCnf);
		m_fMkHHP = ProfStr::GetProfileIntFrom(pszApp, "MkHHP", 0, m_strCnf) ? true : false;
		m_fLow = ProfStr::GetProfileIntFrom(pszApp, "LowProcessPriority", 0, m_strCnf) ? true : false;
		m_fEntityModel = ProfStr::GetProfileIntFrom(pszApp, "EntityModel", 0, m_strCnf) ? true : false;
		m_wndSelTar.m_iSel = ProfStr::GetProfileIntFrom(pszApp, "RendererTar", 0, m_strCnf);
		m_iGameSel = ProfStr::GetProfileIntFrom(pszApp, "GameSel", 0, m_strCnf);
	} else {
		ProfStr::SaveProfileStringTo(pszApp, "Doom3", m_strDoom3, m_strCnf);
		ProfStr::SaveProfileStringTo(pszApp, "NewFolder", m_strNewFolder, m_strCnf);
	//	ProfStr::SaveProfileIntTo(pszApp, "Expose", m_fExpose, m_strCnf);
		ProfStr::SaveProfileIntTo(pszApp, "Gamma", m_fGamma, m_strCnf);
		ProfStr::SaveProfileIntTo(pszApp, "MakeAvi", m_fMakeAvi, m_strCnf);
		ProfStr::SaveProfileIntTo(pszApp, "UseARB_fragment_program", m_fARB_fragment_program, m_strCnf);
		ProfStr::SaveProfileIntTo(pszApp, "UseARB_texture_env_dot3", m_fARB_texture_env_dot3, m_strCnf);
		ProfStr::SaveProfileIntTo(pszApp, "Make_entityDef", m_fMake_entityDef, m_strCnf);
		ProfStr::SaveProfileIntTo(pszApp, "Make_Mapobjs", m_fMake_Mapobjs, m_strCnf);
		ProfStr::SaveProfileIntTo(pszApp, "Only_entityDef", m_cwk.fOnly_entityDef, m_strCnf);
		ProfStr::SaveProfileIntTo(pszApp, "Only_mapobjs", m_cwk.fOnly_mapobjs, m_strCnf);
		CProfStr::ReplaceProfileSection("entityDefExport", Sol::MkText(m_cwk.entityDefExport), m_strCnf);
		CProfStr::ReplaceProfileSection("mapobjsExport", Sol::MkText(m_cwk.mapobjsExport), m_strCnf);
		ProfStr::SaveProfileIntTo(pszApp, "MkHHP", m_fMkHHP, m_strCnf);
		ProfStr::SaveProfileIntTo(pszApp, "LowProcessPriority", m_fLow, m_strCnf);
		ProfStr::SaveProfileIntTo(pszApp, "EntityModel", m_fEntityModel, m_strCnf);
		ProfStr::SaveProfileIntTo(pszApp, "RendererTar", m_wndSelTar.m_iSel, m_strCnf);
		ProfStr::SaveProfileIntTo(pszApp, "GameSel", m_iGameSel, m_strCnf);
	}
}

void CExpDlg::OnBnClickedButton1()
{
	CAboutDlg wndDlg;
	wndDlg.DoModal();
}

void CExpDlg::OnStnClickedStaticSelRendererTar()
{
	if (!UpdateData()) return;

	m_wndSelTar.DoModal();
}
