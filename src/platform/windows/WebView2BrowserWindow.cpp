// SPDX-License-Identifier: BSD-2-Clause

#include "WebView2BrowserWindow.hpp"

#define WINDOW_CLASS_NAME "Jb"
#define WINDOW_NAME "jbw"

LRESULT CALLBACK WebView2BrowserWindow::WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message) {
    case WM_CREATE: {
        auto* instance = new WebView2BrowserWindow();

        SetWindowLongPtr(hwnd, 0, (LONG_PTR)instance);

    } break;
    case WM_NCDESTROY:
        delete Get(hwnd);

        SetWindowLongPtr(hwnd, 0, 0);

        break;
    case EventType::BROWSER_WINDOW_DESTROY:
        Get(hwnd)->Destroy();

        break;
    case EventType::BROWSER_WINDOW_RESIZE:
        break;
    case EventType::BROWSER_WINDOW_NAVIGATE:
        break;
    default:
        return DefWindowProc(hwnd, message, wParam, lParam);
    }
}

HINSTANCE WebView2BrowserWindow::Register()
{
    /**
     * Keep a static reference to our module handle.
     *
     * - This allows for easy access to our HINSTANCE where the browser window
     * was created.
     * - This restricts us to only registering the browser window a single time.
     */
    static HMODULE module = nullptr;

    if (!module) {
        if (GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
                reinterpret_cast<LPCSTR>(&Register), &module)) {
            WNDCLASSEX wc = { sizeof(WNDCLASSEX) };
            wc.lpfnWndProc = &WndProc;
            wc.lpszClassName = WINDOW_CLASS_NAME;
            wc.cbWndExtra = sizeof(WebView2BrowserWindow*);
            wc.hInstance = module;

            RegisterClassEx(&wc);
        }
    }

    return module;
}

HWND WebView2BrowserWindow::Create(HWND parentWindow)
{
    HINSTANCE instance = WebView2BrowserWindow::Register();

    if (instance == nullptr) {
        return nullptr;
    }

    return CreateWindowEx(WMSZ_LEFT, WINDOW_CLASS_NAME, WINDOW_NAME, WS_CHILDWINDOW | WS_HSCROLL, 0, 0, CW_USEDEFAULT,
        CW_USEDEFAULT, parentWindow, nullptr, instance, nullptr);
}

void WebView2BrowserWindow::Destroy() { }
void WebView2BrowserWindow::Resize(int32_t width, int32_t height) { }
void WebView2BrowserWindow::Navigate(std::string_view destination) { }