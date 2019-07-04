/*****************************************************************************
Copyright:
File name:
Description: 
Author: 作者
Version: 版本
Date: 完成日期
History: 修改历史记录列表，每条修改记录应包括修改日期、修改者及修改内容简述。
*****************************************************************************/
#include "FIFO.h"
#include <stdlib.h>

int CreateFIFO(pFIFO pFIFO_Object)
{
	pFIFO_Object->FIFO_Buff = (void*)malloc(pFIFO_Object->TypeLength * pFIFO_Object->BuffSize);  //开辟TypeLength*BuffSize内存空间
	if (pFIFO_Object->FIFO_Buff == NULL)
	{
		return FIFO_ERROR;
	}
	pFIFO_Object->front = 0;  //front指向当前第一个数据
	pFIFO_Object->rear = 0;   //rear指向当前最后一个数据的后一个
	return FIFO_OK;
}

void DestroyFIFO(pFIFO pFIFO_Object)
{
	if (pFIFO_Object->FIFO_Buff != NULL)
	{
		free(pFIFO_Object->FIFO_Buff);
		pFIFO_Object->FIFO_Buff = NULL;
	}
}

int32_t IsFifoFull(pFIFO pFIFO_Object)
{

}

int32_t IsFifoEmty(pFIFO pFIFO_Object)
{
	
}

int32_t GetBuffOccupation(const pFIFO pFIFO_Object)
{
	if (pFIFO_Object->rear >= pFIFO_Object->front)
	{
		return (pFIFO_Object->rear - pFIFO_Object->front);
	}
	else
	{
		return (pFIFO_Object->BuffSize - pFIFO_Object->front + pFIFO_Object->rear);
	}
}

int32_t GetBuffSpace(const pFIFO pFIFO_Object)
{
	if (pFIFO_Object->rear >= pFIFO_Object->front)
	{
		return (pFIFO_Object->BuffSize - pFIFO_Object->rear + pFIFO_Object->front);
	}
	else
	{
		return (pFIFO_Object->front - pFIFO_Object->rear);
	}
}

int32_t SuperWrite(pFIFO pFIFO_Object, const void* SrcData, int32_t DataInLen)
{
	if (GetBuffSpace(pFIFO_Object) < DataInLen)  //判断剩余空间是否充足
	{
		return FIFO_ERROR;    //空间不够，返回错误
	}
	else
	{
		if (pFIFO_Object->rear + DataInLen <= pFIFO_Object->BuffSize)
		//表示rear后有足够的空间
		{
			if (pFIFO_Object->TypeLength == 1)
			{
				memcpy((int8_t *)(pFIFO_Object->FIFO_Buff) + pFIFO_Object->rear, SrcData, DataInLen);
			}
			else if (pFIFO_Object->TypeLength == 2)
			{
				memcpy((int16_t *)(pFIFO_Object->FIFO_Buff) + pFIFO_Object->rear, SrcData, DataInLen*2);
			}
			else if (pFIFO_Object->TypeLength == 4)
			{
				memcpy((int32_t *)(pFIFO_Object->FIFO_Buff) + pFIFO_Object->rear, SrcData, DataInLen * 4);
			}
			else if (pFIFO_Object->TypeLength == 8)
			{
				memcpy((double *)(pFIFO_Object->FIFO_Buff) + pFIFO_Object->rear, SrcData, DataInLen * 8);
			}
			else
			{
				return FIFO_ERROR;
			}
			pFIFO_Object->rear = pFIFO_Object->rear + DataInLen;
			if (pFIFO_Object->rear == pFIFO_Object->BuffSize)  //尾部写满，rear指针翻转
			{
				pFIFO_Object->rear = 0;
			}
		}
		else
		{
			int32_t firstLen = pFIFO_Object->BuffSize - pFIFO_Object->rear;
			int32_t secondLen = DataInLen - firstLen;
			if (pFIFO_Object->TypeLength == 1)
			{
				memcpy((int8_t*)(pFIFO_Object->FIFO_Buff) + pFIFO_Object->rear, SrcData, firstLen);
				memcpy(pFIFO_Object->FIFO_Buff, (int8_t *)SrcData + firstLen, secondLen);
			}
			else if (pFIFO_Object->TypeLength == 2)
			{
				memcpy((int16_t*)(pFIFO_Object->FIFO_Buff) + pFIFO_Object->rear, SrcData, firstLen * 2);
				memcpy(pFIFO_Object->FIFO_Buff, (int16_t *)SrcData + firstLen, secondLen*2);
			}
			else if (pFIFO_Object->TypeLength == 4)
			{
				memcpy((int32_t*)(pFIFO_Object->FIFO_Buff) + pFIFO_Object->rear, SrcData, firstLen * 4);
				memcpy(pFIFO_Object->FIFO_Buff, (int32_t *)SrcData + firstLen, secondLen * 4);
			}
			else if (pFIFO_Object->TypeLength == 8)
			{
				memcpy((double*)(pFIFO_Object->FIFO_Buff) + pFIFO_Object->rear, SrcData, firstLen * 8);
				memcpy(pFIFO_Object->FIFO_Buff, (double *)SrcData + firstLen, secondLen * 8);
			}
			else
			{
				return FIFO_ERROR;
			}
			pFIFO_Object->rear = secondLen;

		}
	}
	return FIFO_OK;
}

int32_t SuperRead(pFIFO pFIFO_Object, const void* DstData, int32_t DataOutLen)
{
	if (GetBuffOccupation(pFIFO_Object) < DataOutLen)  //判断是否有足够数据
	{
		return FIFO_ERROR;
	}
	else
	{
		if (pFIFO_Object->front + DataOutLen <= pFIFO_Object->BuffSize)
		{
			if (pFIFO_Object->TypeLength == 1)
			{
				memcpy(DstData, (int8_t*)(pFIFO_Object->FIFO_Buff) + pFIFO_Object->front, DataOutLen);
			}
			else if (pFIFO_Object->TypeLength == 2)
			{
				memcpy(DstData, (int16_t*)(pFIFO_Object->FIFO_Buff) + pFIFO_Object->front, DataOutLen * 2);
			}
			else if (pFIFO_Object->TypeLength == 4)
			{
				memcpy(DstData, (int32_t*)(pFIFO_Object->FIFO_Buff) + pFIFO_Object->front, DataOutLen * 4);
			}
			else if (pFIFO_Object->TypeLength == 8)
			{
				memcpy(DstData, (double *)(pFIFO_Object->FIFO_Buff) + pFIFO_Object->front, DataOutLen * 8);
			}
			else
			{
				return FIFO_ERROR;
			}
			pFIFO_Object->front += DataOutLen;
			if (pFIFO_Object->front == pFIFO_Object->BuffSize)  //front后的数据全部读出，front指针翻转
			{
				pFIFO_Object->front = 0;
			}
		}
		else
		{
			int32_t firstLen = pFIFO_Object->BuffSize - pFIFO_Object->front;
			int32_t secondLen = DataOutLen - firstLen;
			if (pFIFO_Object->TypeLength == 1)
			{
				memcpy(DstData, (int8_t*)(pFIFO_Object->FIFO_Buff) + pFIFO_Object->front, firstLen);
				memcpy((int8_t *)DstData + firstLen, pFIFO_Object->FIFO_Buff, secondLen);
			}
			else if (pFIFO_Object->TypeLength == 2)
			{
				memcpy(DstData, (int16_t*)(pFIFO_Object->FIFO_Buff) + pFIFO_Object->front, firstLen * 2);
				memcpy((int16_t*)DstData + firstLen, pFIFO_Object->FIFO_Buff, secondLen * 2);
			}
			else if (pFIFO_Object->TypeLength == 4)
			{
				memcpy(DstData, (int32_t*)(pFIFO_Object->FIFO_Buff) + pFIFO_Object->front, firstLen * 4);
				memcpy((int32_t*)DstData + firstLen, pFIFO_Object->FIFO_Buff, secondLen * 4);
			}
			else if (pFIFO_Object->TypeLength == 8)
			{
				memcpy(DstData, (double*)(pFIFO_Object->FIFO_Buff) + pFIFO_Object->front, firstLen * 8);
				memcpy((double *)DstData + firstLen, pFIFO_Object->FIFO_Buff, secondLen * 8);
			}
			else
			{
				return FIFO_ERROR;
			}
			pFIFO_Object->front = secondLen;
		}
		return FIFO_OK;
	}
}
