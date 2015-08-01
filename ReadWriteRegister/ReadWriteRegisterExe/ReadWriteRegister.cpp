#include "stdafx.h"
#include "ReadWriteRegister.h"



BOOL GetRegister(RegInfo &regInfo)
{
	HKEY  hkey;
	LONG nFlag;//�Ƿ�ɹ�

	nFlag = RegOpenKeyEx(    //��һ��ָ����ע����
		regInfo.HKey,   //��Ҫ�򿪵����������������
		regInfo.SubKey, //��Ҫ�򿪵��Ӽ�������
		NULL, // ����ֵ����Ϊ0
		KEY_READ,// ��ȫ���ʱ�ǣ�Ҳ����Ȩ�� 
		&hkey);// �õ��Ľ�Ҫ�򿪼��ľ��
			   // ����������óɹ����򷵻�0��ERROR_SUCCESS����
			   //���򣬷���ֵΪ�ļ�WINERROR.h�ж����һ������Ĵ�����롣

	if (nFlag == ERROR_SUCCESS)
	{
		//RegQueryValueEx����һ���Ѵ򿪵�ע�������ָ����ע���������ͺ�����ֵ��
		//����ֵ�㣨ERROR_SUCCESS����ʾ�ɹ��������κ�ֵ������һ���������
		if (RegQueryValueEx(hkey, //һ���Ѵ���ľ��
			regInfo.ValueName, //Ҫ��ȡֵ�����֣�ע����������
			NULL, //��������ã���Ϊ��
			regInfo.ValueType, ////����װ��ȡ��"����"��һ������
			(LPBYTE)regInfo.ValueData, //����װ��ָ��"��ֵ"��һ��������
			&regInfo.DataLen) //����װ��lpData������"����"��һ�������� 
							  //һ�����أ�������Ϊʵ��װ�ص����������ֽ���
			== ERROR_SUCCESS)
			RegCloseKey(hkey);
		else
			return FALSE;
	}
	else
		return FALSE;
	return TRUE;
}


BOOL SetRegister(const RegInfo &regInfo)
{
	HKEY  hkey;

	if (RegCreateKey(regInfo.HKey, regInfo.SubKey, &hkey)) {
		RegCloseKey(hkey);
		return FALSE;
	}

	//����Ҫ���õ�ע������²���Ĭ��ֵ����������ֵ�����ݺ�����ʱ��
	//���Ե���RegSetValueEx�������ú�������ע�����������ָ��ֵ�����ݺ����͡�
	//����ֵ 0��ERROR_SUCCESS����ʾ�ɹ�
	if (RegSetValueEx(hkey,      //  subkey  handle
		regInfo.ValueName,          //  ΪNULL��д��Ĭ��ֵ
		0,                     //  must  be  zero
		(DWORD)regInfo.ValueType, //  value  type
		(LPBYTE)regInfo.ValueData,       // ָ��һ�����������û�������������Ϊָ��ֵ���ƴ洢�����ݡ�
		regInfo.DataLen) != ERROR_SUCCESS)  //ָ����lpData������ָ������ݵĴ�С����λ���ֽڡ�
	{
		RegCloseKey(hkey);
		return FALSE;
	}

	RegCloseKey(hkey);
	return TRUE;
}


BOOL DelRegisterValue(const RegInfo &regInfo) {
	HKEY hKey;
	//��ע���
	if (RegOpenKeyEx(
		regInfo.HKey,
		regInfo.SubKey,
		NULL,              // ��������NULL
		KEY_WRITE,      // ��Ȩ�ޣ�д��
		&hKey           // ��֮��ľ��
		) != ERROR_SUCCESS) {
		RegCloseKey(hKey);
		return FALSE;
	}
	// ɾ�� regInfo.HKey\regInfo.SubKey ���ValueName
	if (RegDeleteValue(hKey, regInfo.ValueName) != ERROR_SUCCESS) {
		RegCloseKey(hKey);
		return FALSE;
	}
	else
	{
		//�ر�ע���:
		RegCloseKey(hKey);
		return TRUE;
	}

}


BOOL DelRegisterKey(const RegInfo &regInfo) {
	HKEY hKey;
	//��ע���
	// ɾ�� regInfo.SubKey������Ǹ��Ӽ�
	WCHAR finalKey[MAX_PATH];
	WCHAR lastKey[MAX_PATH];
	wcscpy(finalKey, regInfo.SubKey);
	wcscpy(lastKey, regInfo.SubKey);
	while ((wcsstr(finalKey, L"\\")) != NULL) {
		wcscpy(finalKey, wcsstr(finalKey, L"\\") + 1);
	}
	lastKey[wcsstr(lastKey,finalKey) - lastKey] = L'\0';
	if (RegOpenKeyEx(
		regInfo.HKey,
		lastKey,//��Ҫɾ����֮ǰ��·����
		NULL,              // ��������NULL
		KEY_WRITE,      // ��Ȩ�ޣ�д��
		&hKey           // ��֮��ľ��
		) != ERROR_SUCCESS) {
		RegCloseKey(hKey);
		return FALSE;
	}

	if (RegDeleteKey(hKey, finalKey) != ERROR_SUCCESS) {
		RegCloseKey(hKey);
		return FALSE;
	}
	else
	{
		//�ر�ע���:
		RegCloseKey(hKey);
		return TRUE;
	}

}

BOOL SetAutoRun(const WCHAR* exePath) {
	HKEY hKey;
	//��ע���
	if (RegOpenKeyEx(
		HKEY_LOCAL_MACHINE, // ����ע���Ҫ�򿪵�ע���
		L"Software\\Microsoft\\Windows\\CurrentVersion\\Run", // Ҫ�򿪵�ע���������
		0,              // �������������� 0
		KEY_SET_VALUE,  // ��Ȩ�ޣ�д��
		&hKey           // ��֮��ľ��
		) != ERROR_SUCCESS) {
		RegCloseKey(hKey);
		return FALSE;
	}
	//����������exe·���õ�exe��������Ϊ����
	WCHAR keyName[MAX_PATH];
	wcscpy(keyName, exePath);
	while ((wcsstr(keyName, L"\\")) != NULL) {
		wcscpy(keyName, wcsstr(keyName, L"\\") + 1);
	}
	auto temp = wcsstr(keyName, L".exe");
	if (temp != NULL) {
		keyName[temp - keyName] = L'\0';
	}
	// ��ע���������(û���������һ��ֵ)
	if (RegSetValueEx(
		hKey,
		keyName, // ����
		0,                  // �������������� 0
		REG_SZ,             // ��ֵ����Ϊ�ַ���
		(LPBYTE)exePath, // �ַ����׵�ַ
		wcslen(exePath)*sizeof(WCHAR)        // �ַ�������
		) != ERROR_SUCCESS) {
		RegCloseKey(hKey);
		return FALSE;
	}

	//�ر�ע���:
	RegCloseKey(hKey);
	return TRUE;
}


