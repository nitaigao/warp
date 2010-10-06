#define _WIN32_IE 0x0501
#define _WIN32_WINDOWS 0x0501

#include <windows.h>
#include <shellapi.h>
#include <stdio.h>

#include "Constants.hpp"
#include "Message.h"
#include "Exit.h"

#include "resource.h"

#ifdef UNICODE
#define stringcopy wcscpy
#else
#define stringcopy strcpy
#endif

#define ID_TRAY_APP_ICON                5000
#define ID_TRAY_EXIT_CONTEXT_MENU_ITEM  3000
#define WM_TRAYICON ( WM_USER + 1 )

#pragma region constants and globals
UINT WM_TASKBARCREATED = 0 ;

HWND g_hwnd ;
HMENU g_menu ;

NOTIFYICONDATA g_notifyIconData ;
#pragma endregion

bool quit = false;

LRESULT CALLBACK WndProc (HWND, UINT, WPARAM, LPARAM);

void InitNotifyIconData()
{
  memset( &g_notifyIconData, 0, sizeof( NOTIFYICONDATA ) ) ;
  g_notifyIconData.cbSize = sizeof(NOTIFYICONDATA);

  g_notifyIconData.hWnd = g_hwnd;
  g_notifyIconData.uID = ID_TRAY_APP_ICON;
  g_notifyIconData.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
  g_notifyIconData.uCallbackMessage = WM_TRAYICON;
  g_notifyIconData.hIcon = (HICON)LoadImage(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_ICON1), IMAGE_ICON, 0, 0, LR_SHARED);
  stringcopy(g_notifyIconData.szTip, TEXT("Wormhole"));
}

int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR args, int iCmdShow )
{
  TCHAR className[] = TEXT( "tray icon class" );
  WM_TASKBARCREATED = RegisterWindowMessageA("TaskbarCreated") ;

  WNDCLASSEX wnd = { 0 };

  wnd.hInstance = hInstance;
  wnd.lpszClassName = className;
  wnd.lpfnWndProc = WndProc;
  wnd.style = CS_HREDRAW | CS_VREDRAW ;
  wnd.cbSize = sizeof (WNDCLASSEX);

  wnd.hIcon = LoadIcon (NULL, IDI_APPLICATION);
  wnd.hIconSm = LoadIcon (NULL, IDI_APPLICATION);
  wnd.hCursor = LoadCursor (NULL, IDC_ARROW);
  wnd.hbrBackground = (HBRUSH)COLOR_APPWORKSPACE ;

  if (!RegisterClassEx(&wnd))
  {
    FatalAppExit( 0, TEXT("Couldn't register window class!") );
  }

  g_hwnd = CreateWindowEx(0, className, TEXT( "Using the system tray" ), WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 400, 400, NULL, NULL, hInstance, NULL);

  InitNotifyIconData();
  Shell_NotifyIcon(NIM_ADD, &g_notifyIconData);

  Exit exit;
  exit.start_listening(SERVER_PORT);
 
  MSG msg ;
  while (!quit)
  {
	PeekMessage(&msg, 0, 0, 0, PM_REMOVE);
	exit.receive();
    TranslateMessage(&msg);
    DispatchMessage(&msg);
  }

  Shell_NotifyIcon(NIM_DELETE, &g_notifyIconData);
 
  return msg.wParam;
}

LRESULT CALLBACK WndProc (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
  if (message == WM_TASKBARCREATED && !IsWindowVisible( g_hwnd ) )
  {
    return 0;
  }

  switch (message)
  {
  case WM_CREATE:
    g_menu = CreatePopupMenu();
    AppendMenu(g_menu, MF_STRING, ID_TRAY_EXIT_CONTEXT_MENU_ITEM,  TEXT( "Exit" ) );
    break;

  case WM_TRAYICON:
    {
      if (lParam == WM_RBUTTONDOWN)
      {
        POINT curPoint ;
        GetCursorPos( &curPoint ) ;
        SetForegroundWindow(hwnd); 
		UINT clicked = TrackPopupMenu(g_menu, TPM_RETURNCMD | TPM_NONOTIFY, curPoint.x,curPoint.y,0,hwnd,NULL);
       
        if (clicked == ID_TRAY_EXIT_CONTEXT_MENU_ITEM)
        {
          // quit the application.
          printf("I have posted the quit message, biatch\n");
          quit = true;
        }
      }
    }
    break;

  case WM_CLOSE:
    printf( "Got an actual WM_CLOSE Message!  Woo hoo!\n" ) ;
    return 0;
    break;

  case WM_DESTROY:
    printf( "DESTROY!!\n" );
    PostQuitMessage (0);
    break;

  }

  return DefWindowProc( hwnd, message, wParam, lParam ) ;
}