#include "stdafx.h"
#include "YLChangeQuicklyIcom.h"
#include <shlobj.h>



YLChangeQuicklyIcom::YLChangeQuicklyIcom()
{

}
YLChangeQuicklyIcom::~YLChangeQuicklyIcom()
{

}
void StringReplace(std::string &strBase, std::string strSrc, std::string strDes)
{
	std::string::size_type pos = 0;
	std::string::size_type srcLen = strSrc.size();
	std::string::size_type desLen = strDes.size();
	pos = strBase.find(strSrc, pos);
	while ((pos != std::string::npos))
	{
		strBase.replace(pos, srcLen, strDes);
		pos = strBase.find(strSrc, (pos + desLen));
	}
}
void CharToTchar(const char * _char, TCHAR * tchar)
{
	int iLength;

	iLength = MultiByteToWideChar(CP_ACP, 0, _char, strlen(_char) + 1, NULL, 0);
	MultiByteToWideChar(CP_ACP, 0, _char, strlen(_char) + 1, tchar, iLength);
}
void TcharToChar(const TCHAR * tchar, char * _char)
{
	int iLength;
	//获取字节长度   
	iLength = WideCharToMultiByte(CP_ACP, 0, tchar, -1, NULL, 0, NULL, NULL);
	//将tchar值赋给_char    
	WideCharToMultiByte(CP_ACP, 0, tchar, -1, _char, iLength, NULL, NULL);
}
/*
 * wchar转化为char类型
*/
char* WcharToChar(wchar_t* wc)
{
	
	int len = WideCharToMultiByte(CP_ACP, 0, wc, wcslen(wc), NULL, 0, NULL, NULL);
	CHAR* m_char = new char[len + 1];
	WideCharToMultiByte(CP_ACP, 0, wc, wcslen(wc), m_char, len, NULL, NULL);
	m_char[len] = '\0';
	return m_char;
}
/*
* char转化为wchar类型
*/
wchar_t* CharToWchar(char* c)
{
	
	int len = MultiByteToWideChar(CP_ACP, 0, c, strlen(c), NULL, 0);
	WCHAR* m_wchar = new wchar_t[len + 1];
	MultiByteToWideChar(CP_ACP, 0, c, strlen(c), m_wchar, len);
	m_wchar[len] = '\0';
	return m_wchar;
}
/*
*获取快捷键的方式
*/
TCHAR* Get_run_path_app()
{
	TCHAR exeFullPath[MAX_PATH];
	CHAR  exeFullPathChar[MAX_PATH];
	GetModuleFileName(NULL, exeFullPath, MAX_PATH);
	TcharToChar(exeFullPath, exeFullPathChar);
	std::string exeFullPathStr = exeFullPathChar;
	StringReplace(exeFullPathStr,std::string(CUSTOMARY_NAME),std::string(RUN_APP_NAME));
	TCHAR tempPath[MAX_PATH];
	CharToTchar(exeFullPathStr.c_str(), tempPath);
	return tempPath;
}


/*
* regeditPath 要打开的注册表的位置
* regeditKey  要获取的注册表中value的key
*/
bool GetRegeditValueOfKey(WCHAR* regeditPath, WCHAR* regeditKey, std::string& regeditValue, ShortcutsType shortcutsType)
{
	/*
	*chwDir 用于存储注册表中读出来的Desktop的值
	*result 打开注册表返回的结果
	*dwType 用于装载取回数据类型的一个变量
	*dwSize 用于装载lpData缓冲区长度的一个变量。 一旦返回，它会设为实际装载到缓冲区的字节数
	*/
	WCHAR chwDir[512];
	long result;  
	HKEY hKey = 0;
	DWORD dwType;
	DWORD dwSize;
	/*
	* 在这里注意 KEY_WOW64_64KEY 权限值必须使用否则将查找不到指定的路径,返回2时是找不到注册表路径
	* 这里可以根据自己的需求根据自己的需求来修改
	*/
	if (shortcutsType == ShortcutsTypeDesktop)
	{
		result = RegOpenKeyEx(HKEY_LOCAL_MACHINE, regeditPath, NULL, KEY_WOW64_64KEY | KEY_READ, &hKey);
	}
	else if(shortcutsType == ShortcutsTypeStartMenu)
	{
		result = RegOpenKeyEx(HKEY_LOCAL_MACHINE, regeditPath, NULL, KEY_WOW64_64KEY | KEY_READ, &hKey);
	}
	
	//result = RegOpenKeyEx(HKEY_CLASSES_ROOT, regeditPath, NULL, KEY_WOW64_64KEY | KEY_READ, &hKey);
	if (result == 0)
	{
		//std::cout << "@@@@@@@@@@@@@@@@@@@@注册表打开成功@@@@@@@@@@@@@@@@@@@@" << endl;
		dwSize = 512;
		// 查询Desktop的键值并把查询到的结果保存在字符串“chwDir”中（因为Windows并不一定安装在c盘中，所以要查询注册表）
		result = RegQueryValueEx(hKey, regeditKey, 0, &dwType, (LPBYTE)chwDir, &dwSize);
		if (result == 0)
		{
		
			//std::cout << "@@@@@@@@@@@@@@@@@@@@获取value数据成功@@@@@@@@@@@@@@@@@@@@" << endl;
			regeditValue.append(WcharToChar(chwDir));
			
		}
		else
		{


			return false;
		}
		
	}
	else
	{
		return false;
	}
	result = RegCloseKey(hKey);
	return true;
}

//向开始菜单中加入快捷方式
void YLChangeQuicklyIcom::AddToStartMenu(std::string shortcutDisplay)
{
	
	std::string chsDir;
	// 如果要放在用户的自己的桌面下面将"Common Programs"改成"Programs"
	bool result = GetRegeditValueOfKey(KEY_PATHS_START_MENU, L"Common Programs", chsDir,ShortcutsTypeStartMenu);
	if (result)
	{
		//chsDir.clear();
		chsDir.append("\\");
		//chsDir.append(std::string(NOW_APP_GROUP));
		//chsDir.append("\\");
		chsDir.append(NOW_APP_NAME);
		chsDir.append(".lnk");
		//在“开始“菜单里建立“Myshortcut”菜单项
		HRESULT hres;
		/*得到IshellLink接口指针  在这里需要添加shlobj.h文件*/
		IShellLink* pShellLink; 
		/*初始化COM类库*/
		CoInitialize(NULL);     
		hres = CoCreateInstance(CLSID_ShellLink,NULL, CLSCTX_INPROC_SERVER, IID_IShellLink,(LPVOID *)&pShellLink);
		/*
		*判断是否成功的获取了IShellLink
		*/
		if (SUCCEEDED(hres))
		{
			//std::cout << "@@@@@@@@@@@@@@@@@@@@IShellLink获取成功@@@@@@@@@@@@@@@@@@@@" << endl;
			IPersistFile* pPf;
			// 得到当前运行的可执行程序的全路径名
			//GetModuleFileName(NULL, exeFullPath, MAX_PATH);
			pShellLink->SetPath(Get_run_path_app());
			// 设置快捷方式的路径
			pShellLink->SetDescription(L"Yun");  // 设置快捷方式的描述
			hres = pShellLink->QueryInterface(IID_IPersistFile, (LPVOID *)&pPf);
 			if (SUCCEEDED(hres))
			{
				//std::cout << "@@@@@@@@@@@@@@@@@@@@开始菜单添加成功@@@@@@@@@@@@@@@@@@@@" << endl;
				WCHAR sz[MAX_PATH];
				// 字符串为ANSI 格式，须转化为Unicode格式 
				MultiByteToWideChar(CP_ACP, 0, chsDir.c_str(), -1, sz, MAX_PATH);
				hres = pPf->Save(sz, TRUE);   // 保存链接
				pPf->Release();
			}
			pShellLink->Release();
		}
		else
		{
			//std::cout << "@@@@@@@@@@@@@@@@@@@@IShellLink获取失败@@@@@@@@@@@@@@@@@@@@" << endl;
		}
	}
	else
	{
		//std::cout << "@@@@@@@@@@@@@@@@@@@@注册表打开失败@@@@@@@@@@@@@@@@@@@@" << endl;
		//std::cout << result;
	}
	
}

void YLChangeQuicklyIcom::AddToDesktop(std::string shortcutDisplay)
{
	//std::cout << "向桌面加入快捷方式" << endl;
	std::string chsDir;
	// 如果要放在用户的自己的桌面下面将"Common Desktop"改成"Desktop"
	bool result = GetRegeditValueOfKey(KEY_PATHS_DESKTOP, L"Common Desktop", chsDir, ShortcutsTypeDesktop);
	if (result)
	{
		chsDir.append("\\");
		chsDir.append(std::string(NOW_APP_NAME));
		chsDir.append(".lnk");
		/*CoCreateInstance的返回值*/
		HRESULT hres;
		/*得到IshellLink接口指针  在这里需要添加shlobj.h文件*/
		IShellLink* pShellLink;
		/*初始化COM类库*/
		CoInitialize(NULL);
		hres = CoCreateInstance(CLSID_ShellLink,NULL, CLSCTX_INPROC_SERVER, IID_IShellLink, (LPVOID *)&pShellLink);
		if (SUCCEEDED(hres))
		{
			/*
			* 获得IshellLink接口指针 成功
			*/
			IPersistFile* pPf;
			
			

			pShellLink->SetPath(Get_run_path_app());
			pShellLink->SetDescription(L"Yun");
				hres = pShellLink->QueryInterface(IID_IPersistFile, (LPVOID *)&pPf);
			if (SUCCEEDED(hres))
			{
				//std::cout << "@@@@@@@@@@@@@@@@@@@@桌面菜单添加成功@@@@@@@@@@@@@@@@@@@@" << endl;
				WCHAR sz[MAX_PATH];
				// 字符串为ANSI 格式，须转化为Unicode格式 
				MultiByteToWideChar(CP_ACP, 0, chsDir.c_str(), -1, sz, MAX_PATH);
				hres = pPf->Save(sz, TRUE);   // 保存链接
				pPf->Release();
			}
			pShellLink->Release();
		}
	}
}
void YLChangeQuicklyIcom::DeleteStartMenu(std::string shortcutDisplay)
{
	//std::cout << "删除开始菜单中的快捷方式" << endl;
	std::string chsDir;
	bool result = GetRegeditValueOfKey(KEY_PATHS_START_MENU, L"Common Programs", chsDir, ShortcutsTypeStartMenu);
	if (result)
	{
		CFileFind findfile;
		//WCHAR str[MAX_PATH] = );
		SetCurrentDirectory(CharToWchar((char*)chsDir.c_str()));
		/*if (!findfile.FindFile(CharToWchar((char*)shortcutDisplay.c_str())))
		{
			std::cout << "没有文件";
			return;
		}*/
		//strcat(chDir, "//Myshortcut.lnk");
		chsDir.append("\\");
		//chsDir.append(CUSTOMARY_APP_GROUP);
		//chsDir.append("\\");
		chsDir.append(CUSTOMARY_APP_NAME);
		chsDir.append(".lnk");
		char lpDelDir[512];
		memset(lpDelDir, 0, 512);
		strcat_s(lpDelDir, chsDir.c_str());
		//这里直接删除
		bool deleteSu = DeleteFile(CharToWchar(lpDelDir));
		if (deleteSu)
		{
			//std::cout << "删除成功";
		}
		else
		{
			//std::cout << "删除失败";
		}
	}
}
void YLChangeQuicklyIcom::DeleteDesktop(std::string shortcutDisplay)
{
	//std::cout << "删除桌面的快捷方式" << endl;
	std::string chsDir;
	bool result = GetRegeditValueOfKey(KEY_PATHS_DESKTOP, L"Common Desktop", chsDir, ShortcutsTypeDesktop);
	if (result)
	{
		CFileFind findfile;
		//WCHAR str[MAX_PATH] = );
		SetCurrentDirectory(CharToWchar((char*)chsDir.c_str()));
		/*if (!findfile.FindFile(CharToWchar((char*)std::string(CUSTOMARY_APP_NAME).c_str())))
		{
			std::cout << "没有文件";
			return;
		}*/
		//strcat(chDir, "//Myshortcut.lnk");
		chsDir.append("\\");
		chsDir.append(std::string(CUSTOMARY_APP_NAME));
		chsDir.append(".lnk");
		char lpDelDir[512];
		memset(lpDelDir, 0, 512);
		strcat_s(lpDelDir, chsDir.c_str());
		//这里直接删除
		bool deleteSu = DeleteFile(CharToWchar(lpDelDir));
		if (deleteSu)
		{
			//std::cout << "删除成功";
		}
		else
		{
			//std::cout << "删除失败";
		}
	}

}