#pragma once
#include "hal.h"

//信息结构 实现变长
typedef struct _TAGITEM
{
  int nLen;
  int nOffset;
} TAGITEM, *PTAGITEM;

//联系人结构
typedef struct _TAGLINKMAN
{
  TAGITEM name;
  TAGITEM phone;
} TAGLINKMAN, *PTAGLINKMAN;

//统计结构
typedef struct _TOTOLINFO
{
  char chLetter;
  int count;
} TOTOLINFO, *PTOTOLINFO;

/*
添加联系人数据指针索引
*/
bool AddDataIndex(DTYPE *fp, void *data, size_t size);

/*
添加可回收垃圾数据指针索引
*/
bool AddRecoverableDataIndex(DTYPE *fp, void *data, size_t size);

/*
获取索引数量
*/
int GetDataIndexCount(DTYPE *fp);

/*
获取索引数据项
*/
int GetDataBinSize(DTYPE *fp);

/*
获取可回收垃圾索引数量
*/
int GetRecoverableDataIndexCount(DTYPE *fp);

/*
是否回收
*/
bool IsRecoverable(DTYPE *fp, int nOffset);

/*
获取索引数据项
*/
bool GetDataIndex(DTYPE *fp, int index, PTAGLINKMAN data);

/*
删除分配表索引数据项
*/
bool DeleteDataIndex(DTYPE *fp, int index);

//更新分配表信息
bool UpdateDateIndex(DTYPE *fp, int index, void *buf, size_t size);

//更新分配表信息
bool UpdateDateBin(DTYPE *fp, int pos, void *buf, size_t size);

/*
获取可回收垃圾索引数据项
*/
bool GetRecoverableDataIndex(DTYPE *fp, int index, PTAGLINKMAN data);

bool DeleteRecoverableByIndex(DTYPE *fp, int index);

/*
添加联系人二进制数据到磁盘
返回磁盘中的offset
*/
int AddDataBin(DTYPE *fp, char *gName, size_t nLen);

/*
读取二进制数据
*/
bool GetDataBin(DTYPE *fp, int nOffset, size_t size, void *data);

/*
初始化设备
*/
bool InitDevice(DTYPE **dOffset, DTYPE **dData);

/*
关闭设备
*/
void CloseDevice(DTYPE *dOffset, DTYPE *dData);