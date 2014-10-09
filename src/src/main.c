/*Very simple IRC client, this was a personal project, not stable not tested a lot either and not supported.

Author: Frederic Garnier
Language: C
Created: 3/2/2008
Modified: 9/10/2014
Supported platforms: Windows.
Compiler: Microsoft Visual Studio 2013 v120.


Memory leaks have not been checked, and so there may be a few leaks here and there, some resources may have not been released, including thread handles,
dynamically allocated memory (usually provided by standard library functions - getaddrinfo etc).
Some structures are defined but unused at the moment, including IRCContext etc.

The windows are kept in an array, a linked list was used before but there was no point as none of the windows are removed at the moment. The windows
are resized by iterating through the array and changing each windows's size. This is an array of HWND's. Another array is used to store the channel names,
so that messages are sent to the approriate channels when the window is switched (alt-left or alt-right - see Window.h). The windows are also stored in a
very simple hash table with the channel names as keys if it's a joined channel, or the USER's ID if it's a PM, this is used so that whenever a /join
command is sent, a new window is not opened if it's already been opened, and if a another PM is sent, it is sent to the original window.

BUGS:
The window is editable, but this is simply a matter of setting the ES_READONLY flag, crashes can occur when more than 3 channels are opened. The time
is not appended to the sent messages, but that's for later. Switching channels does not always return the correct channel, so a message that is to be sent
for channel X can be sent to channel Y while the window of channel X is opened. Other bugs are to be added here.

The MODE function checks for the '+' prefix, this prefix may not appear and cause issues, not tested this client on any other server apart from irc.quakenet.org.
Some commands have been removed, including QUIT, PART, etc as they were unstable and are thus not supported at this moment.
*/

#include "Hash.h"
#include <stdio.h>
#include <Windows.h>
#include <tchar.h>
#include "Window.h"
#include "Debug.h"
#include "Array.h"
#include "IRC.h"
#include "Parse.h"

char* ParseTypeBuffer(char* szString);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
TCHAR szParentClass[] = TEXT("GarnerIRC");

PHash pHash;
PArray pArray;
PFArraystr ChannelNames;
Context ContextIRC;

LRESULT CALLBACK TypeBufferProc(HWND, UINT, WPARAM, LPARAM);

int APIENTRY _tWinMain(
	HINSTANCE hInstance, 
	HINSTANCE hPrevInstance, 
	LPSTR lpCmdLine, 
	int nCmdShow
	)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	MSG msg;
	WNDCLASSEX wcex;
	HWND hParent;
	ATOM LeftKey = 0, RightKey = 0;

	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hbrBackground = (HBRUSH)BLACK_BRUSH;
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wcex.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	wcex.hInstance = hInstance;
	wcex.lpfnWndProc = WndProc;
	wcex.lpszClassName = szParentClass;
	wcex.lpszMenuName = NULL;
	wcex.style = CS_VREDRAW | CS_HREDRAW;

	if (!RegisterClassEx(&wcex))
		exit(-1);

	hParent = CreateWindowEx(WS_EX_CLIENTEDGE, szParentClass, TEXT("Garner IRC v0.01"), WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);
	assert(hParent);

	ShowWindow(hParent, nCmdShow);
	UpdateWindow(hParent);
	InitContext();
	SetupWindowKeys(hParent, LeftKey, RightKey);
	ContextIRC.hParent = hParent;
	pHash = Hash_Create(pHash, HashTransform);
	ChannelNames = strArray_Init(ChannelNames);
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return msg.wParam;
}

LRESULT CALLBACK WndProc(
	HWND hWnd,
	UINT nMessage,
	WPARAM wParam,
	LPARAM lParam
	)
{
	RECT ClientRect;
	switch (nMessage)
	{
	case WM_CREATE_IRC_WINDOW:
		if (Hash_Get(pHash, (char*)wParam))
		{
			ContextIRC.hCurrentWindow = Hash_Get(pHash, (char*)wParam);
		}
		else
		{
			static HWND __hWnd__;
			__hWnd__ = Create_Window(__hWnd__, hWnd);
			if (((char*)wParam)[0] == '#')
			{
				ShowWindow(ContextIRC.hCurrentWindow, SW_HIDE);
				ContextIRC.hCurrentWindow = __hWnd__;
				strArray_Add(ChannelNames, szChannel);
				Hash_Insert(pHash, ContextIRC.hCurrentWindow, (char*)wParam);
				ShowWindow(Hash_Get(pHash, (char*)wParam), SW_SHOW);
				ResizeWindows(ContextIRC.dwOldWidth, ContextIRC.dwOldHeight);
				SendMessage(ContextIRC.hCurrentWindow, WM_SETFONT, (WPARAM)ContextIRC.hFont, (LPARAM)TRUE);
			}
			else
			{
				ContextIRC.hCurrentWindow = Array_Index(pArray, Array_Num(pArray));
				ContextIRC.hCurrentWindow = __hWnd__;
				Hash_Insert(pHash, ContextIRC.hCurrentWindow, (char*)wParam);
				ResizeWindows(ContextIRC.dwOldWidth, ContextIRC.dwOldHeight);
				SendMessage(ContextIRC.hCurrentWindow, WM_SETFONT, (WPARAM)ContextIRC.hFont, (LPARAM)TRUE);
			}
			ContextIRC.hCurrentWindow = Array_Index(pArray, Array_Num(pArray));
			Hash_Insert(pHash, ContextIRC.hCurrentWindow, (char*)wParam);
			SendMessage(ContextIRC.hCurrentWindow, WM_SETFONT, (WPARAM)ContextIRC.hFont, (LPARAM)TRUE);
			break;
	}
	case WM_CREATE:
		pArray = Array_Init(pArray);
		ContextIRC.hCurrentWindow = Create_Window(ContextIRC.hCurrentWindow, hWnd);
		ContextIRC.hFont = InitFont(ContextIRC.hCurrentWindow);
		ContextIRC.hTypeBuffer = InitTypeBuffer(ContextIRC.hFont, hWnd);
		ContextIRC.hParentProc = SetWindowLongPtr(ContextIRC.hTypeBuffer, GWL_WNDPROC, (LONG_PTR)TypeBufferProc);
		ContextIRC.hNetworkThread = CreateThread(NULL, 0, Connect, NULL, 0, &ContextIRC.hNetworkThreadID);
		break;
	case WM_DESTROY:
		DestroyContext();
		PostQuitMessage(0);
		break;
	case WM_HOTKEY:
	{
					  register uint32_t nIndex = 0;
					  if (wParam == IRCContext->LeftKey)
					  {
						  if (Array_Num(pArray) <= 0)
							  break;
						  for (nIndex = Array_Num(pArray); nIndex > 0; nIndex--)
						  {
							  if (ContextIRC.hCurrentWindow == Array_Index(pArray, 0))
							  {
								  ShowWindow(ContextIRC.hCurrentWindow, SW_HIDE);
								  ContextIRC.hCurrentWindow = Array_Index(pArray, Array_Num(pArray));

								  strcpy_s(szChannel, 96, strArray_Index(ChannelNames, strArray_Num(ChannelNames)));
								  
								  ShowWindow(ContextIRC.hCurrentWindow, SW_SHOW);
								  MoveWindow(ContextIRC.hCurrentWindow, 0, 0, ContextIRC.dwOldWidth, ContextIRC.dwOldHeight, TRUE);
								  break;
							  }
							  nIndex--;
							  ShowWindow(ContextIRC.hCurrentWindow, SW_HIDE);
							  ContextIRC.hCurrentWindow = Array_Index(pArray, nIndex);

							  strcpy_s(szChannel, 96, strArray_Index(ChannelNames, nIndex - 1));

							  ShowWindow(ContextIRC.hCurrentWindow, SW_SHOW);
							  MoveWindow(ContextIRC.hCurrentWindow, 0, 0, ContextIRC.dwOldWidth, ContextIRC.dwOldHeight, TRUE);
							  break;
						  }
					  }
					  if (wParam == IRCContext->RightKey)
					  {
						  for (nIndex = 0; nIndex <= Array_Num(pArray); nIndex++)
						  {
							  if (Array_Num(pArray) <= 0)
								  break;
							  if (ContextIRC.hCurrentWindow == Array_Index(pArray, nIndex))
							  {
								  if (ContextIRC.hCurrentWindow == Array_Index(pArray, Array_Num(pArray)))
								  {
									  ShowWindow(ContextIRC.hCurrentWindow, SW_HIDE);
									  ContextIRC.hCurrentWindow = Array_Index(pArray, 0);

									  strcpy_s(szChannel, 96, strArray_Index(ChannelNames, 0));

									  ShowWindow(ContextIRC.hCurrentWindow, SW_SHOW);
									  MoveWindow(ContextIRC.hCurrentWindow, 0, 0, ContextIRC.dwOldWidth, ContextIRC.dwOldHeight, TRUE);
									  break;
								  }
								  nIndex++;
								  ShowWindow(ContextIRC.hCurrentWindow, SW_HIDE);
								  ContextIRC.hCurrentWindow = Array_Index(pArray, nIndex);

								  strcpy_s(szChannel, 96, strArray_Index(ChannelNames, nIndex - 1));

								  ShowWindow(ContextIRC.hCurrentWindow, SW_SHOW);
								  MoveWindow(ContextIRC.hCurrentWindow, 0, 0, ContextIRC.dwOldWidth, ContextIRC.dwOldHeight, TRUE);
								  break;
							  }
						  }
					  }
					  break;
	}
	case WM_SIZE:
		ResizeWindows(LOWORD(lParam), HIWORD(lParam) - IRCContext->nTypeHeight);
		GetClientRect(hWnd, &ClientRect);
		ContextIRC.dwOldWidth = LOWORD(lParam);
		ContextIRC.dwOldHeight = HIWORD(lParam);
		MoveWindow(ContextIRC.hTypeBuffer, 0, ClientRect.bottom - IRCContext->nTypeHeight, LOWORD(lParam), IRCContext->nTypeHeight, TRUE);
		break;
	default:
		return DefWindowProc(hWnd, nMessage, wParam, lParam);
	}
	return 0;
}

LRESULT CALLBACK TypeBufferProc(
	HWND hWnd,
	UINT nMessage,
	WPARAM wParam,
	LPARAM lParam)
{
	static HDC __hdc__;
	switch (nMessage)
	{
	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_RETURN:
			GetWindowText(hWnd, ContextIRC.szTypeBuffer, 511);
			SetWindowText(hWnd, "");
			ParseTypeBuffer(ContextIRC.szTypeBuffer);
			break;
		}
	default:
		return CallWindowProc((WNDPROC)ContextIRC.hParentProc, hWnd, nMessage, wParam, lParam);
	}
	return CallWindowProc((WNDPROC)ContextIRC.hParentProc, hWnd, nMessage, wParam, lParam);
}

char* ParseTypeBuffer(char* szString)
{
	char* szTemp = malloc(sizeof(char*)*strlen(szString));
	memset(szTemp, 0, sizeof(char*)*strlen(szString));
	strcpy_s(szTemp, strlen(szString) + 1, szString);
	if (strstr(szTemp, "/join"))
	{
		if (szTemp[0] == '/' && szTemp[4] == 'n')
		{
			char* szStr = Substring(szTemp, "/", 1);
			strcpy_s(szTemp, strlen(szStr) + 1, szStr);
			strcpy_s(szString, strlen(szTemp) + 1, szTemp);
			free(szTemp);
			char* __Channel = szBoyer_Moore(szString, strlen(szString), "#", strlen("#"));
			if (__Channel)
			{
				strcpy_s(ContextIRC.szHashKey, strlen(__Channel) + 1, CharLowerA(__Channel));
				strcpy_s(szChannel, 96, __Channel);
				assert(ContextIRC.hParent);
				SendMessage(ContextIRC.hParent, WM_CREATE_IRC_WINDOW, (WPARAM)ContextIRC.szHashKey, 0);
				strcpy_s(ContextIRC.szTypeBuffer, strlen(szString) + 1, szString);
				strcat_s(ContextIRC.szTypeBuffer, 511, "\r\n");
				send(Socket, ContextIRC.szTypeBuffer, strlen(ContextIRC.szTypeBuffer), 0);
				return szString;
			}
		}
	}
	else
	{
		char* szMessage = "PRIVMSG ";
		char* szCached = malloc(512);
		memset(szCached, 0, 512);
		strcpy_s(szCached, 511, ContextIRC.szTypeBuffer);
		memset(ContextIRC.szTypeBuffer, 0, 511);
		strcpy_s(ContextIRC.szTypeBuffer, strlen(szMessage) + 1 , szMessage);
		strcat_s(ContextIRC.szTypeBuffer, 511, szChannel);
		strcat_s(ContextIRC.szTypeBuffer, 511, " :");
		strcat_s(ContextIRC.szTypeBuffer, 511, szCached);
		strcat_s(ContextIRC.szTypeBuffer, 511, "\r\n");

		send(Socket, ContextIRC.szTypeBuffer, strlen(ContextIRC.szTypeBuffer), 0);
		char* TempNick = malloc(sizeof(szNick)+511);
		memset(TempNick, 0, sizeof(szNick)+511);
		TempNick = AddBrackets(szNick, TempNick);
		strcat_s(TempNick, 511, " ");
		strcat_s(TempNick, 511, szCached);
		strcat_s(TempNick, 511, "\r\n");

		SetIRCWindowText(TempNick);
		free(TempNick);
		TempNick = NULL;

		free(szCached);
		szCached = NULL;
	}
	return szString;
}