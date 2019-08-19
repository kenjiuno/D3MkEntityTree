// D3Pk4OleLib.cpp : DLL �G�N�X�|�[�g�̎����ł��B

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

// �I�[�o�[���C�h
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


// DLL �� OLE �ɂ���ăA�����[�h�ł���悤�ɂ��邩�ǂ������w�肵�܂��B
STDAPI DllCanUnloadNow(void)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());
    return (AfxDllCanUnloadNow()==S_OK && _AtlModule.GetLockCount()==0) ? S_OK : S_FALSE;
}


// �v�����ꂽ�^�̃I�u�W�F�N�g���쐬����N���X �t�@�N�g����Ԃ��܂��B
STDAPI DllGetClassObject(REFCLSID rclsid, REFIID riid, LPVOID* ppv)
{
    return _AtlModule.DllGetClassObject(rclsid, riid, ppv);
}


// DllRegisterServer - �G���g�����V�X�e�� ���W�X�g���ɒǉ����܂��B
STDAPI DllRegisterServer(void)
{
    // �I�u�W�F�N�g�A�^�C�v ���C�u��������у^�C�v ���C�u�������̑S�ẴC���^�[�t�F�C�X��o�^���܂�
    HRESULT hr = _AtlModule.DllRegisterServer();
	return hr;
}


// DllUnregisterServer - �G���g�������W�X�g������폜���܂��B
//
STDAPI DllUnregisterServer(void)
{
	HRESULT hr = _AtlModule.DllUnregisterServer();
	return hr;
}
