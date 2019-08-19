// SelRendererTarDlg.cpp : 実装ファイル
//

#include "stdafx.h"
#include "D3MkEntityTree.h"
#include "SelRendererTarDlg.h"
#include "Doom3OpenGLWnd.h"

// CSelRendererTarDlg ダイアログ

IMPLEMENT_DYNAMIC(CSelRendererTarDlg, CDialog)
CSelRendererTarDlg::CSelRendererTarDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSelRendererTarDlg::IDD, pParent)
	, m_iSel(0)
{
}

CSelRendererTarDlg::~CSelRendererTarDlg()
{
}

void CSelRendererTarDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_CBIndex(pDX, IDC_COMBO_TAR, m_iSel);
	DDX_Control(pDX, IDC_STATIC_SAMPLE, m_wndSample);
	DDX_Control(pDX, IDC_STATIC_PV, m_wndPv);
	DDX_Control(pDX, IDC_COMBO_TAR, m_wndSel);
	DDX_Control(pDX, IDC_STATIC_DEV, m_wndDev);
}

BEGIN_MESSAGE_MAP(CSelRendererTarDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_TEST, OnBnClickedButtonTest)
END_MESSAGE_MAP()

// CSelRendererTarDlg メッセージ ハンドラ

void CSelRendererTarDlg::OnBnClickedButtonTest()
{
	CWaitCursor wc;

	HBITMAP hbm = NULL;

	int iSel = m_wndSel.GetCurSel();

	Renderer::CDoom3OpenGLWnd wndDlg;
	wndDlg.rrt = (Renderer::OpenGLRenderTar)iSel;

	if (wndDlg.CreateDef(NULL, CSize(100, 100))) {
		hbm = wndDlg.CaptureTestScene();
		if (hbm == NULL) {
			AfxMessageBox("Capture rendered image failed. Unknown reason.", 0 |MB_ICONEXCLAMATION);
		}
		wndDlg.Close();
	} else {
		wndDlg.ResetCaps();
		AfxMessageBox("Renderer creation failed. Your VGA may not support required caps.", 0 |MB_ICONEXCLAMATION);
	}

	HBITMAP hbmPrior = m_wndPv.SetBitmap(hbm);
	if (hbmPrior != NULL) DeleteObject(hbmPrior);

	Invalidate();

	{
		CString strDev = "...";
		if (wndDlg.nDevColorBits == 0) {

		} else {
			CString strClass;
			if (wndDlg.nDevClass == 0) {
				strClass.Format(
					"%s"
					, wndDlg.fDevHW ? "HW" : "SW"
					);
			} else {
				strClass.Format(
					"%s"
					,
						(wndDlg.nDevClass == 0) ? "SW" : 
						(
							(wndDlg.nDevClass == 1) ? "Unknown" :
							(
								(wndDlg.nDevClass == 'I') ? "HW(ICD)" :
								(
									(wndDlg.nDevClass == 'M') ? "HW(MCD)" :
									(
										"?"
									)
								)
							)
						)
					);
			}
			strDev.Format(
				"ColorBits:%d, AlphaBits:%d, DepthBits:%d, Class:%s"
				, wndDlg.nDevColorBits
				, wndDlg.nDevAlphaBits
				, wndDlg.nDevDepthBits
				, (LPCTSTR)strClass
				);
		}
		m_wndDev.SetWindowText(strDev);
	}
}
