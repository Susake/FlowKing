// DlgTaoAlexaSetting.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "TaoFlow.h"
#include "DlgTaoAlexaSetting.h"
#include "afxdialogex.h"
#include "def.h"

extern std::vector<TTaskAttribute> tasks;

// CDlgTaoAlexaSetting �Ի���

IMPLEMENT_DYNAMIC(CDlgTaoAlexaSetting, CDialogEx)

CDlgTaoAlexaSetting::CDlgTaoAlexaSetting(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgTaoAlexaSetting::IDD, pParent)
	, m_bEntryType(FALSE)
	, m_bTaskIsRunning(TRUE)
	, m_strNetAddr(_T(""))
{

}
CDlgTaoAlexaSetting::CDlgTaoAlexaSetting(TTaskAttribute &task, CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgTaoAlexaSetting::IDD, pParent)
	, m_bEntryType(FALSE)
	, m_bTaskIsRunning(TRUE)
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
	m_strNetAddr = task.strNetAddr;
	m_nTaskIndex = task.nTaskIndex;
}

CDlgTaoAlexaSetting::~CDlgTaoAlexaSetting()
{
}

void CDlgTaoAlexaSetting::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_EditNetAddr);
	DDX_Control(pDX, IDOK, m_BtnAddStartTask);
	DDX_Control(pDX, IDC_BUTTON_ModTask, m_BtnModTask);
	DDX_Control(pDX, IDC_BUTTON_DelTask, m_BtnDelTask);
	DDX_Control(pDX, IDC_BUTTON_StopTask, m_BtnStopTask);
	DDX_Control(pDX, IDC_BUTTON_StartTask, m_BtnStartTask);
	DDX_Text(pDX, IDC_EDIT1, m_strNetAddr);
}


BEGIN_MESSAGE_MAP(CDlgTaoAlexaSetting, CDialogEx)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDOK, &CDlgTaoAlexaSetting::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON_StartTask, &CDlgTaoAlexaSetting::OnBnClickedButtonStarttask)
	ON_BN_CLICKED(IDC_BUTTON_StopTask, &CDlgTaoAlexaSetting::OnBnClickedButtonStoptask)
	ON_BN_CLICKED(IDC_BUTTON_DelTask, &CDlgTaoAlexaSetting::OnBnClickedButtonDeltask)
	ON_BN_CLICKED(IDC_BUTTON_ModTask, &CDlgTaoAlexaSetting::OnBnClickedButtonModtask)
END_MESSAGE_MAP()


// CDlgTaoAlexaSetting ��Ϣ�������


BOOL CDlgTaoAlexaSetting::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	m_EditNetAddr.SetCueBanner(_T("�ڴ˴�������ҪˢAlexa����վ��ַ"));
	m_EditNetAddr.SetWindowTextW(m_strNetAddr);
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


HBRUSH CDlgTaoAlexaSetting::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  �ڴ˸��� DC ���κ�����
	if ((pWnd->GetDlgCtrlID() == IDC_STATIC_AlexaZhu) ||
		(pWnd->GetDlgCtrlID() == IDC_STATIC_AlexaZhu1) ||
		(pWnd->GetDlgCtrlID() == IDC_STATIC_AlexaZhu2))
	{
		pDC->SetTextColor(RGB(144,144,144));
	}
	// TODO:  ���Ĭ�ϵĲ������軭�ʣ��򷵻���һ������
	return hbr;
}


void CDlgTaoAlexaSetting::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (tasks.size() < MAXTASKCOUNT)
	{
		UpdateData(TRUE);
		if (m_strNetAddr.IsEmpty())
		{
			AfxMessageBox(_T("��ַ����Ϊ��"));
			return;
		}
		TTaskAttribute task;
		task.nTaskIndex = tasks.size();
		task.strNetAddr =  m_strNetAddr;
		task.eType = Alexa;
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


void CDlgTaoAlexaSetting::OnBnClickedButtonStarttask()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_bTaskIsRunning = true;
	TTaskAttribute &task = tasks.at(m_nTaskIndex);
	task.eStat = Running;
	task.eAction = StartTask;
	OnOK();
}


void CDlgTaoAlexaSetting::OnBnClickedButtonStoptask()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_bTaskIsRunning = false;
	TTaskAttribute &task = tasks.at(m_nTaskIndex);
	task.eStat = Stop;
	task.eAction = StopTask;
	OnOK();
}


void CDlgTaoAlexaSetting::OnBnClickedButtonDeltask()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	TTaskAttribute &task = tasks.at(m_nTaskIndex);
	task.eAction = DelTask;
	OnOK();
}


void CDlgTaoAlexaSetting::OnBnClickedButtonModtask()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	TTaskAttribute &task = tasks.at(m_nTaskIndex);
	task.eAction = ModTask;
	UpdateData(TRUE);
	task.eType = Alexa;
	task.strNetAddr = m_strNetAddr;
	OnOK();
}
