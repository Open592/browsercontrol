// SPDX-License-Identifier: BSD-2-Clause

#include <iostream>

#include "WebView2BrowserWindow.hpp"

using namespace Microsoft::WRL;

LRESULT CALLBACK WebView2BrowserWindow::WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message) {
    case WM_CREATE: {
        auto*& createStruct = reinterpret_cast<const CREATESTRUCT*&>(lParam);
        auto* data = static_cast<std::shared_ptr<BrowserData>*>(createStruct->lpCreateParams);
        auto* instance = new WebView2BrowserWindow(createStruct->hwndParent, data);

        SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)instance);
    } break;
    case WM_DESTROY:
        delete Get(hwnd);

        SetWindowLongPtr(hwnd, GWLP_USERDATA, 0);

        PostQuitMessage(EXIT_SUCCESS);

        return EXIT_SUCCESS;
    case static_cast<UINT>(EventType::DESTROY):
        Get(hwnd)->Destroy();

        SetParent(hwnd, nullptr);
        DestroyWindow(hwnd);

        break;
    case static_cast<UINT>(EventType::RESIZE):
        Get(hwnd)->Resize();
        break;
    case static_cast<UINT>(EventType::NAVIGATE): {
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
            wc.lpszClassName = WindowClassName;
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

    return UnregisterClass(WindowClassName, instance);
}

HWND WebView2BrowserWindow::Create(HWND hostWindow, std::shared_ptr<BrowserData> data)
{
    HINSTANCE instance = WebView2BrowserWindow::Register();

    if (instance == nullptr) {
        return nullptr;
    }

    return CreateWindowEx(WMSZ_LEFT, WindowClassName, WindowName, WS_CHILDWINDOW | WS_HSCROLL, 0, 0, CW_USEDEFAULT,
        CW_USEDEFAULT, hostWindow, nullptr, instance, &data);
}

WebView2BrowserWindow::WebView2BrowserWindow(HWND parentWindow, const std::shared_ptr<BrowserData>* data)
    : m_data(*data)
    , m_parentWindow(parentWindow)
{
    InitializeWebView();
}

void WebView2BrowserWindow::InitializeWebView() noexcept
{
    CreateCoreWebView2Environment(Callback<ICoreWebView2CreateCoreWebView2EnvironmentCompletedHandler>(
        [&](HRESULT result, ICoreWebView2Environment* env) mutable noexcept -> HRESULT {
            if (FAILED(result)) {
                std::cout << "Failed to init env" << '\n';
                return result;
            }

            return env->CreateCoreWebView2Controller(m_parentWindow,
                Callback<ICoreWebView2CreateCoreWebView2ControllerCompletedHandler>(
                    [&](HRESULT result, ICoreWebView2Controller* controller) -> HRESULT {
                        if (FAILED(result)) {
                            std::cout << "Failed to init controller" << '\n';
                            return result;
                        }

                        if (controller != nullptr) {
                            m_controller = controller;
                            m_controller->get_CoreWebView2(&m_webView);
                        }

                        RECT bounds;
                        GetClientRect(m_parentWindow, &bounds);
                        m_controller->put_Bounds(bounds);

                        std::wcout << "Attempting to navigate to " << m_data->GetDestination() << '\n';
                        m_webView->Navigate(m_data->GetDestination().c_str());

                        return S_OK;
                    })
                    .Get());
        }).Get());
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
    std::wcout << m_data->GetDestination() << '\n';
}
