#include <cstdio>
#include <vector>
#include <stack>
#include <algorithm>
#include <map>

#define MAX_NUM 50

// #define KRUSKAL // 采用KRUSKAL算法 否则采用PRIM算法

// 边信息
typedef struct _Elist
{
	int u;
	int v;
	double dist;
} elist, *elist_ptr;

typedef struct _Station
{
	int seq;
	int id;
} station, *station_ptr;

// KRUSKAL并查集操作
#ifdef KRUSKAL
int get_father(int k, int(&father)[MAX_NUM]) {
	return father[k] == k ? k : father[k] = get_father(father[k], father);
};

void union_node(int a, int b, int(&father)[MAX_NUM]) {
	int fathera = get_father(a, father);
	int fatherb = get_father(b, father);
	if (fathera != fatherb)
		father[fatherb] = fathera;
};
#endif // KRUSKAL

int main(int argc, char* argv[])
{
	station station[MAX_NUM];
	double dbDist[MAX_NUM][MAX_NUM];
	auto test = [&](const char szOpenFileName[], int station_cnt) {
		FILE* lpRead = nullptr;
		fopen_s(&lpRead, szOpenFileName, "r");

		for (int i = 1; i <= station_cnt; ++i)
			fscanf_s(lpRead, "%d", &station[i].seq);
		for (int i = 1; i <= station_cnt; ++i)
			fscanf_s(lpRead, "%d", &station[i].id);
		std::map<int, int> id_to_seq;

#ifdef KRUSKAL
		std::vector<elist> e_vec;
#else
		std::vector<elist> e_heap; // PRIM算法用堆搜索最近点
#endif // KRUSKAL

		for (int i = 1; i <= station_cnt; ++i)
		{
			fscanf_s(lpRead, "%d%d", &station[i].seq, &station[i].id);
			id_to_seq[station[i].id] = station[i].seq;
			for (int j = 1; j <= station_cnt; ++j)
			{
				fscanf_s(lpRead, "%lf", &dbDist[i][j]);
#ifdef KRUSKAL
				if (i < j && dbDist[i][j] > 0)
					e_vec.push_back({ i, j, dbDist[i][j] });
#endif // KRUSKAL
			}
		}

		std::vector<elist> e_minitree; // 生成树边记录
#ifdef KRUSKAL
		std::sort(e_vec.begin(), e_vec.end(), [](const elist& l, const elist& r) {
			return l.dist < r.dist;
		});

		int father[MAX_NUM];
		for (int i = 1; i <= station_cnt; ++i)
			father[i] = i;

		for (std::vector<elist>::const_iterator it = e_vec.begin(); it != e_vec.end() && e_minitree.size() < station_cnt - 1; ++it)
			if (get_father(it->u, father) != get_father(it->v, father))
			{
				union_node(it->u, it->v, father);
				e_minitree.push_back({ it->u, it->v, it->dist });
			}
#else
		bool b_visit[MAX_NUM];
		for (auto& elem : b_visit)
			elem = false;
		b_visit[1] = true;
		for (int i = 1; i <= station_cnt; ++i)
			if (dbDist[1][i] > 0)
				e_heap.push_back({ 1, i, dbDist[1][i] });
		std::make_heap(e_heap.begin(), e_heap.end(), [](const elist& l, const elist& r) {
			return l.dist > r.dist;
		});

		while (!e_heap.empty() && e_minitree.size() < station_cnt - 1) 
		{
			int v = e_heap.front().v;
			e_minitree.push_back(e_heap.front());
			std::pop_heap(e_heap.begin(), e_heap.end(), [](const elist& l, const elist& r) {
				return l.dist > r.dist;
			});
			e_heap.pop_back();

			b_visit[v] = true;
			for (int i = 1; i <= station_cnt; ++i)
				if (!b_visit[i] && dbDist[v][i] > 0)
				{
					e_heap.push_back({ v, i, dbDist[v][i] });
					std::push_heap(e_heap.begin(), e_heap.end(), [](const elist& l, const elist& r) {
						return l.dist > r.dist;
					});
				}
			while (!e_heap.empty() && b_visit[e_heap.front().v])
			{
				std::pop_heap(e_heap.begin(), e_heap.end(), [](const elist& l, const elist& r) {
					return l.dist > r.dist;
				});
				e_heap.pop_back();
			}
		}
#endif // KRUSKAL

		double total = 0.0;
		printf_s("The minimal spanning tree is :\n");
		for (std::vector<elist>::const_iterator it = e_minitree.begin(); it != e_minitree.end(); ++it)
		{
			printf_s("(%2d, %2d) : %lf\n", it->u, it->v, it->dist);
			total += it->dist;
		}
		printf_s("The total weight is %lf.\n", total);

		fclose(lpRead);
	};
	printf_s("test1 : 42 stations\n");
	test("附件1-1.基站图的邻接矩阵-v1.txt", 42);

	printf_s("\ntest2 : 22 stations\n");
	test("附件1-1.基站图的邻接矩阵-v2.txt", 22);
	return 0;
}