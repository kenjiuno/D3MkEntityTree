========================================================================
    �A�N�e�B�u �e���v���[�g ���C�u���� : D3Pk4OleLib �v���W�F�N�g�̊T�v
========================================================================

AppWizard ���쐬�������� D3Pk4OleLib �A�v���P�[�V�����ɂ́A
�_�C�i�~�b�N �����N ���C�u���� (DLL) �̊�{�I�Ȏg������������Ă��܂��B�A�v���P�[�V�����쐬�̂ЂȌ^�Ƃ��Ă��g�����������B

���̃t�@�C���ɂ̓v���W�F�N�g���\�����Ă���e�t�@�C����
�T�v���܂܂�Ă��܂��B

D3Pk4OleLib.vcproj
    ����̓A�v���P�[�V���� �E�B�U�[�h�Ő������ꂽ VC++ �v���W�F�N�g�̃��C�� �v���W�F�N�g �t�@�C���ł��B
    �t�@�C�����������ꂽ Visual C++ �̃o�[�W������񂪊܂܂�Ă��܂��B 
    �܂��A�A�v���P�[�V���� �E�B�U�[�h�őI�������v���b�g�t�H�[���A�\������уv���W�F�N�g�@�\�Ɋւ���
    �����܂܂�Ă��܂��B

D3Pk4OleLib.idl
    ���̃t�@�C���̓^�C�v ���C�u������ IDL ��`�A�v���W�F�N�g�Œ�`���ꂽ�C���^�[�t�F�C�X�A
    ����уR�N���X���܂�ł��܂��B
    ���̃t�@�C���� MIDL �R���p�C���ɂ���ď�������A�ȉ��̃t�@�C���𐶐����܂� :
        C++ �C���^�[�t�F�C�X��`����� GUID �錾 (D3Pk4OleLib.h)
        GUID �錾                                (D3Pk4OleLib_i.c)
        �^�C�v ���C�u����                                  (D3Pk4OleLib.tlb)
        �}�[�V�������O �R�[�h                                 (D3Pk4OleLib_p.c and dlldata.c)

D3Pk4OleLib.h
    ���̃t�@�C���� D3Pk4OleLib.idl �Œ�`���ꂽ���ڂ� C++ �C���^�[�t�F�C�X��`����� GUID �錾
    ���܂�ł��܂��B���̃t�@�C���� MIDL �ɂ���ăR���p�C�����ɍĐ�������܂��B
D3Pk4OleLib.cpp
    ���̃t�@�C���̓I�u�W�F�N�g �}�b�v����� DLL �G�N�X�|�[�g�̎������܂�ł��܂��B
D3Pk4OleLib.rc
    ����̓v���O�������g�p���� Microsoft Windows �̃��\�[�X��
    �ꗗ�t�@�C���ł��B

D3Pk4OleLib.def
    ���̃��W���[����`�t�@�C���́ADLL �ŕK�v�ȃG�N�X�|�[�g�Ɋւ�����ւ̃����J��񋟂��A
    ���̃G�N�X�|�[�g�����܂�ł��܂� :
        DllGetClassObject  
        DllCanUnloadNow    
        GetProxyDllInfo    
        DllRegisterServer	
        DllUnregisterServer

/////////////////////////////////////////////////////////////////////////////
���̑��̕W���t�@�C�� :

StdAfx.h, StdAfx.cpp
    �����̃t�@�C���̓v���R���p�C���ς݃w�b�_�[ (PCH) �t�@�C�� D3Pk4OleLib.pch�A
    ����уv���R���p�C�����ꂽ�^�� (PCT) �t�@�C�� stdafx.obj ���r���h���邽�߂Ɏg���܂��B

Resource.h
    ���̃t�@�C���̓��\�[�X ID ���`����W���w�b�_�[ �t�@�C���ł��B

/////////////////////////////////////////////////////////////////////////////
�v���L�V/�X�^�u DLL �v���W�F�N�g����у��W���[����`�t�@�C�� :

D3Pk4OleLibps.vcproj
    ���̃t�@�C���͕K�v�ɉ����ăv���L�V/�X�^�u�̃r���h�Ɏg�p�����v���W�F�N�g �t�@�C���ł��B
	��ȃv���W�F�N�g�� IDL �t�@�C���ɂ͏��Ȃ��Ƃ��C���^�[�t�F�C�X�� 1 �܂݁A
	�v���L�V/�X�^�u DLL ���r���h����O�� IDL �t�@�C�����R���p�C������K�v������܂��B	���̉ߒ���
\�v���L�V/�X�^�u DLL ���r���h����̂ɕK�v�� tdlldata.c�AD3Pk4OleLib_i.c ����� D3Pk4OleLib_p.c ��
	��������܂��B

D3Pk4OleLibps.def
    ���̃��W���[����`�t�@�C���́A�v���L�V/�X�^�u�ŕK�v�ȃG�N�X�|�[�g�Ɋւ���
    ���ւ̃����J��񋟂��܂��B
/////////////////////////////////////////////////////////////////////////////
���̑��̒��� :

	MFC  �T�|�[�g�I�v�V�����́AMFC  �N���X�A�I�u�W�F�N�g����ъ֐����g�p�ł���悤�ɁA
	�X�P���g�� �A�v���P�[�V������ Microsoft Foundation Class ���C�u�������r���h���܂��B
/////////////////////////////////////////////////////////////////////////////
