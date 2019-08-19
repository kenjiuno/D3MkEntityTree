// D3Pk4OleLib.cpp : DLL エクスポートの実装です。

#include "stdafx.h"
#include "resource.h"
#include "D3Pk4OleLib.h"

class CD3Pk4OleLibModule : public CAtlDllModuleT< CD3Pk4OleLibModule >
{
public :
	DECLARE_LIBID(LIBID_D3Pk4OleLibLib)
	DECLARE_REGISTRY_APPID_RESOURCEID(IDR_D3PK4OLELIB, "{DBB7FC7E-A47B-4430-A70E-B7669230B4C2}")
};

CD3Pk4OleLibModule _AtlModule;

class CD3Pk4OleLibApp : public CWinApp
{
public:

// オーバーライド
    virtual BOOL InitInstance();
    virtual int ExitInstance();

    DECLARE_MESSAGE_MAP()
};

BEGIN_MESSAGE_MAP(CD3Pk4OleLibApp, CWinApp)
END_MESSAGE_MAP()

CD3Pk4OleLibApp theApp;

BOOL CD3Pk4OleLibApp::InitInstance()
{
    return CWinApp::InitInstance();
}

int CD3Pk4OleLibApp::ExitInstance()
{
    return CWinApp::ExitInstance();
}


// DLL を OLE によってアンロードできるようにするかどうかを指定します。
STDAPI DllCanUnloadNow(void)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());
    return (AfxDllCanUnloadNow()==S_OK && _AtlModule.GetLockCount()==0) ? S_OK : S_FALSE;
}


// 要求された型のオブジェクトを作成するクラス ファクトリを返します。
STDAPI DllGetClassObject(REFCLSID rclsid, REFIID riid, LPVOID* ppv)
{
    return _AtlModule.DllGetClassObject(rclsid, riid, ppv);
}


// DllRegisterServer - エントリをシステム レジストリに追加します。
STDAPI DllRegisterServer(void)
{
    // オブジェクト、タイプ ライブラリおよびタイプ ライブラリ内の全てのインターフェイスを登録します
    HRESULT hr = _AtlModule.DllRegisterServer();
	return hr;
}


// DllUnregisterServer - エントリをレジストリから削除します。
//
STDAPI DllUnregisterServer(void)
{
	HRESULT hr = _AtlModule.DllUnregisterServer();
	return hr;
}
