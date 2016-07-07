// DlgTaoQSpaceSettings.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "TaoFlow.h"
#include "DlgTaoQSpaceSettings.h"
#include "afxdialogex.h"

extern std::vector<TTaskAttribute> tasks;

// CDlgTaoQSpaceSettings �Ի���

IMPLEMENT_DYNAMIC(CDlgTaoQSpaceSettings, CDialogEx)

CDlgTaoQSpaceSettings::CDlgTaoQSpaceSettings(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgTaoQSpaceSettings::IDD, pParent)
	, m_bEntryType(FALSE)
	, m_bTaskIsRunning(TRUE)
	, m_strQNumber(_T(""))
{

}

CDlgTaoQSpaceSettings::CDlgTaoQSpaceSettings(TTaskAttribute &task, CWnd* pParent)
	: CDialogEx(CDlgTaoQSpaceSettings::IDD, pParent)
{
	if (task.eStat == Running)
	{
		m_bTaskIsRunning = true;
	}
	else
	{
		m_bTaskIsRunning = false;
	}
	m_bEntryType = true;
	m_strQNumber = task.strQNumber;
	m_nTaskIndex = task.nTaskIndex;
}

CDlgTaoQSpaceSettings::~CDlgTaoQSpaceSettings()
{
}

void CDlgTaoQSpaceSettings::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_EditQNumber);
	DDX_Text(pDX, IDC_EDIT1, m_strQNumber);
	DDX_Control(pDX, IDOK, m_BtnAddStartTask);
	DDX_Control(pDX, IDC_BUTTON_ModTask, m_BtnModTask);
	DDX_Control(pDX, IDC_BUTTON_DelTask, m_BtnDelTask);
	DDX_Control(pDX, IDC_BUTTON_StopTask, m_BtnStopTask);
	DDX_Control(pDX, IDC_BUTTON_StartTask, m_BtnStartTask);
}


BEGIN_MESSAGE_MAP(CDlgTaoQSpaceSettings, CDialogEx)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDOK, &CDlgTaoQSpaceSettings::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON_StartTask, &CDlgTaoQSpaceSettings::OnBnClickedButtonStarttask)
	ON_BN_CLICKED(IDC_BUTTON_StopTask, &CDlgTaoQSpaceSettings::OnBnClickedButtonStoptask)
	ON_BN_CLICKED(IDC_BUTTON_DelTask, &CDlgTaoQSpaceSettings::OnBnClickedButtonDeltask)
	ON_BN_CLICKED(IDC_BUTTON_ModTask, &CDlgTaoQSpaceSettings::OnBnClickedButtonModtask)
END_MESSAGE_MAP()


// CDlgTaoQSpaceSettings ��Ϣ�������


BOOL CDlgTaoQSpaceSettings::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	m_EditQNumber.SetCueBanner(_T("���ڴ˴���������QQ��"));
	m_EditQNumber.SetWindowTextW(m_strQNumber);
	if (m_bEntryType)
	{
		m_BtnAddStartTask.EnableWindow(FALSE);
	}
	else
	{
		m_BtnDelTask.EnableWindow(FALSE);
		m_BtnStopTask.EnableWindow(FALSE);
		m_BtnStartTask.EnableWindow(FALSE);
		m_BtnModTask.EnableWindow(FALSE);
	}
	if (m_bTaskIsRunning)
	{
		m_BtnStartTask.EnableWindow(FALSE);
	}
	else
	{
		m_BtnStopTask.EnableWindow(FALSE);
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}


HBRUSH CDlgTaoQSpaceSettings::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  �ڴ˸��� DC ���κ�����
	if ((pWnd->GetDlgCtrlID() == IDC_STATIC_Zhu) ||
		(pWnd->GetDlgCtrlID() == IDC_STATIC_Zhu1) ||
		(pWnd->GetDlgCtrlID() == IDC_STATIC_Zhu2))
	{
		pDC->SetTextColor(RGB(144,144,144));
	}
	// TODO:  ���Ĭ�ϵĲ������軭�ʣ��򷵻���һ������
	return hbr;
}


void CDlgTaoQSpaceSettings::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (tasks.size() < MAXTASKCOUNT)
	{
		UpdateData(TRUE);
		if (m_strQNumber.IsEmpty())
		{
			AfxMessageBox(_T("����������QQ��"));
			return;
		}
		TTaskAttribute task;
		task.nTaskIndex = tasks.size();
		task.strQNumber =  m_strQNumber;
		task.eType = QSpace;
		task.eStat = Running;
		tasks.push_back(task);
	}
	else
	{
		AfxMessageBox(_T("���������ǻ�Ա����������ֻ�ܺ��ź��ظ�������\n���Ŀɽ��������Ѵ����ޣ�����ɾ���������ע���Ա������๦�ܡ�"));
		OnCancel();
		return;
	}
	CDialogEx::OnOK();
}


void CDlgTaoQSpaceSettings::OnBnClickedButtonStarttask()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_bTaskIsRunning = true;
	TTaskAttribute &task = tasks.at(m_nTaskIndex);
	task.eStat = Running;
	task.eAction = StartTask;
	OnOK();
}


void CDlgTaoQSpaceSettings::OnBnClickedButtonStoptask()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_bTaskIsRunning = false;
	TTaskAttribute &task = tasks.at(m_nTaskIndex);
	task.eStat = Stop;
	task.eAction = StopTask;
	OnOK();
}


void CDlgTaoQSpaceSettings::OnBnClickedButtonDeltask()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	TTaskAttribute &task = tasks.at(m_nTaskIndex);
	task.eAction = DelTask;
	OnOK();
}


void CDlgTaoQSpaceSettings::OnBnClickedButtonModtask()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	TTaskAttribute &task = tasks.at(m_nTaskIndex);
	task.eAction = ModTask;
	UpdateData(TRUE);
	task.eType = QSpace;
	task.strQNumber = m_strQNumber;
	OnOK();
}
