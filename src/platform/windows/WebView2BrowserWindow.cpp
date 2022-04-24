// SPDX-License-Identifier: BSD-2-Clause

#include <iostream>

#include "WebView2BrowserWindow.hpp"

#define WINDOW_CLASS_NAME "Jb"
#define WINDOW_NAME "jbw"

LRESULT CALLBACK WebView2BrowserWindow::WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message) {
    case WM_CREATE: {
        auto*& createStruct = reinterpret_cast<const CREATESTRUCT*&>(lParam);
        auto* data = static_cast<std::shared_ptr<BrowserData>*>(createStruct->lpCreateParams);
        auto* instance = new WebView2BrowserWindow(data);

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
        Get(hwnd)->Resize();
        break;
    case EventType::BROWSER_WINDOW_NAVIGATE: {
        Get(hwnd)->Navigate();
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

HWND WebView2BrowserWindow::Create(HWND parentWindow, std::shared_ptr<BrowserData> data)
{
    HINSTANCE instance = WebView2BrowserWindow::Register();

    if (instance == nullptr) {
        return nullptr;
    }

    return CreateWindowEx(WMSZ_LEFT, WINDOW_CLASS_NAME, WINDOW_NAME, WS_CHILDWINDOW | WS_HSCROLL, 0, 0, CW_USEDEFAULT,
        CW_USEDEFAULT, parentWindow, nullptr, instance, &data);
}

WebView2BrowserWindow::WebView2BrowserWindow(const std::shared_ptr<BrowserData>* data)
    : m_data(*data)
{
    std::cout << m_data->GetDestination() << '\n';
}

void WebView2BrowserWindow::Destroy() { std::cout << "Attempting to destroy browser window"; }

void WebView2BrowserWindow::Resize()
{
    std::cout << m_data->GetWidth() << '\n';
    std::cout << m_data->GetHeight() << '\n';
}
void WebView2BrowserWindow::Navigate()
{
    std::cout << "Attempting to navigate to: ";
    std::cout << m_data->GetDestination() << '\n';
}
