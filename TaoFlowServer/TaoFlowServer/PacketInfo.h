#ifndef PACKETINFO_H_
#define PACKETINFO_H_

#include "ClientInfo.h"

enum EPackType
{
    RequestClient = 0,
    RespondClient = 1,
    RequestServer = 2,
    RespondServer = 3,
};

enum EPackAct
{
	//����
    PackTask = 0,
	//����
    PackBreath = 1,
	//��¼
    PackLogin = 2,
	//ע��
    PackLogout = 3,
};

enum EClientType
{
    Server = 0,
    //liuliang ban
    FlowClient = 1,
    //guaji ban
    HangOnClient = 2,
    //server sync data
    SyncDataServer = 3,
};

//��ͷ
typedef struct PackHead
{
    EPackAct eAct;
    EPackType ePType;
    EClientType eCType;
	int nBodySize;
}PackHead_T;

//����

//�ͻ���
//��¼��
typedef struct LoginClient
{
    char MachCode[MACHINECODE_SIZE];
}LogCli_T


//�����
//��¼��Ӧ��

#define PACKHEAD_SIZE sizeof(PackHead_T)

#endif
