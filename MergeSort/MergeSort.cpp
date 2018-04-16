#include <iostream>
#include <ctime>
#include <random>
#include "random_genertator.h"

const static unsigned MAXN = 389;

template <unsigned NSIZE>
void nonrecursive_merge_sort(unsigned(&a)[NSIZE], unsigned l, unsigned r);

template <unsigned NSIZE>
void recursive_merge_sort(unsigned(&a)[NSIZE], unsigned l, unsigned r);

template <unsigned NSIZE>
void plain_merge(unsigned(&a)[NSIZE], unsigned l, unsigned m, unsigned r);

int main(int argc, char* argv[])
{
	unsigned a[MAXN] = { 0 };
	unsigned long long dd_of_seq = my_random_generator(a);
	unsigned long long add_of_seq = dd_of_seq / MAXN;

	for (unsigned i = 0; i != MAXN; ++i)
		std::cout << a[i] << " ";
	std::cout << std::endl;

	std::cout << "DD=" << dd_of_seq << std::endl;
	std::cout << "ADD=" << add_of_seq << std::endl;

	recursive_merge_sort(a, 0, MAXN);

	for (unsigned i = 0; i != MAXN; ++i)
		std::cout << a[i] << " ";
	std::cout << std::endl;

	return 0;
}

template <unsigned NSIZE>
void nonrecursive_merge_sort(unsigned(&a)[NSIZE], unsigned l, unsigned r)
{
	unsigned length = 1;
	while (length < NSIZE)
	{
		unsigned i = 0, j = length * 2;
		for (; j < NSIZE; i = j, j += length * 2)
			plain_merge(a, i, i + length, j);
		if (i + length < NSIZE)
			plain_merge(a, i, i + length, NSIZE);
		length *= 2;
	}
}

template <unsigned NSIZE>
void recursive_merge_sort(unsigned(&a)[NSIZE], unsigned l, unsigned r)
{
	if (l + 1 < r)
	{
		recursive_merge_sort(a, l, (l + r) / 2);
		recursive_merge_sort(a, (l + r) / 2, r);
		plain_merge(a, l, (l + r) / 2, r);
	}
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