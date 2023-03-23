// SPDX-License-Identifier: BSD-2-Clause

#include "src/BrowserContext.hpp"

#include "WebView2BrowserWindow.hpp"

using namespace Microsoft::WRL;

LRESULT CALLBACK WebView2BrowserWindow::WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message) {
    case WM_CREATE: {
        auto* instance = new WebView2BrowserWindow(hwnd);

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

bool WebView2BrowserWindow::EnsureWebViewIsAvailable() noexcept
{
    wchar_t* version;
    HRESULT hr = GetAvailableCoreWebView2BrowserVersionString(nullptr, &version);

    if (FAILED(hr) || version == nullptr) {
        return InstallWebView();
    }

    // TODO: Log version found
    CoTaskMemFree(version);

    return true;
}

bool WebView2BrowserWindow::InstallWebView() noexcept
{
    // Provides a download link for the evergreen installer. This will install the WebView2 runtime
    // on the user's computer. After installation Microsoft will keep the runtime updated automatically.
    const wchar_t* evergreenInstallerURL = L"https://go.microsoft.com/fwlink/p/?LinkId=2124703";
    const wchar_t* installerFilename = L"MicrosoftEdgeWebview2Setup.exe";
    std::wstring installerPath = (std::filesystem::temp_directory_path() / installerFilename).wstring();

    HRESULT hr = URLDownloadToFile(nullptr, evergreenInstallerURL, installerPath.c_str(), 0, nullptr);

    if (hr != S_OK) {
        return false;
    }

    STARTUPINFO si;
    PROCESS_INFORMATION pi;

    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));

    std::wstring command = std::format(L"{} /silent /install", installerPath);
    bool success = CreateProcess(nullptr, command.data(), nullptr, nullptr, false, 0, nullptr, nullptr, &si, &pi);

    if (!success) {
        return false;
    }

    // Wait until installation is complete
    if (WaitForSingleObject(pi.hProcess, INFINITE) == WAIT_FAILED) {
        goto handle_error_and_close;
    }

    DWORD exit_code;
    if (!SUCCEEDED(GetExitCodeProcess(pi.hProcess, &exit_code))) {
        goto handle_error_and_close;
    }

    if (exit_code != EXIT_SUCCESS) {
        goto handle_error_and_close;
    }

    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);

    return true;

handle_error_and_close:
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);

    return false;
}

std::optional<std::wstring> WebView2BrowserWindow::GetUserDataDirectory() noexcept
{
    PWSTR localAppDataPath = nullptr;
    auto hr = SHGetKnownFolderPath(FOLDERID_LocalAppData, KF_FLAG_DEFAULT, nullptr, &localAppDataPath);

    if (!SUCCEEDED(hr)) {
        return std::nullopt;
    }

    try {
        auto path = std::filesystem::path(localAppDataPath);

        return std::filesystem::canonical(path).wstring();
    } catch (std::filesystem::filesystem_error&) {
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
                reinterpret_cast<LPCWSTR>(&Register), &module)) {
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

HWND WebView2BrowserWindow::Create(HWND hostWindow)
{
    HINSTANCE instance = WebView2BrowserWindow::Register();

    if (instance == nullptr) {
        return nullptr;
    }

    auto height = BrowserContext::the().GetBrowserData()->GetHeight();
    auto width = BrowserContext::the().GetBrowserData()->GetWidth();

    return CreateWindowEx(WS_EX_LEFT, WindowClassName, WindowName, WS_CHILDWINDOW | WS_VISIBLE, 0, 0, width, height,
        hostWindow, nullptr, instance, nullptr);
}

WebView2BrowserWindow::WebView2BrowserWindow(HWND parentWindow)
    : m_parentWindow(parentWindow)
{
    if (!EnsureWebViewIsAvailable()) {
        BrowserContext::the().GetBrowserData()->SetState(BrowserData::State::FAILED_TO_START);

        return;
    }

    if (!InitializeWebView()) {
        BrowserContext::the().GetBrowserData()->SetState(BrowserData::State::FAILED_TO_START);
    } else {
        // We have successfully created our browser window and are prepared to start accepting
        // messages. At this point initialization has finished, and we can signal success back to
        // the caller
        BrowserContext::the().GetBrowserData()->SetState(BrowserData::State::RUNNING);
    }
}

bool WebView2BrowserWindow::InitializeWebView() noexcept
{
    auto userDataDirectory = GetUserDataDirectory();

    if (!userDataDirectory.has_value()) {
        return false;
    }

    HRESULT hr = CreateCoreWebView2EnvironmentWithOptions(nullptr, userDataDirectory->c_str(), nullptr,
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

    if (SUCCEEDED(hr)) {
        return true;
    } else {
        return false;
    }
}

void WebView2BrowserWindow::Destroy()
{
    // At this point we are about to destroy the backing windows of the browser control. Any further calls to
    // exported functions will result in failures, so we must mark the browser control as terminated.
    BrowserContext::the().GetBrowserData()->SetState(BrowserData::State::TERMINATED);

    SetParent(m_parentWindow, nullptr);
    DestroyWindow(m_parentWindow);
}

void WebView2BrowserWindow::Resize()
{
    auto width = BrowserContext::the().GetBrowserData()->GetWidth();
    auto height = BrowserContext::the().GetBrowserData()->GetHeight();

    SetWindowPos(
        m_parentWindow, nullptr, 0, 0, width, height, SWP_NOZORDER | SWP_NOACTIVATE | SWP_NOMOVE | SWP_FRAMECHANGED);

    if (m_controller != nullptr) {
        RECT bounds;
        GetClientRect(m_parentWindow, &bounds);

        m_controller->put_Bounds(bounds);
    }
}

void WebView2BrowserWindow::Navigate()
{
    if (m_webView != nullptr) {
        m_webView->Navigate(BrowserContext::the().GetBrowserData()->GetDestination().c_str());
    }
}
