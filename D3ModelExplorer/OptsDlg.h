#pragma once


// COptsDlg �_�C�A���O

class COptsDlg : public CDialog
{
	DECLARE_DYNAMIC(COptsDlg)

public:
	COptsDlg(CWnd* pParent = NULL);   // �W���R���X�g���N�^
	virtual ~COptsDlg();

// �_�C�A���O �f�[�^
	enum { IDD = IDD_OPTS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g

	DECLARE_MESSAGE_MAP()
public:
	CString m_strDoom3;
	afx_msg void OnBnClickedButtonDoom3();
};
