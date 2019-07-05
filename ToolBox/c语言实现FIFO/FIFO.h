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


/*****************************************************************************
FIFO结构体
*****************************************************************************/
typedef struct FIFO
{
	int32_t TypeLength;  //指buffer数据类型的长度，sizeof(数据类型)，比如int8_t
	int32_t BuffSize;    //buffer容量= TypeLength*BuffSize
	void* FIFO_Buff;    //fifo的buffer
	volatile int32_t front;  //FIFO的头部，指向第一个数据
	volatile int32_t rear;   //rear指向当前最后一个数据的后一个
}FIFO;

typedef FIFO* pFIFO;

/*****************************************************************************
函数名：CreateFIFO
功能描述:根据结构体创建FIFO
参数:
	pFIFO_Object:结构体变量，保存创建FIFO的参数，包括数据类型，FIFO容量
返回值 : 成功返回FIFO_OK，否则返回FIFO_ERROR
*****************************************************************************/
int CreateFIFO(pFIFO pFIFO_Object);

/*****************************************************************************
函数名：DestroyFIFO
功能描述:销毁FIFO，主要是释放buffer空间
参数:
	pFIFO_Object:FIFO结构体变量
返回值 : 无
*****************************************************************************/
void DestroyFIFO(pFIFO pFIFO_Object);

/*****************************************************************************
函数名：IsFifoFull
功能描述:判断FIFO是否已满
参数:
	pFIFO_Object:FIFO结构体变量
返回值 : 无
*****************************************************************************/
int32_t IsFifoFull(pFIFO pFIFO_Object);

/*****************************************************************************
函数名：GetBuffOccupation
功能描述:获取FIFO已经占用的空间
参数:
	pFIFO_Object:FIFO结构体变量
返回值 : 已经占用的空间长度，如返回5，表示已经占用5*TypeLength的空间
*****************************************************************************/
int32_t GetBuffOccupation(const pFIFO pFIFO_Object);

/*****************************************************************************
函数名：GetBuffSpace
功能描述:获取FIFO剩余空间
参数:
	pFIFO_Object:FIFO结构体变量
返回值 : 剩余空间长度，如返回5，表示剩余5*TypeLength的空间
*****************************************************************************/
int32_t GetBuffSpace(const pFIFO pFIFO_Object);

/*****************************************************************************
函数名：SuperWrite
功能描述:向FIFO写入数据
参数:
	pFIFO_Object:FIFO结构体变量
	SrcData：待写入数据的缓存
	DataInLen：写入数据的长度
返回值 : 成功或失败
*****************************************************************************/
//写FIFO时，SrcData的数据长度和TypeLength必须一致，否则导致出错。读FIFO也是如此
int32_t SuperWrite(pFIFO pFIFO_Object, const void* SrcData, int32_t DataInLen);

/*****************************************************************************
函数名：SuperRead
功能描述:向FIFO读出数据
参数:
	pFIFO_Object:FIFO结构体变量
	SrcData：读出数据的目的缓存
	DataInLen：读出数据的长度
返回值 : 成功或失败
*****************************************************************************/
int32_t SuperRead(pFIFO pFIFO_Object, const void* DstData, int32_t DataOutLen);

#endif