
#define KEY_PATHS_DESKTOP L"Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Shell Folders"
#define KEY_PATHS_START_MENU L"Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Shell Folders"



//��Ҫɾ���Ŀ��ͼ�������
#define CUSTOMARY_APP_NAME "ChangIcon"
/*
*��Ҫɾ���Ŀ��ͼ������ڵķ��� 
*/
//#define CUSTOMARY_APP_GROUP "GoCom"
////��Ҫɾ���Ŀ��ͼ�������
#define  NOW_APP_NAME "ChangIcon"
//��Ҫ��ӵĿ��ͼ������ڵķ���  ���û������ļ��Ļ����Դ���  ���ҵĴ�����û�д���  ��Ϊ���û���������
//#define  NOW_APP_GROUP "GoCom"
//���ڳ�������ɵ�
#define CUSTOMARY_NAME  "ChangIcon.exe"
#define RUN_APP_NAME   "ChangIcon.exe"


#include "targetver.h"
#include <stdio.h>
#include <tchar.h>
#include <iostream>
#include <afx.h>
#include <shellapi.h>

typedef enum ShortcutsType
{
	ShortcutsTypeStartMenu,
	ShortcutsTypeDesktop
}ShortcutsType;
#pragma once
class YLChangeQuicklyIcom
{
public:
	YLChangeQuicklyIcom();
	~YLChangeQuicklyIcom();
public:
	//��ʼ�˵��м����ݷ�ʽ
	static void AddToStartMenu(std::string shortcutDisplay);

	static void AddToDesktop(std::string shortcutDisplay);

	static void DeleteStartMenu(std::string shortcutDisplay);

	static void DeleteDesktop(std::string shortcutDisplay);
};

