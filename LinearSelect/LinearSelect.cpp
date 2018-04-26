#include <cstdio>
#include <algorithm>

// 模式选择
#define THREEPARTITION	// 一分为三

typedef struct _Station
{
	unsigned id;
	double dist;
} STATION, *LPSTATION;

template <unsigned NSIZE>
STATION linear_select(STATION(&a)[NSIZE], unsigned l, unsigned r, unsigned k);

template <unsigned NSIZE>
void bubble_sort(STATION(&a)[NSIZE], unsigned l, unsigned r);

template<unsigned NSIZE>
unsigned plain_partiton(STATION(&a)[NSIZE], unsigned l, unsigned r);

const static unsigned MAXN = 1033;
static unsigned LEVEL_CNT = 0;

typedef FILE *LPFILE;
int main(int argc, char* argv[])
{
	LPFILE lpRead = nullptr;
	if (!fopen_s(&lpRead, "1033个基站数据.txt", "r"))
	{
		char lpszID[10];
		char lpszLNG[10];
		char lpszLAT[10];
		char lpszDIS[10];
		fscanf_s(lpRead, "%s\t%s\t%s\t%s\n", lpszID, _countof(lpszID), lpszLNG, _countof(lpszLNG), lpszLAT, _countof(lpszLAT), lpszDIS, _countof(lpszDIS));

		STATION station_set[MAXN];
		unsigned cnt = 0;
		double lng, lat;	// 不使用
		while (fscanf_s(lpRead, "%u\t%lf\t%lf\t%lf\n", &station_set[cnt].id, &lng, &lat, &station_set[cnt].dist) != EOF)
			++cnt;
		unsigned k = 0;
		printf_s("Please input the kth number you want to find ( 1 ~ %u) : ", cnt);
		scanf_s("%u", &k);
		STATION res = linear_select(station_set, 0, cnt, k - 1);
		printf_s("The station'is is %u and its distance is %lf.\nThe level is %u.\n", res.id, res.dist, LEVEL_CNT);
	}
	return 0;
}

template <unsigned NSIZE>
STATION linear_select(STATION(&a)[NSIZE], unsigned l, unsigned r, unsigned k)
{
	if (k < l || k >= r)
		return {UINT_MAX, -1};
	++LEVEL_CNT;
	const static unsigned length = 5;
	if (r - l <= 20)
	{
		bubble_sort(a, l, r);
		return a[k];
	}
	else
	{
		for (unsigned i = 0; i < (r - l) / length; ++i)
			bubble_sort(a, l + length * i, l + length * (i + 1)), std::swap(a[l + i], a[l + length * i + length / 2]);
		linear_select(a, l, l + (r - l) / length, l + (r - l) / length / 2);
		std::swap(a[l], a[l + (r - l) / length / 2]);
		unsigned m = plain_partiton(a, l, r);
#ifdef THREEPARTITION
		if (m == k)
			return a[m];
		else if (m > k)
			return linear_select(a, l, m, k);
		else
			return linear_select(a, m + 1, r, k);
#else
		if (m > k)
			return linear_select(a, l, m, k);
		else
			return linear_select(a, m, r, k);
#endif // THREEPARTITION
	}
}

template<unsigned NSIZE>
void bubble_sort(STATION(&a)[NSIZE], unsigned l, unsigned r)
{
	for (unsigned i = l; i < r; ++i)
		for (unsigned j = l; j < l + r - i - 1; ++j)
			if (a[j].dist > a[j + 1].dist)
				std::swap(a[j], a[j + 1]);
}

template<unsigned NSIZE>
unsigned plain_partiton(STATION(&a)[NSIZE], unsigned l, unsigned r)
{
	unsigned i = l, j = r;
	STATION k = a[l];
	while (true)
	{
		while (a[++i].dist < k.dist && i < r - 1);
		while (a[--j].dist > k.dist);
		if (i >= j) break;
		std::swap(a[i], a[j]);
	}
	a[l] = a[j];
	a[j] = k;
	return j;
}