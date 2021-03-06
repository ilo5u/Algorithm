#include <iostream>
#include <chrono>
#include <random>

// 模式选择
#define RANDOM	// 启动随机

const static unsigned MAXN = 30000;

// 朴素快速排序
template <size_t array_size>
int my_qsort(int(&a)[array_size], const int& first, const int& last);

template <size_t array_size>
int my_partiton(int(&a)[array_size], const int& first, const int& last);

// 基于随机策略的快速排序
template <size_t array_size>
int my_qsort_random(int(&a)[array_size], const int& first, const int& last);

template <size_t array_size>
int my_partiton_random(int(&a)[array_size], const int& first, const int& last);

template <class Ty>
void my_swap(Ty& l, Ty& r)
{
	Ty t = l;
	l = r;
	r = t;
}

int main()
{
	int a[MAXN];

	// 读取样本模块
	FILE *lpRead = nullptr;
	fopen_s(&lpRead, "36.txt", "r");
	unsigned long long dd_of_seq = 0;
	unsigned long long add_of_seq = 0;
	fscanf_s(lpRead, "DD=%llu ADD=%llu\n", &dd_of_seq, &add_of_seq);

	int cnt = 0;
	while (fscanf_s(lpRead, "%d", &a[cnt]) != EOF) ++cnt;

	/*// 样本生成模块
	srand(time(NULL));
	unsigned long long dd_of_seq = my_random_generator(a);
	unsigned long long add_of_seq = dd_of_seq / MAXN;

	FILE *lpWrite = nullptr;
	fopen_s(&lpWrite, "30.txt", "w");
	fprintf_s(lpWrite, "DD=%llu ADD=%llu\n", dd_of_seq, add_of_seq);
	for (int i = 0; i < MAXN; ++i)
	fprintf_s(lpWrite, "%u ", a[i]);
	fclose(lpWrite);
	// */

	std::cout << "DD=" << dd_of_seq << std::endl;
	std::cout << "ADD=" << add_of_seq << std::endl;
	std::cout << "NUM=" << cnt << std::endl;

	std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();

#ifdef RANDOM
	unsigned level = my_qsort_random(a, 0, cnt - 1);
#else
	unsigned level = my_qsort(a, 0, cnt - 1);
#endif // NONRECURSIVE

	std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();

	std::chrono::steady_clock::duration dur = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
	std::cout << "COST=";
	std::cout <<
		static_cast<double>(static_cast<double>(dur.count()) * std::chrono::microseconds::period::num / std::chrono::microseconds::period::den) << "ms" << std::endl;
	std::cout << "LEVEL=" << level << std::endl;
    return 0;
}

template <size_t array_size>
int my_qsort(int(&a)[array_size], const int& first, const int& last)
{
	for (int i = first; i < last; ++i)
		if (a[i] > a[i + 1])
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
	return 1;
}

template <size_t array_size>
int my_partiton(int(&a)[array_size], const int& first, const int& last)
{
	int left = first, right = last + 1;
	int key = a[first];
	while (true)
	{
		while (a[++left] < key && left < last);
		while (a[--right] > key);

		if (left >= right) break;

		my_swap(a[left], a[right]);
	}
	a[first] = a[right];
	a[right] = key;
	return right;
}

template <size_t array_size>
int my_qsort_random(int(&a)[array_size], const int& first, const int& last)
{
	for (int i = first; i < last; ++i)
		if (a[i] > a[i + 1])
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
	return 1;
}

template <size_t array_size>
int my_partiton_random(int(&a)[array_size], const int& first, const int& last)
{
	int left = first + rand() % (last - first + 1), right = last + 1;
	my_swap(a[left], a[first]);

	left = first;
	int key = a[first];
	while (true)
	{
		while (a[++left] < key && left < last);
		while (a[--right] > key);

		if (left >= right) break;

		my_swap(a[left], a[right]);
	}
	a[first] = a[right];
	a[right] = key;
	return right;
}

