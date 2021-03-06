//DrawGreedy.cpp: 定义应用程序的入口点。
//

#include "stdafx.h"
#include "DrawGreedy.h"

#include <cstdio>
#include <algorithm>
#include <vector>

#define MAX_NUM        40
#define MAX_LOADSTRING 100

#define EARTH_RADIUS 6378.137
#define PI           3.1415926

typedef struct _Station
{
	int    id;
	double lng;
	double lat;
	int    num;
} station;

void partition(std::vector<int> node_vec, double dist[][MAX_NUM], std::vector<std::pair<int, int>>& connect_vec)
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

void draw(HDC hdc, const std::vector<std::pair<int, int>>& connect_vec, int total)
{
	for (std::vector<std::pair<int, int>>::const_iterator it = connect_vec.begin(); it != connect_vec.end(); ++it)
	{
		MoveToEx(hdc, 600 - 300 * std::sin(it->first * (360.0 / (double)total) * PI / 180.0), 350 - 300 * std::cos(it->first * (360.0 / (double)total) * PI / 180.0), NULL);
		LineTo(hdc, 600 - 300 * std::sin(it->second * (360.0 / (double)total) * PI / 180.0), 350 - 300 * std::cos(it->second * (360.0 / (double)total) * PI / 180.0));

		TCHAR szID[MAX_LOADSTRING];
		wsprintf(szID, TEXT("%d"), it->first);
		TextOut(hdc, 600 - 320 * std::sin(it->first * (360.0 / (double)total) * PI / 180.0), 350 - 320 * std::cos(it->first * (360.0 / (double)total) * PI / 180.0), szID, lstrlen(szID));

		wsprintf(szID, TEXT("%d"), it->second);
		TextOut(hdc, 600 - 320 * std::sin(it->second * (360.0 / (double)total) * PI / 180.0), 350 - 320 * std::cos(it->second * (360.0 / (double)total) * PI / 180.0), szID, lstrlen(szID));
	}
}

#define MAX_LOADSTRING 100

// 全局变量: 
HINSTANCE hInst;                                // 当前实例
WCHAR szTitle[MAX_LOADSTRING];                  // 标题栏文本
WCHAR szWindowClass[MAX_LOADSTRING];            // 主窗口类名

// 此代码模块中包含的函数的前向声明: 
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: 在此放置代码。

    // 初始化全局字符串
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_DRAWGREEDY, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 执行应用程序初始化: 
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_DRAWGREEDY));

    MSG msg;

    // 主消息循环: 
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  函数: MyRegisterClass()
//
//  目的: 注册窗口类。
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_DRAWGREEDY));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_DRAWGREEDY);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   函数: InitInstance(HINSTANCE, int)
//
//   目的: 保存实例句柄并创建主窗口
//
//   注释: 
//
//        在此函数中，我们在全局变量中保存实例句柄并
//        创建和显示主程序窗口。
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // 将实例句柄存储在全局变量中

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  函数: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  目的:    处理主窗口的消息。
//
//  WM_COMMAND  - 处理应用程序菜单
//  WM_PAINT    - 绘制主窗口
//  WM_DESTROY  - 发送退出消息并返回
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // 分析菜单选择: 
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            
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

			auto test = [&](const char szOpenFileName[]) {
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

				double dist[MAX_NUM][MAX_NUM];
				for (int i = 0; i < station_cnt; ++i)
					for (int j = 0; j < station_cnt; ++j)
						dist[i][j] = get_distance(i, j);

				std::vector<std::pair<int, int>> connect_vec;
				for (int i = 0; i < station_cnt; ++i)
					connect_vec.push_back({ i, (i + 1) % station_cnt });
				std::vector<int> node_vec;
				for (int i = 0; i < station_cnt; ++i)
					node_vec.push_back(i);

				partition(node_vec, dist, connect_vec);

				double res = 0.0;
				for (std::vector<std::pair<int, int>>::const_iterator it = connect_vec.begin(); it != connect_vec.end(); ++it)
					res += 2 * dist[it->first][it->second];

				fclose(lpRead);

				draw(hdc, connect_vec, station_cnt);

				return res;
			};

			TCHAR szWeight[MAX_LOADSTRING];
			//swprintf_s(szWeight, TEXT("The total weight = %lf"), test("附件3-1.21个基站凸多边形数据2017.txt"));
			swprintf_s(szWeight, TEXT("The total weight = %lf"), test("附件3-2.29个基站凸多边形数据2017.txt"));
			TextOut(hdc, 0, 0, szWeight, lstrlen(szWeight));
			test("附件3-2.29个基站凸多边形数据2017.txt");

            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// “关于”框的消息处理程序。
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
