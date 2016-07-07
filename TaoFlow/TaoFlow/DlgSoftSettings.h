#pragma once
#include "afxcmn.h"
#include "afxwin.h"


// CDlgSoftSettings �Ի���

class CDlgSoftSettings : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgSoftSettings)

public:
	CDlgSoftSettings(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgSoftSettings();

// �Ի�������
	enum { IDD = IDD_DIALOG_SoftSetting };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CHotKeyCtrl m_hotShowHideWindow;

	CButton m_CheckStartWithWindows;

	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCheckStartwithwindows();
	virtual BOOL OnInitDialog();

	//���CHotKeyCtrl��RegisterHotKey��alt��shift�����෴������
	void RegSwitchAltShift();
	//����/ȡ����������
	void SetStartWithWindows();
	void DontStartWithWindows();
	
	
//	BOOL M_BStartWithWindows;
};
