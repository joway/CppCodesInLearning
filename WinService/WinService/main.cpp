#include <stdio.h>
#include <windows.h>

#define SLEEP_TIME 5000	//SLEEP_TIME ָ������������ѯ�����ڴ�֮��ĺ�������
#define LOGFILE "E:\\1.txt" //��־�ļ���·��

SERVICE_STATUS ServiceStatus;
SERVICE_STATUS_HANDLE hStatus;

bool brun = false;
int error;

void WINAPI ServiceMain(int argc, char** argv);
void WINAPI CtrlHandler(DWORD request);


int WriteToLog(char* str)
{
	FILE* log;
	log = fopen(LOGFILE, "a+");
	if (log == NULL)
		return -1;
	fprintf(log, "%s\n", str);
	fclose(log);
	return 0;
}

void main()
{
	//����һ��SERVICE_TABLE_ENTRY �ṹ
	SERVICE_TABLE_ENTRY ServiceTable[2];

	//һ��������ܰ������ɸ�����ÿһ�����񶼱�������ר�ŵķ��ɱ���
	//��Ϊ�˸ó�������һ�� ServiceTable �ṹ���飩(ÿ���������Ӧ��ÿ������(�������һ������))
	//������е�ÿһ�Ҫ�� SERVICE_TABLE_ENTRY �ṹ֮�С�

	ServiceTable[0].lpServiceName = (LPWSTR)"testservice";
	ServiceTable[0].lpServiceProc = (LPSERVICE_MAIN_FUNCTION)ServiceMain;

	//���ɱ�����һ������Ƿ������ͷ������������ NULL ָ��
	ServiceTable[1].lpServiceName = NULL;
	ServiceTable[1].lpServiceProc = NULL;

	// ��������Ŀ��Ʒ��ɻ��߳�
	StartServiceCtrlDispatcher(ServiceTable);
}

//������ƹ�������SCM��Services Control Manager����һ������ϵͳ���з���Ľ��̡�
//�� SCM ����ĳ������ʱ�����ȴ�ĳ�����̵����߳������� StartServiceCtrlDispatcher ������
//�����ɱ��ݸ� StartServiceCtrlDispatcher���⽫�ѵ��ý��̵����߳�ת��Ϊ���Ʒ�������
//�÷���������һ�����̣߳����߳����з��ɱ���ÿ������� ServiceMain ����������������ֻ��һ������
//�����������ӳ��������з����ִ�������Ȼ������������������ SCM ��������
//ע�⣺��� StartServiceCtrlDispatcher ����30��û�б����ã���ᱨ��
//Ϊ�˱���������������Ǳ����� ServiceMain �����У��μ��������ӣ�
//���ڷ��������ĵ����߳��г�ʼ��������ɱ�
//�����������ķ�����Ҫ���������������
//���ɱ������еķ���ִ����֮�����磬�û�ͨ�������񡱿���������ֹͣ���ǣ����߷�������ʱ,
//StartServiceCtrlDispatcher ���÷��ء�Ȼ����������ֹ��


/*ServiceMain�������ú����Ƿ������ڵ㡣
��������һ���������̵߳��У�����߳����ɿ��Ʒ����������ġ�
ServiceMain Ӧ�þ���������Ϊ����ע����ƴ�������
��Ҫͨ������ RegisterServiceCtrlHadler ������ʵ�֡�
��Ҫ�������������ݸ��˺�������������ָ�� ControlHandlerfunction ��ָ�롣
��ָʾ���Ʒ��������� ControlHandler �������� SCM ��������
ע������ƴ�����֮�󣬻��״̬�����hStatus����
ͨ������ SetServiceStatus �������� hStatus �� SCM ��������״̬��

����չʾ�����ָ�������������䵱ǰ״̬����ʼ�� ServiceStatus �ṹ��
ServiceStatus �ṹ��ÿ����������;��

dwServiceType��ָʾ�������ͣ����� Win32 ���񡣸�ֵ SERVICE_WIN32��
dwCurrentState��ָ������ĵ�ǰ״̬����Ϊ����ĳ�ʼ��������û����ɣ����������״̬Ϊ SERVICE_START_PENDING��
`dwWin32ExitCode �� dwServiceSpecificExitCode����������������ֹ���񲢱����˳�ϸ��ʱ�����á���ʼ������ʱ�����˳�����ˣ����ǵ�ֵΪ 0��
dwCheckPoint �� dwWaitHint�����������ʾ��ʼ��ĳ���������ʱҪ30�����ϡ��������ӷ���ĳ�ʼ�����̺̣ܶ��������������ֵ��Ϊ 0��

���� SetServiceStatus ������ SCM ��������״̬ʱ��Ҫ�ṩ hStatus ����� ServiceStatus �ṹ��ע�� ServiceStatus һ��ȫ�ֱ�������������Կ�������ʹ������ServiceMain �����У�����ṹ�ļ�����ֵ�������ڷ������е����������ж����ֲ��䣬���磺dwServiceType��

�ڱ����˷���״̬֮������Ե��� InitService ��������ɳ�ʼ�����������ֻ�����һ��˵�����ַ�������־�ļ��������������ʾ��
�����ʼ��*/


//�� ServiceMain �У���� InitService �����ķ���ֵ��
//�����ʼ���д���Ϊ�п���д��־�ļ�ʧ�ܣ���
//�򽫷���״̬��Ϊ��ֹ���˳� ServiceMain��

void WINAPI ServiceMain(int argc, char **argv){

	ServiceStatus.dwServiceType = SERVICE_WIN32;
	ServiceStatus.dwCurrentState = SERVICE_START_PENDING;
	ServiceStatus.dwControlsAccepted = SERVICE_ACCEPT_SHUTDOWN
		| SERVICE_ACCEPT_STOP;
	ServiceStatus.dwWin32ExitCode = 0;
	ServiceStatus.dwServiceSpecificExitCode = 0;
	ServiceStatus.dwCheckPoint = 0;
	ServiceStatus.dwWaitHint = 0;

	hStatus = ::RegisterServiceCtrlHandler((LPCWSTR)"testservice", CtrlHandler);

	if (hStatus == 0)
	{
		WriteToLog("RegisterServiceCtrlHandler failed");
		// ��ʼ��ʧ�ܣ���ֹ����
		ServiceStatus.dwCurrentState = SERVICE_STOPPED;
		ServiceStatus.dwWin32ExitCode = -1;
		SetServiceStatus(hStatus, &ServiceStatus);
		// �˳� ServiceMain
		return;
	}

	WriteToLog("RegisterServiceCtrlHandler success");

	//��SCM ��������״̬
	ServiceStatus.dwCurrentState = SERVICE_RUNNING;
	SetServiceStatus(hStatus, &ServiceStatus);

	//����Ϳ�ʼ����ѭ���ˣ������������Լ�ϣ���������Ĺ���

	//�����Ǽ�¼�����ڴ浽1.TXT��,start{
	brun = true;
	MEMORYSTATUS memstatus;

	char str[100];
	memset(str, '\0', 100);
	while (brun)
	{
		GlobalMemoryStatus(&memstatus);

		int availmb = memstatus.dwAvailPhys / 1024 / 1024;

		sprintf_s(str, 100, "available memory is %dMB", availmb);

		WriteToLog(str);

		Sleep(SLEEP_TIME);
	}

	WriteToLog("service stopped");
	//end}






}
//ѭ��һֱ�������״̬Ϊ SERVICE_RUNNING ����־�ļ�д�����Ϊֹ��
//״̬������ ControlHandler ������Ӧ SCM ��������ʱ�޸ġ�




/*
ǰ���� ServiceMain ����ע���˿��ƴ�����������
���ƴ�������� SCM ������ʲô���󲢲�ȡ��Ӧ�ж���
ÿ������� SetServiceStatus ������ʱ�򣬱���ָ��������� STOP �� SHUTDOWN ����
����Щ����Ҫ�� ControlHandler �����д������ǡ�
STOP ������ SCM ��ֹ�����ʱ���͵ġ�
���磬����û��ڡ����񡱿���������ֶ���ֹ����
SHUTDOWN �����ǹرջ���ʱ���� SCM ���͸����������з��������
��������Ĵ���ʽ��ͬ��д��־�ļ�������ֹͣ����SCM ����SERVICE_STOPPED ״̬��

���� ServiceStatus �ṹ���������������Ϊȫ������
ServiceStatus �еĹ���ѭ���ڵ�ǰ״̬�ı�������ֹ��ֹͣ��
�����Ŀ��������磺PAUSE �� CONTINUE �ڱ��ĵ�����û�д���
���ƴ������������뱨�����״̬������ SCM ÿ�η��Ϳ��������ʱ��״̬������ͬ��
��ˣ�������Ӧʲô���󣬶�Ҫ���� SetServiceStatus��
*/


void WINAPI CtrlHandler(DWORD request)
{
	switch (request)
	{
	case SERVICE_CONTROL_STOP:
		brun = false;
		ServiceStatus.dwCurrentState = SERVICE_STOPPED;
		break;
	case SERVICE_CONTROL_SHUTDOWN:
		brun = false;
		ServiceStatus.dwCurrentState = SERVICE_STOPPED;
		break;
	default:
		break;
	}
	SetServiceStatus(hStatus, &ServiceStatus);
}


