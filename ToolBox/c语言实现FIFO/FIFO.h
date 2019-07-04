/*****************************************************************************
Copyright: 
File name:
Description: 
Author: 翦林鹏
Version: 版本
Date: 完成日期
History: 修改历史记录列表，每条修改记录应包括修改日期、修改者及修改内容简述。
*****************************************************************************/
#ifndef __FIFO_H
#define __FIFO_H

#include <stdint.h>

#ifndef FIFO_ERROR
#define FIFO_ERROR -1
#endif

#ifndef FIFO_OK
#define FIFO_OK 0
#endif // !FIFO_OK

typedef struct FIFO
{
	int32_t TypeLength;  //指buffer数据类型的长度，sizeof(数据类型)，比如int8_t
	int32_t BuffSize;    //buffer容量= TypeLength*BuffSize
	void* FIFO_Buff;    //fifo的buffer
	volatile int32_t front;  //FIFO的头部，指向第一个数据
	volatile int32_t rear;   //rear指向当前最后一个数据的后一个
}FIFO;

typedef FIFO* pFIFO;

int CreateFIFO(pFIFO pFIFO_Object);
void DestroyFIFO(pFIFO pFIFO_Object);
int32_t GetBuffOccupation(const pFIFO pFIFO_Object);
int32_t GetBuffSpace(const pFIFO pFIFO_Object);

//写FIFO时，SrcData的数据长度和TypeLength必须一致，否则导致出错。读FIFO也是如此
int32_t SuperWrite(pFIFO pFIFO_Object, const void* SrcData, int32_t DataInLen);
int32_t SuperRead(pFIFO pFIFO_Object, const void* DstData, int32_t DataOutLen);

#endif