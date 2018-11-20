#pragma once
#include "hal.h"
#include "interface.h"

//ÿҳ��ʾ����
#define PAGE_NUM 15

#define _FORMAT_STRING(X) "%" #X "s"
#define FORMAT_STRING(x) _FORMAT_STRING(x)
#define STRING_LENGTH 40

//��ʾ����˵�
int ShowLinkmanManagerMenu(DTYPE *dOffset, DTYPE *dData);

//����
void Clear();

//�����ϵ��
void AddLinkman(DTYPE *dOffset, DTYPE *dData);

//ɾ����ϵ��
void DeleteLinkman(DTYPE *dOffset, DTYPE *dData);

//������ϵ��
void FindLinkman(DTYPE *dOffset, DTYPE *dData);

//�޸���ϵ��
void ChangeLinkman(DTYPE *dOffset, DTYPE *dData);

//ͳ����Ϣ
void TotolLinkman(DTYPE *dOffset, DTYPE *dData);

//�洢��Ϣ
void ShowMemoryInfo(DTYPE *dOffset, DTYPE *dData);

//��ʾ��ϵ��
void ShowLinkman(DTYPE *dOffset, DTYPE *dData, int index);

/*========================================================����*/
/*
*/
int isAryTotol(PTOTOLINFO pAry, int nLen, char ch);

/*
	��Ƭ����
*/
void Debris(DTYPE *dOffset, DTYPE *dData);