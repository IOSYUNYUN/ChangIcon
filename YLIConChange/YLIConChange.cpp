// YLIConChange.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "YLChangeQuicklyIcom.h"
#include <shlobj.h>
int _tmain(int argc, _TCHAR* argv[])
{
	//ɾ��
	YLChangeQuicklyIcom::DeleteDesktop("");
	YLChangeQuicklyIcom::DeleteStartMenu("");
	//���
	YLChangeQuicklyIcom::AddToDesktop("");
	YLChangeQuicklyIcom::AddToStartMenu("");
	//�����Ҫˢ��������  Ҫ���������Ȼɾ���� ��������ͼ�껹���ڵĽ��
	SHChangeNotify(SHCNE_ASSOCCHANGED, SHCNF_IDLIST, 0, 0);
	
	return 0;
}

