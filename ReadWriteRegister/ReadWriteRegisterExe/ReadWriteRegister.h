#pragma once

class RegInfo
{
public:
	HKEY HKey;//�������֣�ֵΪHKEY_LOCAL_MACHINE�Ⱥ꣩eg:HKEY_LOCAL_MACHINE
	LPCWSTR SubKey;//�Ӽ� eg��"*\\shell\\Joway"
	LPCWSTR ValueName;//ֵ�� eg:Wong

	LPDWORD ValueType;
	//ֵ���� eg:REG_SZ(�̶����ȵ��ı��ַ�����)

	WCHAR ValueData[MAX_PATH];//ֵ���ݣ�ValueData�Ļ������� 
    //eg:{09a28848-0e97-4cef-b950-cea037161155}
	DWORD  DataLen = 256; //װ��ValueData�Ļ�����"����"��һ������

	RegInfo() {
		ValueType = (LPDWORD)REG_NONE;//����ָ����ʼֵ�����û�д�С��
		DataLen = 256;
	}

	//һ�������Ժ󷴶�����ֶѴ���	���������ȷ�����ڴ�й©����
	//�����Ѷ�̬������ڴ�ȫ�����������Ժ��������
	//~RegInfo() {
	//	free(ValueData);
	//	free(ValueType);
	//}
};



//GetRegister()��Ҫ�����RegInfo����ָ����hKey,subKey,ValueName
//���ú�����ValueType��ValueData��DataLen
//����TRUE��ʾ�ɹ���
/*
���÷�����
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
*/
BOOL GetRegister(RegInfo &regInfo);



//SetRegister()��Ҫ�����RegInfo����ȫ��ָ��hKey,subKey,ValueName,ValueType,ValueData,DataLen
//ValueNameΪNULL��д��Ĭ��ֵ
//����TRUE��ʾ�ɹ���
/*
���÷�����
RegInfo ri;
ri.HKey = HKEY_CLASSES_ROOT;
ri.SubKey = L"*\\shell\\Joway";
ri.ValueName = NULL;
if (GetRegister(ri)) {
MessageBox(NULL,(ri.ValueData), L"", MB_OK);
}
else
MessageBox(NULL,L"Fail!", L"", MB_OK);
*/
BOOL SetRegister(const RegInfo &regInfo);



//ɾ��ע����Ӽ���ֵ
//Ҫ�����hKey,subKey,ValueName
/*
���÷�����
RegInfo ri;
ri.HKey = HKEY_CLASSES_ROOT;
ri.SubKey = L"*\\shell\\Joway";
ri.ValueName = NULL;
if (DelRegisterValue(ri))
MessageBox(NULL, L"Success!", L"", MB_OK);
else
MessageBox(NULL, L"Fail!", L"", MB_OK);
*/
BOOL DelRegisterValue(const RegInfo &regInfo);



//ɾ��ע����Ӽ�
//Ҫ�����hKey,subKey
//ɾ��subKey·�������һ���Ӽ���
/*
���÷�����
RegInfo ri;
ri.HKey = HKEY_CLASSES_ROOT;
ri.SubKey = L"*\\shell\\Joway";
if (DelRegisterKey(ri))
MessageBox(NULL, L"Success!", L"", MB_OK);
else
MessageBox(NULL, L"Fail!", L"", MB_OK);
*/
BOOL DelRegisterKey(const RegInfo &regInfo);



//���ÿ����Զ�����
/*���÷�����
SetAutoRun(L"\"D:\\��Ŀ\\Visual C++ ��Ŀ\\��Ŀ--�����ļ�����\\��Ŀ--�����ļ�����\\Product\\x64\\SMFedDeamon.exe\"")
*/
BOOL SetAutoRun(const WCHAR* exePath);



////�о�ĳ��ע�����֮�µ���������ʵ��(�е㸴�ӣ���ʱ������)
//BOOL GetRegisterQueryKey(const RegInfo &regInfo);
//�ο����ӣ�http://www.lellansin.com/windows-api-%E6%95%99%E7%A8%8B%EF%BC%88%E5%8D%81%EF%BC%89-%E6%B3%A8%E5%86%8C%E8%A1%A8%E6%93%8D%E4%BD%9C.html