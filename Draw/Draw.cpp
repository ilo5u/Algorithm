//Draw.cpp: 定义应用程序的入口点。
//

#include "stdafx.h"
#include "Draw.h"

#include <cstdio>
#include <algorithm>

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

void draw(HDC hdc, int(&connect)[MAX_NUM][MAX_NUM], int l, int r, int total)
{
	if (connect[l][r] >= l - 1 && connect[l][r] <= r)
	{
		MoveToEx(hdc, 600 - 300 * std::sin((l - 1) * (360.0 / (double)total) * PI / 180.0), 350 - 300 * std::cos((l - 1) * (360.0 / (double)total) * PI / 180.0), NULL);
		LineTo(hdc, 600 - 300 * std::sin(connect[l][r] * (360.0 / (double)total) * PI / 180.0), 350 - 300 * std::cos(connect[l][r] * (360.0 / (double)total) * PI / 180.0));

		MoveToEx(hdc, 600 - 300 * std::sin(r * (360.0 / (double)total) * PI / 180.0), 350 - 300 * std::cos(r * (360.0 / (double)total) * PI / 180.0), NULL);
		LineTo(hdc, 600 - 300 * std::sin(connect[l][r] * (360.0 / (double)total) * PI / 180.0), 350 - 300 * std::cos(connect[l][r] * (360.0 / (double)total) * PI / 180.0));

		MoveToEx(hdc, 600 - 300 * std::sin((l - 1) * (360.0 / (double)total) * PI / 180.0), 350 - 300 * std::cos((l - 1) * (360.0 / (double)total) * PI / 180.0), NULL);
		LineTo(hdc, 600 - 300 * std::sin(r * (360.0 / (double)total) * PI / 180.0), 350 - 300 * std::cos(r * (360.0 / (double)total) * PI / 180.0));

		TCHAR szID[MAX_LOADSTRING];
		wsprintf(szID, TEXT("%d"), l - 1);
		TextOut(hdc, 600 - 320 * std::sin((l - 1) * (360.0 / (double)total) * PI / 180.0), 350 - 320 * std::cos((l - 1) * (360.0 / (double)total) * PI / 180.0), szID, lstrlen(szID));

		wsprintf(szID, TEXT("%d"), r);
		TextOut(hdc, 600 - 320 * std::sin(r * (360.0 / (double)total) * PI / 180.0), 350 - 320 * std::cos(r * (360.0 / (double)total) * PI / 180.0), szID, lstrlen(szID));

		wsprintf(szID, TEXT("%d"), connect[l][r]);
		TextOut(hdc, 600 - 320 * std::sin(connect[l][r] * (360.0 / (double)total) * PI / 180.0), 350 - 320 * std::cos(connect[l][r] * (360.0 / (double)total) * PI / 180.0), szID, lstrlen(szID));

		draw(hdc, connect, l, connect[l][r], total);
		draw(hdc, connect, connect[l][r] + 1, r, total);
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
    LoadStringW(hInstance, IDC_DRAW, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 执行应用程序初始化: 
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_DRAW));

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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_DRAW));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_DRAW);
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
            
			auto rad = [](double lat_or_lng) {
				return lat_or_lng * PI / 180.0;
			};

			station station_set[MAX_NUM];
			auto get_distance = [&station_set, rad](int x, int y) {
				double rad_lngx = rad(station_set[x].lng);
				double rad_latx = rad(station_set[x].lat);
				double rad_lngy = rad(station_set[y].lng);
				double rad_laty = rad(station_set[y].lat);
				double dist = std::acos(std::cos(rad_latx) * std::cos(rad_laty) *
					std::cos(rad_lngx - rad_lngy) + std::sin(rad_latx) * std::sin(rad_laty));
				return std::round(dist * EARTH_RADIUS * 1000.0);
			};

			auto get_wgt = [get_distance](int a, int b, int c) {
				return get_distance(a, b) + get_distance(b, c) + get_distance(a, c);
			};

			auto test = [hdc, &station_set, get_wgt](const char* szOpenFileName) {
				FILE *lpRead = nullptr;
				fopen_s(&lpRead, szOpenFileName, "r");

				char szID[MAX_LOADSTRING];
				char szLNG[MAX_LOADSTRING];
				char szLAT[MAX_LOADSTRING];
				char szNUM[MAX_LOADSTRING];
				fscanf_s(lpRead, "%s\t%s\t%s\t%s\n", szID, MAX_LOADSTRING, szLNG, MAX_LOADSTRING, szLAT, MAX_LOADSTRING, szNUM, MAX_LOADSTRING);

				int station_cnt = 0;
				while (fscanf_s(lpRead, "%d\t%lf\t%lf\t%d\n", &station_set[station_cnt].id, &station_set[station_cnt].lng, &station_set[station_cnt].lat, &station_set[station_cnt].num) != EOF)
					++station_cnt;
				fclose(lpRead);

				double wgt[MAX_NUM][MAX_NUM];
				int    connect[MAX_NUM][MAX_NUM];
				for (int i = 0; i <= station_cnt; ++i)
					for (int j = 0; j <= station_cnt; ++j)
						wgt[i][j] = INT_MAX;
				for (int i = 1; i < station_cnt; ++i)
					wgt[i][i] = 0;
				for (int len = 2; len <= station_cnt; ++len)
				{
					for (int l = 1; l <= station_cnt - len + 1; ++l)
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

				draw(hdc, connect, 1, station_cnt - 1, station_cnt);
			};

			//test("附件3-1.21个基站凸多边形数据2017.txt");
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
