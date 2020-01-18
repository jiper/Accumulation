#include <iostream>
#include <vector>
#include <stdlib.h>

using namespace std;

//#define BubbleSortTest     //冒泡排序
//#define SelectSortTest       //简单选择排序
//#define InsertSortTest        //直接插入排序
//#define ShellSortTest        //希尔排序
//#define HeapSortTest		  //堆排序
//#define MergeSortTest        //归并排序
#define QuickSortTest      //快速排序

void HeapSort(int* dataRaw, int length);
void MergeSort(int* dataRaw, int length);
void QuickSort(int* dataRaw, int length);



void swap(int* dataRaw, int i, int j)
{
	int temp = dataRaw[i];
	dataRaw[i] = dataRaw[j];
	dataRaw[j] = temp;
}
//冒泡排序,复杂度o(n²)
void BubbleSort(int* dataRaw,int dataLength)
{
	bool ifSorted = false;
	for (int j = 0; (j < dataLength - 1) && (!ifSorted); j++)
	{
		ifSorted = true;
		for (int i = dataLength - 1; i > 0; --i)
		{
			if (dataRaw[i] < dataRaw[i - 1])
			{
				swap(dataRaw, i, i - 1);
				ifSorted = false;
			}
		}
	}
}

//简单选择排序，复杂度o(n²)
void SelectSort(int* dataRaw, int dataLength)
{
	int min;
	for (int i = 0; i < dataLength -1; i++)
	{
		min = i;
		for (int j = i + 1; j < dataLength; j++)
		{
			if (dataRaw[min] > dataRaw[j])
			{
				min = j;
			}
		}
		if (min != i)
		{
			swap(dataRaw, min, i);
		}
	}
}

//直接插入排序，复杂度o(n²)
void  InsertSort(int* dataRaw, int dataLength)
{
	for (int i = 1; i < dataLength; ++i)
	{
		if (dataRaw[i] < dataRaw[i - 1])
		{
			int j;
			int temp = dataRaw[i];
			for (j = i - 1; dataRaw[j] > temp; --j)
			{
				dataRaw[j + 1] = dataRaw[j];
			}
			dataRaw[j + 1] = temp;
		}
	}
}

//希尔排序，复杂度o(n^3/2)
void ShellSort(int* dataRaw, int dataLength)
{
	int increment = dataLength;
	do
	{
		increment = increment / 3 + 1;
		for (int i = increment; i < dataLength; i++)
		{
			if (dataRaw[i] < dataRaw[i - increment])
			{
				int j;
				int temp = dataRaw[i];
				for (j = i - increment; j >= 0 && temp < dataRaw[j]; j -= increment)
				{
					dataRaw[j + increment] = dataRaw[j];
				}
				dataRaw[j + increment] = temp;
			}
		}
	}
	while (increment > 1);
}
#include <time.h>
int main()
{
#if 1
	int length;
	cin >> length;
	int* dataRaw = (int*)malloc(sizeof(int) * length);
	for (int i = 0; i < length; i++)
	{
		/*srand((unsigned)time(NULL));*/
		dataRaw[i] = length -i;
		cout << dataRaw[i] << " ";
	}
	cout << endl;
#else
	int length = 9;
	int dataRaw[9] = { 9,1,5,8,3,7,4,6,2 };
#endif

#ifdef BubbleSortTest
	BubbleSort(dataRaw, length);
#endif
#ifdef SelectSortTest
	SelectSort(dataRaw, length);
#endif
#ifdef InsertSortTest
	InsertSort(dataRaw, length);
#endif
#ifdef ShellSortTest
	ShellSort(dataRaw, length);
#endif
#ifdef HeapSortTest
	HeapSort(dataRaw, length);
#endif
#ifdef MergeSortTest
	MergeSort(dataRaw, length);
#endif
#ifdef QuickSortTest
	QuickSort(dataRaw, length);
#endif
	for (int i = 0; i < length; i++)
	{
		cout << dataRaw[i] << " ";
	}
	return 0;
}

