// PhotosIndexCreater.cpp : ����Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "PhotosIndexCreater.h"
#include <Windows.h>
#include <fstream>
#include <Commdlg.h>
#include <opencv2/core/core.hpp>  
#include <opencv2/highgui/highgui.hpp>
#define MAX_LOADSTRING 100
using namespace cv;
using namespace std;

// ȫ�ֱ���: 
HINSTANCE hInst;                                // ��ǰʵ��


char szLink[MAX_PATH];//���Ŀ¼���ӣ���'/��β'
char szFileName[128 * MAX_PATH];//��ѡ�е�ͼƬ�ļ�����֮����'\n'�����
char szExePath[MAX_PATH];
char szHeadFileName[MAX_PATH];
char szEndFileName[MAX_PATH];
char szIndexFileName[MAX_PATH];

INT_PTR CALLBACK    PhotosIndexCreater(HWND, UINT, WPARAM, LPARAM);

/*		�Ӵ�·���������ļ����г�ȡ���ļ�����������׺����
������
fname���ļ���������·����
fullname������·�����ļ���

����ֵ��
1����ȡ�ɹ�
0����ȡʧ��
*/
int getFileNamefromFullName(const char* fullname, char *fname);

/*		�Ӵ�·���������ļ����г�ȡ��·����
������
fullname������·�����ļ���
pname��·�������������һ����б��'\'��
*/
void getPathNamefromFullName(const char* fullname, char *pname);

PSTR pWCharTopChar(PWSTR pWChar);
PWSTR pCharTopWChar(PCSTR pChar);





int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);
	hInst = hInstance;

	// TODO: �ڴ˷��ô��롣
	DialogBoxParam(hInst, MAKEINTRESOURCE(IDD_DIALOG1), NULL, (DLGPROC)(PhotosIndexCreater), NULL);

}




INT_PTR CALLBACK PhotosIndexCreater(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		else if (LOWORD(wParam) == IDC_BUTTON_OPEN) {

			OPENFILENAME ofn;
			char szOpenFileNames[128 * MAX_PATH] = "";
			char szPath[MAX_PATH];


			char* p;
			int nLen = 0;
			ZeroMemory(&ofn, sizeof(ofn));

			ofn.lStructSize = sizeof(ofn);
			ofn.hwndOwner = NULL;
			ofn.lpstrFile = szOpenFileNames;
			ofn.nMaxFile = sizeof(szOpenFileNames);
			ofn.lpstrFilter = "�����ļ�\0*.*\0�ı��ļ�\0*.txt\0JPEG�ļ�\0*.jpg\0";
			ofn.nFilterIndex = 3;
			ofn.lpstrFileTitle = NULL;
			ofn.nMaxFileTitle = 0;
			ofn.lpstrInitialDir = NULL;
			ofn.Flags = OFN_EXPLORER | OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_ALLOWMULTISELECT;

			if (GetOpenFileName(&ofn))
			{
				//�ѵ�һ���ļ���ǰ�ĸ��Ƶ�szPath,��:
				//���ֻѡ��һ���ļ�,�͸��Ƶ����һ��'/'
				//���ѡ�˶���ļ�,�͸��Ƶ���һ��NULL�ַ�
				lstrcpyn(szPath, szOpenFileNames, ofn.nFileOffset);

				//��ֻѡ��һ���ļ�ʱ,�������NULL�ַ��Ǳ����.
				//���ﲻ����Դ�ѡ��һ���Ͷ���ļ������
				szPath[ofn.nFileOffset] = '\0';
				nLen = lstrlen(szPath);

				if (szPath[nLen - 1] != '\\')   //���ѡ�˶���ļ�,��������'\\'
				{
					lstrcat(szPath, "\\");
				}

				strcpy(szIndexFileName, szPath);//��imgĿ¼·��������index.html
				strcat(szIndexFileName, "index.html");

				p = szOpenFileNames + ofn.nFileOffset; //��ָ���Ƶ���һ���ļ�
				ZeroMemory(szFileName, sizeof(szFileName));
				while (*p)
				{
					lstrcat(szFileName, szPath);  //���ļ�������·��  
					lstrcat(szFileName, p);    //�����ļ���    
					//lstrcat(szFileName, TEXT("/n")); //����//�������У�Ҫ��'\0'���滻����
					//�ַ������滻��'\0'����ʹ�ú������ᱻ�ضϵģ�
					szFileName[lstrlen(szFileName)] = '\n';
					p += lstrlen(p) + 1;     //������һ���ļ�	
				}
			}
			//���ı�������ʾ�ļ�·��  
			HWND hEdt = GetDlgItem(hDlg, IDC_EDIT2);
			SendMessage(hEdt, WM_SETTEXT, NULL, (LPARAM)szFileName);
		}
		else if (LOWORD(wParam) == IDC_BUTTON_CREAT) {
			/*���裺
			����head.html��index.html�������ͼƬ�ߴ�,�Զ�Ӧ��ʽ׷�ӣ�
			���ļ�end.html��׷�ӡ��ر������ļ���
			PS:���﷢�����ڵ�ǰexeĿ¼�´�head.html��ʧ�ܣ�����ֻ���ֶ������ļ�·���ˡ�
			   ˳���index.htmlҲ�����ɵ�ͼƬ·����
			*/
			GetModuleFileName(NULL, szExePath, MAX_PATH);
			(strchr(szExePath, '\\'))[1] = '\0';//ɾ���ļ�����ֻ���·��

			strcpy(szHeadFileName, szExePath);
			strcpy(szEndFileName, szExePath);
			strcat(szHeadFileName, "head.html");
			strcat(szEndFileName, "end.html");

			if (!CopyFile(szHeadFileName, szIndexFileName, FALSE)) {
				MessageBox(NULL, "failed", "", MB_OK);
				return 1;
			}
			wofstream fout(szIndexFileName, wfstream::app);
			if (fout.fail()) {
				MessageBox(NULL, "fout.fail()", "", MB_OK);
				return 1;
			}
			GetWindowText(GetDlgItem(hDlg, IDC_EDIT1), szLink, MAX_PATH);
			int count = 0;
			char *szFileNameBox[MAX_PATH];
			char *buf = szFileName;
			char width[10], height[10];
			while ((szFileNameBox[count] = strtok(buf, "\n")) != NULL) {
				string strFullLink = szLink;
				char fname[MAX_PATH];
				getFileNamefromFullName(szFileNameBox[count], fname);
				strFullLink += fname;
				IplImage* img = cvLoadImage(szFileNameBox[count]);
				sprintf(width, "%d", img->width);
				sprintf(height, "%d", img->height);
				fout << "<div class=\"pic\"> <a href=\"#\" class=\"img imgclasstag\" imggroup=\"gal\" bigimgwidth=\"" << width << "\" bigimgheight=\"" << height << "\" bigimgsrc = \"" << strFullLink.c_str() << "\"> <img src = \"" << strFullLink.c_str() << "\" / > </a> </div>" << endl;
				cvReleaseImage(&img);
				count++;
				buf = NULL;
			}
			fout.close();

			HANDLE hFile;
			HANDLE hAppend;
			DWORD  dwBytesRead, dwBytesWritten, dwPos;
			BYTE   buff[4096];
			// Open the existing file. 
			hFile = CreateFile(szEndFileName, // open
				GENERIC_READ,             // open for reading 
				0,                        // do not share 
				NULL,                     // no security 
				OPEN_EXISTING,            // existing file only 
				FILE_ATTRIBUTE_NORMAL,    // normal file 
				NULL);                    // no attr. template 
			if (hFile == INVALID_HANDLE_VALUE)
			{
				MessageBox(NULL, "fail", "INVALID_HANDLE_VALUE", MB_OK);
				return 1;
			}
			// Open the existing file, or if the file does not exist, 
			// create a new file. 
			hAppend = CreateFile(szIndexFileName, // open Two.txt 
				GENERIC_WRITE,            // open for writing 
				FILE_SHARE_READ,          // allow multiple readers 
				NULL,                     // no security 
				OPEN_ALWAYS,              // open or create 
				FILE_ATTRIBUTE_NORMAL,    // normal file 
				NULL);                    // no attr. template 
			if (hAppend == INVALID_HANDLE_VALUE)
			{
				MessageBox(NULL, "fail", "INVALID_HANDLE_VALUE", MB_OK);
				return 1;
			}
			// Append the first file to the end of the second file. 
			// Lock the second file to prevent another process from 
			// accessing it while writing to it. Unlock the 
			// file when writing is finished. 
			do
			{
				if (ReadFile(hFile, buff, sizeof(buff), &dwBytesRead, NULL))
				{
					dwPos = SetFilePointer(hAppend, 0, NULL, FILE_END);
					LockFile(hAppend, dwPos, 0, dwBytesRead, 0);
					WriteFile(hAppend, buff, dwBytesRead, &dwBytesWritten, NULL);
					UnlockFile(hAppend, dwPos, 0, dwBytesRead, 0);
				}
			} while (dwBytesRead == sizeof(buff));
			// Close both files. 
			CloseHandle(hFile);
			CloseHandle(hAppend);
			MessageBox(NULL, "Succeed", "", MB_OK);
		}
		break;
	}
	return (INT_PTR)FALSE;
}

int getFileNamefromFullName(const char* fullname, char *fname)
{
	int nstr = strlen(fullname);
	int i, pos;

	if (0 == nstr)
	{
		fname[0] = '\0';
		return 1;
	}

	i = nstr - 1;
	while (fullname[i] != '\\' && i >= 0)
		i--;

	if (i < 0)  //��·�����ļ������Ϸ�
		return 0;

	i++; //�ļ�����һ���ַ�����λ��
	pos = i;
	while (i < nstr)
	{
		fname[i - pos] = fullname[i];
		i++;
	}
	fname[i - pos] = '\0';
	return 1;
}

PSTR pWCharTopChar(PWSTR pWChar) {
	//����ֱֵ�Ӿ������軺������С
	size_t len = WideCharToMultiByte(CP_ACP, 0, pWChar, -1, NULL, 0, NULL, NULL);
	PSTR pChar = (PSTR)malloc(len);
	WideCharToMultiByte(CP_ACP, 0, pWChar, -1, pChar, len, NULL, NULL);
	return pChar;
}

PWSTR pCharTopWChar(PCSTR pChar) {
	//����ֵΪ�㣬�������ػ�����������Ŀ��ַ��������õ��ַ���
	size_t len = MultiByteToWideChar(CP_ACP, 0, pChar, -1, NULL, 0);
	PWSTR pWChar = (PWSTR)malloc(sizeof(wchar_t)*len);
	MultiByteToWideChar(CP_ACP, 0, pChar, -1, pWChar, len);
	return pWChar;
}

void getPathNamefromFullName(const char* fullname, char *pname)
{
	int i;
	strcpy(pname, fullname);

	i = strlen(fullname);

	i--;
	while ((i >= 0) && (pname[i] != '\\'))
		i--;

	pname[i + 1] = '\0';
}
