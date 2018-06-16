#include <cstdio>
#include <algorithm>
#include <vector>
#include <stack>
#include <map>
#include <chrono>

#define MAX_LOADSTRING 100
#define MAX_COST 99999.0

constexpr int max_num = 22;
constexpr int sour = 19;

// #define DEBUG

typedef struct _Station
{
	int seq;
	int id;
} station, *station_ptr;

typedef struct _Elist
{
	int fromv;
	int endv;
	double dist;
} elist, *elist_ptr;

typedef std::vector<elist> elist_table[max_num];

typedef struct _Node
{
	std::vector<int> path;
	double lbound;
} node, *node_ptr;

int main(int argc, char* argv[])
{
	auto test = [](const char szOpenFileName[]) {
		FILE* lpRead = nullptr;
		fopen_s(&lpRead, szOpenFileName, "r");

		station station[max_num];
		std::map<int, int> seq_to_seq;
		for (int i = 0; i < max_num; ++i)
		{
			fscanf_s(lpRead, "%d", &station[i].seq);
			seq_to_seq[i] = station[i].seq;
		}

		for (int i = 0; i < max_num; ++i)
			fscanf_s(lpRead, "%d", &station[i].id);

		elist_table et;
		double dist_matrix[max_num][max_num];
		for (int i = 0; i < max_num; ++i)
		{
			fscanf_s(lpRead, "%d%d", &station[i].seq, &station[i].id);
			for (int j = 0; j < max_num; ++j)
			{
				fscanf_s(lpRead, "%lf", &dist_matrix[i][j]);

				if (dist_matrix[i][j] < MAX_COST)
					et[i].push_back({ i, j, dist_matrix[i][j] });
			}
			std::sort(et[i].begin(), et[i].end(), [](const elist& l, const elist& r) {
				return l.dist < r.dist;
			});
		}

		// 下界
		auto get_lower_bound = [&et, &dist_matrix](std::vector<int>& path)
		{
			bool visit[max_num] = { false };
			double lower_bound = 0.0;

			for (int i = 0; i < path.size(); ++i)
			{
				visit[path[i]] = true;
				if (i < path.size() - 1)
					lower_bound += 2.0 * dist_matrix[path[i]][path[i + 1]];
			}

			if (path.size() > 1)
			{ // 已存在某条路径
				std::vector<elist>::const_iterator it = et[path.front()].begin();
				while (it != et[path.front()].end() && visit[it->endv])
					++it;
				if (it == et[path.front()].end())
					return (double)INT_MAX;
				lower_bound += it->dist;

				it = et[path.back()].begin();
				while (it != et[path.back()].end() && visit[it->endv])
					++it;
				if (it == et[path.back()].end())
					return (double)INT_MAX;
				lower_bound += it->dist;
			}

			for (int i = 0; i < max_num; ++i)
				if (!visit[i])
				{ // 非路径节点选两条最短的相邻路径（注意另一节点只能为非路径节点或者路径两端节点）
					std::vector<elist>::const_iterator it = et[i].begin();
					while (it != et[i].end() && visit[it->endv] && it->endv != path.back() && it->endv != path.front())
						++it;
					if (it == et[i].end())
						return (double)INT_MAX; // 不能构成回路
					lower_bound += it->dist;

					++it;
					while (it != et[i].end() && visit[it->endv] && it->endv != path.front() && it->endv != path.back())
						++it;
					if (it == et[i].end())
						return (double)INT_MAX; // 不能构成回路
					lower_bound += it->dist;
				}

			return lower_bound / 2.0;
		};

		// 上界（默认使用Dirac定理+贪心策略）
		auto get_higher_bound = [&et, &dist_matrix](int sour)
		{
			bool visit[max_num + 1] = { false };
			visit[sour] = true;

#ifdef GREEDY_DFS // DFS+贪心策略
			std::stack<elist> elist_stack;
			std::stack<std::vector<elist>::const_iterator> it_track;
			elist_stack.push({ max_num, sour, 0.0 });
			it_track.push(et[elist_stack.top().endv].begin());
			while (elist_stack.size() < max_num + 1)
			{
				int fromv = elist_stack.top().endv;
				bool back = true;
				for (std::vector<elist>::const_iterator it = it_track.top(); it != et[fromv].end(); ++it)
				{
					if (elist_stack.size() == max_num)
					{
						if (it->endv == sour)
						{
							elist_stack.push(*it);
							back = false;
							break;
						}
					}
					else
					{
						if (!visit[it->endv] && it->endv != sour)
						{
							visit[it->endv] = true;

							elist_stack.push(*it);
							it_track.pop();
							it_track.push(it + 1);
							it_track.push(et[it->endv].begin());
							back = false;
							break;
						}
					}
				}
				if (back)
				{
					visit[fromv] = false;

					elist_stack.pop();
					it_track.pop();
				}
			}

			double higher_bound = 0.0;
			while (!elist_stack.empty())
			{
				higher_bound += elist_stack.top().dist;
				elist_stack.pop();
			}
#else
			std::deque<int> hamiton;
			hamiton.push_back(sour);

			hamiton.push_back(et[sour][0].endv);
			visit[et[sour][0].endv] = true;

			while (true)
			{
				while (true)
				{ // expand to the left
					int fromv = hamiton.front();
					bool flag = true;
					for (std::vector<elist>::const_iterator it = et[fromv].begin();
						it != et[fromv].end(); ++it)
						if (!visit[it->endv])
						{
							hamiton.push_front(it->endv);
							visit[it->endv] = true;
							flag = false;
							break;
						}
					if (flag)
						break;
				}
				while (true)
				{ // expand to the right
					int fromv = hamiton.back();
					bool flag = true;
					for (std::vector<elist>::const_iterator it = et[fromv].begin();
						it != et[fromv].end(); ++it)
						if (!visit[it->endv])
						{
							hamiton.push_back(it->endv);
							visit[it->endv] = true;
							flag = false;
							break;
						}
					if (flag)
						break;
				}
				int leftv = hamiton.front();
				int rightv = hamiton.back();
				size_t pos = 0;
				double min_dist = MAX_COST;
				if (std::fabs(dist_matrix[leftv][rightv] - MAX_COST) < 1e-6)
				{ // front and back are not adjacent
					for (size_t i = 1; i < hamiton.size() - 2; ++i)
						if (dist_matrix[hamiton[i]][rightv] + dist_matrix[hamiton[i + 1]][leftv]
							< min_dist)
						{
							pos = i;
							min_dist =
								dist_matrix[hamiton[i]][rightv] + dist_matrix[hamiton[i + 1]][leftv];
						}

					if (std::fabs(min_dist - MAX_COST) < 1e-6)
						return (double)INT_MAX;
					std::reverse(hamiton.begin() + pos + 1, hamiton.end()); // adjacent now
				}

				if (hamiton.size() == max_num)
					break; // hamiton circuit found

				// cut the loop as a path
				int k = 0;
				int cutv = 0;
				min_dist = MAX_COST;
				for (size_t i = 0; i < hamiton.size(); ++i)
					for (std::vector<elist>::const_iterator it = et[hamiton[i]].begin();
						it != et[hamiton[i]].end(); ++it)
						if (!visit[it->endv])
						{
							if (it->dist < min_dist)
							{
								cutv = it->endv;
								pos = i;
								min_dist = it->dist;
							}
							break;
						}

				if (std::fabs(min_dist - MAX_COST) < 1e-6)
					return (double)INT_MAX;

				visit[cutv] = true;
				std::reverse(hamiton.begin(), hamiton.begin() + pos + 1);
				std::reverse(hamiton.begin() + pos + 1, hamiton.end());
				hamiton.push_front(cutv); // the path is cutv->leftv->rightv->cutv+1
			}

			double higher_bound = 0.0;
			for (int i = 0; i < max_num; ++i)
				higher_bound += dist_matrix[hamiton[i]][hamiton[(i + 1) % max_num]];
#endif // GREEDY_DFS

			return higher_bound;
		};

		std::vector<int> path;
		path.push_back(sour);

		double lower_bound = get_lower_bound(path);
		double higher_bound = (double)INT_MAX;
		for (int i = 0; i < max_num; ++i)
			higher_bound = std::min(get_higher_bound(i), higher_bound);

		printf_s("The initial bound is [%lf, %lf].\n", lower_bound, higher_bound);

		std::vector<int> tsp_path;
		double tsp_dist = (double)INT_MAX;
		std::vector<node> alive_node;
		alive_node.push_back({ path, lower_bound });

		std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();

		int node_count = 0;
		while (!alive_node.empty())
		{
			std::pop_heap(alive_node.begin(), alive_node.end(), [](const node& l, const node& r) {
				return l.lbound > r.lbound;
			});
			node cur = alive_node.back();
			alive_node.pop_back();
			++node_count;

#ifdef DEBUG
			printf_s("path: ");
			for (int i = 0; i < cur.path.size() - 1; ++i)
				printf_s("%d->", cur.path[i]);
			printf_s("%d lb: %lf level: %d\n", cur.path.back(), cur.lbound, cur.path.size());
#endif // DEBUG

			bool visit[max_num] = { false };
			for (int i = 0; i < cur.path.size(); ++i)
				visit[cur.path[i]] = true;

			int fromv = cur.path.back();
			for (std::vector<elist>::const_iterator it = et[fromv].begin(); it != et[fromv].end(); ++it)
			{
				if (!visit[it->endv])
				{
					if (cur.path.size() == max_num - 1)
					{ // max_num stations have been chosen
						if (std::fabs(dist_matrix[it->endv][cur.path.front()] - MAX_COST) < 1e-6)
						{ // is not a loop
							// just remove the node
						}
						else
						{ // is a loop
							cur.path.push_back(it->endv);
							double lb = 0.0;
							for (int i = 0; i < cur.path.size(); ++i)
								lb += dist_matrix[cur.path[i]][cur.path[(i + 1) % max_num]];

							bool flag = true;
							for (int i = 0; i < alive_node.size(); ++i)
								if (lb > alive_node[i].lbound)
									flag = false;
							if (flag)
							{ // the lower bound is the minist
								tsp_path = cur.path;
								tsp_dist = lb;
								alive_node.clear(); // to break the while loop
								break;
							}
							else
								higher_bound = lb; // adjust the higher bound to cut nodes
						}
					}
					else
					{
						std::vector<int> new_path{ cur.path };
						new_path.push_back(it->endv);

						double lb = get_lower_bound(new_path);
						if (!(lb > higher_bound))
						{ // new alive node
							alive_node.push_back({ new_path, lb });
							std::push_heap(alive_node.begin(), alive_node.end(), [](const node& l, const node& r) {
								return l.lbound > r.lbound;
							});
						}
					}
				}
			}

			while (!alive_node.empty() && alive_node.front().lbound > higher_bound)
			{
				std::pop_heap(alive_node.begin(), alive_node.end(), [](const node& l, const node& r) {
					return l.lbound > r.lbound;
				});
				alive_node.pop_back();
			}
		}

		std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();

		std::chrono::steady_clock::duration dur = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
		printf_s("Figuring out the TSP path used %lf ms.\n", static_cast<double>(static_cast<double>(dur.count()) * std::chrono::microseconds::period::num / std::chrono::microseconds::period::den));

		printf_s("\nElist set is :\n");
		for (int i = 0; i < max_num; ++i)
			printf_s("%2d->%2d : %lf\n", seq_to_seq[tsp_path[i]], seq_to_seq[tsp_path[(i + 1) % max_num]], dist_matrix[tsp_path[i]][tsp_path[(i + 1) % max_num]]);
		printf_s("The Hamiton circuit's weight is %lf.\n", tsp_dist);
		printf_s("ANT handled %d nodes in total.\n", node_count);
	};

	char szOpenFileName[MAX_LOADSTRING];
	sprintf_s(szOpenFileName, "附件1-2.基站图的邻接矩阵-v4-20180601-%d.txt", max_num);
	test(szOpenFileName);

	return 0;
}