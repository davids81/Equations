// Equations.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "Equations.h"
#include <CommCtrl.h>

#include "../LuaLib/LuaSrc/lua.hpp"

//extern "C"
//{
//#include "../LuaLib//LuaSrc/lauxlib.h"
//}

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name

// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

void CreateListControl(HINSTANCE);

int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPTSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// TODO: Place code here.
	MSG msg;
	HACCEL hAccelTable;

	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_EQUATIONS, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_EQUATIONS));

	//lua_State* ls = luaL_newstate();
	//
	//luaL_openlibs(ls);



	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int) msg.wParam;
}

//void CreateListControl(HINSTANCE instance)
//{
//	INITCOMMONCONTROLSEX icex;           // Structure for control initialization.
//	icex.dwICC = ICC_LISTVIEW_CLASSES;
//	InitCommonControlsEx(&icex);
//
//	RECT rcClient;                       // The parent window's client area.
//
//	//GetClientRect(, &rcClient);
//
//	// Create the list-view window in report view with label editing enabled.
//	HWND hWndListView = CreateWindow(WC_LISTVIEW,
//		L"",
//		WS_CHILD | LVS_REPORT | LVS_EDITLABELS,
//		0, 0,
//		rcClient.right - rcClient.left,
//		rcClient.bottom - rcClient.top,
//		hwndParent,
//		(HMENU)IDM_CODE_SAMPLES,
//		g_hInst,
//		NULL);
//
//	return (hWndListView);
//}


//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_EQUATIONS));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_EQUATIONS);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance; // Store instance handle in our global variable

   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

int CALLBACK CompareFunc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
	return -1;
}

LRESULT CALLBACK ListViewProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData)
{

	switch (msg)
	{
	case LVM_INSERTITEM:
		int x;
		x = 1;
		break;
	case LVN_COLUMNCLICK:
		int y;
		y = 0;
		break;
	}



	return DefWindowProc(hwnd, msg, wParam, lParam);

	//WNDPROC* wp;
	//wp = (WNDPROC*)(::GetWindowLongPtr(hwnd, GWL_WNDPROC));
	//return ::CallWindowProc(*wp, hwnd, msg, wParam, lParam);
	//return 0;
}
HWND lstView;

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window
//  WM_DESTROY	- post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;


	switch (message)
	{
	//case LVN_COLUMNCLICK:
	//{

	//	NMLISTVIEW* pListView = (NMLISTVIEW*)lParam;
	//	ListView_SortItems(lstView, CompareFunc, pListView);

	//	break;
	//}
	case WM_CREATE:

		RECT rcClient;
		
		GetClientRect(hWnd, &rcClient);

		lstView = CreateWindow(WC_LISTVIEW,
			L"malksdjf",
			WS_CHILD | LV_VIEW_DETAILS | LVS_REPORT | WS_VISIBLE, 0, 0,
			rcClient.right - rcClient.left,
			rcClient.bottom - rcClient.top,
			hWnd, (HMENU)1001, hInst, NULL);

		//SetWindowLongPtr(lstView, GWL_WNDPROC, (LPARAM)&ListViewProc);

		SendMessage(lstView, LVM_SETEXTENDEDLISTVIEWSTYLE, LVS_EX_FULLROWSELECT, LVS_EX_FULLROWSELECT);

		LVCOLUMN lvc;
		lvc.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
		lvc.iSubItem = 0;
		lvc.pszText = L"First Column";
		lvc.cx = 100;
		lvc.fmt = LVCFMT_LEFT;

		if (ListView_InsertColumn(hWnd, 0, &lvc) == -1)
			return -1;

		SendMessage(lstView, LVM_INSERTCOLUMN, 0, (LPARAM)&lvc);
		lvc.pszText = L"Second Column";
		SendMessage(lstView, LVM_INSERTCOLUMN, 1, (LPARAM)&lvc);

		LVITEM templateItem;
		templateItem.mask = LVIF_TEXT;
		templateItem.cchTextMax = 256;
		templateItem.iItem = 0;
		templateItem.iSubItem = 0;
		templateItem.pszText = L"first column text";

		SendMessage(lstView, LVM_INSERTITEM, 0, (LPARAM)&templateItem);

		templateItem.iSubItem = 1;
		templateItem.pszText = L"Second column text";
		SendMessage(lstView, LVM_SETITEM, 0, (LPARAM)&templateItem);




		//LVITEM blah;

		//ListView_SetItem(lstView, &blah);
		SetWindowSubclass(lstView, ListViewProc, 0, 0);
		break;

	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// Parse the menu selections:
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
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		// TODO: Add any drawing code here...
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// Message handler for about box.
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
