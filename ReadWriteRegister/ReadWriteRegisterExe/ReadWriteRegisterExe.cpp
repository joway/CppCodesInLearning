// ReadWriteRegisterExe.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "ReadWriteRegisterExe.h"
#include "ReadWriteRegister.h"
#include <crtdbg.h>

#define MAX_LOADSTRING 100

// 全局变量: 
HINSTANCE hInst;                                // 当前实例

INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);


int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);
	hInst = hInstance;
	DialogBoxParam(hInst, MAKEINTRESOURCE(IDD_DIALOG1),
		NULL, (DLGPROC)(About), (LPARAM)NULL);
	if(_CrtDumpMemoryLeaks())
		MessageBox(NULL, L"_CrtDumpMemoryLeaks!", L"", MB_OK);
}



INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
		else if (LOWORD(wParam) == IDC_BUTTON1) {
			RegInfo ri;
			ri.HKey = HKEY_CLASSES_ROOT;
			ri.SubKey = L"*\\shell\\Joway";
			ri.ValueName = NULL;
			ri.ValueType = (LPDWORD)REG_SZ;
			WCHAR* val;
			wcscpy(ri.ValueData, L"1234567890");
			ri.DataLen = ((wcslen(ri.ValueData) + 1)*sizeof(WCHAR));
			if (SetRegister(ri))
				MessageBox(NULL,L"Success!", L"", MB_OK);
			else
				MessageBox(NULL,L"Fail!", L"", MB_OK);
			if (_CrtDumpMemoryLeaks())
				MessageBox(NULL, L"_CrtDumpMemoryLeaks!", L"", MB_OK);
		}
		else if (LOWORD(wParam) == IDC_BUTTON2) {
			//// TODO: 在此添加控件通知处理程序代码
			RegInfo ri;
			ri.HKey = HKEY_CLASSES_ROOT;
			ri.SubKey = L"*\\shell\\Joway";
			ri.ValueName = NULL;
			if (GetRegister(ri)) {
				MessageBox(NULL,(ri.ValueData), L"", MB_OK);
			}
			else
				MessageBox(NULL,L"Fail!", L"", MB_OK);
			if (_CrtDumpMemoryLeaks())
				MessageBox(NULL, L"_CrtDumpMemoryLeaks!", L"", MB_OK);
		}
		else if (LOWORD(wParam) == IDC_BUTTON3) {
			RegInfo ri;
			ri.HKey = HKEY_CLASSES_ROOT;
			ri.SubKey = L"*\\shell\\Joway";
			ri.ValueName = NULL;
			if (DelRegisterValue(ri))
				MessageBox(NULL, L"Success!", L"", MB_OK);
			else
				MessageBox(NULL, L"Fail!", L"", MB_OK);
			if (_CrtDumpMemoryLeaks())
				MessageBox(NULL, L"_CrtDumpMemoryLeaks!", L"", MB_OK);
		}
		else if (LOWORD(wParam) == IDC_BUTTON4) {
			RegInfo ri;
			ri.HKey = HKEY_CLASSES_ROOT;
			ri.SubKey = L"*\\shell\\Joway";
			if (DelRegisterKey(ri))
				MessageBox(NULL, L"Success!", L"", MB_OK);
			else
				MessageBox(NULL, L"Fail!", L"", MB_OK);
			if (_CrtDumpMemoryLeaks())
				MessageBox(NULL, L"_CrtDumpMemoryLeaks!", L"", MB_OK);	

		}
		else if (LOWORD(wParam) == IDC_BUTTON5) {
			if (SetAutoRun(L"\"D:\\项目\\Visual C++ 项目\\项目--密码文件加密\\项目--密码文件加密\\Product\\x64\\SMFedDeamon.exe\""))
				MessageBox(NULL, L"Success", L"", MB_OK);
			else
				MessageBox(NULL, L"Fail", L"", MB_OK);
		}
        break;
    }
    return (INT_PTR)FALSE;
}
