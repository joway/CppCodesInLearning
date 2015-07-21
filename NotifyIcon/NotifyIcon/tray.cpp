#include <windows.h>
#include "resource.h"

#define WM_TRAY (WM_USER + 100)
#define WM_TASKBAR_CREATED RegisterWindowMessage(TEXT("TaskbarCreated"))

#define APP_NAME	TEXT("���̳���")
#define APP_TIP		TEXT("Win32 API ʵ��ϵͳ���̳���")

NOTIFYICONDATA nid;		//��������
HMENU hMenu;			//���̲˵�

						//ʵ��������
void InitTray(HINSTANCE hInstance, HWND hWnd)
{
	nid.cbSize = sizeof(	);
	nid.hWnd = hWnd;
	nid.uID = IDI_TRAY;
	nid.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP | NIF_INFO;
	nid.uCallbackMessage = WM_TRAY;
	nid.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_TRAY));
	lstrcpy(nid.szTip, APP_NAME);

	hMenu = CreatePopupMenu();//�������̲˵�
							  //Ϊ���̲˵��������ѡ��
	AppendMenu(hMenu, MF_STRING, ID_SHOW, TEXT("��ʾ"));
	AppendMenu(hMenu, MF_STRING, ID_EXIT, TEXT("�˳�"));

	Shell_NotifyIcon(NIM_ADD, &nid);
}

//��ʾ������������
void ShowTrayMsg()
{
	lstrcpy(nid.szInfoTitle, APP_NAME);
	lstrcpy(nid.szInfo, TEXT("�յ�һ����Ϣ��"));
	nid.uTimeout = 1000;
	Shell_NotifyIcon(NIM_MODIFY, &nid);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_TRAY:
		switch (lParam)
		{
		case WM_RBUTTONDOWN:
		{
			//��ȡ�������
			POINT pt; GetCursorPos(&pt);

			//����ڲ˵��ⵥ������˵�����ʧ������
			SetForegroundWindow(hWnd);

			//ʹ�˵�ĳ����
			//EnableMenuItem(hMenu, ID_SHOW, MF_GRAYED);	

			//��ʾ����ȡѡ�еĲ˵�
			int cmd = TrackPopupMenu(hMenu, TPM_RETURNCMD, pt.x, pt.y, NULL, hWnd,
				NULL);
			if (cmd == ID_SHOW)
				MessageBox(hWnd, APP_TIP, APP_NAME, MB_OK);
			if (cmd == ID_EXIT)
				PostMessage(hWnd, WM_DESTROY, NULL, NULL);
		}
		break;
		case WM_LBUTTONDOWN:
			MessageBox(hWnd, APP_TIP, APP_NAME, MB_OK);
			break;
		case WM_LBUTTONDBLCLK:
			break;
		}
		break;
	case WM_DESTROY:
		//��������ʱɾ������
		Shell_NotifyIcon(NIM_DELETE, &nid);
		PostQuitMessage(0);
		break;
	case WM_TIMER:
		ShowTrayMsg();
		KillTimer(hWnd, wParam);
		break;
	}
	if (uMsg == WM_TASKBAR_CREATED)
	{
		//ϵͳExplorer��������ʱ�����¼�������
		//�мǣ�����nidҪ���ó�ȫ�ֱ����������������ɵ�ͼ������Ч��
		//���ɼ��ģ�
		Shell_NotifyIcon(NIM_ADD, &nid);
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE /*hPrevInstance*/,
	LPSTR /*lpCmdLine*/, int iCmdShow)
{
	HWND hWnd;
	MSG msg;
	WNDCLASS wc = { 0 };
	wc.style = NULL;
	wc.hIcon = NULL;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.lpfnWndProc = WndProc;
	wc.hbrBackground = NULL;
	wc.lpszMenuName = NULL;
	wc.lpszClassName = APP_NAME;
	wc.hCursor = NULL;

	if (!RegisterClass(&wc)) return 0;

	hWnd = CreateWindowEx(WS_EX_TOOLWINDOW, APP_NAME, APP_NAME, WS_POPUP, CW_USEDEFAULT,
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, hInstance, NULL);

	ShowWindow(hWnd, iCmdShow);
	UpdateWindow(hWnd);

	InitTray(hInstance, hWnd);			//ʵ��������
	SetTimer(hWnd, 3, 1000, NULL);		//��ʱ����Ϣ����ʾ������ʾ����

	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return msg.wParam;
}