#include "platformfeature.h"
#ifdef Q_OS_WIN
#include <QWidget>
#include <windows.h>
#include <windowsx.h>
#include <uxtheme.h>
#include <dwmapi.h>
#include <qt_windows.h>


// *************************** PlatformFeaturePrivate ***********************************
class PlatformFeaturePrivate final
{
public:
    explicit PlatformFeaturePrivate();
    virtual ~PlatformFeaturePrivate();
};

PlatformFeaturePrivate::PlatformFeaturePrivate()
{

}

PlatformFeaturePrivate::~PlatformFeaturePrivate()
{

}

// *************************** PlatformFeature_Win **************************************
PlatformFeature::PlatformFeature() : pfp(new PlatformFeaturePrivate)
{

}

PlatformFeature::~PlatformFeature()
{

}

void PlatformFeature::initialize(QWidget *widget)
{
    auto windowFlags = widget->windowFlags();
    windowFlags |= Qt::FramelessWindowHint;
    windowFlags |= Qt::Window;
    windowFlags |= Qt::WindowSystemMenuHint;
    windowFlags |= Qt::WindowMinimizeButtonHint;
    widget->setWindowFlags(windowFlags);

    HWND hwnd = (HWND)widget->winId();
    DWORD style = ::GetWindowLong(hwnd, GWL_STYLE);
    ::SetWindowLong(hwnd, GWL_STYLE, style | WS_MAXIMIZEBOX | WS_THICKFRAME | WS_CAPTION);

    const MARGINS shadow = { 1, 1, 1, 1 };
    DwmExtendFrameIntoClientArea(hwnd, &shadow);
}

bool PlatformFeature::nativeEventWrapper(QWidget* widget, const QByteArray &eventType, void *message, long *result, std::function<bool (const QPoint& pos, bool isGlobal)> jujdePointInTitleFunctor)
{
    if (eventType == QByteArrayLiteral("windows_generic_MSG")) {
        if (auto msg = static_cast<MSG* >(message)) {
            switch (msg->message) {
            case WM_NCCALCSIZE: {
                //NCCALCSIZE_PARAMS& params = *reinterpret_cast<NCCALCSIZE_PARAMS*>(msg->lParam);
                //                if (params.rgrc[0].top != 0)
                //                    params.rgrc[0].top -= 1;

                //this kills the window frame and title bar we added with WS_THICKFRAME and WS_CAPTION
                *result = WVR_REDRAW;
                return true;
            }
            case WM_NCHITTEST: {
                *result = 0;

                RECT winrect = {};
                GetWindowRect(HWND(widget->winId()), &winrect);

                auto x = GET_X_LPARAM(msg->lParam);
                auto y = GET_Y_LPARAM(msg->lParam);

                bool resizeWidth = widget->minimumWidth() != widget->maximumWidth();
                bool resizeHeight = widget->minimumHeight() != widget->maximumHeight();
                if(resizeWidth) {
                    //left border
                    if (x >= winrect.left && x < winrect.left + BorderWidth)
                        *result = HTLEFT;

                    //right border
                    if (x < winrect.right && x >= winrect.right - BorderWidth)
                        *result = HTRIGHT;
                }

                if(resizeHeight) {
                    //bottom border
                    if (y < winrect.bottom && y >= winrect.bottom - BorderWidth)
                        *result = HTBOTTOM;

                    //top border
                    if (y >= winrect.top && y < winrect.top + BorderWidth)
                        *result = HTTOP;
                }

                if(resizeWidth && resizeHeight) {
                    //bottom left corner
                    if (x >= winrect.left && x < winrect.left + BorderWidth && y < winrect.bottom && y >= winrect.bottom - BorderWidth)
                        *result = HTBOTTOMLEFT;

                    //bottom right corner
                    if (x < winrect.right && x >= winrect.right - BorderWidth && y < winrect.bottom && y >= winrect.bottom - BorderWidth)
                        *result = HTBOTTOMRIGHT;

                    //top left corner
                    if (x >= winrect.left && x < winrect.left + BorderWidth && y >= winrect.top && y < winrect.top + BorderWidth)
                        *result = HTTOPLEFT;

                    //top right corner
                    if (x < winrect.right && x >= winrect.right - BorderWidth && y >= winrect.top && y < winrect.top + BorderWidth)
                        *result = HTTOPRIGHT;
                }

                if (0 != *result)
                    return true;

                if (jujdePointInTitleFunctor) {
                    double dpr = widget->devicePixelRatioF();
                    auto pos = QPoint(x / dpr,y / dpr);
                    if (jujdePointInTitleFunctor(pos, true)) {
                        *result = HTCAPTION;
                        return true;
                    }
                }
                return false;
            }
            case WM_GETMINMAXINFO: {
                if (::IsZoomed(msg->hwnd)) {
                    RECT frame = { 0, 0, 0, 0 };
                    AdjustWindowRectEx(&frame, WS_OVERLAPPEDWINDOW, FALSE, 0);

                    frame.left = qAbs(frame.left);
                    frame.top = qAbs(frame.bottom);
                    frame.right = qAbs(frame.right);
                    frame.bottom = qAbs(frame.bottom);
                    widget->setContentsMargins(frame.left, frame.top, frame.right, frame.bottom);
                }else {
                    widget->setContentsMargins(0, 0, 0, 0);
                }
                return false;
            }
            default:
                break;
            }
        }
    }
    return false;
}

void PlatformFeature::resizeEventWrapper(QWidget *widget)
{
    Q_UNUSED(widget);
}

void PlatformFeature::mousePressEventWrapper(QWidget *widget, QMouseEvent *event, std::function<bool (const QPoint& pos)> jujdePointInTitleFunctor)
{
    Q_UNUSED(widget);
    Q_UNUSED(event);
    Q_UNUSED(jujdePointInTitleFunctor);
}

void PlatformFeature::mouseReleaseEventWrapper()
{

}

void PlatformFeature::eventWrapper(QWidget *widget, QEvent *event)
{
    Q_UNUSED(widget);
    Q_UNUSED(event);
}

bool PlatformFeature::eventFilterWrapper(QWidget *widget, QEvent *event)
{
    Q_UNUSED(widget);
    Q_UNUSED(event);
    return false;
}
#endif
