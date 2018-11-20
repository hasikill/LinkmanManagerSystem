#include <stdio.h>
#include <stdlib.h>
#include "hal.h"
#include "interface.h"
#include "service.h"

int main()
{
	//索引文件
	DTYPE *deviceOffset = NULL;

	//数据文件
	DTYPE *deviceData = NULL;

	bool bIsInit = InitDevice(&deviceOffset, &deviceData);
	if (bIsInit == false)
	{
		goto ExitProc;
	}

	int option = 0;
	do
	{
		//显示管理菜单
		option = ShowLinkmanManagerMenu(deviceOffset, deviceData);

		switch (option)
		{
		case 1://增加
			AddLinkman(deviceOffset, deviceData);
			break;
		case 2://删除
			DeleteLinkman(deviceOffset, deviceData);
			break;
		case 3://修改
			ChangeLinkman(deviceOffset, deviceData);
			break;
		case 4://查询
			FindLinkman(deviceOffset, deviceData);
			break;
		case 5://统计
			TotolLinkman(deviceOffset, deviceData);
			break;
		case 6: //存储信息
			ShowMemoryInfo(deviceOffset, deviceData);
			break;
		case 0://退出
			break;

		default:
			Clear();
			printf("输入错误,请重新输入!!");
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
