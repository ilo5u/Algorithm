#include <cstdio>
#include <algorithm>
#include <stack>

#define MAX_ITEM_NUM   4
#define MAX_LOADSTRING 1200

typedef struct _STR_SET
{
	char m_szId[MAX_ITEM_NUM][MAX_LOADSTRING];
	char m_szSet[MAX_ITEM_NUM][MAX_LOADSTRING];
} STRSET, *LPSTRSET;

int main(int argc, char* argv[])
{
	FILE *lpRead;
	fopen_s(&lpRead, "附件1.最长公共子序列输入文件2017-4-26.txt", "r");

	STRSET strset;
	for (int i = 0; i < MAX_ITEM_NUM; ++i)
	{
		fscanf_s(lpRead, "%s\n%s\n", strset.m_szId[i], MAX_LOADSTRING, strset.m_szSet[i], MAX_LOADSTRING);
		if (i == 0)
		{
			char szMore[MAX_LOADSTRING];
			fscanf_s(lpRead, "%s\n", szMore, MAX_LOADSTRING);
			strcat_s(strset.m_szSet[i] + strlen(strset.m_szSet[i]), MAX_LOADSTRING - strlen(strset.m_szSet[i]), szMore);
		}
	}

	int res[MAX_LOADSTRING][MAX_LOADSTRING];
	std::stack<char> path;
	auto test = [&res, &path, &strset](int a, int b) {
		int lena = static_cast<int>(strlen(strset.m_szSet[a]));
		int lenb = static_cast<int>(strlen(strset.m_szSet[b]));

		for (int i = 0; i < lena; ++i)
			for (int j = 0; j < lenb; ++j)
				if (!i || !j)
					res[i][j] = 0;
				else
				{
					res[i][j] = std::max(res[i][j - 1], res[i - 1][j]);
					if (strset.m_szSet[a][i] == strset.m_szSet[b][j])
						res[i][j] = std::max(res[i - 1][j - 1] + 1, res[i][j]);
				}

		int cur_a = lena - 1;
		int cur_b = lenb - 1;
		while (cur_a && cur_b)
			if (strset.m_szSet[a][cur_a] == strset.m_szSet[b][cur_b])
			{
				path.push(strset.m_szSet[a][cur_a]);
				--cur_a, --cur_b;
			}
			else
				if (res[cur_a][cur_b] == res[cur_a - 1][cur_b])
					--cur_a;
				else
					--cur_b;

		printf_s("lcs is: ");
		while (!path.empty())
		{
			printf_s("%c", path.top());
			path.pop();
		}
		printf_s("\n");

		return res[lena - 1][lenb - 1];
	};

	printf_s("test1: A and B = %d\n", test(0, 1));
	printf_s("test2: C and D = %d\n", test(2, 3));
	printf_s("test3: A and D = %d\n", test(0, 3));
	printf_s("test4: B and C = %d\n", test(1, 2));

	return 0;
}