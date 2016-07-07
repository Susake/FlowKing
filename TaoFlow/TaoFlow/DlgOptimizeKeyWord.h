#pragma once
#include "afxwin.h"
#include "TaskAttribute.h"

// CDlgOptimizeKeyWord �Ի���

class CDlgOptimizeKeyWord : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgOptimizeKeyWord)

public:
	CDlgOptimizeKeyWord(CWnd* pParent = NULL);   // ��׼���캯��
	CDlgOptimizeKeyWord(TTaskAttribute &task, CWnd* pParent = NULL);
	virtual ~CDlgOptimizeKeyWord();

// �Ի�������
	enum { IDD = IDD_DIALOG_KeyWordRanking };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnBnClickedButtonStarttask();
	afx_msg void OnBnClickedButtonStoptask();
	afx_msg void OnBnClickedButtonDeltask();
	afx_msg void OnBnClickedButtonModtask();
	afx_msg void OnBnClickedOk();
	
	CEdit m_EditNetAddr;
	CEdit m_EditKeyWord;
	CButton m_BtnAddStartTask;
	CButton m_BtnDelTask;
	CButton m_BtnModTask;
	CButton m_BtnStopTask;
	CButton m_BtnStartTask;
	CComboBox m_ComboSearchEngine;

	int m_nTaskIndex;
	int m_nSearchEngineSel;
	bool m_bEntryType;
	bool m_bTaskIsRunning;
	CString m_StrNetAddr;
	CString m_strKeyWord;

};
