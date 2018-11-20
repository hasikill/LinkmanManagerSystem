#include <stdio.h>
#include <stdlib.h>
#include "hal.h"
#include "interface.h"
#include "service.h"

int main()
{
	//�����ļ�
	DTYPE *deviceOffset = NULL;

	//�����ļ�
	DTYPE *deviceData = NULL;

	bool bIsInit = InitDevice(&deviceOffset, &deviceData);
	if (bIsInit == false)
	{
		goto ExitProc;
	}

	int option = 0;
	do
	{
		//��ʾ����˵�
		option = ShowLinkmanManagerMenu(deviceOffset, deviceData);

		switch (option)
		{
		case 1://����
			AddLinkman(deviceOffset, deviceData);
			break;
		case 2://ɾ��
			DeleteLinkman(deviceOffset, deviceData);
			break;
		case 3://�޸�
			ChangeLinkman(deviceOffset, deviceData);
			break;
		case 4://��ѯ
			FindLinkman(deviceOffset, deviceData);
			break;
		case 5://ͳ��
			TotolLinkman(deviceOffset, deviceData);
			break;
		case 6: //�洢��Ϣ
			ShowMemoryInfo(deviceOffset, deviceData);
			break;
		case 0://�˳�
			break;

		default:
			Clear();
			printf("�������,����������!!");
			getchar();
			getchar();
			break;
		}


	} while (option != 0);

ExitProc:
	CloseDevice(deviceOffset, deviceData);
	system("pause");
	return 0;
}
