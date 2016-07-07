
#include "stdafx.h"
#include "def.h"

CString GetSerialNum()
{
	TCHAR     Volume[256];//�����   
	TCHAR     FileSysName[256];   
	DWORD   SerialNum;//���к�   
	DWORD   FileNameLength;   
	DWORD   FileSysFlag;   
	::GetVolumeInformation(_T("c:\\"),   
		Volume,   
		256,
		&SerialNum,   
		&FileNameLength,   
		&FileSysFlag,   
		FileSysName,   
		256);
	CString Serial;
	Serial.Format(_T("1%04x"),SerialNum ^ 0x34795814);
	return Serial;
}

/**
 * ������: CString2Char
 * ����1: CString str       ��ת���ַ���
 * ����2: char ch[]         ת����Ҫ�����λ��
 * ��Unicode�µ�CStringת��Ϊchar*
 */
void CString2Char(CString str, char ch[])
{
    int i;
    char *tmpch;
    int wLen = WideCharToMultiByte(CP_ACP, 0, str, -1, NULL, 0, NULL, NULL);//�õ�Char�ĳ���
    tmpch = new char[wLen + 1];                                             //��������ĵ�ַ��С
    WideCharToMultiByte(CP_ACP, 0, str, -1, tmpch, wLen, NULL, NULL);       //��CStringת����char*
    for(i = 0; tmpch[i] != '\0'; i++) ch[i] = tmpch[i];
    ch[i] = '\0';
}

