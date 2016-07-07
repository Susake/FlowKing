
// TaoFlowDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "TaoFlow.h"
#include "TaoFlowDlg.h"
#include "afxdialogex.h"
#include "TaskAttribute.h"
#include "def.h"
#include "DlgTaoAlexaSetting.h"
#include "DlgTaoQSpaceSettings.h"
#include "DlgSoftSettings.h"
#include "PacketInfo.h"

#include "windows.h"
#include "tlhelp32.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define CONFVALSIZE 150
#define BUFSIZE 8192
#define RECONNECT_SERVER 10*1000
#define READ_TIME_STEP 1000
#define TIMEOUT_COUNT 100
#define WAITFORMAIN_TIME 3000
#define BREATH_STEP 30

static const CString strVerSion("Ver 1.8.15 �ڲ��");
std::vector<TTaskAttribute> tasks;
//ȫ�ֿ�ݼ�ID
int g_nHotKeyId;

//Ĭ��ȫ���ȼ�alt+f6
WORD g_wVirtualKeyCode = VK_F6;
WORD g_wModifiers = HOTKEYF_ALT;
//��������
BOOL g_BStartWithWindows = FALSE;
//���߳�ʱ������
int g_nTimeout = TIMEOUT_COUNT;

// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

// CTaoFlowDlg �Ի���

CTaoFlowDlg::CTaoFlowDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CTaoFlowDlg::IDD, pParent)
	, m_nHour(0)
	, m_nMin(0)
	, m_nSecond(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CTaoFlowDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_SYSLINK_Register, m_linkRegister);
	DDX_Control(pDX, IDC_LIST_TaskList, m_ListTask);
	DDX_Control(pDX, IDC_STATIC_TaoVersionVar, m_strTaoVersion);
	DDX_Control(pDX, IDC_STATIC_OnlineTime, m_StcOnlineTime);
	DDX_Control(pDX, IDC_STATIC_DynamicInfo, m_stcDycInfo);
	DDX_Control(pDX, IDC_STATIC_ExInfo, m_stcExInfo);
	DDX_Control(pDX, IDC_STATIC_Taoed, m_stcTaoed);
}

BEGIN_MESSAGE_MAP(CTaoFlowDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()

	ON_MESSAGE(WM_HOTKEY,OnHotKey)
	ON_STN_CLICKED(IDC_STATIC_ShowPic, &CTaoFlowDlg::OnStnClickedStaticShowpic)
	ON_NOTIFY(NM_CLICK, IDC_SYSLINK_Settings, &CTaoFlowDlg::OnNMClickSyslinkSettings)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BUTTON_TaoFlowSettings, &CTaoFlowDlg::OnBnClickedButtonTaoflowsettings)
	ON_NOTIFY(NM_CLICK, IDC_SYSLINK_About, &CTaoFlowDlg::OnNMClickSyslinkAbout)
	ON_NOTIFY(NM_CLICK, IDC_SYSLINK_Register, &CTaoFlowDlg::OnNMClickSyslinkRegister)
//	ON_BN_CLICKED(IDC_BUTTON_Login, &CTaoFlowDlg::OnBnClickedButtonLogin)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_TaskList, &CTaoFlowDlg::OnDblclkListTasklist)
	ON_WM_CREATE()
ON_WM_CREATE()
ON_BN_CLICKED(IDC_BUTTON_TaoKeyWordSetting, &CTaoFlowDlg::OnBnClickedButtonTaokeywordsetting)
ON_WM_TIMER()
ON_BN_CLICKED(IDC_BUTTON_TaoAlexaSetting, &CTaoFlowDlg::OnBnClickedButtonTaoalexasetting)
ON_BN_CLICKED(IDC_BUTTON_TaoQQSpaceSetting, &CTaoFlowDlg::OnBnClickedButtonTaoqqspacesetting)
ON_BN_CLICKED(IDC_BUTTON_HideToBG, &CTaoFlowDlg::OnBnClickedButtonHidetobg)
ON_NOTIFY(NM_CLICK, IDC_SYSLINK_FAQ, &CTaoFlowDlg::OnNMClickSyslinkFaq)
ON_WM_CLOSE()
END_MESSAGE_MAP()


// CTaoFlowDlg ��Ϣ�������

BOOL CTaoFlowDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	m_ListTask.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT | LVS_NOSORTHEADER);
	m_ListTask.InsertColumn(0, _T("��������"), LVCFMT_CENTER, 80, 0);
	m_ListTask.InsertColumn(1, _T("Ŀ����ַ"), LVCFMT_CENTER, 465, 1);
	m_ListTask.InsertColumn(2, _T("״̬"), LVCFMT_CENTER, 60, 2);
	m_stcDycInfo.SetWindowTextW(_T("��������������������"));
	m_strTaoVersion.SetWindowTextW(strVerSion);
	m_stcExInfo.SetWindowTextW(NULL);
	
	LoadConfigureFile();
	LoadTaskDataFile();
	
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

//�������ļ�
void CTaoFlowDlg::LoadConfigureFile()
{
	CString strPath = _T(".//conf.ini");
	//��ȡ��ݼ�
	g_wVirtualKeyCode = GetPrivateProfileInt(_T("��ݼ�"), _T("VirtualKeyCode"), 0, strPath);
	g_wModifiers = GetPrivateProfileInt(_T("��ݼ�"), _T("Modifiers"), 0, strPath);
	//���ÿ�ݼ�
	g_nHotKeyId = GlobalAddAtom(_T("MyHotKey")) - 0xC000; 
	//Ĭ����ΪAlt+F6
	if ((0 == g_wVirtualKeyCode) || (0 == g_wModifiers))
	{
		//shift��alt�෴
		g_wModifiers = MOD_SHIFT;
		g_wVirtualKeyCode = VK_F6;
	}
	RegisterHotKey(AfxGetMainWnd()->m_hWnd, g_nHotKeyId, g_wModifiers, g_wVirtualKeyCode);
	//��ȡ��������
	g_BStartWithWindows = GetPrivateProfileInt(_T("��������"), _T("Value"), 0, strPath);
}

//�������ļ�
void CTaoFlowDlg::LoadTaskDataFile()
{
	CString strPath = _T(".\\wkdata");
	GetTaoed();
	int nTaskCount = GetPrivateProfileInt(_T("TaskCount"), _T("Value"), 0, strPath);
	 for(int i = 1; i <= nTaskCount; ++i)
	 {
		 CString strSection, strKey = _T("TaskType"), strValue;
		 strSection.Format(_T("%s%d"),  _T("Task"), i);
		 GetPrivateProfileString(strSection, strKey, _T(""),
			 strValue.GetBuffer(CONFVALSIZE), CONFVALSIZE, strPath);
		  
		 if(strValue == _T("Flow"))
		 {
			 GetFlowDataF(strSection, strKey, strPath, strValue);
		 }
		 else if(strValue == _T("KeyWord"))
		 {}
		 else if(strValue == _T("Alexa"))
		 {}
		 else if(strValue == _T("QSpacing"))
		 {}
	 }
}
void CTaoFlowDlg::GetFlowDataF(CString & strSection, CString & strKey,
							   CString & strPath, CString & strValue)
{
	TTaskAttribute task;
	strKey = _T("isTiming");
	task.bFlowTiming = GetPrivateProfileInt(strSection, strKey, 0, strPath)? true:false;
	strKey = _T("isMaxIp");
	task.bMaxIpPerHour = GetPrivateProfileInt(strSection, strKey, 0, strPath)? true:false;
	strKey = _T("NetAddr");
	strValue.ReleaseBuffer();
	GetPrivateProfileString(strSection, strKey, _T(""),
		strValue.GetBuffer(CONFVALSIZE), CONFVALSIZE, strPath);
	task.strNetAddr = strValue;
	strKey = _T("StartTime");
	task.nStarTiming = GetPrivateProfileInt(strSection, strKey, 0, strPath);
	strKey = _T("EndTime");
	task.nEndTiming = GetPrivateProfileInt(strSection, strKey, 0, strPath);
	strKey = _T("TaskIndex");
	task.nTaskIndex = GetPrivateProfileInt(strSection, strKey, 0, strPath);
	strKey = _T("IpPerHour");
	task.nMaxIpPerHour = GetPrivateProfileInt(strSection, strKey, 0, strPath);
	strKey = _T("IpPvRadio");
	task.nIP_PV_Ratio = GetPrivateProfileInt(strSection, strKey, 0, strPath);
	strKey = _T("stat");
	task.eStat = GetPrivateProfileInt(strSection, strKey, 0, strPath)? Running:Stop;
	strKey = _T("IpPvRadio");
	task.nIP_PV_Ratio = GetPrivateProfileInt(strSection, strKey, 0, strPath);
	CString strSource = strSection + _T("Source");
	strValue.ReleaseBuffer();
	DWORD ret = GetPrivateProfileSection(strSource, strValue.GetBuffer(BUFSIZE), BUFSIZE, strPath);

	DWORD nPos = 0;
	DWORD nIndex = 0;
	while (nPos < (ret - 2))
	{
		nPos = strValue.Find(_T("=\0"), nIndex);
		CString str = strValue.Mid(nIndex, nPos-nIndex);
		if (str.IsEmpty())
		{
			break;
		}
		Tlist list;
		list.strType = _T("�Զ�����Դ");
		list.strTypeAddr = str;
		task.vecSourceAddr.push_back(list);
		nIndex = nPos + 2;
	}

	CString strClick = strSection + _T("Click");
	strValue.ReleaseBuffer();
	ret = GetPrivateProfileSection(strClick, strValue.GetBuffer(BUFSIZE), BUFSIZE, strPath);

	nPos = 0;
	nIndex = 0;
	while (nPos < (ret - 2))
	{
		nPos = strValue.Find(_T("=\0"), nIndex);
		CString str = strValue.Mid(nIndex, nPos-nIndex);
		if (str.IsEmpty())
		{
			break;
		}
		Tlist list;
		list.strType = _T("�Զ�����");
		list.strTypeAddr = str;
		task.vecClickAddr.push_back(list);
		nIndex = nPos + 2;
	}
	strKey = _T("TaoType");
	task.nTaoType = GetPrivateProfileInt(strSection, strKey, 0, strPath);
	int nCount = m_ListTask.GetItemCount();
	switch (task.nTaoType)
	{
	case 0:
		m_ListTask.InsertItem(nCount, _T("������"));
		break;
	case 1:
		m_ListTask.InsertItem(nCount, _T("������"));
		break;
	case 2:
		m_ListTask.InsertItem(nCount, _T("�Թ��"));
		break;
	}
	m_ListTask.SetItemText(nCount, 1, task.strNetAddr);
	switch (task.eStat)
	{
	case Running:
		m_ListTask.SetItemText(nCount, 2, _T("������"));
		break;
	case Stop:
		m_ListTask.SetItemText(nCount, 2, _T("��ֹͣ"));
		break;
	case Finished:
		m_ListTask.SetItemText(nCount, 2, _T("�����"));
		break;
	}
	tasks.push_back(task);
}

void CTaoFlowDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{

	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CTaoFlowDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CTaoFlowDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

//banner
void CTaoFlowDlg::OnStnClickedStaticShowpic()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

}

//����
void CTaoFlowDlg::OnNMClickSyslinkSettings(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CDlgSoftSettings dlg;
	int nRet = dlg.DoModal();
	if (IDOK == nRet)
	{

	}
	*pResult = 0;
}

//ע���Ϊ�߼���Ա
void CTaoFlowDlg::OnNMClickSyslinkRegister(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	PNMLINK pNMLink = (PNMLINK)pNMHDR;
	::ShellExecute(m_hWnd, _T("open"), pNMLink->item.szUrl, NULL, NULL, SW_SHOWNORMAL);
	*pResult = 0;
}

//������
void CTaoFlowDlg::OnBnClickedButtonTaoflowsettings()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CDlgTaoFlowSettings dlgTaoFlowSet;
	dlgTaoFlowSet.SetCaption(_T("����������"));
	INT_PTR dlgRet = dlgTaoFlowSet.DoModal();
	
	if (IDOK == dlgRet)
	{
		std::vector<TTaskAttribute>::reverse_iterator rit = tasks.rbegin();
		int nCount = m_ListTask.GetItemCount();
		CString strTaskType;
		switch (rit->nTaoType)
		{
		case 0:
			strTaskType = _T("������");
			break;
		case 1:
			strTaskType = _T("������");
			break;
		case 2:
			strTaskType = _T("�Թ��");
			break;
		}
		m_ListTask.InsertItem(nCount, strTaskType);
		m_ListTask.SetItemText(nCount, 1, rit->strNetAddr);
		m_ListTask.SetItemText(nCount, 2, _T("������"));

		SetFlowDataF(tasks.size());
	}
}

void CTaoFlowDlg::CloseProgram(CString strProgram)
{
	HANDLE handle; //����CreateToolhelp32Snapshotϵͳ���վ�� 
	HANDLE handle1; //����Ҫ�������̾�� 
	handle = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);//���ϵͳ���վ�� 
	PROCESSENTRY32 *info; //����PROCESSENTRY32�ṹ��ָ 
	//PROCESSENTRY32 �ṹ�� dwSize ��Ա���ó� sizeof(PROCESSENTRY32) 

	info = new PROCESSENTRY32; 
	info->dwSize = sizeof(PROCESSENTRY32); 
	//����һ�� Process32First �������ӿ����л�ȡ�����б� 
	Process32First(handle, info); 
	//�ظ����� Process32Next��ֱ���������� FALSE Ϊֹ 
	while(Process32Next(handle, info) != FALSE) 
	{   
		CString strTmp = info->szExeFile;     //ָ���������   
		//strcmp�ַ����ȽϺ���ͬҪ������ͬ   
		//if(strcmp(c, info->szExeFile) == 0 )   
		if (strProgram.CompareNoCase(info->szExeFile) == 0 )   
		{   
			//PROCESS_TERMINATE��ʾΪ����������,FALSE=�ɼ̳�,info->th32ProcessID=����ID    
			handle1 = OpenProcess(PROCESS_TERMINATE, FALSE, info->th32ProcessID); 
			//��������    
			TerminateProcess(handle1, 0);    
		}   
	}
	delete info;

	CloseHandle(handle);
}

void CTaoFlowDlg::SetFlowDataF(int nIndex)
{
	TTaskAttribute rit = tasks.at(nIndex - 1);
	//���˸�д�����ļ��ٿ�
	CloseProgram(_T("wk.exe"));
	CString strPathName = _T(".\\wkdata");
	CString str, str1;
	str.Format(_T("%d"), tasks.size());
	//������
	WritePrivateProfileString(_T("TaskCount"), _T("Value"), str, strPathName);
	//������ʱ��Ŀ����ַ��IP / PV������
	str.Format(_T("%s%d"), _T("Task"), nIndex);
	WritePrivateProfileString(str, _T("NetAddr"), rit.strNetAddr, strPathName);
	WritePrivateProfileString(str, _T("TaskType"), _T("Flow"), strPathName);
	str1.Format(_T("%d"), nIndex - 1);
	WritePrivateProfileString(str, _T("TaskIndex"), str1, strPathName);
	str1.Format(_T("%d"), rit.nTaoType);
	WritePrivateProfileString(str, _T("TaoType"), str1, strPathName);
	str1.Format(_T("%d"), rit.nStarTiming);
	WritePrivateProfileString(str, _T("StartTime"), str1, strPathName);
	str1.Format(_T("%d"), rit.nEndTiming);
	WritePrivateProfileString(str, _T("EndTime"), str1, strPathName);
	str1.Format(_T("%d"), rit.bFlowTiming);
	WritePrivateProfileString(str, _T("isTiming"), str1, strPathName);
	str1.Format(_T("%d"), rit.bMaxIpPerHour);
	WritePrivateProfileString(str, _T("isMaxIp"), str1, strPathName);
	str1.Format(_T("%d"), rit.nMaxIpPerHour);
	WritePrivateProfileString(str, _T("IpPerHour"), str1, strPathName);
	str1.Format(_T("%d"), rit.nIP_PV_Ratio);
	WritePrivateProfileString(str, _T("IpPvRadio"), str1, strPathName);
	str.Format(_T("%s%d"), _T("Task"), nIndex);
	str1.Format(_T("%d"), rit.eStat);
	WritePrivateProfileString(str, _T("stat"), str1, strPathName);
	//��Դ��ַ
	std::vector<Tlist>::iterator sit = rit.vecSourceAddr.begin();
	str += _T("Source");
	for (; sit != rit.vecSourceAddr.end(); sit++)
	{
		WritePrivateProfileString(str, sit->strTypeAddr, _T(""), strPathName);
	}

	//�����ַ
	str.Format(_T("%s%d"), _T("Task"), nIndex);
	str += _T("Click");
	std::vector<Tlist>::iterator cit = rit.vecClickAddr.begin();
	for (; cit != rit.vecClickAddr.end(); cit++)
	{
		WritePrivateProfileString(str, cit->strTypeAddr, _T("ClickAddr"), strPathName);
	}

	WinExec(".\\wk.exe",SW_HIDE);
}
void CTaoFlowDlg::SetKeyWordDataF(int nIndex)
{
	TTaskAttribute rit = tasks.at(nIndex - 1);
	//���˸�д�����ļ��ٿ�
	CloseProgram(_T("wk.exe"));
	CString strPathName = _T(".\\wkdata");
	CString str, str1;
	str.Format(_T("%d"), tasks.size());
	//������
	WritePrivateProfileString(_T("TaskCount"), _T("Value"), str, strPathName);
	str.Format(_T("%s%d"), _T("Task"), nIndex);
	WritePrivateProfileString(str, _T("TaskType"), _T("KeyWord"), strPathName);
	//Ŀ����ַ
	WritePrivateProfileString(str, _T("NetAddr"), rit.strNetAddr, strPathName);
	//�ؼ���
	WritePrivateProfileString(str, _T("KeyWord"), rit.strKeyWord, strPathName);
	str1.Format(_T("%d"), rit.nSearchEngineSel);
	WritePrivateProfileString(str, _T("SearchEngine"), str1, strPathName);
	str1.Format(_T("%d"), rit.eStat);
	WritePrivateProfileString(str, _T("stat"), str1, strPathName);
	WinExec(".\\wk.exe",SW_HIDE);
}
void CTaoFlowDlg::SetAlexaDataF(int nIndex)
{
	TTaskAttribute rit = tasks.at(nIndex - 1);
	//���˸�д�����ļ��ٿ�
	CloseProgram(_T("wk.exe"));
	CString strPathName = _T(".\\wkdata");
	CString str, str1;
	str.Format(_T("%d"), tasks.size());
	//������
	WritePrivateProfileString(_T("TaskCount"), _T("Value"), str, strPathName);
	str.Format(_T("%s%d"), _T("Task"), nIndex);
	WritePrivateProfileString(str, _T("TaskType"), _T("Alexa"), strPathName);
	//Ŀ����ַ
	WritePrivateProfileString(str, _T("NetAddr"), rit.strNetAddr, strPathName);
	str1.Format(_T("%d"), rit.eStat);
	WritePrivateProfileString(str, _T("stat"), str1, strPathName);
	WinExec(".\\wk.exe",SW_HIDE);
}
void CTaoFlowDlg::SetQSpaceDataF(int nIndex)
{
	TTaskAttribute rit = tasks.at(nIndex - 1);
	//���˸�д�����ļ��ٿ�
	CloseProgram(_T("wk.exe"));
	CString strPathName = _T(".\\wkdata");
	CString str1;
	str1.Format(_T("%d"), tasks.size());
	//������
	WritePrivateProfileString(_T("TaskCount"), _T("Value"), str1, strPathName);
	str1.Format(_T("%s%d"), _T("Task"), nIndex);
	WritePrivateProfileString(str1, _T("TaskType"), _T("QSpacing"), strPathName);
	//Ŀ����ַ
	CString str = _T("http://user.qzone.qq.com/");
	str += rit.strQNumber;
	WritePrivateProfileString(str1, _T("NetAddr"), str, strPathName);
	str1.Format(_T("%d"), rit.eStat);
	str.Format(_T("%s%d"), _T("Task"), nIndex);
	WritePrivateProfileString(str, _T("stat"), str1, strPathName);
	WinExec(".\\wk.exe",SW_HIDE);
}

//�Թؼ���
void CTaoFlowDlg::OnBnClickedButtonTaokeywordsetting()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CDlgOptimizeKeyWord dlg;
	int nRet = dlg.DoModal();
	if (IDOK == nRet)
	{
		std::vector<TTaskAttribute>::reverse_iterator rit = tasks.rbegin();
		int nCount = m_ListTask.GetItemCount();
		m_ListTask.InsertItem(nCount, _T("�Թؼ���"));
		m_ListTask.SetItemText(nCount, 1, rit->strNetAddr);
		m_ListTask.SetItemText(nCount, 2, _T("������"));

		SetKeyWordDataF(tasks.size());
	}
}

//��Alexa
void CTaoFlowDlg::OnBnClickedButtonTaoalexasetting()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CDlgTaoAlexaSetting dlg;
	int nRet = dlg.DoModal();
	if (IDOK == nRet)
	{
		std::vector<TTaskAttribute>::reverse_iterator rit = tasks.rbegin();
		int nCount = m_ListTask.GetItemCount();
		m_ListTask.InsertItem(nCount, _T("��Alexa"));
		m_ListTask.SetItemText(nCount, 1, rit->strNetAddr);
		m_ListTask.SetItemText(nCount, 2, _T("������"));

		SetAlexaDataF(tasks.size());
	}
}

//��QQ�ռ�
void CTaoFlowDlg::OnBnClickedButtonTaoqqspacesetting()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CDlgTaoQSpaceSettings dlg;
	int nRet = dlg.DoModal();
	if (IDOK == nRet)
	{
		std::vector<TTaskAttribute>::reverse_iterator rit = tasks.rbegin();
		CString str = _T("http://user.qzone.qq.com/");
		str += rit->strQNumber;
		int nCount = m_ListTask.GetItemCount();
		m_ListTask.InsertItem(nCount, _T("��QQ�ռ�"));
		m_ListTask.SetItemText(nCount, 1, str);
		m_ListTask.SetItemText(nCount, 2, _T("������"));

		SetQSpaceDataF(tasks.size());
	}
}

//����
void CTaoFlowDlg::OnNMClickSyslinkAbout(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CDlgAboutBox about;
	about.DoModal();

	*pResult = 0;
}



//�����б�
void CTaoFlowDlg::OnDblclkListTasklist(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	if (pNMListView->iItem < 0)
	{
		return;
	}
	TTaskAttribute &task = tasks.at(pNMListView->iItem);
	int nRet;
	CDlgTaoFlowSettings dlg(task);
	CDlgOptimizeKeyWord dlg1(task);
	CDlgTaoAlexaSetting dlg2(task);
	CDlgTaoQSpaceSettings dlg3(task);
	switch (task.eType)
	{
	default:
		nRet = dlg.DoModal();
		break;
	case Alexa:
		nRet = dlg2.DoModal();
		break;
	case KeyWord:
		nRet = dlg1.DoModal();
		break;
	case QSpace:
		nRet = dlg3.DoModal();
		break;
	}
	if (IDOK == nRet)
	{
		CString str, str1;
		CString strPathName = _T(".\\wkdata");
		int nTaskCount = 0;
		unsigned int i = 0;
		switch (task.eAction)
		{
		default:
			break;
		case StartTask:
			m_ListTask.SetItemText(pNMListView->iItem, 2, _T("������"));
			CloseProgram(_T("wk.exe"));
			str1.Format(_T("%d"), 1);
			str.Format(_T("%s%d"), _T("Task"), pNMListView->iItem + 1);
			WritePrivateProfileString(str, _T("stat"), str1, strPathName);
			WinExec(".\\wk.exe",SW_HIDE);
			break;
		case StopTask:
			m_ListTask.SetItemText(pNMListView->iItem, 2, _T("��ֹͣ"));
			CloseProgram(_T("wk.exe"));
			str1.Format(_T("%d"), 0);
			str.Format(_T("%s%d"), _T("Task"), pNMListView->iItem + 1);
			WritePrivateProfileString(str, _T("stat"), str1, strPathName);
			WinExec(".\\wk.exe",SW_HIDE);
			break;
		case DelTask:
			tasks.erase(tasks.begin() + pNMListView->iItem);			
			CloseProgram(_T("wk.exe"));
			nTaskCount = GetPrivateProfileInt(_T("TaskCount"), _T("Value"), 0, strPathName);
			str.Format(_T("%d"), nTaskCount - 1);
			WritePrivateProfileString(_T("TaskCount"), _T("Value"), str, strPathName);
			DelAllTaskDataF();
			for (size_t i = 1; i <= tasks.size(); ++i)
			{
				SetFlowDataF(i);
			}
			m_ListTask.DeleteItem(pNMListView->iItem);
			WinExec(".\\wk.exe",SW_HIDE);
			break;
		case ModTask:
			CloseProgram(_T("wk.exe"));
			switch (task.eType)
			{
			default:
				m_ListTask.SetItemText(pNMListView->iItem, 1, task.strNetAddr);
				str.Format(_T("%s%d"), _T("Task"), pNMListView->iItem + 1);
				str += _T("Source");
				WritePrivateProfileString(str, NULL, NULL, strPathName);
				str.Format(_T("%s%d"), _T("Task"), pNMListView->iItem + 1);
				str += _T("Click");
				WritePrivateProfileString(str, NULL, NULL, strPathName);
				SetFlowDataF(pNMListView->iItem + 1);
				break;
			case KeyWord:
				m_ListTask.SetItemText(pNMListView->iItem, 1, task.strNetAddr);
				SetKeyWordDataF(pNMListView->iItem + 1);
				break;
			case Alexa:
				m_ListTask.SetItemText(pNMListView->iItem, 1, task.strNetAddr);
				SetAlexaDataF(pNMListView->iItem + 1);
				break;
			case QSpace:
				CString str = _T("user.qzone.qq.com/");
				str += task.strQNumber;
				m_ListTask.SetItemText(pNMListView->iItem, 1, str);
				SetQSpaceDataF(pNMListView->iItem + 1);
				break;
			}		
			WinExec(".\\wk.exe",SW_HIDE);
			break;
		}
	}
	
	*pResult = 0;
}

void CTaoFlowDlg::DelAllTaskDataF()
{
	CString str;
	for (int i = 1; i < MAXTASKCOUNT; ++i)
	{		
		str.Format(_T("%s%d"), _T("Task"), i);
		WritePrivateProfileString(str, NULL, NULL, _T(".\\wkdata"));
	}
}

//��������̨�һ�
void CTaoFlowDlg::OnBnClickedButtonHidetobg()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	ShowWindow(SW_HIDE);
}

HBRUSH CTaoFlowDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  �ڴ˸��� DC ���κ�����
	/*
	CFont * cFont=new CFont; 
	cFont->CreateFont(16,0,0,0,FW_SEMIBOLD,FALSE,FALSE,0,  ANSI_CHARSET,OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY, DEFAULT_PITCH&FF_SWISS,_T("Arial"));
	*/
	if ((pWnd->GetDlgCtrlID() == IDC_STATIC_DynamicInfo) ||
		(pWnd->GetDlgCtrlID() == IDC_STATIC_TaoVersionVar) ||
		(pWnd->GetDlgCtrlID() == IDC_STATIC_TaoVesionTxt))
	{ 
		pDC->SetTextColor(RGB(255, 0, 0));
	}
	if ((pWnd->GetDlgCtrlID() == IDC_STATIC_HotKeyNotice) ||
		(pWnd->GetDlgCtrlID() == IDC_STATIC_Taoed))
	{
		pDC->SetTextColor(RGB(0, 0, 255));
	}

	// TODO:  ���Ĭ�ϵĲ������軭�ʣ��򷵻���һ������
	return hbr;
}

void CTaoFlowDlg::GetTaoed()
{
	//��Taoed
	CString strPath = _T(".\\wkdata");
	int nTaoed = GetPrivateProfileInt(_T("Taoed"), _T("Value"), 0, strPath);
	CString str;
	str.Format(_T("%d"), nTaoed);
	m_stcTaoed.SetWindowTextW(str);
}

void CTaoFlowDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	CDialogEx::OnTimer(nIDEvent);
	
	CString strOnlineTime;
	CString strHour, strMin, strSecond;
	m_nSecond += 1;
	if (60 == m_nSecond)
	{
		m_nMin += 1;
		m_nSecond = 0;
		//ÿ���Ӷ�һ��Taoed
		GetTaoed();
	}
	if (60 == m_nMin)
	{
		m_nHour += 1;
		m_nMin = 0;
	}
	strHour.Format(_T("%d%s"), m_nHour, _T("Сʱ"));
	strMin.Format(_T("%d%s"), m_nMin, _T("����"));
	strSecond.Format(_T("%d%s"), m_nSecond, _T("��"));
	strOnlineTime += strHour;
	strOnlineTime += strMin;
	strOnlineTime += strSecond;
	if (m_nHour >= MAXINT)
	{
		//-��-
		m_StcOnlineTime.SetWindowTextW(_T("Very Very Long Time"));
	} 
	else
	{
		m_StcOnlineTime.SetWindowTextW(strOnlineTime);
	}

}

UINT ThreadFunc(LPVOID ThreadArg)
{  
	int nBreathStep = 0;
	unsigned long ul = 1;
	Sleep(WAITFORMAIN_TIME);
	CTaoFlowDlg *pDlg = (CTaoFlowDlg *)ThreadArg;
	pDlg->m_Sock.InitSock();
ConnectToServer:
	int sock = pDlg->m_Sock.ConnectTo(DEFAULT_SERVER_PORT, "192.168.1.2");
	if (sock <= 0)
	{
		pDlg->m_stcDycInfo.SetWindowTextW(_T("���ӷ�����ʧ�ܣ���������"));
		Sleep(RECONNECT_SERVER);
		goto ConnectToServer;
	}
	ioctlsocket(sock, FIONBIO, (unsigned long *)&ul);
LoginToServer:
	//����¼��
	PackHead_T head;
	head.ePType = LoginReq;
	head.eCType = FlowClient;
	LogCli_T body;
	CString strSerial = GetSerialNum();
	CString2Char(strSerial, body.MachCode);
	body.nSecretCode = SECRET_CODE;
	head.nBodySize = sizeof(LogCli_T);
	int flag = sizeof(PackHead_T) + sizeof(LogCli_T);
	std::string str;
	str.resize(flag + PACKSIZE_FLAG);
	memcpy((void *)(str.data()), &flag, PACKSIZE_FLAG);
	memcpy((void *)(str.data() + PACKSIZE_FLAG), &head, sizeof(head));
	memcpy((void *)(str.data() + PACKSIZE_FLAG + sizeof(head)), &body, sizeof(body));
	int n = send(sock, str.data(), flag + PACKSIZE_FLAG, 0);
	if (n >= flag + PACKSIZE_FLAG)
	{
		while (1)
		{
			str.resize(BUFFERSIZE);
			n = recv(sock, (char *)str.data(), BUFFERSIZE, 0);
			if (n >= PACKHEAD_SIZE)
			{
				PackHead_T head;
				memcpy(&head, (void *)(str.data() + PACKSIZE_FLAG), PACKHEAD_SIZE);
				if (head.ePType == BreathRes)
				{
					//���õ��߳�ʱ������
					g_nTimeout = TIMEOUT_COUNT;
				}
				else
				{
					int flagsize;
					memcpy(&flagsize, (void *)str.data(), PACKSIZE_FLAG);
					if (n >= flagsize)
					{
						switch (head.ePType)
						{
						case LoginRes:
							LogSer_T body;
							memcpy(&body, (void *)(str.data() + PACKSIZE_FLAG + PACKHEAD_SIZE), sizeof(LogSer_T));
							if (body.bOk)
							{
								pDlg->m_stcDycInfo.SetWindowTextW(_T("�ѳɹ�������������������"));
							}
							else
							{
								Sleep(RECONNECT_SERVER);
								goto LoginToServer;
							}
							break;
						case TaskRes:
							break;
						}
					}
				}
			}
			//��������
			nBreathStep++;
			if (nBreathStep > 30)
			{
				PackHead_T head;
				head.ePType = BreathReq;
				head.eCType = FlowClient;
				head.nBodySize = 0;
				int flag = PACKHEAD_SIZE;
				str.resize(flag + PACKSIZE_FLAG);
				memcpy((void *)(str.data()), &flag, PACKSIZE_FLAG);
				memcpy((void *)(str.data() + PACKSIZE_FLAG), &head, sizeof(head));
				n = send(sock, str.data(), flag + PACKSIZE_FLAG, 0);
				nBreathStep = 0;
			}
			Sleep(READ_TIME_STEP);
			//��ʱ������
			g_nTimeout--;
			if (g_nTimeout <= 0)
			{
				pDlg->m_stcDycInfo.SetWindowTextW(_T("�ӷ������������ѶϿ�����������"));
				closesocket(sock);
				goto ConnectToServer;
			}
		}
	}
	else
	{
		closesocket(sock);
		goto LoginToServer;
	}
	closesocket(sock);
	WSACleanup();
	return 0;
}

int CTaoFlowDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialogEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  �ڴ������ר�õĴ�������

	//socket thread
	m_ThreadHandle = AfxBeginThread(ThreadFunc,
		this,
		THREAD_PRIORITY_NORMAL,
		0,
		0,
		NULL);
	SetTimer(1, 990, NULL);
	WinExec(".\\wk.exe",SW_HIDE);
	return 0;
}

HRESULT CTaoFlowDlg::OnHotKey(WPARAM wParam,LPARAM lParam)
{
	if (this->IsWindowVisible())
	{
		this->ShowWindow(SW_HIDE);
	}
	else
	{
		this->ShowWindow(SW_SHOW);
	}
	
	return TRUE;
}

//FAQ
void CTaoFlowDlg::OnNMClickSyslinkFaq(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	*pResult = 0;
}


void CTaoFlowDlg::OnClose()
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if(MessageBox(_T("ȷ��Ҫ�˳���"), _T("�˳�������"), MB_YESNO) == IDYES)
	{
		//д�����ļ�
		CString strPathName = _T(".\\conf.ini");
		CString strVirtualKeyCode;
		CString strModifiers;
		strVirtualKeyCode.Format(_T("%hu"), g_wVirtualKeyCode);
		strModifiers.Format(_T("%hu"), g_wModifiers);
		WritePrivateProfileString(_T("��ݼ�"), _T("VirtualKeyCode"), strVirtualKeyCode, strPathName);
		WritePrivateProfileString(_T("��ݼ�"), _T("Modifiers"), strModifiers, strPathName);
		CString str;
		str.Format(_T("%d"), g_BStartWithWindows);
		WritePrivateProfileString(_T("��������"), _T("Value"), str, strPathName);
		
		CloseProgram(_T("wk.exe"));
		CDialogEx::OnClose();
	}
}


