#include <iostream>
#include <ctime>
#include <chrono>
#include <random>
#include <cstring>
#include "random_genertator.h"

// 模式选择
// #define NONRECURSIVE	// 非递归模式

const static unsigned MAXN = 30000;

template <unsigned NSIZE>
void nonrecursive_merge_sort(unsigned(&a)[NSIZE], unsigned l, unsigned r);

template <unsigned NSIZE>
unsigned recursive_merge_sort(unsigned(&a)[NSIZE], unsigned l, unsigned r);

template <unsigned NSIZE>
void plain_merge(unsigned(&a)[NSIZE], unsigned l, unsigned m, unsigned r);

int main(int argc, char* argv[])
{
	unsigned a[MAXN] = { 0 };

	//* 读取样本模块
	FILE *lpRead = nullptr;
	fopen_s(&lpRead, "36.txt", "r");
	unsigned long long dd_of_seq = 0;
	unsigned long long add_of_seq = 0;
	fscanf_s(lpRead, "DD=%llu ADD=%llu\n", &dd_of_seq, &add_of_seq);
	
	unsigned cnt = 0;
	while (fscanf_s(lpRead, "%u", &a[cnt]) != EOF) ++cnt;
	//*/
	/*// 样本生成模块
	srand(time(NULL));
	unsigned cnt = MAXN;
	unsigned long long dd_of_seq = my_random_generator(a);
	unsigned long long add_of_seq = dd_of_seq / MAXN;

	FILE *lpWrite = nullptr;
	fopen_s(&lpWrite, "36.txt", "w");
	fprintf_s(lpWrite, "DD=%llu ADD=%llu\n", dd_of_seq, add_of_seq);
	for (int i = 0; i < MAXN; ++i)
		fprintf_s(lpWrite, "%u ", a[i]);
	fclose(lpWrite);
	// */

	std::cout << "DD=" << dd_of_seq << std::endl;
	std::cout << "ADD=" << add_of_seq << std::endl;
	std::cout << "NUM=" << cnt << std::endl;

	std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();

#ifdef NONRECURSIVE
	nonrecursive_merge_sort(a, 0, cnt);
#else
	unsigned level = recursive_merge_sort(a, 0, cnt);
#endif // NONRECURSIVE

	std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();

	std::chrono::steady_clock::duration dur = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
	std::cout << "COST=";
	std::cout << 
		static_cast<double>(static_cast<double>(dur.count()) * std::chrono::microseconds::period::num / std::chrono::microseconds::period::den) << "ms" << std::endl;

#ifndef NONRECURSIVE
	std::cout << "LEVEL=" << level << std::endl;
#endif // NONRECURSIVE
	
	return 0;
}

template <unsigned NSIZE>
void nonrecursive_merge_sort(unsigned(&a)[NSIZE], unsigned l, unsigned r)
{
	unsigned length = 1;
	while (length < r - l)
	{
		unsigned i = l, j = length * 2;
		for (; j < r; i = j, j += length * 2)
			plain_merge(a, i, i + length, j);
		if (i + length < r)
			plain_merge(a, i, i + length, r);
		length *= 2;
	}
}

template <unsigned NSIZE>
unsigned recursive_merge_sort(unsigned(&a)[NSIZE], unsigned l, unsigned r)
{
	if (l + 1 < r)
	{
		unsigned llevel = recursive_merge_sort(a, l, (l + r) / 2);
		unsigned rlevel = recursive_merge_sort(a, (l + r) / 2, r);
		plain_merge(a, l, (l + r) / 2, r);
		return std::max(llevel, rlevel) + 1;
	}
	return 0;
}

template<unsigned NSIZE>
void plain_merge(unsigned(&a)[NSIZE], unsigned l, unsigned m, unsigned r)
{
	unsigned i = l;
	unsigned j = m;
	unsigned k = 0;
	unsigned b[NSIZE];
	while (i < m && j < r)
		if (a[i] < a[j])
			b[k++] = a[i++];
		else
			b[k++] = a[j++];
	while (i < m)
		b[k++] = a[i++];
	while (j < r)
		b[k++] = a[j++];
	memcpy_s(a + l, (r - l) * sizeof(unsigned), b, k * sizeof(unsigned));
}