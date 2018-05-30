#include <cstdio>
#include <algorithm>

typedef struct _Item
{
	int weight;
	int value;

	_Item() :
		weight(0x0), value(0x0)
	{ }
} item;

int main(int argc, char* argv[])
{
	FILE *pRead;
	freopen_s(&pRead, "附件4.背包问题输入数据2017.txt", "r", stdin);

	auto test = [](int item_cnt) {
		int item_capacity;
		scanf_s("%d", &item_capacity);

		const int max_item_cnt = 100;
		item item[max_item_cnt + 1];
		for (int i = 1; i <= item_cnt; ++i)
			scanf_s("%d", &item[i].weight);
		for (int i = 1; i <= item_cnt; ++i)
			scanf_s("%d", &item[i].value);

		const int max_item_capacity = 1000;
		int res[max_item_cnt + 1][max_item_capacity];
		[&res, &item_cnt, &item_capacity] {
			for (int i = 0; i <= item_cnt; ++i)
				for (int j = 0; j <= item_capacity; ++j)
					res[i][j] = 0;
		}();

		printf_s("The max value is: %d\n", [&res, &item, &item_cnt, &item_capacity] {
			for (int i = 1; i <= item_cnt; ++i)
				for (int j = item[i].weight; j <= item_capacity; ++j)
					if (res[i - 1][j - item[i].weight] + item[i].value >= res[i - 1][j])
						res[i][j] = res[i - 1][j - item[i].weight] + item[i].value;
					else
						res[i][j] = res[i - 1][j];

			return res[item_cnt][item_capacity];
		}());

		printf_s("The plan is:\n");
		[&res, &item, &item_cnt, &item_capacity] {
			int cur_item_id = item_cnt;
			int cur_backpack_wgt = item_capacity;
			while (cur_item_id > 0)
				if (res[cur_item_id][cur_backpack_wgt] == res[cur_item_id - 1][cur_backpack_wgt])
					printf_s("id: %d is not chosen.\n", cur_item_id--);
				else
					cur_backpack_wgt -= item[cur_item_id].weight, printf_s("id: %d is chosen.\n", cur_item_id--);
		}();
	};

	printf_s("Test 1:\n");
	test(50);

	printf_s("Test 2:\n");
	test(100);

	return 0;
}