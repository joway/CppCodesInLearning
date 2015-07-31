// AddPostByGit.cpp : ����Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "AddPostByGit.h"
#include <fstream>
#define MAX_LOADSTRING 100
#include <string>

using namespace std;
// ȫ�ֱ���: 
HINSTANCE hInst;                                // ��ǰʵ��


// �˴���ģ���а����ĺ�����ǰ������: 
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

	// TODO: �ڴ˷��ô��롣
	Info info;
	auto rVal = DialogBoxParam(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), NULL, (EditProc), (LPARAM)&info);
	while ((*info.Title == NULL || *info.Tags == NULL) && rVal == IDC_OK) {
		MessageBox(NULL, "���Ʋ���Ϊ�գ�", "", MB_OK);
		rVal = DialogBoxParam(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), NULL, (EditProc), (LPARAM)&info);
	}
	if (rVal == IDCLOSE || rVal == IDCANCEL)
		return 1;
	string finalText;
	string text1 = "---\nlayout: post\ntitle: \"";
	finalText = text1 + info.Title + "\"\ncomments: true\nshare: true\ntags: " + info.Tags
		+ "\n---\n\n\n\n---\n####  �ο����ϣ�  ####\n- ";

	SYSTEMTIME st;
	GetLocalTime(&st);
	char time[20];
	sprintf(time, "%u-%u-%u", st.wYear, st.wMonth, st.wDay);
	string fileName = time;
	fileName = fileName + "-" + info.Title + ".markdown";
	fstream fout(fileName, ios::out);
	if (!fout) {
		MessageBox(NULL, "���´���ʧ�ܣ�", "", MB_OK);
		return 1;
	}
	fout << finalText << endl;
	fout.close();
	MessageBox(NULL, "�������ɳɹ���", "", MB_OK);
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

