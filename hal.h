#pragma once
#include <stdio.h>

//设备名
#define DEVICE_NAME				"./data.bin"

//偏移数据设备名
#define OFFSET_DEVICE_NAME		"./offsetdata.bin"

//数据文件
#define DATAFILESIZE			(1024 * 5120)

//分配表文件
#define CONFIGFILESIZE			((DATAFILESIZE * 4) * 2)

//分配表数据索引首偏移
#define OFFSETHEAD_INDEX		0

//分配表垃圾回收数据偏移
#define OFFSETHEAD_RECOVERABLE	(CONFIGFILESIZE / 2)

//数据文件操作偏移指针
#define OFFSETTAIL_DATA			0

//数据文件数据存储区起始偏移
#define OFFSETSTART_DATA		4

typedef enum { false, true } bool;

typedef struct _DEVICE_INFO
{
	int size;
} DEVICE_INFO,*PDEVICE_INFO;

//文件指针类型
typedef	FILE	DTYPE;

/*
硬件抽象层
文件设备写操作
*/
bool device_write(DTYPE *fp, int pos, void *buffer, size_t size);

/*
硬件抽象层
文件设备读操作
*/
bool device_read(DTYPE *fp, int pos, void *buffer, size_t size);

/*
硬件抽象层
打开设备
*/
DTYPE* device_open(char *device_name);

/*
创建偏移数据文件设备
*/
bool create_offset_bin();

/*
创建数据文件
*/
bool create_data_bin();

/*
硬件抽象层
关闭设备
*/
void device_close(DTYPE *fp);