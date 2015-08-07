// PhotosIndexCreater.cpp : 定义应用程序的入口点。
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

// 全局变量: 
HINSTANCE hInst;                                // 当前实例


char szLink[MAX_PATH];//相册目录链接，以'/结尾'
char szFileName[128 * MAX_PATH];//多选中的图片文件名，之间以'\n'间隔！
char szExePath[MAX_PATH];
char szHeadFileName[MAX_PATH];
char szEndFileName[MAX_PATH];
char szIndexFileName[MAX_PATH];

INT_PTR CALLBACK    PhotosIndexCreater(HWND, UINT, WPARAM, LPARAM);

/*		从带路径的完整文件名中抽取出文件名（包括后缀名）
参数：
fname：文件名（不带路径）
fullname：带有路径的文件名

返回值：
1：抽取成功
0：抽取失败
*/
int getFileNamefromFullName(const char* fullname, char *fname);

/*		从带路径的完整文件名中抽取出路径名
参数：
fullname：带有路径的文件名
pname：路径名（包括最后一个反斜杠'\'）
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

	// TODO: 在此放置代码。
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
			ofn.lpstrFilter = "所有文件\0*.*\0文本文件\0*.txt\0JPEG文件\0*.jpg\0";
			ofn.nFilterIndex = 3;
			ofn.lpstrFileTitle = NULL;
			ofn.nMaxFileTitle = 0;
			ofn.lpstrInitialDir = NULL;
			ofn.Flags = OFN_EXPLORER | OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_ALLOWMULTISELECT;

			if (GetOpenFileName(&ofn))
			{
				//把第一个文件名前的复制到szPath,即:
				//如果只选了一个文件,就复制到最后一个'/'
				//如果选了多个文件,就复制到第一个NULL字符
				lstrcpyn(szPath, szOpenFileNames, ofn.nFileOffset);

				//当只选了一个文件时,下面这个NULL字符是必需的.
				//这里不区别对待选了一个和多个文件的情况
				szPath[ofn.nFileOffset] = '\0';
				nLen = lstrlen(szPath);

				if (szPath[nLen - 1] != '\\')   //如果选了多个文件,则必须加上'\\'
				{
					lstrcat(szPath, "\\");
				}

				strcpy(szIndexFileName, szPath);//在img目录路径下生成index.html
				strcat(szIndexFileName, "index.html");

				p = szOpenFileNames + ofn.nFileOffset; //把指针移到第一个文件
				ZeroMemory(szFileName, sizeof(szFileName));
				while (*p)
				{
					lstrcat(szFileName, szPath);  //给文件名加上路径  
					lstrcat(szFileName, p);    //加上文件名    
					//lstrcat(szFileName, TEXT("/n")); //换行//这样不行，要把'\0'给替换掉！
					//字符串不替换掉'\0'后期使用函数都会被截断的！
					szFileName[lstrlen(szFileName)] = '\n';
					p += lstrlen(p) + 1;     //移至下一个文件	
				}
			}
			//在文本框中显示文件路径  
			HWND hEdt = GetDlgItem(hDlg, IDC_EDIT2);
			SendMessage(hEdt, WM_SETTEXT, NULL, (LPARAM)szFileName);
		}
		else if (LOWORD(wParam) == IDC_BUTTON_CREAT) {
			/*步骤：
			复制head.html至index.html，计算出图片尺寸,以对应格式追加，
			打开文件end.html，追加。关闭所有文件。
			PS:这里发现我在当前exe目录下打开head.html会失败，后来只能手动加上文件路径了。
			   顺便把index.html也给生成到图片路径下
			*/
			GetModuleFileName(NULL, szExePath, MAX_PATH);
			(strchr(szExePath, '\\'))[1] = '\0';//删除文件名，只获得路径

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

	if (i < 0)  //带路径的文件名不合法
		return 0;

	i++; //文件名第一个字符所在位置
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
	//返回值直接就是所需缓冲区大小
	size_t len = WideCharToMultiByte(CP_ACP, 0, pWChar, -1, NULL, 0, NULL, NULL);
	PSTR pChar = (PSTR)malloc(len);
	WideCharToMultiByte(CP_ACP, 0, pWChar, -1, pChar, len, NULL, NULL);
	return pChar;
}

PWSTR pCharTopWChar(PCSTR pChar) {
	//若此值为零，函数返回缓冲区所必需的宽字符数，即得到字符数
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
