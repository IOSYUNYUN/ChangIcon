// YLIConChange.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "YLChangeQuicklyIcom.h"
#include <shlobj.h>
int _tmain(int argc, _TCHAR* argv[])
{
	//删除
	YLChangeQuicklyIcom::DeleteDesktop("");
	YLChangeQuicklyIcom::DeleteStartMenu("");
	//添加
	YLChangeQuicklyIcom::AddToDesktop("");
	YLChangeQuicklyIcom::AddToStartMenu("");
	//在最后要刷新下桌面  要不会出现虽然删除了 但是桌面图标还存在的结果
	SHChangeNotify(SHCNE_ASSOCCHANGED, SHCNF_IDLIST, 0, 0);
	
	return 0;
}

