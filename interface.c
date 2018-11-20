#include "interface.h"
#include <stdlib.h>

/*
添加联系人数据指针索引
*/
bool AddDataIndex(DTYPE *fp, void *data, size_t size)
{
  int nDataLength = 0;
  bool isTrue = false;

  isTrue = device_read(fp, OFFSETHEAD_INDEX, &nDataLength, sizeof(int));
  if (false == isTrue)
  {
    return false;
  }

  isTrue = device_write(fp, OFFSETHEAD_INDEX + sizeof(int) + nDataLength * size, data, size);
  if (false == isTrue)
  {
    return false;
  }

  nDataLength = nDataLength + 1;

  isTrue = device_write(fp, OFFSETHEAD_INDEX, &nDataLength, sizeof(int));
  if (false == isTrue)
  {
    return false;
  }

  fflush(fp);

  return true;

}

/*
添加可回收垃圾数据指针索引
*/
bool AddRecoverableDataIndex(DTYPE *fp, void *data, size_t size)
{
  int nDataLength = 0;
  bool isTrue = false;

  //读取索引计数
  isTrue = device_read(fp, OFFSETHEAD_RECOVERABLE, &nDataLength, sizeof(int));
  if (false == isTrue)
  {
    return false;
  }

  isTrue = device_write(fp, OFFSETHEAD_RECOVERABLE + sizeof(nDataLength) + nDataLength * size, data, size);
  if (false == isTrue)
  {
    return false;
  }

  nDataLength = nDataLength + 1;

  //写入索引计数
  isTrue = device_write(fp, OFFSETHEAD_RECOVERABLE, &nDataLength, sizeof(int));
  if (false == isTrue)
  {
    return false;
  }

  fflush(fp);

  return true;

}

/*
获取索引数量
*/
int GetDataIndexCount(DTYPE *fp)
{
  int nDataLength = 0;
  bool isTrue = false;

  isTrue = device_read(fp, OFFSETHEAD_INDEX, &nDataLength, sizeof(int));
  if (false == isTrue)
  {
    nDataLength = -1;
  }

  return nDataLength;
}

/*
获取可回收垃圾索引数量
*/
int GetRecoverableDataIndexCount(DTYPE *fp)
{
  int nDataLength = 0;
  bool isTrue = false;

  isTrue = device_read(fp, OFFSETHEAD_RECOVERABLE, &nDataLength, sizeof(int));
  if (false == isTrue)
  {
    nDataLength = -1;
  }

  return nDataLength;
}

/*
获取索引数据项
*/
bool GetDataIndex(DTYPE *fp, int index, PTAGLINKMAN data)
{
  bool isTrue = false;

  isTrue = device_read(fp, OFFSETHEAD_INDEX + 4 + index * sizeof(TAGLINKMAN), data, sizeof(TAGLINKMAN));
  if (false == isTrue)
  {
    return false;
  }

  return true;
}

/*
获取索引数据项
*/
int GetDataBinSize(DTYPE *fp)
{
  int size = 0;

  bool isTrue = false;

  isTrue = device_read(fp, OFFSETTAIL_DATA, &size, sizeof(int));
  if (false == isTrue)
  {
    return 0;
  }

  return size;
}

//更新分配表信息
bool UpdateDateIndex(DTYPE *fp, int index, void *pdata, size_t size)
{
  bool isTrue = false;

  isTrue = device_write(fp, OFFSETHEAD_INDEX + sizeof(int) + index * size, pdata, size);
  if (false == isTrue)
  {
    return false;
  }

  fflush(fp);

  return true;
}

//更新数据
bool UpdateDateBin(DTYPE *fp, int pos, void *pdata, size_t size)
{
  bool isTrue = false;

  isTrue = device_write(fp, pos, pdata, size);
  if (false == isTrue)
  {
    return false;
  }

  fflush(fp);

  return true;
}

/*
获取可回收垃圾索引数据项
*/
bool GetRecoverableDataIndex(DTYPE *fp, int index, PTAGLINKMAN data)
{
  bool isTrue = false;

  isTrue = device_read(fp, OFFSETHEAD_RECOVERABLE + 4 + index * sizeof(TAGLINKMAN), data, sizeof(TAGLINKMAN));
  if (false == isTrue)
  {
    return false;
  }

  return true;
}

/*
获取可回收垃圾项
*/
bool GetRecoverableItemIndex(DTYPE *fp, int index, PTAGITEM data)
{
  bool isTrue = false;

  isTrue = device_read(fp, OFFSETHEAD_RECOVERABLE + 4 + index * sizeof(TAGITEM), data, sizeof(TAGITEM));
  if (false == isTrue)
  {
    return false;
  }

  return true;
}

/*
删除碎片索引
*/
bool DeleteRecoverableByIndex(DTYPE *fp, int index)
{
  int nBegin = OFFSETHEAD_RECOVERABLE + sizeof(int) + index * sizeof(TAGLINKMAN);

  int nLen = GetRecoverableDataIndexCount(fp);

  int nTotol = OFFSETHEAD_RECOVERABLE + sizeof(int) + nLen * sizeof(TAGLINKMAN);

  int nSize = nTotol - nBegin;

  char *buf = (char *)malloc(nSize);
  if (buf == NULL)
  {
    goto EXIT_PRO;
  }

  TAGLINKMAN temp = { 0 };

  //获取数据
  GetRecoverableDataIndex(fp, index, &temp);

  //移除索引
  device_read(fp, nBegin, buf, nSize);

  device_write(fp, OFFSETHEAD_RECOVERABLE + sizeof(int) + index * sizeof(TAGLINKMAN), buf, nSize);

  nLen = nLen - 1;

  if (device_write(fp, OFFSETHEAD_RECOVERABLE, &nLen, sizeof(nLen)) == false)
  {
    printf("Delete Recoverable Error！");
  }

  fflush(fp);

EXIT_PRO:

  if (buf != NULL)
  {
    free(buf);
    buf = NULL;
  }

  return false;

}

/*
是否在回收站
*/
bool IsRecoverable(DTYPE *fp, int nOffset)
{
  int nRecCount = GetRecoverableDataIndexCount(fp);

  for (int i = 0; i < (nRecCount * 2); i++)
  {
    TAGITEM tagTemp = { 0 };
    if (GetRecoverableItemIndex(fp, i, &tagTemp) == true)
    {
      if (nOffset >= tagTemp.nOffset && nOffset <= (tagTemp.nOffset + tagTemp.nLen))
      {
        return true;
      }
    }
    else
    {
      return false;
    }

  }
  return false;
}

/*
添加联系人二进制数据到磁盘
返回磁盘中的offset
*/
int AddDataBin(DTYPE *fp, char *data, size_t size)
{
  int nOffset = 0;
  int ret = 0;
  bool isTrue = false;

  isTrue = device_read(fp, OFFSETTAIL_DATA, &nOffset, sizeof(nOffset));
  if (false == isTrue)
  {
    return -1;
  }

  isTrue = device_write(fp, OFFSETSTART_DATA + nOffset, data, size);
  if (false == isTrue)
  {
    return -1;
  }

  ret = nOffset;
  nOffset = nOffset + size;

  isTrue = device_write(fp, OFFSETTAIL_DATA, &nOffset, sizeof(int));
  if (false == isTrue)
  {
    return -1;
  }

  fflush(fp);

  return ret;
}

/*
读取二进制数据
*/
bool GetDataBin(DTYPE *fp, int nOffset, size_t size, void *data)
{
  bool isTrue = false;

  isTrue = device_read(fp, OFFSETSTART_DATA + nOffset, data, size);
  if (false == isTrue)
  {
    return false;
  }

  return true;
}

/*
删除分配表索引数据项
*/
bool DeleteDataIndex(DTYPE *fp, int index)
{
  int nBegin = OFFSETHEAD_INDEX + sizeof(int) + (index + 1) * sizeof(TAGLINKMAN);

  int nLen = GetDataIndexCount(fp);

  int nTotol = OFFSETHEAD_INDEX + sizeof(int) + nLen * sizeof(TAGLINKMAN);

  int nSize = nTotol - nBegin;

  char *buf = (char *)malloc(nSize);
  if (buf == NULL)
  {
    goto EXIT_PRO;
  }

  TAGLINKMAN temp = { 0 };

  //获取数据
  GetDataIndex(fp, index, &temp);

  //放入回收站
  AddRecoverableDataIndex(fp, &temp, sizeof(temp));

  //移除索引
  device_read(fp, nBegin, buf, nSize);

  device_write(fp, OFFSETHEAD_INDEX + sizeof(int) + index * sizeof(TAGLINKMAN), buf, nSize);

  nLen = nLen - 1;

  if (device_write(fp, OFFSETHEAD_INDEX, &nLen, sizeof(nLen)) == false)
  {
    printf("Delete Linkman Error！");
  }

EXIT_PRO:

  if (buf != NULL)
  {
    free(buf);
    buf = NULL;
  }

  return false;
}

/*
初始化设备
*/
bool InitDevice(DTYPE **dOffset, DTYPE **dData)
{
  //打开偏移索引文件
  *dOffset = device_open(OFFSET_DEVICE_NAME);
  if (NULL == *dOffset)
  {
    //失败表示文件不存在 则创建文件
    puts("Not found data offset file,Is create a file,Please input 'y' or 'Y' to confirm,Otherwise it will be exit (y or Y): ");
    char ch = 0;

    ch = getchar();
    getchar();

    if ('y' == ch || 'Y' == ch)
    {
      if (false == create_offset_bin())
      {
        puts("Create file is fail,program quit!");
        return false;
      }
      else
      {
        *dOffset = device_open(OFFSET_DEVICE_NAME);
      }
    }
    else
    {
      return false;
    }
  }

  //打开数据文件
  *dData = device_open(DEVICE_NAME);
  if (NULL == *dData)
  {
    //失败表示文件不存在 则创建文件
    puts("Not found data file,Is create a file,Please input 'y' or 'Y' to confirm,Otherwise it will be exit (y or Y): ");
    char ch = 0;

    ch = getchar();
    getchar();

    if ('y' == ch || 'Y' == ch)
    {
      if (false == create_data_bin())
      {
        puts("Create file is fail,program quit!");
        return false;
      }
      else
      {
        *dData = device_open(DEVICE_NAME);
      }
    }
    else
    {
      return false;
    }
  }

  return true;
}

/*
关闭设备
*/
void CloseDevice(DTYPE *dOffset, DTYPE *dData)
{
  if (NULL != dOffset)
  {
    device_close(dOffset);
  }

  if (NULL != dData)
  {
    device_close(dData);
  }
}
