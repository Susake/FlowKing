

#include "ThreadPool.h"

extern std::queue<Que_t> g_TaskQue;
extern pthread_mutex_t g_QueMutex;

ThreadInfo_T CThreadPool::m_ThInfo[THREAD_COUNT];

CThreadPool::CThreadPool()
{
	
}

CThreadPool::~CThreadPool()
{

}

void* CThreadPool::RoutineThread(void* arg)
{	
	ThreadInfo_T *pInfo = (ThreadInfo_T *)arg;
	while (pInfo->bRun)
	{

		pthread_mutex_lock(&(pInfo->mtx));
		pthread_cond_wait(&(pInfo->cond), &(pInfo->mtx));
		m_ThInfo->bIdle = false;
		pthread_mutex_lock(&g_QueMutex);
		Que_t& que = g_TaskQue.front();
		g_TaskQue.pop();
		pthread_mutex_unlock(&g_QueMutex);

		switch (que.eQueTaskType)
		{
		default:
			break;
		case ReadTask:
            qDebug()<<"thread"<<pInfo->id;
            pthread_mutex_lock(&(que.pClient->mtx));
			ReadData(que.pClient);
            pthread_mutex_unlock(&(que.pClient->mtx));
			break;
		case WriteTask:
			break;
		}
		pthread_mutex_unlock(&(pInfo->mtx));
		pInfo->bIdle = true;
	}
    return NULL;
}

bool CThreadPool::InitThreadPool()
{
	//�����̷߳�������
	pthread_attr_t  thread_attr;
	pthread_attr_init(&thread_attr);
	//pthread_attr_setdetachstate(&thread_attr, PTHREAD_CREATE_DETACHED);
	//�����̳߳�
	for (int n = 0; n < THREAD_COUNT; ++n)
	{
		pthread_cond_init(&(m_ThInfo[n].cond), NULL);
		pthread_mutex_init(&(m_ThInfo[n].mtx), NULL);
		pthread_attr_setdetachstate(&thread_attr, PTHREAD_CREATE_DETACHED);
		m_ThInfo[n].bIdle = true;
		m_ThInfo[n].bRun = true;
        int ret = pthread_create(&m_ThInfo[n].id, &thread_attr, RoutineThread, (void *)&m_ThInfo[n]);
		if (0 != ret)
		{
			perror("thread create");
			return(false);
		}
	}
	return true;
}

void CThreadPool::DispatchThread()
{
	for (int i = 0; i < THREAD_COUNT; ++i)
	{
		if (m_ThInfo[i].bIdle)
		{
			if (! g_TaskQue.empty())
			{
				printf("call a thread,i = %d\n", i);
				pthread_cond_signal(&m_ThInfo[i].cond);
			//	perror("signal");
				return;
			}
		}
	}
}

