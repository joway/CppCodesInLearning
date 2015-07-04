// DllExe_Win32.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "DllExe_Win32.h"
#include <Windows.h>
#include "MyLib.h"
#include <strsafe.h>
#include <stdlib.h>	




int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPTSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{

	int a=20, b=10;

	TCHAR sz[100];
	StringCchPrintf(sz, _countof(sz), TEXT("%d+%d=%d"),
		a, b, add(a, b));
	MessageBox(NULL, sz, TEXT("Notice"), MB_OK);

	return (0);
}


