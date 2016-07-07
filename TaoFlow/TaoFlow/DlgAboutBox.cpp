// DlgAboutBox.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "TaoFlow.h"
#include "DlgAboutBox.h"
#include "afxdialogex.h"
#include "def.h"

// CDlgAboutBox �Ի���

IMPLEMENT_DYNAMIC(CDlgAboutBox, CDialogEx)

CDlgAboutBox::CDlgAboutBox(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgAboutBox::IDD, pParent)
{

}

CDlgAboutBox::~CDlgAboutBox()
{
}

void CDlgAboutBox::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_Serial, m_stcMachSerial);
}


BEGIN_MESSAGE_MAP(CDlgAboutBox, CDialogEx)
//	ON_WM_CREATE()
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// CDlgAboutBox ��Ϣ�������


//int CDlgAboutBox::OnCreate(LPCREATESTRUCT lpCreateStruct)
//{
//	if (CDialogEx::OnCreate(lpCreateStruct) == -1)
//		return -1;
//
//	// TODO:  �ڴ������ר�õĴ�������
//
//	return 0;
//}


HBRUSH CDlgAboutBox::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  �ڴ˸��� DC ���κ�����
	if ((pWnd->GetDlgCtrlID() == IDC_STATIC_MachineSerial) ||
		(pWnd->GetDlgCtrlID() == IDC_STATIC_Serial))
	{
		pDC->SetTextColor(RGB(0, 0, 255));
	}
	// TODO:  ���Ĭ�ϵĲ������軭�ʣ��򷵻���һ������
	return hbr;
}


BOOL CDlgAboutBox::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	m_stcMachSerial.SetWindowTextW(GetSerialNum());
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}
