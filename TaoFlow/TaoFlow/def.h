#ifndef DEF_H_
#define DEF_H_

#define DEFAULT_SERVER_PORT 8765
#define MAXTASKCOUNT 6
//����Сλ
#define PACKSIZE_FLAG (int)(sizeof(int))

//��������С
#define BUFFERSIZE 1024*8 + PACKSIZE_FLAG

CString GetSerialNum();

void CString2Char(CString str, char ch[]);

#endif
