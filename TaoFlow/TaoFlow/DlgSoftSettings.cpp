// DlgSoftSettings.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "TaoFlow.h"
#include "DlgSoftSettings.h"
#include "afxdialogex.h"

extern int g_nHotKeyId;
extern WORD g_wVirtualKeyCode;
extern WORD g_wModifiers;
extern BOOL g_BStartWithWindows;

// CDlgSoftSettings �Ի���

IMPLEMENT_DYNAMIC(CDlgSoftSettings, CDialogEx)

CDlgSoftSettings::CDlgSoftSettings(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgSoftSettings::IDD, pParent)
{

}

CDlgSoftSettings::~CDlgSoftSettings()
{

}

void CDlgSoftSettings::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_HOTKEY_ShowHideSoft, m_hotShowHideWindow);
	DDX_Control(pDX, IDC_CHECK_StartWithWindows, m_CheckStartWithWindows);
	//  DDX_Check(pDX, IDC_CHECK_StartWithWindows, M_BStartWithWindows);
}


BEGIN_MESSAGE_MAP(CDlgSoftSettings, CDialogEx)
	ON_BN_CLICKED(IDOK, &CDlgSoftSettings::OnBnClickedOk)
	ON_BN_CLICKED(IDC_CHECK_StartWithWindows, &CDlgSoftSettings::OnBnClickedCheckStartwithwindows)
END_MESSAGE_MAP()


// CDlgSoftSettings ��Ϣ�������


void CDlgSoftSettings::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UnregisterHotKey(AfxGetMainWnd()->m_hWnd, g_nHotKeyId);
	m_hotShowHideWindow.GetHotKey(g_wVirtualKeyCode, g_wModifiers);
	RegSwitchAltShift();
	CDialogEx::OnOK();
}


BOOL CDlgSoftSettings::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	m_hotShowHideWindow.SetHotKey(g_wVirtualKeyCode, g_wModifiers);
	m_hotShowHideWindow.GetHotKey(g_wVirtualKeyCode, g_wModifiers);
	RegSwitchAltShift();

	m_CheckStartWithWindows.SetCheck(g_BStartWithWindows);
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CDlgSoftSettings::RegSwitchAltShift()
{
	if ((g_wModifiers & HOTKEYF_ALT) && !(g_wModifiers & HOTKEYF_SHIFT) )//Shift->ALt
	{
		g_wModifiers &= ~HOTKEYF_ALT;
		g_wModifiers |= MOD_ALT;
		RegisterHotKey(AfxGetMainWnd()->m_hWnd, g_nHotKeyId, g_wModifiers, g_wVirtualKeyCode);
		//�ڻָ���CHotKeyCtrl,�Ա��´���ʾ
		g_wModifiers |= HOTKEYF_ALT;
		g_wModifiers &= ~MOD_ALT;
	}
	else if (!(g_wModifiers & HOTKEYF_ALT) && (g_wModifiers & HOTKEYF_SHIFT))//Alt->Shift
	{
		g_wModifiers &= ~HOTKEYF_SHIFT;
		g_wModifiers |= MOD_SHIFT;
		RegisterHotKey(AfxGetMainWnd()->m_hWnd, g_nHotKeyId, g_wModifiers, g_wVirtualKeyCode);

		g_wModifiers |= HOTKEYF_SHIFT;
		g_wModifiers &= ~MOD_SHIFT;  
	}
	else
	{
		RegisterHotKey(AfxGetMainWnd()->m_hWnd, g_nHotKeyId, g_wModifiers, g_wVirtualKeyCode);
	}
}

void CDlgSoftSettings::OnBnClickedCheckStartwithwindows()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (m_CheckStartWithWindows.GetCheck())
	{
		g_BStartWithWindows = TRUE;
		SetStartWithWindows();
	} 
	else
	{
		g_BStartWithWindows = FALSE;
		DontStartWithWindows();
	}
}

void CDlgSoftSettings::SetStartWithWindows()
{
	HKEY hKey;   
	//�ҵ�ϵͳ��������   
	LPCTSTR lpRun = _T("Software\\Microsoft\\Windows\\CurrentVersion\\Run");   
	//��������Key   
	long lRet = RegOpenKeyEx(HKEY_LOCAL_MACHINE, lpRun, 0, KEY_WRITE, &hKey);
	if(lRet == ERROR_SUCCESS)   
	{   
		TCHAR pFileName[MAX_PATH] = {0};   
		//�õ����������ȫ·��   
		DWORD dwRet = GetModuleFileName(NULL, pFileName, MAX_PATH);   
		//���һ����Key,������ֵ
		lRet = RegSetValueEx(hKey, _T("TaoFlow"), 0, REG_SZ, (BYTE *)pFileName, dwRet);   
		//�ر�ע���   
		RegCloseKey(hKey);   
		if(lRet != ERROR_SUCCESS)   
		{   
			AfxMessageBox(_T("ϵͳ��������,������ɿ�����������"));   
		}   
		else  
		{  
			AfxMessageBox(_T("���ÿ��������ɹ�"));   
		}
	}   
}

void CDlgSoftSettings::DontStartWithWindows()
{
	HKEY hKey;   
	//�ҵ�ϵͳ��������   
	LPCTSTR lpRun = _T("Software\\Microsoft\\Windows\\CurrentVersion\\Run");   
	//��������Key   
	long lRet = RegOpenKeyEx(HKEY_LOCAL_MACHINE, lpRun, 0, KEY_WRITE, &hKey);   
	if(lRet == ERROR_SUCCESS)   
	{   
		TCHAR pFileName[MAX_PATH] = {0};   
		//�õ����������ȫ·��   
		DWORD dwRet = GetModuleFileName(NULL, pFileName, MAX_PATH);   
		//���һ����Key,������ֵ
		lRet = RegDeleteValue(hKey, _T("TaoFlow"));  
		//�ر�ע���   
		RegCloseKey(hKey);
		if(lRet != ERROR_SUCCESS)   
		{   
			AfxMessageBox(_T("ϵͳ��������,�������ȡ��������������"));   
		}   
		else{  
			AfxMessageBox(_T("ȡ�����������ɹ�"));   
		} 
	}
}
