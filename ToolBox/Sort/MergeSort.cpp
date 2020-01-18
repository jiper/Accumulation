#include <stdlib.h>
//采用非递归实现

/**
* @Description: 将有序的SR[i...m]和SR[m+1...n]归并为有序的TR[i...n]，数组索引序号从0开始
* @param SR - 待归并数组
* @param TR - 归并后的数组
* @param i - 起始索引
* @param m - 分割索引
* @param n - 结束索引
* @return NULL
*/
void Merge(int SR[],int TR[],int i,int m,int n)
{
	int k,j;
	for (j = m + 1, k = i; j <= n && i <= m; k++)
	{
		if (SR[i] <= SR[j])   //保证其是稳定的
			TR[k] = SR[i++];
		else
			TR[k] = SR[j++];
	}
	if (i <= m)
	{
		for (; i <= m; i++)
			TR[k++] = SR[i];
	}
	else if (j <= n)
	{
		for (; j <= n; j++)
			TR[k++] = SR[j];
	}
}

/**
* @Description: 数组两两归并
* @param SR - 待归并数组
* @param TR - 归并后的数组
* @param s - 归并子序列的长度
* @param n - 待归并数组总长度
* @return NULL
*/
void MergePass(int SR[], int TR[], int s, int n)
{
	int i = 0;
	while (i < n - 2 * s)  //两两归并
	{
		Merge(SR, TR, i, i + s - 1, i + 2 * s - 1);
		i += 2 * s;
	}
	if (i < n - s)  //最后剩下两个序列，归并最后两个序列
	{
		Merge(SR, TR, i, i + s - 1, n-1);
	}
	else  //只剩下一个序列
	{
		for (int j = i; j < n; j++)
		{
			TR[j] = SR[j];
		}
	}
}


void MergeSort(int* dataRaw, int length)
{
	int* TR = (int*)malloc(sizeof(int) * length);
	int k = 1;
	while (k < length)
	{
		MergePass(dataRaw, TR, k, length);
		k = 2 * k;
		MergePass(TR, dataRaw, k, length);
		k = 2 * k;
	}
}