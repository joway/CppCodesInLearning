#pragma once

class RegInfo
{
public:
	HKEY HKey;//根数名字（值为HKEY_LOCAL_MACHINE等宏）eg:HKEY_LOCAL_MACHINE
	LPCWSTR SubKey;//子键 eg："*\\shell\\Joway"
	LPCWSTR ValueName;//值名 eg:Wong

	LPDWORD ValueType;
	//值类型 eg:REG_SZ(固定长度的文本字符串。)

	WCHAR ValueData[MAX_PATH];//值数据（ValueData的缓冲区） 
    //eg:{09a28848-0e97-4cef-b950-cea037161155}
	DWORD  DataLen = 256; //装载ValueData的缓冲区"长度"的一个变量

	RegInfo() {
		ValueType = (LPDWORD)REG_NONE;//若不指定初始值则其会没有大小！
		DataLen = 256;
	}

	//一析构了以后反而会出现堆错误！	但是这里的确存在内存泄漏问题
	//后来把动态分配的内存全部换成数组以后才正常！
	//~RegInfo() {
	//	free(ValueData);
	//	free(ValueType);
	//}
};



//GetRegister()需要构造的RegInfo必须指定了hKey,subKey,ValueName
//调用后填上ValueType、ValueData、DataLen
//返回TRUE表示成功！
/*
调用方法：
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



//SetRegister()需要构造的RegInfo必须全部指定hKey,subKey,ValueName,ValueType,ValueData,DataLen
//ValueName为NULL则写入默认值
//返回TRUE表示成功！
/*
调用方法：
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



//删除注册表子键的值
//要构造出hKey,subKey,ValueName
/*
调用方法：
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



//删除注册表子键
//要构造出hKey,subKey
//删除subKey路径内最后一个子键！
/*
调用方法：
RegInfo ri;
ri.HKey = HKEY_CLASSES_ROOT;
ri.SubKey = L"*\\shell\\Joway";
if (DelRegisterKey(ri))
MessageBox(NULL, L"Success!", L"", MB_OK);
else
MessageBox(NULL, L"Fail!", L"", MB_OK);
*/
BOOL DelRegisterKey(const RegInfo &regInfo);



//设置开机自动启动
/*调用方法：
SetAutoRun(L"\"D:\\项目\\Visual C++ 项目\\项目--密码文件加密\\项目--密码文件加密\\Product\\x64\\SMFedDeamon.exe\"")
*/
BOOL SetAutoRun(const WCHAR* exePath);



////列举某个注册表项之下的所有子项实例(有点复杂，暂时不考虑)
//BOOL GetRegisterQueryKey(const RegInfo &regInfo);
//参考链接：http://www.lellansin.com/windows-api-%E6%95%99%E7%A8%8B%EF%BC%88%E5%8D%81%EF%BC%89-%E6%B3%A8%E5%86%8C%E8%A1%A8%E6%93%8D%E4%BD%9C.html