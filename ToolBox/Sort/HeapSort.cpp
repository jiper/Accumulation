
static void swap(int* dataRaw, int i, int j)
{
	int temp = dataRaw[i];
	dataRaw[i] = dataRaw[j];
	dataRaw[j] = temp;
}

void HeadAdjust(int* dataRaw, int beginPos, int length)
{
	int temp = dataRaw[beginPos];
	for (int j = (beginPos + 1) * 2 - 1; j < length; j = (j + 1) * 2 - 1)
	{
		if (j < length - 1 && dataRaw[j + 1] > dataRaw[j])
			j++;
		if (temp < dataRaw[j])
		{
			dataRaw[beginPos] = dataRaw[j];
			beginPos = j;
		}
	}
	dataRaw[beginPos] = temp;
}

void HeapSort(int* dataRaw, int length)
{
	for (int i = length / 2 - 1; i >= 0; i--)
	{
		HeadAdjust(dataRaw, i, length);
	}
	for (int i = length - 1; i > 0; i--)
	{
		swap(dataRaw, 0, i);
		HeadAdjust(dataRaw, 0, i);
	}
}