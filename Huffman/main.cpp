#include <cstdio>
#include <cctype>
#include <cstring>
#include <vector>
#include <algorithm>
#include <map>

#define MAX_LOADSTRING 3000

typedef struct _Node
{
	std::pair<char, int> rec;
	_Node* lpLeft;
	_Node* lpRight;
} node, *node_ptr;

// 构建哈夫曼树
node_ptr build_huffman_tree(const std::map<char, int>& rec)
{
	std::vector<node_ptr> huffman_forest_heap; // 小顶堆每个元素为森林
	for (std::map<char, int>::const_iterator it = rec.begin(); it != rec.end(); ++it)
		huffman_forest_heap.push_back(new node{ *it, nullptr, nullptr });
	std::make_heap(huffman_forest_heap.begin(), huffman_forest_heap.end(), [](const node_ptr& lpLeft, const node_ptr& lpRight) {
		return lpLeft->rec.second > lpRight->rec.second;
	});

	for (std::vector<node_ptr>::iterator it_end = huffman_forest_heap.end(); it_end != huffman_forest_heap.begin() + 1;)
	{ // 直到堆中只剩下一个森林
		std::pop_heap(huffman_forest_heap.begin(), it_end, [](const node_ptr& lpLeft, const node_ptr& lpRight) {
			return lpLeft->rec.second > lpRight->rec.second;
		});
		std::pop_heap(huffman_forest_heap.begin(), --it_end, [](const node_ptr& lpLeft, const node_ptr& lpRight) {
			return lpLeft->rec.second > lpRight->rec.second;
		});

		// 合并森林
		*(it_end - 1) = new node{ std::pair<char, int>{ 0x0, (*(it_end - 1))->rec.second + (*it_end)->rec.second },
			*(it_end - 1), *it_end };
		std::push_heap(huffman_forest_heap.begin(), it_end, [](const node_ptr& lpLeft, const node_ptr& lpRight) {
			return lpLeft->rec.second > lpRight->rec.second;
		});
	}
	
	return huffman_forest_heap.front();
}

typedef struct _Encode
{
	std::map<char, std::string> encode_map;
} encode, *encode_ptr;

// 递归编码
void get_huffman_code(const node_ptr& lpNode, encode_ptr lpEncode, std::string encode = std::string{ })
{
	for (size_t i = 0; i < encode.size(); ++i)
		printf_s("-");
	printf_s("%d %c\n", lpNode->rec.second, lpNode->rec.first);
	if (!lpNode->lpLeft && !lpNode->lpRight)
		lpEncode->encode_map.insert({ lpNode->rec.first, encode });
	if (lpNode->lpLeft)
		get_huffman_code(lpNode->lpLeft, lpEncode, encode + '0');
	if (lpNode->lpRight)
		get_huffman_code(lpNode->lpRight, lpEncode, encode + '1');
}

int main(int argc, char* argv[])
{
	std::map<char, int> rec;
	char szText[MAX_LOADSTRING];
	memset(szText, '\0', MAX_LOADSTRING);
	auto test = [&](const char szOpenFileName[]) {
		FILE* lpRead = nullptr;
		fopen_s(&lpRead, szOpenFileName, "r");

		char szLineText[MAX_LOADSTRING];
		while (fscanf_s(lpRead, "%s", szLineText, MAX_LOADSTRING) != EOF)
			strcat_s(szText, szLineText);
		
		// 统计a-z#
		int total = 0;
		for (size_t pos = 0; pos < strlen(szText); ++pos)
			if (szText[pos] > 0 && (islower(szText[pos]) || szText[pos] == '#'))
				++rec[szText[pos]], ++total;

		printf_s("Charactors(a-z#) in total : %d\n", total);
		for (std::map<char, int>::const_iterator it = rec.begin(); it != rec.end(); ++it)
			printf_s("%c: %d\t%.2lf%%\n", it->first, it->second, (double)it->second / (double)total * 100.0);

		printf_s("\nFixed-length encoding needs %d bits in total, with %d bits per charactor.\n", total * 5, 5);

		node_ptr huffman_head_ptr = build_huffman_tree(rec);
		encode   huffman_code;
		get_huffman_code(huffman_head_ptr, &huffman_code);
		
		int bits = 0;
		for (std::map<char, std::string>::const_iterator it = huffman_code.encode_map.begin(); it != huffman_code.encode_map.end(); ++it)
			bits += (int)it->second.size() * rec[it->first];
		printf_s("\nWhile huffman encoding needs %d bits in total, with the following encoding table.\n", bits);
		for (std::map<char, std::string>::const_iterator it = huffman_code.encode_map.begin(); it != huffman_code.encode_map.end(); ++it)
			printf_s("%c: %s\n", it->first, it->second.c_str());

		fclose(lpRead);
	};
	test("附件2.哈夫曼编码输入文本.txt");
	return 0;
}