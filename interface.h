#pragma once
#include "hal.h"

//��Ϣ�ṹ ʵ�ֱ䳤
typedef struct _TAGITEM
{
  int nLen;
  int nOffset;
} TAGITEM, *PTAGITEM;

//��ϵ�˽ṹ
typedef struct _TAGLINKMAN
{
  TAGITEM name;
  TAGITEM phone;
} TAGLINKMAN, *PTAGLINKMAN;

//ͳ�ƽṹ
typedef struct _TOTOLINFO
{
  char chLetter;
  int count;
} TOTOLINFO, *PTOTOLINFO;

/*
�����ϵ������ָ������
*/
bool AddDataIndex(DTYPE *fp, void *data, size_t size);

/*
��ӿɻ�����������ָ������
*/
bool AddRecoverableDataIndex(DTYPE *fp, void *data, size_t size);

/*
��ȡ��������
*/
int GetDataIndexCount(DTYPE *fp);

/*
��ȡ����������
*/
int GetDataBinSize(DTYPE *fp);

/*
��ȡ�ɻ���������������
*/
int GetRecoverableDataIndexCount(DTYPE *fp);

/*
�Ƿ����
*/
bool IsRecoverable(DTYPE *fp, int nOffset);

/*
��ȡ����������
*/
bool GetDataIndex(DTYPE *fp, int index, PTAGLINKMAN data);

/*
ɾ�����������������
*/
bool DeleteDataIndex(DTYPE *fp, int index);

//���·������Ϣ
bool UpdateDateIndex(DTYPE *fp, int index, void *buf, size_t size);

//���·������Ϣ
bool UpdateDateBin(DTYPE *fp, int pos, void *buf, size_t size);

/*
��ȡ�ɻ�����������������
*/
bool GetRecoverableDataIndex(DTYPE *fp, int index, PTAGLINKMAN data);

bool DeleteRecoverableByIndex(DTYPE *fp, int index);

/*
�����ϵ�˶��������ݵ�����
���ش����е�offset
*/
int AddDataBin(DTYPE *fp, char *gName, size_t nLen);

/*
��ȡ����������
*/
bool GetDataBin(DTYPE *fp, int nOffset, size_t size, void *data);

/*
��ʼ���豸
*/
bool InitDevice(DTYPE **dOffset, DTYPE **dData);

/*
�ر��豸
*/
void CloseDevice(DTYPE *dOffset, DTYPE *dData);