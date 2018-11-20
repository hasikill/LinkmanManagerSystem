#include "hal.h"
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#pragma warning(disable:4996)

DTYPE* device_open(char *device_name)
{
	DTYPE *fp = fopen(device_name, "rb+");
	return fp;
}

/*
	硬件抽象层
	关闭设备
*/
void device_close(DTYPE *fp)
{
	fclose(fp);
}

/*
硬件抽象层
文件读操作
*/
bool device_read(DTYPE *fp, int pos, void *buffer, size_t size)
{
	int nPos = fseek(fp, pos, SEEK_SET);
	if (nPos == -1)
	{
		return false;
	}

	size_t sSize = fread(buffer, size, 1, fp);
	if (sSize == 0)
	{
		if (feof(fp) != 0)
		{
			puts("Error : >> 已到文件末尾 <<");
		}
		return false;
	}
	return true;
}

/*
硬件抽象层
文件写操作
*/
bool device_write(DTYPE *fp, int pos, void *buffer, size_t size)
{
	int nPos = fseek(fp, pos, SEEK_SET);
	if (nPos == -1)
	{
		return false;
	}

	if (size == 0)
	{
		return true;
	}

	size_t sSize = fwrite(buffer, size, 1, fp);
	if (sSize == 0)
	{
		printf("write error,error code : %d\r\n", ferror(fp));
		if (feof(fp) != 0)
		{
			puts("Error : >> 已到文件末尾 <<");
		}
		return false;
	}
	return true;
}

//读设备信息
int _get_info(PDEVICE_INFO info)
{
	struct stat st;

	int ret = stat(DEVICE_NAME, &st);

	info->size = st.st_size;

	return 0;
}

/*
创建偏移数据文件设备
*/
bool create_offset_bin()
{
	FILE *fp = fopen(OFFSET_DEVICE_NAME, "wb+");
	if (fp == NULL)
	{
		return false;
	}
	puts("start write ...");
	for (int i = 0; i < CONFIGFILESIZE; i++)
	{
		fputc(0, fp);
	}
	puts("write end!");

	if (NULL != fp)
	{
		fclose(fp);
	}

	return true;
}

/*
创建数据文件
*/
bool create_data_bin()
{
	FILE *fp = fopen(DEVICE_NAME, "wb+");
	if (fp == NULL)
	{
		return false;
	}

	puts("start write ...");

	for (int i = 0; i < DATAFILESIZE; i++)
	{
		fputc(-1, fp);
	}

	int nT = 0;

	fseek(fp, 0, SEEK_SET);
	if (fwrite(&nT, sizeof(nT), 1, fp) == 0)
	{
		return false;
	}

	puts("write end!");

	if (NULL != fp)
	{
		fclose(fp);
	}

	return true;
}