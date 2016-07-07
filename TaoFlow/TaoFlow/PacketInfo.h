#ifndef PACKETINFO_H_
#define PACKETINFO_H_

#include "ClientInfo.h"

#define SECRET_CODE 0x3806

enum EPackType
{
    //login
    LoginReq = 1,
    LoginRes = -1,
    //breath
    BreathReq = 2,
    BreathRes = -2,
    //task
    TaskReq = 3,
    TaskRes = -3,
    //LogOut
    LogOutReq = 4,
    LogOutRes = -4,
};

enum ETerminalType
{
    Server = 0,
    //liuliang ban
    FlowClient = 1,
    //guaji ban
    HangOnClient = 2,
    //server sync data
    SyncDataServer = 3,
};

/**��ͷ*/
typedef struct PackHead
{
    EPackType ePType;
    ETerminalType eCType;
	int nBodySize;
}PackHead_T;

/**����*/
//�ͻ��˵�¼��
typedef struct LoginClient
{
	int nSecretCode;
    char MachCode[MACHINECODE_SIZE];
}LogCli_T;

//����˵�¼��Ӧ��
typedef struct LoginServer
{
    bool bOk;
}LogSer_T;



#define PACKHEAD_SIZE sizeof(PackHead_T)

#endif
