#pragma once

template <unsigned NSIZE>
unsigned long long my_random_generator(unsigned(&a)[NSIZE]);

template <unsigned NSIZE>
unsigned long long get_DD_of_seq(unsigned(&a)[NSIZE], unsigned l, unsigned r);

template <unsigned NSIZE>
unsigned long long merge_seq(unsigned(&a)[NSIZE], unsigned l, unsigned m, unsigned r);

template <unsigned NSIZE>
unsigned long long  my_random_generator(unsigned(&a)[NSIZE])
{
	for (unsigned i = 0; i != NSIZE; ++i)
		a[i] = i;
	std::default_random_engine e;
	e.seed((unsigned)time(NULL));
	for (unsigned i = 0; i != NSIZE; ++i)
		std::swap(a[e() % NSIZE], a[e() % NSIZE]);
	unsigned b[NSIZE];
	memcpy_s(b, NSIZE * sizeof(unsigned), a, NSIZE * sizeof(unsigned));
	return get_DD_of_seq(b, 0, NSIZE);
}

template <unsigned NSIZE>
unsigned long long get_DD_of_seq(unsigned(&a)[NSIZE], unsigned l, unsigned r)
{
	if (l + 1 < r)
		return get_DD_of_seq(a, l, (l + r) / 2) + get_DD_of_seq(a, (l + r) / 2, r) + merge_seq(a, l, (l + r) / 2, r);
	return 0;
}

template <unsigned NSIZE>
unsigned long long merge_seq(unsigned(&a)[NSIZE], unsigned l, unsigned m, unsigned r)
{
	unsigned i = l;
	unsigned j = m;
	unsigned long long dd = 0;
	unsigned k = 0;
	unsigned b[NSIZE];
	while (i < m && j < r)
		if (a[i] < a[j])
			b[k] = a[i], ++k, ++i;
		else
			b[k] = a[j], dd += m - i, ++k, ++j;
	while (i < m)
		b[k++] = a[i++];
	while (j < r)
		b[k++] = a[j++];
	memcpy_s(a + l, (r - l) * sizeof(unsigned), b, k * sizeof(unsigned));
	return dd;
}
