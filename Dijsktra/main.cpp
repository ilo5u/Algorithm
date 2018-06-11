#include <cstdio>
#include <algorithm>
#include <map>
#include <stack>

#define MAX_NUM 30

typedef struct _Station
{
	int seq;
	int id;
} station, *station_ptr;

int main(int argc, char* argv[])
{
	station station[MAX_NUM];
	double dbDist[MAX_NUM][MAX_NUM];
	auto test = [&](const char szOpenFileName[]) {
		FILE* lpRead = nullptr;
		fopen_s(&lpRead, "附件1-1.基站图的邻接矩阵-v1.txt", "r");

		int station_cnt = 22;
		for (int i = 1; i <= station_cnt; ++i)
			fscanf_s(lpRead, "%d", &station[i].seq);
		for (int i = 1; i <= station_cnt; ++i)
			fscanf_s(lpRead, "%d", &station[i].id);
		std::map<int, int> id_to_seq;
		for (int i = 1; i <= station_cnt; ++i)
		{
			fscanf_s(lpRead, "%d%d", &station[i].seq, &station[i].id);
			id_to_seq[station[i].id] = station[i].seq;
			for (int j = 1; j <= station_cnt; ++j)
				fscanf_s(lpRead, "%lf", &dbDist[i][j]);
		}

		double dbRes[MAX_NUM];
		for (int i = 1; i <= station_cnt; ++i)
			dbRes[i] = INT_MAX;
		dbRes[id_to_seq[567443]] = 0;

		bool bVisit[MAX_NUM];
		for (int i = 1; i <= station_cnt; ++i)
			bVisit[i] = false;

		int iPath[MAX_NUM];
		for (int i = 1; i <= station_cnt; ++i)
			iPath[i] = 0;

		for (int k = 1; k < station_cnt; ++k)
		{
			double min = INT_MAX;
			int v = id_to_seq[567443];
			for (int j = 1; j <= station_cnt; ++j)
				if (!bVisit[j] && dbRes[j] < min)
				{
					min = dbRes[j];
					v = j;
				}

			bVisit[v] = true;
			for (int j = 1; j <= station_cnt; ++j)
				if (!bVisit[j] && dbDist[v][j] > 0 && dbRes[v] + dbDist[v][j] < dbRes[j])
					dbRes[j] = dbRes[v] + dbDist[v][j], iPath[j] = v;
		}

		for (int i = 1; i <= station_cnt; ++i)
		{
			printf_s("ID = %2d 567443 to %6d : %lf\t", i, station[i].id, dbRes[i]);

			int cur = i;
			std::stack<int> path_stack;
			while (iPath[cur] != 0)
				path_stack.push(cur), cur = iPath[cur];

			printf_s("Path = %d", id_to_seq[567443]);
			while (!path_stack.empty())
				printf_s("->%d", path_stack.top()), path_stack.pop();
			printf_s("\n");
		}

		fclose(lpRead);
	};
	test("附件1-1.基站图的邻接矩阵-v1.txt");
	return 0;
}