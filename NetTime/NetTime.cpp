#include "stdafx.h"
#include <winsock.h>
#include <stdio.h>
#include <cstring>
#include <crtdbg.h>
#pragma comment(lib, "ws2_32.lib")

#define NET_TIME_PORT   37
#define TIMEOUT_CONNECT 1
#define TIMEOUT_RECEIVE	1

BOOL  GetTimeFromInternet(SYSTEMTIME &st);


BOOL GetRemoteTime(char* szSever, unsigned long& ulTime);
SYSTEMTIME TransToSystemTime(unsigned long ulTime);
SYSTEMTIME TransToSystemTime(unsigned long ulTime);

char* sever[] =
{
		"129.6.15.28",
		"129.6.15.29",
		"132.163.4.101",
		"132.163.4.102",
		"132.163.4.103",
		"128.138.140.44",
	    //以下不知为何连接不上
		//"210.72.145.44",//
		//"133.100.11.8",//
		//"192.43.244.18",//
		//"131.107.1.10",//
		//"69.25.96.13",//
		//"216.200.93.8",//
		//"208.184.49.9",//
		//"207.126.98.204",//
		//"207.200.81.113",//
		//"64.236.96.53",//
		//"10.255.255.2",//
		//"81.6.42.224",//
		NULL
};

BOOL GetRemoteTime(char* szSever, unsigned long& ulTime)
{
	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);	//使用TCP协议

	if (sock == INVALID_SOCKET)
	{
		return FALSE;
	}

	unsigned long flag = 1;
	if ((ioctlsocket(sock, FIONBIO, &flag) == 0))
	{
		sockaddr_in severAddr;
		severAddr.sin_family = AF_INET;
		severAddr.sin_port = htons(NET_TIME_PORT);
		severAddr.sin_addr.S_un.S_addr = inet_addr(szSever);

		connect(sock, (sockaddr*)&severAddr, sizeof(severAddr));

		struct fd_set mask;
		FD_ZERO(&mask);
		FD_SET(sock, &mask);

		struct timeval timeout;
		timeout.tv_sec = TIMEOUT_CONNECT;
		timeout.tv_usec = 0;
		if (select(0, NULL, &mask, NULL, &timeout) == 1)
		{
			timeout.tv_sec = TIMEOUT_RECEIVE;
			if (select(0, &mask, NULL, NULL, &timeout) == 1)
			{
				if (recv(sock, (char*)&ulTime, 4, 0) == 4)
				{
					ulTime = ntohl(ulTime);
					closesocket(sock);
					return TRUE;
				}
			}
		}
	}
	closesocket(sock);
	return FALSE;
}

//变成SYSTEMTIME结构体形式的时间
SYSTEMTIME TransToSystemTime(unsigned long ulTime)
{

	FILETIME ft;
	SYSTEMTIME st;

	st.wYear = 1900;
	st.wMonth = 1;
	st.wDay = 1;
	st.wHour = 0;
	st.wMinute = 0;
	st.wSecond = 0;
	st.wMilliseconds = 0;

	SystemTimeToFileTime(&st, &ft);
	LARGE_INTEGER li = *(LARGE_INTEGER*)&ft;
	li.QuadPart += (LONGLONG)10000000 * ulTime;
	ft = *(FILETIME*)&li;

	//在WIN32下，FILETIME永远为格林尼治时间(UTC)
	//所以这里的ft仍旧是UTC时间，要通过APIFileTimeToLocalFileTime()来转换成计算机本地时间!
	FILETIME localFt;
	FileTimeToLocalFileTime(&ft, &localFt);
	FileTimeToSystemTime(&localFt, &st);
	//至此，已经转化成了SYSTEMTIME结构体的时间了
	return st;

	//更新系统时间！
	//SetSystemTime(&st);
}


BOOL  GetTimeFromInternet(SYSTEMTIME &st) {
	//初始化TCP协议
	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		return FALSE;
	}

	int i = 0;
	unsigned long ulTime = 0;

	while (sever[i] != NULL)
	{
		if (GetRemoteTime(sever[i], ulTime))
		{
			st = TransToSystemTime(ulTime);
			return TRUE;
		}
		i++;
	}
	WSACleanup();
	return FALSE;
}

int APIENTRY WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR     lpCmdLine,
	int       nCmdShow)
{
	SYSTEMTIME st;
	if (!GetTimeFromInternet(st)) {
		MessageBox(NULL, "Error", "", MB_OK);
	}
	char ch[1000];
	sprintf(ch, "%d %d %d %d %d %d", st.wYear, st.wMonth, st.wDayOfWeek, st.wDay, st.wHour, st.wMinute);
	MessageBox(NULL, ch, "", MB_OK);
}



