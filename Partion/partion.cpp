#include <cstdio>
#include <algorithm>

#define MAX_NUM        40
#define MAX_LOADSTRING 100

#define EARTH_RADIUS 6378.137
#define PI           3.1415926

// 基站信息记录
typedef struct _Station
{
	int    id;
	double lng;
	double lat;
	int    num;
} station;

// 构造点与点之间的连接
void construct(int (&connect)[MAX_NUM][MAX_NUM], int l, int r)
{
	if (connect[l][r] >= l - 1 && connect[l][r] <= r)
	{
		printf_s("pair: (%d, %d, %d)\n", l - 1, r, connect[l][r]);
		construct(connect, l, connect[l][r]);
		construct(connect, connect[l][r] + 1, r);
	}
}

int main(int argc, char* argv[])
{
	auto rad = [](double lat_or_lng) {
		return lat_or_lng * PI / 180.0;
	}; // 转化为弧度

	station station_set[MAX_NUM];
	auto get_distance = [&station_set, rad](int x, int y) {
		double rad_lngx = rad(station_set[x].lng);
		double rad_latx = rad(station_set[x].lat);
		double rad_lngy = rad(station_set[y].lng);
		double rad_laty = rad(station_set[y].lat);
		double dist = std::acos(std::cos(rad_latx) * std::cos(rad_laty) *
			std::cos(rad_lngx - rad_lngy) + std::sin(rad_latx) * std::sin(rad_laty));
		return std::round(dist * EARTH_RADIUS * 1000.0);
	}; // 获取x, y基站的距离

	auto get_wgt = [get_distance](int a, int b, int c) {
		return get_distance(a, b) + get_distance(b, c) + get_distance(a, c);
	}; // 获取a, b, c三个基站构成的三角形的周长

	auto test = [&station_set, get_wgt](const char szOpenFileName[]) {
		FILE *lpRead = nullptr;
		fopen_s(&lpRead, szOpenFileName, "r");

		char szID[MAX_LOADSTRING];
		char szLNG[MAX_LOADSTRING];
		char szLAT[MAX_LOADSTRING];
		char szNUM[MAX_LOADSTRING];
		fscanf_s(lpRead,
			"%s\t%s\t%s\t%s\n", 
			szID, MAX_LOADSTRING, szLNG, MAX_LOADSTRING, szLAT, MAX_LOADSTRING, szNUM, MAX_LOADSTRING);

		int station_cnt = 0;
		while (fscanf_s(lpRead, 
			"%d\t%lf\t%lf\t%d\n", 
			&station_set[station_cnt].id, &station_set[station_cnt].lng, &station_set[station_cnt].lat, &station_set[station_cnt].num) != EOF)
			++station_cnt;
		fclose(lpRead);

		double wgt[MAX_NUM][MAX_NUM];
		int    connect[MAX_NUM][MAX_NUM];
		for (int i = 0; i <= station_cnt; ++i)
			for (int j = 0; j <= station_cnt; ++j)
				wgt[i][j] = INT_MAX;
		for (int i = 1; i < station_cnt; ++i)
			wgt[i][i] = 0;
		for (int len = 2; len < station_cnt; ++len)
		{
			for (int l = 1; l < station_cnt - len + 1; ++l)
			{
				int r = len + l - 1;
				wgt[l][r] = wgt[l + 1][r] + get_wgt(l - 1, l, r);
				connect[l][r] = l;
				for (int m = l + 1; m < r; ++m)
				{
					if (wgt[l][m] + wgt[m + 1][r] + get_wgt(l - 1, m, r) < wgt[l][r])
					{
						wgt[l][r] = wgt[l][m] + wgt[m + 1][r] + get_wgt(l - 1, m, r);
						connect[l][r] = m;
					}
				}
			}
		}

		construct(connect, 1, station_cnt - 1);
		return wgt[1][station_cnt - 1];
	};

	printf_s("test1: the weight = %lf\n", test("附件3-1.21个基站凸多边形数据2017.txt"));
	printf_s("\n");
	printf_s("test2: the weight = %lf\n", test("附件3-2.29个基站凸多边形数据2017.txt"));

	return 0;
}