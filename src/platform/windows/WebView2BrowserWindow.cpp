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
        auto* instance = new WebView2BrowserWindow(hwnd, data);

        SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)instance);
    } break;
    case WM_DESTROY:
        delete Get(hwnd);

        SetWindowLongPtr(hwnd, GWLP_USERDATA, 0);
        PostQuitMessage(EXIT_SUCCESS);

        return EXIT_SUCCESS;
    case static_cast<UINT>(EventType::DESTROY):
        Get(hwnd)->Destroy();
        break;
    case static_cast<UINT>(EventType::RESIZE):
        Get(hwnd)->Resize();
        break;
    case static_cast<UINT>(EventType::NAVIGATE):
        Get(hwnd)->Navigate();
        break;
    default:
        return DefWindowProc(hwnd, message, wParam, lParam);
    }

    return 0;
}

std::optional<std::wstring> WebView2BrowserWindow::GetUserDataDirectory() noexcept
{
    try {
        return std::filesystem::temp_directory_path().wstring();
    } catch (std::filesystem::filesystem_error& e) {
        return std::nullopt;
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

    return CreateWindowEx(WS_EX_LEFT, WindowClassName, WindowName, WS_CHILDWINDOW | WS_VISIBLE, 0, 0, data->GetWidth(),
        data->GetHeight(), hostWindow, nullptr, instance, &data);
}

WebView2BrowserWindow::WebView2BrowserWindow(HWND parentWindow, const std::shared_ptr<BrowserData>* data)
    : m_data(*data)
    , m_parentWindow(parentWindow)
{
    InitializeWebView();
}

void WebView2BrowserWindow::InitializeWebView() noexcept
{
    auto userDataDirectory = GetUserDataDirectory();

    if (!userDataDirectory.has_value()) {
        return;
    }

    CreateCoreWebView2EnvironmentWithOptions(nullptr, userDataDirectory->c_str(), nullptr,
        Callback<ICoreWebView2CreateCoreWebView2EnvironmentCompletedHandler>(
            [&](HRESULT result, ICoreWebView2Environment* env) -> HRESULT {
                if (FAILED(result) || !env) {
                    return E_FAIL;
                }

                env->CreateCoreWebView2Controller(m_parentWindow,
                    Callback<ICoreWebView2CreateCoreWebView2ControllerCompletedHandler>(
                        [&](HRESULT result, ICoreWebView2Controller* controller) -> HRESULT {
                            if (FAILED(result) || !controller) {
                                return E_FAIL;
                            }

                            m_controller = controller;
                            m_controller->get_CoreWebView2(&m_webView);

                            ICoreWebView2Settings* settings;
                            m_webView->get_Settings(&settings);

                            settings->put_AreDefaultContextMenusEnabled(false);
                            settings->put_AreDefaultScriptDialogsEnabled(false);
                            settings->put_IsBuiltInErrorPageEnabled(false);
                            settings->put_AreDevToolsEnabled(false);
                            settings->put_IsStatusBarEnabled(false);
                            settings->put_IsZoomControlEnabled(false);

                            Resize();
                            Navigate();

                            return S_OK;
                        })
                        .Get());
                return S_OK;
            })
            .Get());
}

void WebView2BrowserWindow::Destroy()
{
    SetParent(m_parentWindow, nullptr);
    DestroyWindow(m_parentWindow);

    std::cout << "Attempting to destroy browser window" << '\n';
}

void WebView2BrowserWindow::Resize()
{
    SetWindowPos(m_parentWindow, nullptr, 0, 0, m_data->GetWidth(), m_data->GetHeight(),
        SWP_NOZORDER | SWP_NOACTIVATE | SWP_NOMOVE | SWP_FRAMECHANGED);

    if (m_controller != nullptr) {
        RECT bounds;
        GetClientRect(m_parentWindow, &bounds);

        m_controller->put_Bounds(bounds);
    }
}

void WebView2BrowserWindow::Navigate()
{
    if (m_webView != nullptr) {
        m_webView->Navigate(m_data->GetDestination().c_str());
    }
}
