#include "service.h"
#include <string.h>
#include <stdlib.h>
#pragma warning(disable:4996)

//显示管理菜单
int ShowLinkmanManagerMenu(DTYPE *dOffset, DTYPE *dData)
{
	char input = 0;
	int nDataIndexCount = GetDataIndexCount(dOffset);

	Clear();

	puts("\tWellcome to Linkman Mannger System");
	puts("1.Add Linkman");
	puts("2.Dellete Linkman");
	puts("3.Change Linkman");
	puts("4.Find Linkman");
	puts("5.Totol Linkman");
	puts("6.Show Memory Infomation");
	puts("0.Exit");

	printf("\nLinkmanNum : %d\tPageNum : %d\tCurrentPage : %d\n", nDataIndexCount, PAGE_NUM, 1);
	puts("--------------------------------------------------------------");
	for (int i = 0; i < nDataIndexCount; i++)
	{
		TAGLINKMAN tempLinkman = { 0 };

		char gName[STRING_LENGTH] = { 0 };
		char gPhone[STRING_LENGTH] = { 0 };

		GetDataIndex(dOffset, i, &tempLinkman);

		GetDataBin(dData, tempLinkman.name.nOffset, tempLinkman.name.nLen, gName);
		
		GetDataBin(dData, tempLinkman.phone.nOffset, tempLinkman.phone.nLen, gPhone);

		printf("%d\t%-20s%s\r\n", i + 1, gName, gPhone);

	}
	puts("--------------------------------------------------------------");

	printf("\nPlease input option number:");

	input = getchar();
	getchar();

	return (int) (input - 0x30);
}

//清屏
void Clear()
{
	system("cls");
}

//添加联系人
void AddLinkman(DTYPE *dOffset, DTYPE *dData)
{
	char gName[STRING_LENGTH] = { 0 };
	char gPhone[STRING_LENGTH] = { 0 };
	TAGLINKMAN tempLinkman = { 0 };

	Clear();
	
	//姓名
	puts("Please input additional linkman name:");

	scanf(FORMAT_STRING(STRING_LENGTH), gName);
	getchar();

	//电话号码
	puts("Please input additional linkman Phone Number:");

	scanf(FORMAT_STRING(STRING_LENGTH), gPhone);
	getchar();

	int nNameLen = 0;
	int nPhoneLen = 0;
	int nOffset = 0;
	//写入名字长度
	nNameLen = strlen(gName) + 1;
	nPhoneLen = strlen(gPhone) + 1;

	//检查碎片区
	int nRecLen = GetRecoverableDataIndexCount(dOffset);

	for (int i = 0; i < nRecLen; i++)
	{
		GetRecoverableDataIndex(dOffset, i, &tempLinkman);
		//碎片区是否能存下联系人
		if ((tempLinkman.name.nLen + tempLinkman.phone.nLen) >= (nNameLen + nPhoneLen))
		{
			//填充名字长度
			if (device_write(dData, OFFSETSTART_DATA + tempLinkman.name.nOffset, gName, nNameLen) == false)
			{
				printf("AddLinkman write error!");
				getchar();
				return;
			}
			tempLinkman.name.nLen = nNameLen;

			//填充号码长度
			if (device_write(dData, OFFSETSTART_DATA + tempLinkman.phone.nOffset, gPhone, nPhoneLen) == false)
			{
				printf("AddLinkman write error!");
				getchar();
				return;
			}
			tempLinkman.phone.nLen = nPhoneLen;

			//添加到分配表
			AddDataIndex(dOffset, &tempLinkman, sizeof(tempLinkman));

			//删除碎片索引
			DeleteRecoverableByIndex(dOffset, i);

			//存完返回
			return;
		}
		
	}

	//添加
	nOffset = AddDataBin(dData, gName, nNameLen);
	if (-1 == nOffset)
	{
		printf("AddLinkman write error!");
		getchar();
		return;
	}
	tempLinkman.name.nOffset = nOffset;
	tempLinkman.name.nLen = nNameLen;

	//填充号码长度
	
	nOffset =  AddDataBin(dData, gPhone, nPhoneLen);
	if (-1 == nOffset)
	{
		printf("AddLinkman write error!");
		getchar();
		return;
	}
	tempLinkman.phone.nOffset = nOffset;
	tempLinkman.phone.nLen = nPhoneLen;

	//写入索引分配表进行管理
	AddDataIndex(dOffset, &tempLinkman, sizeof(tempLinkman));

}

/*
	删除联系人
*/
void DeleteLinkman(DTYPE *dOffset, DTYPE *dData)
{
	int index = 0;

	Clear();

	printf("Please input index :");
	scanf("%d", &index);

	int nOperationPoint = GetDataIndexCount(dOffset);

	puts("\n-------------------------------------------------------");
	if (index > nOperationPoint || index < 1)
	{
		puts("Invalid index!");
	}
	else
	{
		
		DeleteDataIndex(dOffset, index - 1);

		//打印信息
		printf("Delete suceess! index : %d\n", index);
	}

	puts("-------------------------------------------------------");

	getchar();
	getchar();
}

/*
	查找联系人
*/
void FindLinkman(DTYPE *dOffset, DTYPE *dData)
{
	char szBuffer[STRING_LENGTH] = { 0 };
	int option = 0;
	int nOperationPoint = GetDataIndexCount(dOffset);

	Clear();
	puts("-------------------------------------------------------");
	puts(" 1.Find linkman by name");
	puts(" 2.Find linkman by phone");
	puts(" 3.Find linkman by index");
	puts("-------------------------------------------------------");

	printf("\nPlease input number : ");
	scanf("%d", &option);
	getchar();

	switch (option)
	{
	case 1: //根据字符串查找
	{
		int tLen = 0;
		bool flag = false;

		puts("Please input find name :");
		scanf(FORMAT_STRING(STRING_LENGTH), szBuffer);
		getchar();

		tLen = strlen(szBuffer);

		puts("-------------------------------------------------------");
		for (int i = 0; i < nOperationPoint; i++)
		{

			TAGLINKMAN tagTemp = { 0 };
			GetDataIndex(dOffset, i, &tagTemp);

			char szName[STRING_LENGTH] = { 0 };
			char szPhone[STRING_LENGTH] = { 0 };

			GetDataBin(dData, tagTemp.name.nOffset, tagTemp.name.nLen, szName);
			GetDataBin(dData, tagTemp.phone.nOffset, tagTemp.phone.nLen, szPhone);

			for (size_t j = 0; j < strlen(szName) - tLen + 1; j++)
			{
				if (strncmp(szName + j, szBuffer, tLen) == 0)
				{
					printf("%d\t%-20s%s\r\n", i + 1, szName, szPhone);
					flag = true;
					break;
				}
			}

		}
		if (!flag)
		{
			puts("Not Found");
		}
		puts("-------------------------------------------------------");
	}
	getchar();
	break;
	case 2://根据号码查找
	{
		int tLen = 0;
		bool flag = false;

		puts("Please input find phone :");
		scanf(FORMAT_STRING(STRING_LENGTH), szBuffer);
		getchar();

		tLen = strlen(szBuffer);

		puts("-------------------------------------------------------");
		for (int i = 0; i < nOperationPoint; i++)
		{
			TAGLINKMAN tagTemp = { 0 };
			GetDataIndex(dOffset, i, &tagTemp);

			char szName[STRING_LENGTH] = { 0 };
			char szPhone[STRING_LENGTH] = { 0 };

			GetDataBin(dData, tagTemp.name.nOffset, tagTemp.name.nLen, szName);
			GetDataBin(dData, tagTemp.phone.nOffset, tagTemp.phone.nLen, szPhone);

			for (size_t j = 0; j < strlen(szPhone) - tLen + 1; j++)
			{
				if (strncmp(szPhone + j, szBuffer, tLen) == 0)
				{
					printf("%d\t%-20s%s\r\n", i + 1, szName, szPhone);
					flag = true;
					break;
				}
			}

		}
		if (!flag)
		{
			puts("Not Found");
		}
		puts("-------------------------------------------------------");
	}
	getchar();
	break;
	case 3://根据索引来查找
	{
		int index = 0;

		printf("Please input index number (MaxIndex: %d):", nOperationPoint);
		scanf("%d", &index);

		puts("-------------------------------------------------------");
		if (index >= nOperationPoint || index < 0)
		{
			puts("Invalid index!");
		}
		else
		{
			TAGLINKMAN tagTemp = { 0 };
			GetDataIndex(dOffset, index - 1, &tagTemp);

			char szName[STRING_LENGTH] = { 0 };
			char szPhone[STRING_LENGTH] = { 0 };

			GetDataBin(dData, tagTemp.name.nOffset, tagTemp.name.nLen, szName);
			GetDataBin(dData, tagTemp.phone.nOffset, tagTemp.phone.nLen, szPhone);

			printf("%d\t%-20s%s\r\n", index, szName, szPhone);
		}
		puts("-------------------------------------------------------");
	}
	getchar();
	getchar();
	break;
	default:
		puts("没有此选项!");
		getchar();
		getchar();
		break;
	}

}

//修改联系人
void ChangeLinkman(DTYPE *dOffset, DTYPE *dData)
{
	char szNewName[STRING_LENGTH] = { 0 };
	char szNewPhone[STRING_LENGTH] = { 0 };

	int index = 0;

	Clear();

	//输入修改的索引
	printf("Please input change index:");
	scanf("%d", &index);

	//打印出信息
	ShowLinkman(dOffset, dData, index);

	getchar();
	//输入修改后的字符串
	printf("Please input change name,input '-' is not Change : ");
	scanf(FORMAT_STRING(STRING_LENGTH), szNewName);

	printf("Please input change phone,input '-' is not Change : ");
	scanf(FORMAT_STRING(STRING_LENGTH), szNewPhone);

	int nNameLen = strlen(szNewName) + 1;
	int nPhoneLen = strlen(szNewPhone) + 1;

	int nOperationPoint = GetDataIndexCount(dOffset);

	puts("\n-------------------------------------------------------");
	if (index > nOperationPoint || index < 0)
	{
		puts("Invalid index!");
	}
	else
	{
		TAGLINKMAN tagTemp = { 0 };
		int nOffset = 0;
		GetDataIndex(dOffset, index - 1, &tagTemp);

		int nCountLen = 0;
		int nTempLen = tagTemp.name.nLen + tagTemp.phone.nLen;

		//名字
		if (strcmp(szNewName, "-") != 0)
		{
			nCountLen = nCountLen + nNameLen;
			tagTemp.name.nLen = nNameLen;
		}
		else
		{
			nCountLen = nCountLen + tagTemp.name.nLen;
			//读取名称
			GetDataBin(dData, tagTemp.name.nOffset, tagTemp.name.nLen, szNewName);
			
		}

		//电话
		if (strcmp(szNewPhone, "-") != 0)
		{
			nCountLen = nCountLen + nPhoneLen;
			tagTemp.phone.nLen = nPhoneLen;
		}
		else
		{
			nCountLen = nCountLen + tagTemp.phone.nLen;
			//读取号码
			GetDataBin(dData, tagTemp.phone.nOffset, tagTemp.phone.nLen, szNewPhone);
		}

		if (nCountLen > nTempLen)
		{
			//添加回收
			AddRecoverableDataIndex(dOffset, &tagTemp, sizeof(tagTemp));

			nOffset = AddDataBin(dData, szNewName, nNameLen);
			tagTemp.name.nOffset = nOffset;
			
			nOffset = AddDataBin(dData, szNewPhone, tagTemp.phone.nLen);
			tagTemp.phone.nOffset = nOffset;

		}
		else
		{
			//更新二进制
			UpdateDateBin(dData, OFFSETSTART_DATA + tagTemp.name.nOffset, szNewName, nNameLen);
			UpdateDateBin(dData, OFFSETSTART_DATA + tagTemp.phone.nOffset, szNewPhone, nPhoneLen);
		}

		//更新索引
		UpdateDateIndex(dOffset, index - 1, &tagTemp, sizeof(TAGLINKMAN));

		//打印信息
		printf("Change suceess! index : %d\n", index);
	}
	puts("-------------------------------------------------------");

	getchar();
	getchar();
	return;
}

//显示联系人信息
void ShowLinkman(DTYPE *dOffset, DTYPE *dData, int index)
{
	int nOperationPoint = GetDataIndexCount(dOffset);

	puts("\n-------------------------------------------------------");
	if (index > nOperationPoint || index < 0)
	{
		puts("Invalid index!");
	}
	else
	{
		TAGLINKMAN tagTemp = { 0 };

		GetDataIndex(dOffset, index - 1, &tagTemp);

		char szName[STRING_LENGTH] = { 0 };
		char szPhone[STRING_LENGTH] = { 0 };

		GetDataBin(dData, tagTemp.name.nOffset, tagTemp.name.nLen, szName);
		GetDataBin(dData, tagTemp.phone.nOffset, tagTemp.phone.nLen, szPhone);

		printf("%d\t%-20s%s\r\n", index, szName, szPhone);
	}

	puts("-------------------------------------------------------\n");
	
}

/*
	统计信息
*/
void TotolLinkman(DTYPE *dOffset, DTYPE *dData)
{
	int nCount = GetDataIndexCount(dOffset);

	TOTOLINFO aryTotolInfo[40] = { 0 };
	int nTotolLen = 0;

	Clear();
	puts("\tLetterTotol");
	puts("-------------------------------------------------------\n");
	for (int i = 0; i < nCount; i++)
	{
		TAGLINKMAN tagTemp = { 0 };

		GetDataIndex(dOffset, i, &tagTemp);

		char szName[ STRING_LENGTH ] = { 0 };

		GetDataBin(dData, tagTemp.name.nOffset, tagTemp.name.nLen, szName);

		//统计名字里的字符
		for (int j = 0; j < tagTemp.name.nLen; j++)
		{
			int nIndex = isAryTotol(aryTotolInfo, nTotolLen, szName[j]);

			if (nIndex == -1)
			{
				aryTotolInfo[nTotolLen].chLetter = szName[j];
				aryTotolInfo[nTotolLen].count++;
				nTotolLen ++;
			}
			else
			{
				aryTotolInfo[nIndex].count++;
			}
		}
		
	}

	for (int j = 0; j < nTotolLen; j++)
	{
		printf("'%c' == %d\r\n", aryTotolInfo[j].chLetter, aryTotolInfo[j].count);
	}
	puts("\n-------------------------------------------------------\n");

	printf("input enter exit!");

	getchar();
}

/*
	
*/
int isAryTotol(PTOTOLINFO pAry, int nLen, char ch)
{
	for (int i = 0; i < nLen; i++)
	{
		if (pAry[ i ].chLetter == ch)
		{
			return i;
		}
	}

	return -1;
}

/*
	显示内存信息
*/
void ShowMemoryInfo(DTYPE *dOffset, DTYPE *dData)
{
	char szBuf[100] = { 0 };

	int size = GetDataBinSize(dData);

	int end = size + (10 - (size % 10));

	device_read(dData, OFFSETSTART_DATA, szBuf, 100);

	Clear();

	printf("----------------------------- MEMORY INFO -----------------------------");
	for (int i = 0;i < end && size != 0; i++)
	{
		if (i == 0)
		{
			printf("\n[0x%08X - 0x%08X]  ", i, i + 10);
		}

		if (IsRecoverable(dOffset, i) == true)
		{
			printf("%s ", "F");
		}
		else
		{
			if (szBuf[i] != -1)
			{
				printf("%s ", "U");
			}
			else
			{
				printf("%s ", "F");
			}
			
		}

		if ((i + 1) % 10 == 0 && i != 0)
		{
			printf("  ");

			for (int j = 0; j < 10; j++)
			{
				char ch = szBuf[(i - 9) + j];
				printf("%c ", ch == -1 ? '.' : ch);
			}

			if (i + 1 != end)
			{
				printf("\n[0x%08X - 0x%08X]  ", i, i + 10);
			}
		}

	}
	printf("\n-----------------------------------------------------------------------\n");

	printf("是否碎片整理? (y) : ");

	char ch = getchar();

	if (ch == 'y')
	{
		Debris(dOffset, dData);
	}
}

/*
	碎片整理
*/
void Debris(DTYPE *dOffset, DTYPE *dData)
{
	//计数
	int nCount = GetDataIndexCount(dOffset);

	TAGLINKMAN tagTemp1 = { 0 };
	TAGLINKMAN tagTemp0 = { 0 };

	//先排序
	for (int i = 0; i < nCount; i++)
	{
		for (int j = 1; j < nCount - i; j++)
		{

			GetDataIndex(dOffset, j, &tagTemp1);
			GetDataIndex(dOffset, j - 1, &tagTemp0);

			if (tagTemp0.name.nOffset > tagTemp1.name.nOffset)
			{
				device_write(dOffset, OFFSETHEAD_INDEX + sizeof(int) + j * sizeof(tagTemp0), &tagTemp0, sizeof(tagTemp0));
				device_write(dOffset, OFFSETHEAD_INDEX + sizeof(int) + (j - 1) * sizeof(tagTemp1), &tagTemp1, sizeof(tagTemp1));
			}
		}
	}

	//从后往前整理碎片
	//先排序
	for (int i = 0; i < nCount; i++)
	{
		if (i == 0) //头
		{
			GetDataIndex(dOffset, i, &tagTemp1);
			if (tagTemp1.name.nOffset > OFFSETTAIL_DATA)
			{
				//计算数据大小
				int nSize = tagTemp1.name.nLen + tagTemp1.phone.nLen;

				//分配交换空间
				char *swapBuf = (char *) malloc(nSize);
				if (swapBuf == NULL)
				{
					puts("Debris fail!");
					getchar();
					return;
				}

				//开始交换
				if (device_read(dData, OFFSETSTART_DATA + tagTemp1.name.nOffset, swapBuf, nSize) == true)
				{
					if (device_write(dData, OFFSETSTART_DATA, swapBuf, nSize) == true)
					{
						//提交物理写盘
						fflush(dData);
						//修复分配表
						tagTemp1.name.nOffset = OFFSETTAIL_DATA;
						tagTemp1.phone.nOffset = tagTemp1.name.nOffset + tagTemp1.name.nLen;
						UpdateDateIndex(dOffset, i, &tagTemp1, sizeof(tagTemp1));
					}
				}

				if (swapBuf != NULL)
				{
					free(swapBuf);
					swapBuf = NULL;
				}
			}
		}
		else
		{
			GetDataIndex(dOffset, i, &tagTemp1);
			GetDataIndex(dOffset, i - 1, &tagTemp0);

			int nOffset0 = tagTemp0.name.nOffset + tagTemp0.name.nLen + tagTemp0.phone.nLen;

			//计算空隙
			int nInterspace = tagTemp1.name.nOffset - nOffset0;

			if (nInterspace > 0)
			{
				//计算数据大小
				int nSize = tagTemp1.name.nLen + tagTemp1.phone.nLen;

				//分配交换空间
				char *swapBuf = (char *)malloc(nSize);
				if (swapBuf == NULL)
				{
					puts("Debris fail!");
					getchar();
					return;
				}

				//开始交换
				if (device_read(dData, OFFSETSTART_DATA + tagTemp1.name.nOffset, swapBuf, nSize) == true)
				{
					if (device_write(dData, OFFSETSTART_DATA + nOffset0, swapBuf, nSize) == true)
					{
						//提交物理写盘
						fflush(dData);
						//修复分配表
						tagTemp1.name.nOffset = nOffset0;
						tagTemp1.phone.nOffset = tagTemp1.name.nOffset + tagTemp1.name.nLen;
						UpdateDateIndex(dOffset, i, &tagTemp1, sizeof(tagTemp1));
					}
				}

				if (swapBuf != NULL)
				{
					free(swapBuf);
					swapBuf = NULL;
				}
			}
		}
	}

	//修复尾巴
	int nOffsetTail = OFFSETSTART_DATA + tagTemp1.name.nOffset + tagTemp1.name.nLen + tagTemp1.phone.nLen;
	int nSize = GetDataBinSize(dData) + OFFSETSTART_DATA;

	fseek(dData, nOffsetTail, SEEK_SET);
	for (int i = 0; i < nSize - nOffsetTail; i++)
	{
		fputc(-1, dData);
	}

	nOffsetTail = nOffsetTail - 4;
	device_write(dData, OFFSETTAIL_DATA, &nOffsetTail, sizeof(int));

	//清除回收站
	int n = 0;
	device_write(dOffset, OFFSETHEAD_RECOVERABLE, &n, sizeof(n));
	fflush(dData);
	fflush(dOffset);
	puts("Debris success!");

	getchar();
	getchar();
	return;
}