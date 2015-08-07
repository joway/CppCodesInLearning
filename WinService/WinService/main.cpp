#include <stdio.h>
#include <windows.h>

#define SLEEP_TIME 5000	//SLEEP_TIME 指定两次连续查询可用内存之间的毫秒间隔。
#define LOGFILE "E:\\1.txt" //日志文件的路径

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
	//定义一个SERVICE_TABLE_ENTRY 结构
	SERVICE_TABLE_ENTRY ServiceTable[2];

	//一个程序可能包含若干个服务。每一个服务都必须列于专门的分派表中
	//（为此该程序定义了一个 ServiceTable 结构数组）(每个数组相对应于每个服务(除了最后一个数组))
	//这个表中的每一项都要在 SERVICE_TABLE_ENTRY 结构之中。

	ServiceTable[0].lpServiceName = (LPWSTR)"testservice";
	ServiceTable[0].lpServiceProc = (LPSERVICE_MAIN_FUNCTION)ServiceMain;

	//分派表的最后一项必须是服务名和服务主函数域的 NULL 指针
	ServiceTable[1].lpServiceName = NULL;
	ServiceTable[1].lpServiceProc = NULL;

	// 启动服务的控制分派机线程
	StartServiceCtrlDispatcher(ServiceTable);
}

//服务控制管理器（SCM：Services Control Manager）是一个管理系统所有服务的进程。
//当 SCM 启动某个服务时，它等待某个进程的主线程来调用 StartServiceCtrlDispatcher 函数。
//将分派表传递给 StartServiceCtrlDispatcher。这将把调用进程的主线程转换为控制分派器。
//该分派器启动一个新线程，该线程运行分派表中每个服务的 ServiceMain 函数（本文例子中只有一个服务）
//分派器还监视程序中所有服务的执行情况。然后分派器将控制请求从 SCM 传给服务。
//注意：如果 StartServiceCtrlDispatcher 函数30秒没有被调用，便会报错，
//为了避免这种情况，我们必须在 ServiceMain 函数中（参见本文例子）
//或在非主函数的单独线程中初始化服务分派表。
//本文所描述的服务不需要防范这样的情况。
//分派表中所有的服务执行完之后（例如，用户通过“服务”控制面板程序停止它们）或者发生错误时,
//StartServiceCtrlDispatcher 调用返回。然后主进程终止。


/*ServiceMain（），该函数是服务的入口点。
它运行在一个单独的线程当中，这个线程是由控制分派器创建的。
ServiceMain 应该尽可能早早为服务注册控制处理器。
这要通过调用 RegisterServiceCtrlHadler 函数来实现。
你要将两个参数传递给此函数：服务名和指向 ControlHandlerfunction 的指针。
它指示控制分派器调用 ControlHandler 函数处理 SCM 控制请求。
注册完控制处理器之后，获得状态句柄（hStatus）。
通过调用 SetServiceStatus 函数，用 hStatus 向 SCM 报告服务的状态。

下面展示了如何指定服务特征和其当前状态来初始化 ServiceStatus 结构，
ServiceStatus 结构的每个域都有其用途：

dwServiceType：指示服务类型，创建 Win32 服务。赋值 SERVICE_WIN32；
dwCurrentState：指定服务的当前状态。因为服务的初始化在这里没有完成，所以这里的状态为 SERVICE_START_PENDING；
`dwWin32ExitCode 和 dwServiceSpecificExitCode：这两个域在你终止服务并报告退出细节时很有用。初始化服务时并不退出，因此，它们的值为 0；
dwCheckPoint 和 dwWaitHint：这两个域表示初始化某个服务进程时要30秒以上。本文例子服务的初始化过程很短，所以这两个域的值都为 0。

调用 SetServiceStatus 函数向 SCM 报告服务的状态时。要提供 hStatus 句柄和 ServiceStatus 结构。注意 ServiceStatus 一个全局变量，所以你可以跨多个函数使用它。ServiceMain 函数中，你给结构的几个域赋值，它们在服务运行的整个过程中都保持不变，比如：dwServiceType。

在报告了服务状态之后，你可以调用 InitService 函数来完成初始化。这个函数只是添加一个说明性字符串到日志文件。如下面代码所示：
服务初始化*/


//在 ServiceMain 中，检查 InitService 函数的返回值。
//如果初始化有错（因为有可能写日志文件失败），
//则将服务状态置为终止并退出 ServiceMain：

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
		// 初始化失败，终止服务
		ServiceStatus.dwCurrentState = SERVICE_STOPPED;
		ServiceStatus.dwWin32ExitCode = -1;
		SetServiceStatus(hStatus, &ServiceStatus);
		// 退出 ServiceMain
		return;
	}

	WriteToLog("RegisterServiceCtrlHandler success");

	//向SCM 报告运行状态
	ServiceStatus.dwCurrentState = SERVICE_RUNNING;
	SetServiceStatus(hStatus, &ServiceStatus);

	//下面就开始任务循环了，你可以添加你自己希望服务做的工作

	//下面是记录可用内存到1.TXT中,start{
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
//循环一直到服务的状态为 SERVICE_RUNNING 或日志文件写入出错为止。
//状态可能在 ControlHandler 函数响应 SCM 控制请求时修改。




/*
前面用 ServiceMain 函数注册了控制处理器函数。
控制处理器检查 SCM 发送了什么请求并采取相应行动。
每次你调用 SetServiceStatus 函数的时候，必须指定服务接收 STOP 和 SHUTDOWN 请求。
而这些请求要在 ControlHandler 函数中处理它们。
STOP 请求是 SCM 终止服务的时候发送的。
例如，如果用户在“服务”控制面板中手动终止服务。
SHUTDOWN 请求是关闭机器时，由 SCM 发送给所有运行中服务的请求。
两种情况的处理方式相同：写日志文件，监视停止；向SCM 报告SERVICE_STOPPED 状态。

由于 ServiceStatus 结构对于整个程序而言为全局量，
ServiceStatus 中的工作循环在当前状态改变或服务终止后停止。
其它的控制请求如：PAUSE 和 CONTINUE 在本文的例子没有处理。
控制处理器函数必须报告服务状态，即便 SCM 每次发送控制请求的时候状态保持相同。
因此，不管响应什么请求，都要调用 SetServiceStatus。
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


