// Equations.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "Equations.h"
#include <CommCtrl.h>
#include <string.h>

#include "../LuaLib/LuaSrc/lua.hpp"

struct blah
{
	wchar_t* first;
	wchar_t* second;
};

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
//"x = 1 \n y = 1 \n 
char* pLuaText = "function plus1 (va) return va + 1 end if x > y then return plus1(x) else return y end";
void* pLuaByteCode = NULL;
size_t byteSize;

typedef struct {
	size_t *len;
	char **data;
} BS_DESCRIP;

int scriptMemoryWriter(lua_State* ls, const void* p, size_t sz, void* ud)
{
	BS_DESCRIP* bd = (BS_DESCRIP*)ud;
	char* newData = (char*)realloc(*(bd->data), (*(bd->len)) + sz);

	if (newData)
	{
		memcpy(newData + (*(bd->len)), p, sz);
		*(bd->data) = newData;
		*(bd->len) += sz;

	}
	else {
		free(newData);

		return 1;
	}

	return 0;
}
const char* reader(lua_State *L, void *data, size_t* sz)
{
	BS_DESCRIP* bd = (BS_DESCRIP*)data;
	*sz = *(bd->len);
	return *(bd->data);

	//*sz = byteSize;
	//if (byteSize == 0)
	//{
	//	return NULL;
	//}
	//else
	//{
	//	byteSize -= byteSize;
	//	return (const char*)pLuaByteCode;
	//}
}

char* userVars =
"local U={} "
"_G[\"uservariables\"]=U "
"local function trace1(t,k,v) "
"    U[k]=true "
"    rawset(t,k,v) "
"end "
"setmetatable(_G, { __index = trace1 })";

char* sysVars =
"local S={} "
"_G[\"systemvariables\"]=S "
"for k in pairs(_G) do S[k]=true end";

void PrintUserVariables(lua_State* ls)
{
	//lua_getglobal(ls, "uservariables");
	
	lua_pushglobaltable(ls);
	lua_pushnil(ls);
	while (lua_next(ls, -2) != 0)
	{
		const char* s = lua_tostring(ls, -2);
		OutputDebugStringA(s);
		OutputDebugStringA("\n");
		const char* tn1 = lua_typename(ls, lua_type(ls, -2));
		const char* tn2 = lua_typename(ls, lua_type(ls, -1));
		//OutputDebugStringA(tn1);
		lua_pop(ls, 1);
	}
	OutputDebugStringA("\n");
}

void PrintSysVariables(lua_State* ls)
{
	//lua_rawgeti(ls, LUA_REGISTRYINDEX, LUA_RIDX_GLOBALS);
	lua_getglobal(ls, "uservariables");
	//lua_pushglobaltable(ls);
	lua_pushnil(ls);
	while (lua_next(ls, -2) != 0)
	{
		const char* s = lua_tostring(ls, -2);
		OutputDebugStringA(s);
		OutputDebugStringA("\n");
		lua_pop(ls, 1);
	}
	lua_pop(ls, 1);
	OutputDebugStringA("\n");
}

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

	lua_State* ls = luaL_newstate();
	
	luaL_openlibs(ls);

	int err;
	err = luaL_dostring(ls, userVars);
	err = luaL_dostring(ls, sysVars);

	char* bytecode = 0L;
	size_t bytecode_len = 0;
	BS_DESCRIP bd = { &bytecode_len, &bytecode };

	err = luaL_loadstring(ls, pLuaText);
	err = lua_pcall(ls, 0, 1, 0);
	PrintSysVariables(ls);
	lua_pop(ls, 1);

	int blah = lua_gettop(ls);


	err = luaL_loadstring(ls, pLuaText);
	err = lua_dump(ls, (lua_Writer)scriptMemoryWriter, &bd, 0);
	lua_pop(ls, 1);
	
	lua_getglobal(ls, "uservariables");
	lua_pushnil(ls);
	while (lua_next(ls, -2) != 0)
	{
		const char* varName = lua_tostring(ls, -2);
		if (strcmp(varName, "x") == 0)
		{
			lua_pushnumber(ls, 10);
		}
		else
		{
			lua_pushnumber(ls, 1);
		}

		lua_setglobal(ls, varName);
		lua_pop(ls, 1);
	}
	lua_pop(ls, 1);

	err = luaL_dostring(ls, "uservariables = nil");
	//err = luaL_dostring(ls, userVars);
	//PrintSysVariables(ls);
	err = luaL_loadbuffer(ls, bytecode, bytecode_len, "somename");

	err = lua_pcall(ls, 0, /*LUA_MULTRET*/1, 0);
	//PrintSysVariables(ls);
	int t = lua_gettop(ls);

	double calc = lua_tonumber(ls, -1);
	

	//err = luaL_loadstring(ls, pLuaText);
	PrintSysVariables(ls);

	//luaL_loadbuffer(ls, pLuaByteCode, sizeof(pLuaByteCode), "blah");
	lua_pushnumber(ls, 5);
	lua_setglobal(ls, "x");

	lua_pushnumber(ls, 10);
	lua_setglobal(ls, "y");


	err = lua_pcall(ls, 0, /*LUA_MULTRET*/1, 0);

	int top;
	double val;
	switch (err)
	{
	case LUA_OK:
		top = lua_gettop(ls);

		val = lua_tonumber(ls, -1);

		break;
	case LUA_ERRRUN:
		printf("run time error");
		break;
	case LUA_ERRERR:
		printf("error error");
		break;
	case LUA_ERRMEM:
		printf("error memory");
		break;
	case LUA_ERRGCMM:
		printf("garbage collection error");
		break;
	}
	// here we can now get the variables of the loaded string

	//lua_pushinteger(ls, 1);
	//lua_setglobal(ls, "x");
	//lua_pushinteger(ls, 2);
	//lua_setglobal(ls, "y");

	//int top = lua_gettop(ls);

	//err = lua_pcall(ls, 0, 0, 0);



	


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

bool forward = true;
bool f2 = true;
int CALLBACK CompareFunc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
	LPNMLISTVIEW viewInfo = (LPNMLISTVIEW)lParamSort;
	blah* p1 = (blah*)lParam1;
	blah* p2 = (blah*)lParam2;

	if (viewInfo->iSubItem == 0)
	{
		if (forward)
		{
			forward = false;
			return wcscmp(p1->first, p2->first);
		}
		else
		{
			forward = true;
			return wcscmp(p2->first, p1->first);
		}
	}
	else
	{
		if (f2)
		{
			f2 = false;
			return wcscmp(p1->second, p2->second);
		}
		else
		{
			f2 = true;
			return wcscmp(p2->second, p1->second);
		}
	}
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
HWND eqNameTextbox;

blah* row1;
blah* row2;

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
	case WM_NOTIFY:
	{
		LPNMHDR str = (LPNMHDR)lParam;
		switch (((LPNMHDR)lParam)->code)
		{
		case LVN_GETDISPINFO:
		{
			NMLVDISPINFO* plvdi = (NMLVDISPINFO*)lParam;
			plvdi->item.pszText = ((blah*)plvdi->item.lParam)->second;
		}
		break;
		case LVN_COLUMNCLICK:
			if (str->idFrom == 1001) {
				LPNMLISTVIEW info = (LPNMLISTVIEW)lParam;
				
				ListView_SortItems(info->hdr.hwndFrom, CompareFunc, lParam);
			}
			break;
		}
	}
	break;
	case WM_CREATE:

		row1 = (blah*)malloc(sizeof(blah));
		row2 = (blah*)malloc(sizeof(blah));

		row1->first = L"row 1 col 1 text";
		row1->second = L"row 1 col 2 text";

		row2->first = L"row 2 col 1 text";
		row2->second = L"row 2 col 2 text";
		

		RECT rcClient;
		
		GetClientRect(hWnd, &rcClient);

		lstView = CreateWindowW(WC_LISTVIEW,
			L"EquationsListView",
			WS_CHILD | LV_VIEW_DETAILS | LVS_REPORT | WS_VISIBLE, 0, 0,
			rcClient.right - rcClient.left,
			rcClient.bottom - rcClient.top - 100,
			hWnd, (HMENU)1001, hInst, NULL);
		SendMessage(lstView, LVM_SETEXTENDEDLISTVIEWSTYLE, LVS_EX_FULLROWSELECT, LVS_EX_FULLROWSELECT);

		LVCOLUMN lvc;
		lvc.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
		lvc.pszText = LPSTR_TEXTCALLBACK;
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
		templateItem.mask = LVIF_TEXT | LVIF_PARAM;
		templateItem.cchTextMax = 256;
		templateItem.iItem = 0;
		templateItem.iSubItem = 0;
		templateItem.pszText = row1->first;
		templateItem.lParam = (LPARAM)row1;

		SendMessage(lstView, LVM_INSERTITEM, 0, (LPARAM)&templateItem);

		templateItem.pszText = row2->first;
		templateItem.lParam = (LPARAM)row2;
		templateItem.iItem = 1;
		templateItem.iSubItem = 0;
		ListView_InsertItem(lstView, &templateItem);

		ListView_SetBkColor(lstView, 355);

		eqNameTextbox = CreateWindow(
			WC_BUTTON, 
			L"putingraphbutton", 
			WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON, 
			rcClient.right - 50, 
			rcClient.bottom - 25, 
			50, 
			25, 
			hWnd, 
			NULL, 
			hInst,
			NULL);

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
