#pragma once
#include <Windows.h>
#include <assert.h>
#include "List.h"
#include "Debug.h"
#include "Array.h"

#define WM_CREATE_IRC_WINDOW (WM_USER + 0x0001)


typedef struct __Context{

	HWND hCurrentWindow;
	HWND hTypeBuffer;
	HFONT hFont;
	LONG_PTR hParentProc;
	HANDLE hNetworkThread;
	DWORD hNetworkThreadID;
	char szTypeBuffer[512];
	char szHashKey[512];
	HWND hParent;
	DWORD dwOldWidth;
	DWORD dwOldHeight;
}Context;

extern Context ContextIRC;
typedef struct __IRC{
	/*Left key hotkey (alt+left arrow)*/
	ATOM LeftKey;
	/*Right key hotkey (alt+right arrow)*/
	ATOM RightKey;
	/*Parent HWND, first window created, not an edit control!*/
	HWND hParent;
	/*Current window that is being shown */
	HWND hCurrentWindow;
	/*Doubly linked list that holds the created windows, does not contain hParent!*/
	PDoublyList pList;
	/*Height of the type buffer*/
	uint32_t nTypeHeight;
} *IRC;

/*IRCContext holds the current window, the parent window and other attributes needed*/
IRC IRCContext;
/*Array which holds the windows, not the parent*/
extern PArray pArray;

DWORD WINAPI Connect(LPVOID lpParam);
void InitContext(void);
void DestroyContext(void);
void SetupWindowKeys(HWND hWnd, ATOM LeftKey, ATOM RightKey);
void CreateIRCWindow(HWND hWnd);
void ResizeWindows(DWORD nWidth, DWORD nHeight);
HFONT InitFont(HWND hWnd);
HWND InitTypeBuffer(HFONT hFont, HWND hParent);
HWND Create_Window(HWND hIn, HWND hWnd);
void SetIRCWindowText(char* szString);