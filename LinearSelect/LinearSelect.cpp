#include <iostream>
#include <ctime>
#include <random>
#include "random_genertator.h"

template <unsigned NSIZE>
unsigned linear_select(unsigned(&a)[NSIZE], unsigned l, unsigned r, unsigned k);

template <unsigned NSIZE>
void bubble_sort(unsigned(&a)[NSIZE], unsigned l, unsigned r);

template<unsigned NSIZE>
unsigned plain_partiton(unsigned(&a)[NSIZE], unsigned l, unsigned r);

const static unsigned MAXN = 23877;

int main(int argc, char* argv[])
{
	unsigned a[MAXN] = { 0 };
	unsigned long long dd_of_seq = my_random_generator(a);
	unsigned long long add_of_seq = dd_of_seq / MAXN;

	std::cout << "DD=" << dd_of_seq << std::endl;
	std::cout << "ADD=" << add_of_seq << std::endl;

	unsigned k = 0;
	std::cin >> k;
	std::cout << "K'th=" << linear_select(a, 0, MAXN, k) << std::endl;

	return 0;
}

template <unsigned NSIZE>
unsigned linear_select(unsigned(&a)[NSIZE], unsigned l, unsigned r, unsigned k)
{
	if (k < l + 1 || k > r + 1)
		return -1;

	const static unsigned length = 5;
	if (r - l < 20)
	{
		bubble_sort(a, l, r);
		return a[k - 1];
	}
	else
	{
		for (unsigned i = 0; i < (r - l + 1) / length; ++i)
			bubble_sort(a, l + length * i, l + length * (i + 1)), std::swap(a[l + i], a[l + length * i + length / 2]);
		unsigned x = linear_select(a, l, l + (r - l + 1) / length, l + (r - l + 1) / length / 2 + 1);
		std::swap(a[l], a[l + (r - l + 1) / length / 2]);
		unsigned m = plain_partiton(a, l, r);
		if (m + 1 == k)
			return a[m];
		else if (m + 1 > k)
			return linear_select(a, l, m, k);
		else
			return linear_select(a, m + 1, r, k);
	}
}

template<unsigned NSIZE>
void bubble_sort(unsigned(&a)[NSIZE], unsigned l, unsigned r)
{
	for (unsigned i = l; i < r; ++i)
		for (unsigned j = l; j < l + r - i; ++j)
			if (a[j] > a[j + 1])
				std::swap(a[j], a[j + 1]);
}

template<unsigned NSIZE>
unsigned plain_partiton(unsigned(&a)[NSIZE], unsigned l, unsigned r)
{
	unsigned i = l, j = r + 1;
	unsigned k = a[l];
	while (true)
	{
		while (a[++i] < k && i < r);
		while (a[--j] > k);
		if (i >= j) break;
		std::swap(a[i], a[j]);
	}
	a[l] = a[j];
	a[j] = k;
	return j;
}