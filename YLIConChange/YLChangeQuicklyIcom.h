
#define KEY_PATHS_DESKTOP L"Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Shell Folders"
#define KEY_PATHS_START_MENU L"Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Shell Folders"



//需要删除的快捷图标的名字
#define CUSTOMARY_APP_NAME "ChangIcon"
/*
*需要删除的快捷图标的所在的分组 
*/
//#define CUSTOMARY_APP_GROUP "GoCom"
////需要删除的快捷图标的名字
#define  NOW_APP_NAME "ChangIcon"
//需要添加的快捷图标的所在的分组  如果没有这个文件的话可以创建  在我的代码中没有创建  因为这边没有这个需求
//#define  NOW_APP_GROUP "GoCom"
//现在程序的生成的
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
	//向开始菜单中加入快捷方式
	static void AddToStartMenu(std::string shortcutDisplay);

	static void AddToDesktop(std::string shortcutDisplay);

	static void DeleteStartMenu(std::string shortcutDisplay);

	static void DeleteDesktop(std::string shortcutDisplay);
};

