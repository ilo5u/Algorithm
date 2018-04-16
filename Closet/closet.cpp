#include <algorithm>
#include <cmath>
#include <cstdio>
#include <Windows.h>
#include <set>

static const double EARTH_RADIUS = 6378.137;
static const double PI = 3.1415926;
static const unsigned MAXN = 3000;

double rad(double lat_or_lng)
{
	return lat_or_lng * PI / 180.0;
}

typedef struct _DPOINT
{
	unsigned id;
	double lng;
	double lat;

	bool operator==(const _DPOINT& r) const
	{
		return lng == r.lng && lat == r.lat;
	}

	bool operator<(const _DPOINT& r) const
	{
		return lng < r.lng || (lng == r.lng && lat == r.lat);
	}
} DPOINT, *LPDPOINT;

typedef struct _DPOINTPAIR
{
	DPOINT a;
	DPOINT b;
	double dist;

	bool operator==(const _DPOINTPAIR& r) const
	{
		return dist == r.dist;
	}

	bool operator<(const _DPOINTPAIR& r) const
	{
		return dist < r.dist;
	}
} DPOINTPAIR, *LPDPOINTPAIR;

typedef struct _RESULT
{
	DPOINTPAIR first;
	DPOINTPAIR second;
} RESULT, *LPRESULT;

double get_distance(const DPOINT& a, const DPOINT& b);

template <unsigned NSIZE>
RESULT closet_pair(DPOINT(&x)[NSIZE], DPOINT(&y)[NSIZE], DPOINT(&z)[NSIZE], unsigned l, unsigned r);

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

		DPOINT point[MAXN];
		unsigned cnt = 0;
		double dist;
		std::set<DPOINT> point_set;
		while (fscanf_s(lpRead, "%u\t%lf\t%lf\t%lf\n", &point[cnt].id, &point[cnt].lng, &point[cnt].lat, &dist) != EOF)
			point_set.insert(point[cnt]), ++cnt;
		cnt = 0;
		for (std::set<DPOINT>::const_iterator it = point_set.begin(); it != point_set.end(); ++it, ++cnt)
			point[cnt] = *it;

		DPOINT point_x[MAXN];
		memcpy_s(point_x, cnt * sizeof(DPOINT), point, cnt * sizeof(DPOINT));
		std::sort(point_x, point_x + cnt, [](const DPOINT& a, const DPOINT& b)->bool {
			return a.lng < b.lng || (a.lng == b.lng && a.lat < b.lat);
		});

		DPOINT point_y[MAXN];
		memcpy_s(point_y, cnt * sizeof(DPOINT), point, cnt * sizeof(DPOINT));
		std::sort(point_y, point_y + cnt, [](const DPOINT& a, const DPOINT& b)->bool {
			return a.lat < b.lat || (a.lat == b.lat && a.lng < b.lng);
		});
		 
		RESULT res = closet_pair(point_x, point_y, point, 0, cnt);
		printf_s("最近点对:\nID=%10d\tLNG=%10lf\tLAT=%10lf\nID=%10d\tLNG=%10lf\tLAT=%10lf\nDIS=%10lf\n",
			res.first.a.id, res.first.a.lng, res.first.a.lat,
			res.first.b.id, res.first.b.lng, res.first.b.lat,
			res.first.dist
		);

		printf_s("次近点对:\nID=%10d\tLNG=%10lf\tLAT=%10lf\nID=%10d\tLNG=%10lf\tLAT=%10lf\nDIS=%10lf\n",
			res.second.a.id, res.second.a.lng, res.second.a.lat,
			res.second.b.id, res.second.b.lng, res.second.b.lat,
			res.second.dist
		);
	}
	return 0;
}

template <unsigned NSIZE>
RESULT closet_pair(DPOINT(&x)[NSIZE], DPOINT(&y)[NSIZE], DPOINT(&z)[NSIZE], unsigned l, unsigned r)
{
	if (r - l == 2)
		return { {x[l], x[r - 1], get_distance(x[l], x[r - 1])}, {{UINT_MAX, UINT_MAX, UINT_MAX}, {UINT_MAX, UINT_MAX, UINT_MAX}, ULONG_MAX} };
	if (r - l == 3)
	{
		DPOINTPAIR pair01 = { x[l], x[l + 1], get_distance(x[l], x[l + 1]) };
		DPOINTPAIR pair02 = { x[l], x[r - 1], get_distance(x[l], x[r - 1]) };
		DPOINTPAIR pair12 = { x[l + 1], x[r - 1], get_distance(x[l + 1], x[r - 1]) };

		std::set<DPOINTPAIR> pair_set{ pair01, pair02, pair12 };
		RESULT result;
		result.first = pair_set[0];
		if (pair_set.size() < 2)
			result.second = { { UINT_MAX, UINT_MAX, UINT_MAX },{ UINT_MAX, UINT_MAX, UINT_MAX }, ULONG_MAX };
		else
			result.second = pair_set[1];
		return result;
	}
	else
	{
		unsigned m = (l + r) / 2;
		memcpy_s(z + l, (r - l) * sizeof(DPOINT), x + l, (r - l) * sizeof(DPOINT));

		std::sort(z + l, z + m, [](const DPOINT& a, const DPOINT& b)->bool {
			return a.lat < b.lat || (a.lat == b.lat && a.lng < b.lng);
		});
		std::sort(z + m, z + r, [](const DPOINT& a, const DPOINT& b)->bool {
			return a.lat < b.lat || (a.lat == b.lat && a.lng < b.lng);
		});

		RESULT l_result = closet_pair(x, z, y, l, m);
		RESULT r_result = closet_pair(x, z, y, m, r);

		DPOINTPAIR pair[4] = { l_result.first, l_result.second, r_result.first, r_result.second };
		std::sort(pair, pair + 4, [](const DPOINTPAIR& a, const DPOINTPAIR& b)->bool {
			return a.dist < b.dist;
		});
		RESULT result = { pair[0], pair[1] };

		memcpy_s(y + l, (r - l) * sizeof(DPOINT), z + l, (r - l) * sizeof(DPOINT));
		std::sort(y + l, y + r, [](const DPOINT& a, const DPOINT& b)->bool {
			return a.lat < b.lat || (a.lat == b.lat && a.lng < b.lng);
		});
		unsigned k = l;
		for (unsigned i = l; i < r; ++i)
			if (std::fabs(x[m].lng - y[i].lng) < result.second.dist)
				z[k++] = y[i];
		for (unsigned i = l; i < k; ++i)
			for (unsigned j = i + 1; j < k && z[j].lat - z[i].lat < result.second.dist; ++j)
			{
				double dist = get_distance(z[i], z[j]);
				if (dist < result.first.dist)
					result.second = result.first, result.first = { z[i], z[j], dist };
				else if (dist < result.second.dist)
					result.second = { z[i], z[j], dist };
			}
		return result;
	}
}

double get_distance(const DPOINT& a, const DPOINT& b)
{
	double rad_lnga = rad(a.lng);
	double rad_lata = rad(a.lat);
	double rad_lngb = rad(b.lng);
	double rad_latb = rad(b.lat);
	double dist = std::acos(std::cos(rad_lata) * std::cos(rad_latb) *
		std::cos(rad_lnga - rad_lngb) + std::sin(rad_lata) * std::sin(rad_latb));
	return std::round(dist * EARTH_RADIUS * 1000.0);
}