#pragma once
#include <stdio.h>

//�豸��
#define DEVICE_NAME				"./data.bin"

//ƫ�������豸��
#define OFFSET_DEVICE_NAME		"./offsetdata.bin"

//�����ļ�
#define DATAFILESIZE			(1024 * 5120)

//������ļ�
#define CONFIGFILESIZE			((DATAFILESIZE * 4) * 2)

//���������������ƫ��
#define OFFSETHEAD_INDEX		0

//�����������������ƫ��
#define OFFSETHEAD_RECOVERABLE	(CONFIGFILESIZE / 2)

//�����ļ�����ƫ��ָ��
#define OFFSETTAIL_DATA			0

//�����ļ����ݴ洢����ʼƫ��
#define OFFSETSTART_DATA		4

typedef enum { false, true } bool;

typedef struct _DEVICE_INFO
{
	int size;
} DEVICE_INFO,*PDEVICE_INFO;

//�ļ�ָ������
typedef	FILE	DTYPE;

/*
Ӳ�������
�ļ��豸д����
*/
bool device_write(DTYPE *fp, int pos, void *buffer, size_t size);

/*
Ӳ�������
�ļ��豸������
*/
bool device_read(DTYPE *fp, int pos, void *buffer, size_t size);

/*
Ӳ�������
���豸
*/
DTYPE* device_open(char *device_name);

/*
����ƫ�������ļ��豸
*/
bool create_offset_bin();

/*
���������ļ�
*/
bool create_data_bin();

/*
Ӳ�������
�ر��豸
*/
void device_close(DTYPE *fp);