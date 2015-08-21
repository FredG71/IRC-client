#define UNICODE
#include "Main.hpp"

ATOM Windows::MyRegisterClass(HINSTANCE hInstance) {
    WNDCLASSEXW wcex;

    wcex.cbSize             = sizeof(WNDCLASSEX);
    Windows& WindowsContext = Windows::GetInstance();

    wcex.style              = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc        = WndProc;
    wcex.cbClsExtra         = 0;
    wcex.cbWndExtra         = 0;
    wcex.hInstance          = hInstance;
    wcex.hIcon              = LoadIcon(hInstance, IDI_APPLICATION);
    wcex.hCursor            = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground      = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName       = nullptr;
    wcex.lpszClassName      = WindowsContext.mWindowClass.c_str();
    wcex.hIconSm            = LoadIcon(hInstance, IDI_APPLICATION);

    return RegisterClassExW(&wcex);

}

BOOL Windows::InitializeInstance(HINSTANCE hInstance, int nCmdShow, LONG lWindowStyle = WS_OVERLAPPEDWINDOW) {
    Windows& WindowsContext = Windows::GetInstance();
    WindowsContext.SetInstance(hInstance);

    HWND hWnd = CreateWindowW(WindowsContext.mWindowClass.c_str(), WindowsContext.mTitle.c_str(), lWindowStyle, CW_USEDEFAULT,
        0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);
    if (!hWnd) {
        throw std::exception("Failed to create window!");
    }

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    WindowsContext.SetHandle(hWnd);

    return TRUE;
}

void Windows::PaintWindow() {
    // Do nothing at the moment
}

void Windows::DestroyWindow(int nExitCode) {
    PostQuitMessage(nExitCode);
}

template<typename T>
void
Windows::SetWindowAndClassString(T&& szTitle, T&& szWindow) {
    mTitle          = std::forward<T>(szTitle);
    mWindowClass    = std::forward<T>(szWindow);
}

template<typename... Args>
void
Windows::KeyTableEmplace(Args&&... Params) {
    KeyTable.emplace(std::forward<Args>(Params)...);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    Windows& WindowContext = Windows::GetInstance();

    switch (message)
    {
    case WM_HOTKEY:
        WindowContext.ProcessKeyTable(wParam);
        break;
    case WM_PAINT:
        WindowContext.PaintWindow();
        break;
    case WM_DESTROY:
        WindowContext.DestroyWindow(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

int APIENTRY wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow) {

    Windows& WindowsContext = Windows::GetInstance();

    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    WindowsContext.SetWindowAndClassString(std::wstring(L"Title"), std::wstring(L"WindowClass"));
    WindowsContext.MyRegisterClass(hInstance);
    try {
        WindowsContext.InitializeInstance(hInstance, nCmdShow);
    }
    catch (const std::exception& e) {
        MessageBoxA(nullptr, e.what(), "Exception occured", MB_ABORTRETRYIGNORE);
    }

    CreateHotkey(TEXT("MOD_ALT_VK_LEFT"), WindowsContext.GetHandle(), MOD_ALT, VK_LEFT);
    CreateHotkey(TEXT("VK_UP"), WindowsContext.GetHandle(), 0, VK_UP);
    MSG msg;
    while (GetMessage(&msg, nullptr, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return (int)msg.wParam;
}

void Windows::ProcessKeyTable(WPARAM wParam) {

    Windows& TempContext = Windows::GetInstance();

    if (wParam == GetKeyTableValue(TEXT("MOD_ALT_VK_LEFT"))) {
        MessageBox(TempContext.GetHandle(), TEXT("Works!"), TEXT("Test"), MB_OK);
    }
    if (wParam == GetKeyTableValue(TEXT("VK_UP"))) {
        MessageBox(TempContext.GetHandle(), TEXT("Pressed up"), TEXT("Test"), MB_OK);
    }
}
