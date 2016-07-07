#ifndef CLIENTINFO_H_
#define CLIENTINFO_H_

#include <string>

#define MACHINECODE_SIZE 10

//������Ĭ�ϼ����˿�
#define DEFAULTPORT 8765

//��������С
#define BUFFERSIZE 1024*8 + PACKSIZE_FLAG

#define CLIENTTIMEOUT 10

//���߿ͻ��˽ṹ��
typedef struct ConnectedTerminal
{
	bool bStat;	//�ͻ���״̬ 0-offline;1-online
	int nFd;
	int nTimeout;
	int nReadIndex;
	int nWriteIndex;
	int nPackSize;
	char rBuf[BUFFERSIZE];
    char MachCode[MACHINECODE_SIZE];
}OnlineClient_T;

#endif

