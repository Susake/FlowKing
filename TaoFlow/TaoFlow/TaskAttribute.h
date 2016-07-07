#pragma once

#include <vector>



enum ETaskStat
{
	Running = 1,
	Stop = 0,
	Finished = -1,
};

enum ETaoFlowType
{
	Flow = 0,
	//��ҳ���
	AdsSingle = 1,
	//ȫվ���
	AdsWhole = 2,
	//��������
	BlogPop = -1,
	//�ռ�����
	SpacePop = -2,
	//��̳����
	BbsPop = -3,
	//��Ʒ����
	CommodityPop = -4,
	//��������
	OnlineShopPop = -5,

	//�Թؼ���
	KeyWord = 3,
	//��Alexa
	Alexa = 4,
	//��QQ�ռ�
	QSpace = 5,
};

enum ETaskSetAction
{
	NoAction = 0,
	StartTask = 1,
	StopTask = 2,
	ModTask = 3,
	DelTask = 4,
};

typedef struct Typelist
{
	CString strType;
	CString strTypeAddr;
}Tlist;

struct TTaskAttribute
{
	//��ʱ����
	bool bFlowTiming;
	//ÿСʱ���IP
	bool bMaxIpPerHour;
	//IP / PV ����
	int nIP_PV_Ratio;

	int nMaxIpPerHour;

	int nStarTiming;
	int nEndTiming;

	int nTaskIndex;

	//����״̬
	ETaskStat eStat;
	//��������
	ETaoFlowType eType;
	int nTaoType;
	int nTypeSel;
	//�������
	ETaskSetAction eAction;

	//Ŀ����ַ
	CString strNetAddr;

	//��Դ��ַ�б�
	std::vector<Tlist> vecSourceAddr;
	//�����ַ�б�
	std::vector<Tlist> vecClickAddr;
	//�ؼ���
	CString strKeyWord;
	int nSearchEngineSel;
	//QQ��
	CString strQNumber;
};









