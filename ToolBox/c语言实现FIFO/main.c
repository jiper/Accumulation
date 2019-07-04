#include <stdio.h>
#include "FIFO.h"
#include <stdint.h>

typedef int32_t TestType;


int main()
{
		
	//用例1，创建FIFO
	int Length = 0,space =0;
	FIFO MyFifo = {sizeof(TestType),10};
	pFIFO pMyFifo = &MyFifo;
	TestType Data[10] = {0};
	CreateFIFO(&MyFifo);
	memcpy(Data, MyFifo.FIFO_Buff, 10);

	//用例2，写入
	Length = GetBuffOccupation(&MyFifo);
	space = GetBuffSpace(&MyFifo);
	TestType dataInput[5] = { 0,1,2,3,4 };
	if (SuperWrite(&MyFifo, dataInput, 5) == FIFO_ERROR)
	{
		return -1;
	}
	memcpy(Data, MyFifo.FIFO_Buff, 10);
	Length = GetBuffOccupation(&MyFifo);
	space = GetBuffSpace(&MyFifo);

	//用例3，读一部分数据
	TestType dataOut[10];
	SuperRead(pMyFifo, dataOut, 3);
	Length = GetBuffOccupation(&MyFifo);
	space = GetBuffSpace(&MyFifo);
	memcpy(Data, MyFifo.FIFO_Buff, 10);

	//用例4，写满尾部数据
	if (SuperWrite(&MyFifo, dataInput, 5) == FIFO_ERROR)
	{
		return -1;
	}
	Length = GetBuffOccupation(&MyFifo);
	space = GetBuffSpace(&MyFifo);
	memcpy(Data, MyFifo.FIFO_Buff, 10);

	//用例5，读取所有的数据
	if (SuperRead(&MyFifo, dataOut, 7) == FIFO_ERROR)
	{
		return -1;
	}
	Length = GetBuffOccupation(&MyFifo);
	space = GetBuffSpace(&MyFifo);
	memcpy(Data, MyFifo.FIFO_Buff, 10);


	printf("hello world!\n");
	return 0;
}
