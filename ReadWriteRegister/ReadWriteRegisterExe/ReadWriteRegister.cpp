#include "stdafx.h"
#include "ReadWriteRegister.h"



BOOL GetRegister(RegInfo &regInfo)
{
	HKEY  hkey;
	LONG nFlag;//是否成功

	nFlag = RegOpenKeyEx(    //打开一个指定的注册表键
		regInfo.HKey,   //需要打开的主键（根项）的名称
		regInfo.SubKey, //需要打开的子键的名称
		NULL, // 保留值，设为0
		KEY_READ,// 安全访问标记，也就是权限 
		&hkey);// 得到的将要打开键的句柄
			   // 如果函数调用成功，则返回0（ERROR_SUCCESS）。
			   //否则，返回值为文件WINERROR.h中定义的一个非零的错误代码。

	if (nFlag == ERROR_SUCCESS)
	{
		//RegQueryValueEx检索一个已打开的注册表句柄中指定的注册表键的类型和设置值。
		//返回值零（ERROR_SUCCESS）表示成功。其他任何值都代表一个错误代码
		if (RegQueryValueEx(hkey, //一个已打开项的句柄
			regInfo.ValueName, //要获取值的名字，注册表键的名字
			NULL, //保留项，无用，设为零
			regInfo.ValueType, ////用于装载取回"类型"的一个变量
			(LPBYTE)regInfo.ValueData, //用于装载指定"数值"的一个缓冲区
			&regInfo.DataLen) //用于装载lpData缓冲区"长度"的一个变量。 
							  //一旦返回，它会设为实际装载到缓冲区的字节数
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

	//若想要设置的注册表项下不是默认值，即有名称值的数据和类型时，
	//可以调用RegSetValueEx函数，该函数将在注册表项下设置指定值的数据和类型。
	//返回值 0（ERROR_SUCCESS）表示成功
	if (RegSetValueEx(hkey,      //  subkey  handle
		regInfo.ValueName,          //  为NULL则写入默认值
		0,                     //  must  be  zero
		(DWORD)regInfo.ValueType, //  value  type
		(LPBYTE)regInfo.ValueData,       // 指向一个缓冲区，该缓冲区包含了欲为指定值名称存储的数据。
		regInfo.DataLen) != ERROR_SUCCESS)  //指定由lpData参数所指向的数据的大小，单位是字节。
	{
		RegCloseKey(hkey);
		return FALSE;
	}

	RegCloseKey(hkey);
	return TRUE;
}


BOOL DelRegisterValue(const RegInfo &regInfo) {
	HKEY hKey;
	//打开注册表
	if (RegOpenKeyEx(
		regInfo.HKey,
		regInfo.SubKey,
		NULL,              // 保留参数NULL
		KEY_WRITE,      // 打开权限，写入
		&hKey           // 打开之后的句柄
		) != ERROR_SUCCESS) {
		RegCloseKey(hKey);
		return FALSE;
	}
	// 删除 regInfo.HKey\regInfo.SubKey 里的ValueName
	if (RegDeleteValue(hKey, regInfo.ValueName) != ERROR_SUCCESS) {
		RegCloseKey(hKey);
		return FALSE;
	}
	else
	{
		//关闭注册表:
		RegCloseKey(hKey);
		return TRUE;
	}

}


BOOL DelRegisterKey(const RegInfo &regInfo) {
	HKEY hKey;
	//打开注册表
	// 删除 regInfo.SubKey里最后那个子键
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
		lastKey,//到要删除键之前的路径！
		NULL,              // 保留参数NULL
		KEY_WRITE,      // 打开权限，写入
		&hKey           // 打开之后的句柄
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
		//关闭注册表:
		RegCloseKey(hKey);
		return TRUE;
	}

}

BOOL SetAutoRun(const WCHAR* exePath) {
	HKEY hKey;
	//打开注册表
	if (RegOpenKeyEx(
		HKEY_LOCAL_MACHINE, // 本机注册表（要打开的注册表）
		L"Software\\Microsoft\\Windows\\CurrentVersion\\Run", // 要打开的注册表项名称
		0,              // 保留参数必须填 0
		KEY_SET_VALUE,  // 打开权限，写入
		&hKey           // 打开之后的句柄
		) != ERROR_SUCCESS) {
		RegCloseKey(hKey);
		return FALSE;
	}
	//接下来根据exe路径得到exe的名字作为键名
	WCHAR keyName[MAX_PATH];
	wcscpy(keyName, exePath);
	while ((wcsstr(keyName, L"\\")) != NULL) {
		wcscpy(keyName, wcsstr(keyName, L"\\") + 1);
	}
	auto temp = wcsstr(keyName, L".exe");
	if (temp != NULL) {
		keyName[temp - keyName] = L'\0';
	}
	// 在注册表中设置(没有则会新增一个值)
	if (RegSetValueEx(
		hKey,
		keyName, // 键名
		0,                  // 保留参数必须填 0
		REG_SZ,             // 键值类型为字符串
		(LPBYTE)exePath, // 字符串首地址
		wcslen(exePath)*sizeof(WCHAR)        // 字符串长度
		) != ERROR_SUCCESS) {
		RegCloseKey(hKey);
		return FALSE;
	}

	//关闭注册表:
	RegCloseKey(hKey);
	return TRUE;
}


