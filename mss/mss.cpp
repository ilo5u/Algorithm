#include <cstdio>
#include <algorithm>	

#define MAX_CAPACITY   500
#define MAX_LOADSTRING 100

int main(int argc, char* argv[])
{
	FILE* lpRead = nullptr;
	int nItem[MAX_CAPACITY];
	auto test = [&lpRead, &nItem](const char szFileName[]) {
		fopen_s(&lpRead, szFileName, "r");

		int iCnt = 0;
		while (fscanf_s(lpRead, "%d\n", &nItem[iCnt++]) != EOF);
		fclose(lpRead);

		int res = 0;
		int sum = 0;
		int start = 0, end = 0;
		for (int i = 0; i < iCnt; ++i)
		{
			if (sum > 0)
				sum += nItem[i];
			else
				sum = nItem[i], start = i, end = i;
			if (sum > res)
				res = sum, end = i;
		}
		printf_s("start = %d end = %d\n", start, end);
		while (start < end)
			printf_s("%d ", nItem[start++]);
		printf_s("\n");
		return res;
	};

	printf_s("test1: sum = %d\n", test("附件2.最大子段和输入数据2017-序列1.txt"));
	printf_s("\n");
	printf_s("test2: sum = %d\n", test("附件2.最大子段和输入数据2017-序列2.txt"));

	return 0;
}