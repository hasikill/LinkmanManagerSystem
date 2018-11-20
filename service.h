#pragma once
#include "hal.h"
#include "interface.h"

//每页显示数量
#define PAGE_NUM 15

#define _FORMAT_STRING(X) "%" #X "s"
#define FORMAT_STRING(x) _FORMAT_STRING(x)
#define STRING_LENGTH 40

//显示管理菜单
int ShowLinkmanManagerMenu(DTYPE *dOffset, DTYPE *dData);

//清屏
void Clear();

//添加联系人
void AddLinkman(DTYPE *dOffset, DTYPE *dData);

//删除联系人
void DeleteLinkman(DTYPE *dOffset, DTYPE *dData);

//查找联系人
void FindLinkman(DTYPE *dOffset, DTYPE *dData);

//修改联系人
void ChangeLinkman(DTYPE *dOffset, DTYPE *dData);

//统计信息
void TotolLinkman(DTYPE *dOffset, DTYPE *dData);

//存储信息
void ShowMemoryInfo(DTYPE *dOffset, DTYPE *dData);

//显示联系人
void ShowLinkman(DTYPE *dOffset, DTYPE *dData, int index);

/*========================================================工具*/
/*
*/
int isAryTotol(PTOTOLINFO pAry, int nLen, char ch);

/*
	碎片整理
*/
void Debris(DTYPE *dOffset, DTYPE *dData);