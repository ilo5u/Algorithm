#include <cstdio>
#include <algorithm>
#include <vector>

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

// 选取当前最优剖分线段
void partition(const std::vector<int>& node_vec, double dist[][MAX_NUM], std::vector<std::pair<int, int>>& connect_vec)
{
	int size = (int)node_vec.size();
	if (size < 4)
		return;

	double min_dist = INT_MAX;
	int nodei = 0;
	int nodej = 0;
	for (int i = 0; i < size; ++i)
		for (int j = 2; j < size - 1; ++j)
			if (dist[node_vec[i]][node_vec[(i + j) % size]] < min_dist)
			{
				min_dist = dist[node_vec[i]][node_vec[(i + j) % size]];
				nodei = i;
				nodej = (i + j) % size;
			}
	connect_vec.push_back({ node_vec[nodei], node_vec[nodej] });

	std::vector<int> node_first;
	for (int i = nodei; i <= nodej; ++i)
		node_first.push_back(node_vec[i]);
	partition(node_first, dist, connect_vec);

	std::vector<int> node_second;
	for (int i = 0; i <= nodei; ++i)
		node_second.push_back(node_vec[i]);
	for (int i = nodej; i < size; ++i)
		node_second.push_back(node_vec[i]);
	partition(node_second, dist, connect_vec);
}

int main(int argc, char* argv[])
{
	station station_info[MAX_NUM];

	auto rad = [](double lat_or_lng) {
		return lat_or_lng * PI / 180.0;
	}; // 转化为弧度

	auto get_distance = [&station_info, rad](int x, int y) {
		double rad_lngx = rad(station_info[x].lng);
		double rad_latx = rad(station_info[x].lat);
		double rad_lngy = rad(station_info[y].lng);
		double rad_laty = rad(station_info[y].lat);
		double dist = std::acos(std::cos(rad_latx) * std::cos(rad_laty) *
			std::cos(rad_lngx - rad_lngy) + std::sin(rad_latx) * std::sin(rad_laty));
		return std::round(dist * EARTH_RADIUS * 1000.0);
	}; // 获取x, y基站的距离

	auto test = [&] (const char szOpenFileName[]) {
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
			&station_info[station_cnt].id, &station_info[station_cnt].lng, &station_info[station_cnt].lat, &station_info[station_cnt].num) != EOF)
			++station_cnt;

		// 邻接矩阵
		double dist[MAX_NUM][MAX_NUM];
		for (int i = 0; i < station_cnt; ++i)
			for (int j = 0; j < station_cnt; ++j)
				dist[i][j] = get_distance(i, j);

		std::vector<std::pair<int, int>> connect_vec;
		for (int i = 0; i < station_cnt; ++i)
			connect_vec.push_back({ i , (i + 1) % station_cnt });
		std::vector<int> node_vec;
		for (int i = 0; i < station_cnt; ++i)
			node_vec.push_back(i);

		partition(node_vec, dist, connect_vec);

		double res = 0.0;
		for (std::vector<std::pair<int, int>>::const_iterator it = connect_vec.begin(); it != connect_vec.end(); ++it)
			res += 2 * dist[it->first][it->second], printf_s("pair : (%d, %d)\n", it->first, it->second);

		fclose(lpRead);

		return res;
	};

	printf_s("test1: the weight = %lf\n", test("附件3-1.21个基站凸多边形数据2017.txt"));
	printf_s("\n");
	printf_s("test2: the weight = %lf\n", test("附件3-2.29个基站凸多边形数据2017.txt"));

	return 0;
}