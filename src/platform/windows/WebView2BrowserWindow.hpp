// SPDX-License-Identifier: BSD-2-Clause
#include <jawt.h>
#include <jawt_md.h>
#include <jni.h>

#include "../../AbstractBrowserWindow.h"

/**
 * @brief Implementation of a WebView2 browser window for displaying
 * advertisements within the AppletViewer.
 */
class WebView2BrowserWindow : public AbstractBrowserWindow {
public:
    static JAWT_Win32DrawingSurfaceInfo* resolveDrawingSurfaceInfo(JNIEnv*, jobject);

    explicit WebView2BrowserWindow(JAWT_Win32DrawingSurfaceInfo*);
    virtual void resize(int32_t, int32_t) override;
    virtual void navigate(std::string_view) override;

private:
    JAWT_Win32DrawingSurfaceInfo* m_drawingSurfaceInfo;
};
