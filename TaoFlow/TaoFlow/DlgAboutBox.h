#pragma once
#include "afxwin.h"


// CDlgAboutBox �Ի���

class CDlgAboutBox : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgAboutBox)

public:
	CDlgAboutBox(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgAboutBox();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
//	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	CStatic m_stcMachSerial;
	virtual BOOL OnInitDialog();
};
