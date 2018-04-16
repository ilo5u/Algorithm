// QSort.cpp: 定义控制台应用程序的入口点。
//

#include "stdafx.h"

#define DEBUG

template <size_t array_size>
int my_qsort(int(&a)[array_size], const int& first, const int& last);

template <size_t array_size>
int my_partiton(int(&a)[array_size], const int& first, const int& last);

template <size_t array_size>
int my_qsort_random(int(&a)[array_size], const int& first, const int& last);

template <size_t array_size>
int my_partiton_random(int(&a)[array_size], const int& first, const int& last);

#ifdef DEBUG
	std::chrono::steady_clock::duration duration(0);
	std::chrono::steady_clock::duration duration_random(0);
#endif // DEBUG

template <class Ty>
void my_swap(Ty& l, Ty& r)
{
	Ty t = l;
	l = r;
	r = t;
}

int main()
{
	const int max = 5000000;
	int a[max];
	int b[max];

	std::default_random_engine e;
	e.seed((unsigned int)time(NULL));
	std::uniform_int_distribution<int> u(0, 10000000);

	for (int i = 0; i < max; ++i)
		a[i] = b[i] = u(e);

	
	std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();

	int level_count = my_qsort(a, 0, max - 1);

	std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();

	std::chrono::steady_clock::duration dur = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
	std::cout << static_cast<double>(static_cast<double>(dur.count()) * std::chrono::microseconds::period::num / std::chrono::microseconds::period::den) << "ms" << std::endl;

	start = std::chrono::steady_clock::now();

	int level_count_random = my_qsort_random(b, 0, max - 1);

	end = std::chrono::steady_clock::now();

	dur = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
	std::cout << static_cast<double>(static_cast<double>(dur.count()) * std::chrono::microseconds::period::num / std::chrono::microseconds::period::den) << "ms" << std::endl;

#ifdef DEBUG
	std::cout << static_cast<double>(static_cast<double>(duration.count()) * std::chrono::microseconds::period::num / std::chrono::microseconds::period::den) << "ms" << std::endl;
	std::cout << static_cast<double>(static_cast<double>(duration_random.count()) * std::chrono::microseconds::period::num / std::chrono::microseconds::period::den) << "ms" << std::endl;
#endif // DEBUG


	std::cout << "level_count=" << level_count << std::endl;
	std::cout << "level_count_random=" << level_count_random << std::endl;

    return 0;
}

template <size_t array_size>
int my_qsort(int(&a)[array_size], const int& first, const int& last)
{
	int mid = my_partiton(a, first, last);
	int left_level = 0;
	int right_level = 0;
	if (first < mid - 1)
		left_level = my_qsort(a, first, mid - 1);
	if (mid + 1 < last)
		right_level = my_qsort(a, mid + 1, last);
	return std::max(left_level, right_level) + 1;
}

template <size_t array_size>
int my_partiton(int(&a)[array_size], const int& first, const int& last)
{
	int left = first, right = last + 1;
	int key = a[first];

#ifdef DEBUG
	std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
#endif // DEBUG

	while (true)
	{
		while (a[++left] < key && left < last);
		while (a[--right] > key);

		if (left >= right) break;

		my_swap(a[left], a[right]);
	}
	a[first] = a[right];
	a[right] = key;

#ifdef DEBUG
	std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
	duration += std::chrono::duration_cast<std::chrono::microseconds>(end - start);
#endif // DEBUG

	return right;
}

template <size_t array_size>
int my_qsort_random(int(&a)[array_size], const int& first, const int& last)
{
	int mid = my_partiton_random(a, first, last);
	int left_level = 0;
	int right_level = 0;
	if (first < mid - 1)
		left_level = my_qsort_random(a, first, mid - 1);
	if (mid + 1 < last)
		right_level = my_qsort_random(a, mid + 1, last);
	return std::max(left_level, right_level) + 1;
}

template <size_t array_size>
int my_partiton_random(int(&a)[array_size], const int& first, const int& last)
{
	int left = first + rand() % (last - first + 1), right = last + 1;
	my_swap(a[left], a[first]);

	left = first;
	int key = a[first];

#ifdef DEBUG
	std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
#endif // DEBUG

	while (true)
	{
		while (a[++left] < key && left < last);
		while (a[--right] > key);

		if (left >= right) break;

		my_swap(a[left], a[right]);
	}
	a[first] = a[right];
	a[right] = key;

#ifdef DEBUG
	std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
	duration_random += std::chrono::duration_cast<std::chrono::microseconds>(end - start);
#endif // DEBUG

	return right;
}

