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

node_ptr build_huffman_tree(const std::map<char, int>& rec)
{
	std::vector<node_ptr> huffman_forest_heap;
	for (std::map<char, int>::const_iterator it = rec.begin(); it != rec.end(); ++it)
		huffman_forest_heap.push_back(new node{ *it, nullptr, nullptr });
	for (size_t pos = 0; pos < huffman_forest_heap.size() - 1; ++pos)
	{
		std::make_heap(huffman_forest_heap.begin() + pos, huffman_forest_heap.end(), [](const node_ptr& lpLeft, const node_ptr& lpRight) {
			return lpLeft->rec.second > lpRight->rec.second;
		});

		huffman_forest_heap[pos + 1] =
			new node{ std::pair<char, int>{ 0x0, huffman_forest_heap[pos]->rec.second + huffman_forest_heap[pos + 1]->rec.second },
				huffman_forest_heap[pos], huffman_forest_heap[pos + 1] };
	}
	
	return huffman_forest_heap.back();
}

typedef struct _Encode
{
	std::map<char, std::string> encode_map;
} encode, *encode_ptr;

void get_huffman_code(const node_ptr& lpNode, encode_ptr lpEncode, std::string encode = std::string{ })
{
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
		
		for (size_t pos = 0; pos < strlen(szText); ++pos)
			if (szText[pos] > 0 && (isalpha(szText[pos]) || szText[pos] == '#'))
				++rec[szText[pos]];

		node_ptr huffman_head_ptr = build_huffman_tree(rec);

		encode huffman_code;
		get_huffman_code(huffman_head_ptr, &huffman_code);

		for (std::map<char, std::string>::const_iterator it = huffman_code.encode_map.begin(); it != huffman_code.encode_map.end(); ++it)
			printf_s("%c: %s\n", it->first, it->second.c_str());

		fclose(lpRead);
	};
	test("附件2.哈夫曼编码输入文本.txt");
	return 0;
}