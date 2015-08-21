#pragma once
#include <iostream>
#include <Windows.h>
#include <exception>
#include <unordered_map>

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

namespace Hash {
	auto Equalizer = [](TCHAR* a, TCHAR* b) -> bool {
        return !wcscmp(a, b);
	};
}

class Windows {
public:
	static Windows& GetInstance() {
		static Windows Instance;
		return Instance;
	}
public:
	ATOM MyRegisterClass(HINSTANCE Instance);

	// Creates the actual window, i.e. calls CreateWindow, followed by ShowWindow and UpdateWindow
	// throw if the returned handle from hWnd is null
	BOOL InitializeInstance(HINSTANCE hInstance, int nCmdShow, LONG lWindowStyle);
	void PaintWindow();
	void DestroyWindow(int nExitCode);

	template<typename T>
	void SetWindowAndClassString(T&& szTitle, T&& szWindow);

	template<typename ...Args>
	void KeyTableEmplace(Args&&... __Args);

	void SetInstance(HINSTANCE hNewInstance) {
		if (hNewInstance) {
			mhInstance = hNewInstance;
		}
	}

	void SetHandle(HWND hNewHandle) {
		if (hNewHandle) {
			mhWnd = hNewHandle;
		}
	}

	HINSTANCE GetWindowInstance() const {
		return mhInstance;
	}

	HWND GetHandle() const {
		return mhWnd;
	}

	ATOM GetKeyTableValue(TCHAR* Key) const {
		return KeyTable.at(Key);
	}
	void ProcessKeyTable(WPARAM wParam);

	std::unordered_map<TCHAR*, ATOM, std::hash<TCHAR*>, decltype(Hash::Equalizer)> KeyTable{ 10, std::hash<TCHAR*>(), Hash::Equalizer };
private:
	Windows() {}
	~Windows() {}
	Windows(Windows const&)				= delete;
	Windows& operator=(Windows const&)	= delete;

	Windows(Windows&&)					= delete;
	Windows& operator=(Windows&&)		= delete;

	std::wstring mTitle;
	std::wstring mWindowClass;
	HINSTANCE mhInstance;
	HWND mhWnd;
};

// Creates a hotkey, registers it and adds it to the KeyTable unordered map using the szKeyName Key.
auto CreateHotkey = [](TCHAR* szKeyName, HWND hWnd, UINT fsModifiers, UINT vk) -> ATOM{
	ATOM NewKey = GlobalAddAtom(szKeyName);
	RegisterHotKey(hWnd, NewKey, fsModifiers, vk);
	Windows& WindowsContext = Windows::GetInstance();
	WindowsContext.KeyTableEmplace(szKeyName, NewKey);
	return NewKey;
};
