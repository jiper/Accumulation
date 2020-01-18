
static void swap(int* dataRaw, int i, int j)
{
	int temp = dataRaw[i];
	dataRaw[i] = dataRaw[j];
	dataRaw[j] = temp;
}

//直接插入排序，复杂度o(n?)
static void  InsertSort(int* dataRaw, int dataLength)
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

int Partition(int* dataRaw, int low, int high)
{
	int pivotkey;
	int m = (high + low) / 2;
	if (dataRaw[low] > dataRaw[high])
	{
		swap(dataRaw, low, high);   //保证左端比右端小
	}
	if (dataRaw[m] > dataRaw[high])
	{
		swap(dataRaw, high, m);  //保证中间比右端小
	}
	if (dataRaw[m] > dataRaw[low])
	{
		swap(dataRaw, m, low);  //保证左端比中间大
	}
	pivotkey = dataRaw[low];
	while (low < high)
	{
		while (dataRaw[high] >= pivotkey && low < high)
		{
			high--;
		}
		if (low < high)
		{
			//swap(dataRaw, low, high);
			dataRaw[low] = dataRaw[high];
			low++;
			while (dataRaw[low] <= pivotkey && low < high)
			{
				low++;
			}
			if (low < high)
			{
				//swap(dataRaw, low, high);
				dataRaw[high] = dataRaw[low];
				high--;
			}
		}
	}
	dataRaw[low] = pivotkey;
	return low;
}

#define MAX_LENGTH_INSERT_SORT 7
void QSort(int* dataRaw, int low, int high)
{
	int pivot;
	if (high - low > MAX_LENGTH_INSERT_SORT)
	{
		while (low < high)
		{
			pivot = Partition(dataRaw, low, high);
			QSort(dataRaw, low, pivot - 1);
			low = pivot + 1;
		}
	}
	else
	{
		InsertSort(dataRaw+low, (high - low + 1));
	}
}


void QuickSort(int* dataRaw, int length)
{
	QSort(dataRaw, 0,length-1);
}