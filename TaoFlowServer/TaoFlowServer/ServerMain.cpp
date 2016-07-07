
#include "mainwindow.h"
#include <QApplication>
#include <stdio.h>
#include <pthread.h>
#include <sys/select.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include "TaoFlowServer.h"
#include "ThreadPool.h"
#include <QSqlDatabase>
#include <QSqlError>
#include <QDebug>

int main(int argc, char* argv[])
{
    //QApplication a(argc, argv);
    //MainWindow mw;
    //mw.show();

    //return a.exec();

    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL"); // ʹ��mysql���ݿ�����
            db.setHostName("localhost");
            db.setDatabaseName("jeecms_2012_sp1"); // ����֮ǰ���������ݿ�
            db.setUserName("trist725"); // ���Ǵ����� yunfan �û���
            db.setPassword("chen30"); // yunfan �û�������

            bool ok = db.open(); // �����������ݿ�

            if(ok)
            {
            int a;
            a++;
            }\
            qDebug()<<db.lastError();
	//����������ģ��
	CTcpServer server;

	//��ʼ���̳߳�
	server.InitBreathThread();

	//����������
	server.Run();

	return 0;
}
