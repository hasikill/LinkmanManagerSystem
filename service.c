#include "service.h"
#include <string.h>
#include <stdlib.h>
#pragma warning(disable:4996)

//��ʾ����˵�
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

//����
void Clear()
{
	system("cls");
}

//�����ϵ��
void AddLinkman(DTYPE *dOffset, DTYPE *dData)
{
	char gName[STRING_LENGTH] = { 0 };
	char gPhone[STRING_LENGTH] = { 0 };
	TAGLINKMAN tempLinkman = { 0 };

	Clear();
	
	//����
	puts("Please input additional linkman name:");

	scanf(FORMAT_STRING(STRING_LENGTH), gName);
	getchar();

	//�绰����
	puts("Please input additional linkman Phone Number:");

	scanf(FORMAT_STRING(STRING_LENGTH), gPhone);
	getchar();

	int nNameLen = 0;
	int nPhoneLen = 0;
	int nOffset = 0;
	//д�����ֳ���
	nNameLen = strlen(gName) + 1;
	nPhoneLen = strlen(gPhone) + 1;

	//�����Ƭ��
	int nRecLen = GetRecoverableDataIndexCount(dOffset);

	for (int i = 0; i < nRecLen; i++)
	{
		GetRecoverableDataIndex(dOffset, i, &tempLinkman);
		//��Ƭ���Ƿ��ܴ�����ϵ��
		if ((tempLinkman.name.nLen + tempLinkman.phone.nLen) >= (nNameLen + nPhoneLen))
		{
			//������ֳ���
			if (device_write(dData, OFFSETSTART_DATA + tempLinkman.name.nOffset, gName, nNameLen) == false)
			{
				printf("AddLinkman write error!");
				getchar();
				return;
			}
			tempLinkman.name.nLen = nNameLen;

			//�����볤��
			if (device_write(dData, OFFSETSTART_DATA + tempLinkman.phone.nOffset, gPhone, nPhoneLen) == false)
			{
				printf("AddLinkman write error!");
				getchar();
				return;
			}
			tempLinkman.phone.nLen = nPhoneLen;

			//��ӵ������
			AddDataIndex(dOffset, &tempLinkman, sizeof(tempLinkman));

			//ɾ����Ƭ����
			DeleteRecoverableByIndex(dOffset, i);

			//���귵��
			return;
		}
		
	}

	//���
	nOffset = AddDataBin(dData, gName, nNameLen);
	if (-1 == nOffset)
	{
		printf("AddLinkman write error!");
		getchar();
		return;
	}
	tempLinkman.name.nOffset = nOffset;
	tempLinkman.name.nLen = nNameLen;

	//�����볤��
	
	nOffset =  AddDataBin(dData, gPhone, nPhoneLen);
	if (-1 == nOffset)
	{
		printf("AddLinkman write error!");
		getchar();
		return;
	}
	tempLinkman.phone.nOffset = nOffset;
	tempLinkman.phone.nLen = nPhoneLen;

	//д�������������й���
	AddDataIndex(dOffset, &tempLinkman, sizeof(tempLinkman));

}

/*
	ɾ����ϵ��
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

		//��ӡ��Ϣ
		printf("Delete suceess! index : %d\n", index);
	}

	puts("-------------------------------------------------------");

	getchar();
	getchar();
}

/*
	������ϵ��
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
	case 1: //�����ַ�������
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
	case 2://���ݺ������
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
	case 3://��������������
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
		puts("û�д�ѡ��!");
		getchar();
		getchar();
		break;
	}

}

//�޸���ϵ��
void ChangeLinkman(DTYPE *dOffset, DTYPE *dData)
{
	char szNewName[STRING_LENGTH] = { 0 };
	char szNewPhone[STRING_LENGTH] = { 0 };

	int index = 0;

	Clear();

	//�����޸ĵ�����
	printf("Please input change index:");
	scanf("%d", &index);

	//��ӡ����Ϣ
	ShowLinkman(dOffset, dData, index);

	getchar();
	//�����޸ĺ���ַ���
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

		//����
		if (strcmp(szNewName, "-") != 0)
		{
			nCountLen = nCountLen + nNameLen;
			tagTemp.name.nLen = nNameLen;
		}
		else
		{
			nCountLen = nCountLen + tagTemp.name.nLen;
			//��ȡ����
			GetDataBin(dData, tagTemp.name.nOffset, tagTemp.name.nLen, szNewName);
			
		}

		//�绰
		if (strcmp(szNewPhone, "-") != 0)
		{
			nCountLen = nCountLen + nPhoneLen;
			tagTemp.phone.nLen = nPhoneLen;
		}
		else
		{
			nCountLen = nCountLen + tagTemp.phone.nLen;
			//��ȡ����
			GetDataBin(dData, tagTemp.phone.nOffset, tagTemp.phone.nLen, szNewPhone);
		}

		if (nCountLen > nTempLen)
		{
			//��ӻ���
			AddRecoverableDataIndex(dOffset, &tagTemp, sizeof(tagTemp));

			nOffset = AddDataBin(dData, szNewName, nNameLen);
			tagTemp.name.nOffset = nOffset;
			
			nOffset = AddDataBin(dData, szNewPhone, tagTemp.phone.nLen);
			tagTemp.phone.nOffset = nOffset;

		}
		else
		{
			//���¶�����
			UpdateDateBin(dData, OFFSETSTART_DATA + tagTemp.name.nOffset, szNewName, nNameLen);
			UpdateDateBin(dData, OFFSETSTART_DATA + tagTemp.phone.nOffset, szNewPhone, nPhoneLen);
		}

		//��������
		UpdateDateIndex(dOffset, index - 1, &tagTemp, sizeof(TAGLINKMAN));

		//��ӡ��Ϣ
		printf("Change suceess! index : %d\n", index);
	}
	puts("-------------------------------------------------------");

	getchar();
	getchar();
	return;
}

//��ʾ��ϵ����Ϣ
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
	ͳ����Ϣ
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

		//ͳ����������ַ�
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
	��ʾ�ڴ���Ϣ
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

	printf("�Ƿ���Ƭ����? (y) : ");

	char ch = getchar();

	if (ch == 'y')
	{
		Debris(dOffset, dData);
	}
}

/*
	��Ƭ����
*/
void Debris(DTYPE *dOffset, DTYPE *dData)
{
	//����
	int nCount = GetDataIndexCount(dOffset);

	TAGLINKMAN tagTemp1 = { 0 };
	TAGLINKMAN tagTemp0 = { 0 };

	//������
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

	//�Ӻ���ǰ������Ƭ
	//������
	for (int i = 0; i < nCount; i++)
	{
		if (i == 0) //ͷ
		{
			GetDataIndex(dOffset, i, &tagTemp1);
			if (tagTemp1.name.nOffset > OFFSETTAIL_DATA)
			{
				//�������ݴ�С
				int nSize = tagTemp1.name.nLen + tagTemp1.phone.nLen;

				//���佻���ռ�
				char *swapBuf = (char *) malloc(nSize);
				if (swapBuf == NULL)
				{
					puts("Debris fail!");
					getchar();
					return;
				}

				//��ʼ����
				if (device_read(dData, OFFSETSTART_DATA + tagTemp1.name.nOffset, swapBuf, nSize) == true)
				{
					if (device_write(dData, OFFSETSTART_DATA, swapBuf, nSize) == true)
					{
						//�ύ����д��
						fflush(dData);
						//�޸������
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

			//�����϶
			int nInterspace = tagTemp1.name.nOffset - nOffset0;

			if (nInterspace > 0)
			{
				//�������ݴ�С
				int nSize = tagTemp1.name.nLen + tagTemp1.phone.nLen;

				//���佻���ռ�
				char *swapBuf = (char *)malloc(nSize);
				if (swapBuf == NULL)
				{
					puts("Debris fail!");
					getchar();
					return;
				}

				//��ʼ����
				if (device_read(dData, OFFSETSTART_DATA + tagTemp1.name.nOffset, swapBuf, nSize) == true)
				{
					if (device_write(dData, OFFSETSTART_DATA + nOffset0, swapBuf, nSize) == true)
					{
						//�ύ����д��
						fflush(dData);
						//�޸������
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

	//�޸�β��
	int nOffsetTail = OFFSETSTART_DATA + tagTemp1.name.nOffset + tagTemp1.name.nLen + tagTemp1.phone.nLen;
	int nSize = GetDataBinSize(dData) + OFFSETSTART_DATA;

	fseek(dData, nOffsetTail, SEEK_SET);
	for (int i = 0; i < nSize - nOffsetTail; i++)
	{
		fputc(-1, dData);
	}

	nOffsetTail = nOffsetTail - 4;
	device_write(dData, OFFSETTAIL_DATA, &nOffsetTail, sizeof(int));

	//�������վ
	int n = 0;
	device_write(dOffset, OFFSETHEAD_RECOVERABLE, &n, sizeof(n));
	fflush(dData);
	fflush(dOffset);
	puts("Debris success!");

	getchar();
	getchar();
	return;
}