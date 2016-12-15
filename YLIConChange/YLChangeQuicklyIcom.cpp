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
	//��ȡ�ֽڳ���   
	iLength = WideCharToMultiByte(CP_ACP, 0, tchar, -1, NULL, 0, NULL, NULL);
	//��tcharֵ����_char    
	WideCharToMultiByte(CP_ACP, 0, tchar, -1, _char, iLength, NULL, NULL);
}
/*
 * wcharת��Ϊchar����
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
* charת��Ϊwchar����
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
*��ȡ��ݼ��ķ�ʽ
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
* regeditPath Ҫ�򿪵�ע����λ��
* regeditKey  Ҫ��ȡ��ע�����value��key
*/
bool GetRegeditValueOfKey(WCHAR* regeditPath, WCHAR* regeditKey, std::string& regeditValue, ShortcutsType shortcutsType)
{
	/*
	*chwDir ���ڴ洢ע����ж�������Desktop��ֵ
	*result ��ע����صĽ��
	*dwType ����װ��ȡ���������͵�һ������
	*dwSize ����װ��lpData���������ȵ�һ�������� һ�����أ�������Ϊʵ��װ�ص����������ֽ���
	*/
	WCHAR chwDir[512];
	long result;  
	HKEY hKey = 0;
	DWORD dwType;
	DWORD dwSize;
	/*
	* ������ע�� KEY_WOW64_64KEY Ȩ��ֵ����ʹ�÷��򽫲��Ҳ���ָ����·��,����2ʱ���Ҳ���ע���·��
	* ������Ը����Լ�����������Լ����������޸�
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
		//std::cout << "@@@@@@@@@@@@@@@@@@@@ע���򿪳ɹ�@@@@@@@@@@@@@@@@@@@@" << endl;
		dwSize = 512;
		// ��ѯDesktop�ļ�ֵ���Ѳ�ѯ���Ľ���������ַ�����chwDir���У���ΪWindows����һ����װ��c���У�����Ҫ��ѯע���
		result = RegQueryValueEx(hKey, regeditKey, 0, &dwType, (LPBYTE)chwDir, &dwSize);
		if (result == 0)
		{
		
			//std::cout << "@@@@@@@@@@@@@@@@@@@@��ȡvalue���ݳɹ�@@@@@@@@@@@@@@@@@@@@" << endl;
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

//��ʼ�˵��м����ݷ�ʽ
void YLChangeQuicklyIcom::AddToStartMenu(std::string shortcutDisplay)
{
	
	std::string chsDir;
	// ���Ҫ�����û����Լ����������潫"Common Programs"�ĳ�"Programs"
	bool result = GetRegeditValueOfKey(KEY_PATHS_START_MENU, L"Common Programs", chsDir,ShortcutsTypeStartMenu);
	if (result)
	{
		//chsDir.clear();
		chsDir.append("\\");
		//chsDir.append(std::string(NOW_APP_GROUP));
		//chsDir.append("\\");
		chsDir.append(NOW_APP_NAME);
		chsDir.append(".lnk");
		//�ڡ���ʼ���˵��ｨ����Myshortcut���˵���
		HRESULT hres;
		/*�õ�IshellLink�ӿ�ָ��  ��������Ҫ���shlobj.h�ļ�*/
		IShellLink* pShellLink; 
		/*��ʼ��COM���*/
		CoInitialize(NULL);     
		hres = CoCreateInstance(CLSID_ShellLink,NULL, CLSCTX_INPROC_SERVER, IID_IShellLink,(LPVOID *)&pShellLink);
		/*
		*�ж��Ƿ�ɹ��Ļ�ȡ��IShellLink
		*/
		if (SUCCEEDED(hres))
		{
			//std::cout << "@@@@@@@@@@@@@@@@@@@@IShellLink��ȡ�ɹ�@@@@@@@@@@@@@@@@@@@@" << endl;
			IPersistFile* pPf;
			// �õ���ǰ���еĿ�ִ�г����ȫ·����
			//GetModuleFileName(NULL, exeFullPath, MAX_PATH);
			pShellLink->SetPath(Get_run_path_app());
			// ���ÿ�ݷ�ʽ��·��
			pShellLink->SetDescription(L"Yun");  // ���ÿ�ݷ�ʽ������
			hres = pShellLink->QueryInterface(IID_IPersistFile, (LPVOID *)&pPf);
 			if (SUCCEEDED(hres))
			{
				//std::cout << "@@@@@@@@@@@@@@@@@@@@��ʼ�˵���ӳɹ�@@@@@@@@@@@@@@@@@@@@" << endl;
				WCHAR sz[MAX_PATH];
				// �ַ���ΪANSI ��ʽ����ת��ΪUnicode��ʽ 
				MultiByteToWideChar(CP_ACP, 0, chsDir.c_str(), -1, sz, MAX_PATH);
				hres = pPf->Save(sz, TRUE);   // ��������
				pPf->Release();
			}
			pShellLink->Release();
		}
		else
		{
			//std::cout << "@@@@@@@@@@@@@@@@@@@@IShellLink��ȡʧ��@@@@@@@@@@@@@@@@@@@@" << endl;
		}
	}
	else
	{
		//std::cout << "@@@@@@@@@@@@@@@@@@@@ע����ʧ��@@@@@@@@@@@@@@@@@@@@" << endl;
		//std::cout << result;
	}
	
}

void YLChangeQuicklyIcom::AddToDesktop(std::string shortcutDisplay)
{
	//std::cout << "����������ݷ�ʽ" << endl;
	std::string chsDir;
	// ���Ҫ�����û����Լ����������潫"Common Desktop"�ĳ�"Desktop"
	bool result = GetRegeditValueOfKey(KEY_PATHS_DESKTOP, L"Common Desktop", chsDir, ShortcutsTypeDesktop);
	if (result)
	{
		chsDir.append("\\");
		chsDir.append(std::string(NOW_APP_NAME));
		chsDir.append(".lnk");
		/*CoCreateInstance�ķ���ֵ*/
		HRESULT hres;
		/*�õ�IshellLink�ӿ�ָ��  ��������Ҫ���shlobj.h�ļ�*/
		IShellLink* pShellLink;
		/*��ʼ��COM���*/
		CoInitialize(NULL);
		hres = CoCreateInstance(CLSID_ShellLink,NULL, CLSCTX_INPROC_SERVER, IID_IShellLink, (LPVOID *)&pShellLink);
		if (SUCCEEDED(hres))
		{
			/*
			* ���IshellLink�ӿ�ָ�� �ɹ�
			*/
			IPersistFile* pPf;
			
			

			pShellLink->SetPath(Get_run_path_app());
			pShellLink->SetDescription(L"Yun");
				hres = pShellLink->QueryInterface(IID_IPersistFile, (LPVOID *)&pPf);
			if (SUCCEEDED(hres))
			{
				//std::cout << "@@@@@@@@@@@@@@@@@@@@����˵���ӳɹ�@@@@@@@@@@@@@@@@@@@@" << endl;
				WCHAR sz[MAX_PATH];
				// �ַ���ΪANSI ��ʽ����ת��ΪUnicode��ʽ 
				MultiByteToWideChar(CP_ACP, 0, chsDir.c_str(), -1, sz, MAX_PATH);
				hres = pPf->Save(sz, TRUE);   // ��������
				pPf->Release();
			}
			pShellLink->Release();
		}
	}
}
void YLChangeQuicklyIcom::DeleteStartMenu(std::string shortcutDisplay)
{
	//std::cout << "ɾ����ʼ�˵��еĿ�ݷ�ʽ" << endl;
	std::string chsDir;
	bool result = GetRegeditValueOfKey(KEY_PATHS_START_MENU, L"Common Programs", chsDir, ShortcutsTypeStartMenu);
	if (result)
	{
		CFileFind findfile;
		//WCHAR str[MAX_PATH] = );
		SetCurrentDirectory(CharToWchar((char*)chsDir.c_str()));
		/*if (!findfile.FindFile(CharToWchar((char*)shortcutDisplay.c_str())))
		{
			std::cout << "û���ļ�";
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
		//����ֱ��ɾ��
		bool deleteSu = DeleteFile(CharToWchar(lpDelDir));
		if (deleteSu)
		{
			//std::cout << "ɾ���ɹ�";
		}
		else
		{
			//std::cout << "ɾ��ʧ��";
		}
	}
}
void YLChangeQuicklyIcom::DeleteDesktop(std::string shortcutDisplay)
{
	//std::cout << "ɾ������Ŀ�ݷ�ʽ" << endl;
	std::string chsDir;
	bool result = GetRegeditValueOfKey(KEY_PATHS_DESKTOP, L"Common Desktop", chsDir, ShortcutsTypeDesktop);
	if (result)
	{
		CFileFind findfile;
		//WCHAR str[MAX_PATH] = );
		SetCurrentDirectory(CharToWchar((char*)chsDir.c_str()));
		/*if (!findfile.FindFile(CharToWchar((char*)std::string(CUSTOMARY_APP_NAME).c_str())))
		{
			std::cout << "û���ļ�";
			return;
		}*/
		//strcat(chDir, "//Myshortcut.lnk");
		chsDir.append("\\");
		chsDir.append(std::string(CUSTOMARY_APP_NAME));
		chsDir.append(".lnk");
		char lpDelDir[512];
		memset(lpDelDir, 0, 512);
		strcat_s(lpDelDir, chsDir.c_str());
		//����ֱ��ɾ��
		bool deleteSu = DeleteFile(CharToWchar(lpDelDir));
		if (deleteSu)
		{
			//std::cout << "ɾ���ɹ�";
		}
		else
		{
			//std::cout << "ɾ��ʧ��";
		}
	}

}