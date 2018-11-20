#include "interface.h"
#include <stdlib.h>

/*
�����ϵ������ָ������
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
��ӿɻ�����������ָ������
*/
bool AddRecoverableDataIndex(DTYPE *fp, void *data, size_t size)
{
  int nDataLength = 0;
  bool isTrue = false;

  //��ȡ��������
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

  //д����������
  isTrue = device_write(fp, OFFSETHEAD_RECOVERABLE, &nDataLength, sizeof(int));
  if (false == isTrue)
  {
    return false;
  }

  fflush(fp);

  return true;

}

/*
��ȡ��������
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
��ȡ�ɻ���������������
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
��ȡ����������
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
��ȡ����������
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

//���·������Ϣ
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

//��������
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
��ȡ�ɻ�����������������
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
��ȡ�ɻ���������
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
ɾ����Ƭ����
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

  //��ȡ����
  GetRecoverableDataIndex(fp, index, &temp);

  //�Ƴ�����
  device_read(fp, nBegin, buf, nSize);

  device_write(fp, OFFSETHEAD_RECOVERABLE + sizeof(int) + index * sizeof(TAGLINKMAN), buf, nSize);

  nLen = nLen - 1;

  if (device_write(fp, OFFSETHEAD_RECOVERABLE, &nLen, sizeof(nLen)) == false)
  {
    printf("Delete Recoverable Error��");
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
�Ƿ��ڻ���վ
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
�����ϵ�˶��������ݵ�����
���ش����е�offset
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
��ȡ����������
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
ɾ�����������������
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

  //��ȡ����
  GetDataIndex(fp, index, &temp);

  //�������վ
  AddRecoverableDataIndex(fp, &temp, sizeof(temp));

  //�Ƴ�����
  device_read(fp, nBegin, buf, nSize);

  device_write(fp, OFFSETHEAD_INDEX + sizeof(int) + index * sizeof(TAGLINKMAN), buf, nSize);

  nLen = nLen - 1;

  if (device_write(fp, OFFSETHEAD_INDEX, &nLen, sizeof(nLen)) == false)
  {
    printf("Delete Linkman Error��");
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
��ʼ���豸
*/
bool InitDevice(DTYPE **dOffset, DTYPE **dData)
{
  //��ƫ�������ļ�
  *dOffset = device_open(OFFSET_DEVICE_NAME);
  if (NULL == *dOffset)
  {
    //ʧ�ܱ�ʾ�ļ������� �򴴽��ļ�
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

  //�������ļ�
  *dData = device_open(DEVICE_NAME);
  if (NULL == *dData)
  {
    //ʧ�ܱ�ʾ�ļ������� �򴴽��ļ�
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
�ر��豸
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
