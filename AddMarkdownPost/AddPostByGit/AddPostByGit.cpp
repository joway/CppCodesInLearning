// AddPostByGit.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "AddPostByGit.h"
#include <fstream>
#define MAX_LOADSTRING 100
#include <string>

using namespace std;
// 全局变量: 
HINSTANCE hInst;                                // 当前实例


// 此代码模块中包含的函数的前向声明: 
INT_PTR CALLBACK EditProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);


class Info
{
public:
	char Title[256];
	char Tags[20];
};


int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);
	hInst = hInstance;

	// TODO: 在此放置代码。
	Info info;
	auto rVal = DialogBoxParam(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), NULL, (EditProc), (LPARAM)&info);
	while ((*info.Title == NULL || *info.Tags == NULL) && rVal == IDC_OK) {
		MessageBox(NULL, "名称不能为空！", "", MB_OK);
		rVal = DialogBoxParam(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), NULL, (EditProc), (LPARAM)&info);
	}
	if (rVal == IDCLOSE || rVal == IDCANCEL)
		return 1;
	string finalText;
	string text1 = "---\nlayout: post\ntitle: \"";
	finalText = text1 + info.Title + "\"\ncomments: true\nshare: true\ntags: " + info.Tags
		+ "\n---\n\n\n\n---\n####  参考资料：  ####\n- ";

	SYSTEMTIME st;
	GetLocalTime(&st);
	char time[20];
	sprintf(time, "%u-%u-%u", st.wYear, st.wMonth, st.wDay);
	string fileName = time;
	fileName = fileName + "-" + info.Title + ".markdown";
	fstream fout(fileName, ios::out);
	if (!fout) {
		MessageBox(NULL, "文章创建失败！", "", MB_OK);
		return 1;
	}
	fout << finalText << endl;
	fout.close();
	MessageBox(NULL, "文章生成成功！", "", MB_OK);
}


INT_PTR CALLBACK EditProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	static Info *info = NULL;
	switch (message)
	{
	case WM_INITDIALOG:
	{
		info = (Info *)lParam;
	}
	return (INT_PTR)TRUE;

	case WM_COMMAND:

		if (LOWORD(wParam) == IDC_OK)
		{
			HWND	hwnd = GetDlgItem(hDlg, IDC_EDIT1);
			GetWindowText(hwnd, info->Tags, MAX_PATH);

			hwnd = GetDlgItem(hDlg, IDC_EDIT2);
			GetWindowText(hwnd, info->Title, MAX_PATH);
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		else if (LOWORD(wParam) == IDCANCEL)
		{
			//strcpy(strText, "");
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)FALSE;
		}

		break;

	case WM_SYSCOMMAND:
		switch (LOWORD(wParam))
		{
		case SC_CLOSE:
			//strcpy(strText, "");
			EndDialog(hDlg, IDCLOSE);
			return TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}

