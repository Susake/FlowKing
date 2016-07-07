
// TaoFlowDlg.h : ͷ�ļ�
//

#pragma once
#include "afxwin.h"
#include "afxcmn.h"

#include "DlgTaoFlowSettings.h"
#include "DlgAboutBox.h"
#include "TaoClientSocket.h"
#include "DlgOptimizeKeyWord.h"

// CTaoFlowDlg �Ի���
class CTaoFlowDlg : public CDialogEx
{
// ����
public:
	CTaoFlowDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_TAOFLOW_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:

	afx_msg void OnStnClickedStaticShowpic();
	afx_msg void OnBnClickedButtonTaoflowsettings();
	afx_msg void OnNMClickSyslinkSettings(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMClickSyslinkAbout(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMClickSyslinkRegister(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButtonStarttask2();
	//afx_msg void OnBnClickedButtonLogin();
	afx_msg void OnDblclkListTasklist(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);

	afx_msg void OnBnClickedButtonTaoalexasetting();
	afx_msg void OnBnClickedButtonTaoqqspacesetting();
	afx_msg void OnBnClickedButtonHidetobg();
	afx_msg long OnHotKey(WPARAM wParam,LPARAM lParam);
	afx_msg void OnNMClickSyslinkFaq(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButtonTaokeywordsetting();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnClose();

	CLinkCtrl m_linkRegister;
	CListCtrl m_ListTask;
	CStatic m_strTaoVersion;
	
	//socket
	CTaoClientSocket m_Sock;
	HANDLE m_ThreadHandle;
	
	//for Timer 1
	int m_nHour, m_nMin, m_nSecond;
	CStatic m_StcOnlineTime;

	//��ȡ�����ļ�
	void LoadConfigureFile();
	void LoadTaskDataFile();
	void GetFlowDataF(CString & strSection, CString & strKey,
		CString & strPath, CString & strValue);
	void GetTaoed();

	//�رս���
	void CloseProgram(CString strProgram);

	CStatic m_stcDycInfo;
	CStatic m_stcExInfo;

	//дwkdata����
	void SetFlowDataF(int nIndex);
	void SetKeyWordDataF(int nIndex);
	void SetAlexaDataF(int nIndex);
	void SetQSpaceDataF(int nIndex);

	void DelAllTaskDataF();
	CStatic m_stcTaoed;
};

