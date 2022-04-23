// SPDX-License-Identifier: BSD-2-Clause

#include "WebView2BrowserWindow.hpp"

#define WINDOW_CLASS_NAME "Jb"
#define WINDOW_NAME "jbw"

LRESULT CALLBACK WebView2BrowserWindow::WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message) {
    case WM_CREATE: {
        auto*& createStruct = reinterpret_cast<const CREATESTRUCT*&>(lParam);
        std::string destination(static_cast<const char*>(createStruct->lpCreateParams));
        auto* instance = new WebView2BrowserWindow(std::move(destination));

        SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)instance);
    } break;
    case WM_DESTROY:
        delete Get(hwnd);

        SetWindowLongPtr(hwnd, GWLP_USERDATA, 0);

        PostQuitMessage(EXIT_SUCCESS);

        return 0;
    case EventType::BROWSER_WINDOW_DESTROY:
        Get(hwnd)->Destroy();

        SetParent(hwnd, nullptr);
        DestroyWindow(hwnd);

        break;
    case EventType::BROWSER_WINDOW_RESIZE:
        Get(hwnd)->Resize(static_cast<int32_t>(wParam), static_cast<int32_t>(lParam));
        break;
    case EventType::BROWSER_WINDOW_NAVIGATE: {
        std::string destination(reinterpret_cast<const char*>(lParam));

        Get(hwnd)->Navigate(std::move(destination));
    } break;
    default:
        break;
    }

    return DefWindowProc(hwnd, message, wParam, lParam);
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
            wc.cbWndExtra = 0;
            wc.hInstance = module;

            RegisterClassEx(&wc);
        }
    }

    return module;
}

bool WebView2BrowserWindow::Unregister()
{
    HINSTANCE instance = WebView2BrowserWindow::Register();

    if (instance == nullptr) {
        return false;
    }

    return UnregisterClass(WINDOW_CLASS_NAME, instance);
}

HWND WebView2BrowserWindow::Create(HWND parentWindow, const char* initialDestination)
{
    HINSTANCE instance = WebView2BrowserWindow::Register();

    if (instance == nullptr) {
        return nullptr;
    }

    return CreateWindowEx(WMSZ_LEFT, WINDOW_CLASS_NAME, WINDOW_NAME, WS_CHILDWINDOW | WS_HSCROLL, 0, 0, CW_USEDEFAULT,
        CW_USEDEFAULT, parentWindow, nullptr, instance, (LPVOID)initialDestination);
}

WebView2BrowserWindow::WebView2BrowserWindow(std::string&& destination)
    : m_destination(std::move(destination))
{
}

void WebView2BrowserWindow::Destroy() { }
void WebView2BrowserWindow::Resize(int32_t width, int32_t height) { }
void WebView2BrowserWindow::Navigate(std::string&& destination) { m_destination = std::move(destination); }
