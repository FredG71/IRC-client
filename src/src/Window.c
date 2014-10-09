#include "Window.h"
/*@note: Initalizes the context and zero initializes it*/
void InitContext(void)
{
	/*Don't want to call it twice*/
	assert(!IRCContext);
	IRCContext = (IRC)malloc(sizeof(struct __IRC));
	ZeroMemory(IRCContext, sizeof(struct __IRC));
	IRCContext->nTypeHeight = 50;
	assert(IRCContext);
}

void DestroyContext(void)
{
	assert(IRCContext);
	Array_Destroy(&pArray);
	free(IRCContext);
	IRCContext = NULL;
}

/*@param hWnd: Top level HWND, i.e. the parent
@param LeftKey: Hotkey to switch windows on the left
@param RightKey: Hotkey to switch windows on the right
*/
void SetupWindowKeys(HWND hWnd, ATOM LeftKey, ATOM RightKey)
{
	LeftKey = GlobalAddAtom(TEXT("AltLeft"));
	RightKey = GlobalAddAtom(TEXT("AltRight"));
	assert(LeftKey && RightKey);
	RegisterHotKey(hWnd, LeftKey, MOD_ALT, VK_LEFT);
	RegisterHotKey(hWnd, RightKey, MOD_ALT, VK_RIGHT);
	IRCContext->LeftKey = LeftKey;
	IRCContext->RightKey = RightKey;
}

/*Creates a new text IRC window*/
void CreateIRCWindow(HWND hWnd)
{
	assert(hWnd);
	IRCContext->hCurrentWindow = CreateWindowEx(0, TEXT("EDIT"), NULL, WS_CHILD | WS_VISIBLE |
		WS_VSCROLL | ES_LEFT | ES_MULTILINE | ES_AUTOVSCROLL, 0, 0, 0, 0, hWnd, NULL,
		(HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE), NULL);
	SendMessage(IRCContext->hCurrentWindow, EM_SETREADONLY, TRUE, 0);
	assert(IRCContext->hCurrentWindow);
	//	List.push_back(IRCContext->hCurrentWindow);
}

void ResizeWindows(DWORD nWidth, DWORD nHeight)
{
	for (uint32_t i = 0; i <= Array_Num(pArray); i++)
	{
		MoveWindow(Array_Index(pArray, i), 0, 0, nWidth, nHeight, TRUE);
	}
}

HFONT InitFont(HWND hWnd)
{
	static HFONT __hFont;
	__hFont = CreateFont(13, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, LOCALE_IDEFAULTANSICODEPAGE, OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, TEXT("Consolas"));
	SendMessage(hWnd, WM_SETFONT, (WPARAM)__hFont, (LPARAM)TRUE);
	return __hFont;
}

HWND InitTypeBuffer(HFONT hFont, HWND hParent)
{
	static HWND __HWND__;
	assert(hParent);
	__HWND__ = CreateWindowEx(WS_EX_OVERLAPPEDWINDOW, TEXT("EDIT"), NULL, WS_CHILD | WS_VISIBLE | ES_LEFT | WS_VSCROLL, 0, 0, 0, 0, hParent, NULL,
		(HINSTANCE)(GetWindowLong(hParent, GWL_HINSTANCE)), NULL);
	assert(__HWND__);
	SendMessage(__HWND__, WM_SETFONT, (WPARAM)hFont, (LPARAM)TRUE);
	return __HWND__;
}

HWND Create_Window(HWND hIn, HWND hWnd)
{
	hIn = CreateWindowEx(0, TEXT("Edit"), NULL, WS_CHILD | WS_VISIBLE |
		WS_VSCROLL | ES_LEFT | ES_MULTILINE | ES_AUTOVSCROLL, 0, 0, 0, 0, hWnd, NULL, (HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE),
		NULL);
	assert(pArray);
	Array_Add(pArray, hIn);
	return hIn;
}

void MessageBoxDebug(TCHAR* szText)
{
	assert(szText);
	TCHAR* szBuffer = (TCHAR*)malloc(sizeof(TCHAR*)*strlen(szText));
	assert(szBuffer);
	ZeroMemory(szBuffer, sizeof(TCHAR*)*strlen(szText));
	sprintf_s(szBuffer, strlen(szText) + 1, "%s", szText);
	MessageBox(0, szText, 0, 0);
}

void SetIRCWindowText(char* szString)
{
	assert(szString);
	uint32_t nLength = GetWindowTextLength(ContextIRC.hCurrentWindow);
	SendMessage(ContextIRC.hCurrentWindow, EM_SETSEL, (WPARAM)nLength, (LPARAM)nLength);
	SendMessage(ContextIRC.hCurrentWindow, EM_REPLACESEL, 0, (LPARAM)szString);
}