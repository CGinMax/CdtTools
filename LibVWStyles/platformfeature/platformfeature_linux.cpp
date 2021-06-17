#include "platformfeature.h"
#ifdef Q_OS_LINUX
#include <QWidget>
#include <QMouseEvent>
#ifndef X11_DEPRECATED
#include <xcb/xcb.h>
#include "x11/xutil.h"
#endif

// *************************** PlatformFeaturePrivate ***********************************
class PlatformFeaturePrivate final
{
public:
    explicit PlatformFeaturePrivate();
    virtual ~PlatformFeaturePrivate();

#ifndef X11_DEPRECATED
    XUtils::CornerEdge resizingCornerEdge;
    XUtils::XCursorType xCursorType;
    bool needXMouseButtonRelease;
    bool canMove;
#endif
};

#ifndef X11_DEPRECATED
PlatformFeaturePrivate::PlatformFeaturePrivate() : resizingCornerEdge(XUtils::CornerEdge::kInvalid), xCursorType(XUtils::XCursorType::kInvalid), needXMouseButtonRelease(false), canMove(false)
#else
PlatformFeaturePrivate::PlatformFeaturePrivate()
#endif
{

}

PlatformFeaturePrivate::~PlatformFeaturePrivate()
{

}


// *************************** PlatformFeature_Linux ************************************
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
    widget->setWindowFlags(windowFlags);
}

bool PlatformFeature::nativeEventWrapper(QWidget* widget, const QByteArray &eventType, void *message, long *result, QWidget* titleWidget, std::function<bool (QWidget*)> jujdeMoveFunctor)
{
    Q_UNUSED(result);
    Q_UNUSED(titleWidget);
    Q_UNUSED(jujdeMoveFunctor);
#ifndef X11_DEPRECATED
    Q_D(PlatformFeature);
    if (eventType == QByteArrayLiteral("xcb_generic_event_t")) {
        if (xcb_generic_event_t* xcb_generic_event = static_cast<xcb_generic_event_t* >(message)) {
            if ((xcb_generic_event->response_type & ~0x80) == XCB_GE_GENERIC) {
                if (auto xcb_ge_generic_event = static_cast<xcb_ge_generic_event_t* >(message)) {
                    if (xcb_ge_generic_event->event_type == 7) {//enter
                            if (d->needXMouseButtonRelease) {
                                d->needXMouseButtonRelease = false;

                                auto globalPos = QCursor::pos();
                                XUtils::SendButtonRelease(widget, widget->mapFromGlobal(globalPos), globalPos);
                            }
                    }
                }
            }
        }
    }
#endif
    return false;
}

void PlatformFeature::resizeEventWrapper(QWidget *widget)
{
#ifndef X11_DEPRECATED
    //XUtils::SetWindowExtents(this, this->layout()->contentsMargins(), resizeHandleWidth);
    XUtils::SetWindowExtents(widget, QMargins(0, 0, 0, 0), BorderWidth);
#endif
}

void PlatformFeature::mousePressEventWrapper(QWidget *widget, QMouseEvent *event, QWidget* titleWidget, std::function<bool (QWidget*)> jujdeMoveFunctor)
{
#ifndef X11_DEPRECATED
    Q_D(PlatformFeature);
    if (!titleWidget)
        return;

    if (event->button() == Qt::LeftButton) {
        if (!widget->isMaximized()) {
            auto ce = XUtils::GetCornerEdge(widget, event->x(), event->y(), QMargins(0, 0, 0, 0), BorderWidth);
            if (ce != XUtils::CornerEdge::kInvalid) {
                d->resizingCornerEdge = ce;
                XUtils::StartResizing(widget, QCursor::pos(), ce);
                d->needXMouseButtonRelease = true;
                return;
            }
        }

        auto pos = event->pos();
        if (titleWidget->rect().contains(pos)) {
            auto child = titleWidget->childAt(pos);
            d->canMove = child ? (jujdeMoveFunctor ? jujdeMoveFunctor(child) : false) : true;
        }
    }
#endif
}

void PlatformFeature::mouseReleaseEventWrapper()
{
#ifndef X11_DEPRECATED
    Q_D(PlatformFeature);
    d->resizingCornerEdge = XUtils::CornerEdge::kInvalid;
#endif
}

void PlatformFeature::eventWrapper(QWidget *widget, QEvent *event)
{
#ifndef X11_DEPRECATED
    Q_D(PlatformFeature);
    if (event->type() == QEvent::HoverMove) {
        if (!widget->isMaximized()) {
            if (auto hoverEvent = static_cast<QHoverEvent* >(event)) {
                if (d->resizingCornerEdge == XUtils::CornerEdge::kInvalid) {
                    const int x = hoverEvent->pos().x();
                    const int y = hoverEvent->pos().y();
                    auto xCursorType = XUtils::GetXCursorType(widget, x, y, QMargins(0, 0, 0, 0), BorderWidth);
                    if (xCursorType != XUtils::XCursorType::kInvalid) {
                        XUtils::SetCursorShape(widget, static_cast<unsigned int>(xCursorType));
                    } else {
                        if (d->xCursorType != XUtils::XCursorType::kInvalid) {
                            XUtils::ResetCursorShape(widget);
                        }
                    }
                    d->xCursorType = xCursorType;
                }
            }
        }
    }
#endif
}

bool PlatformFeature::eventFilterWrapper(QWidget *widget, QEvent *event)
{
#ifndef X11_DEPRECATED
    Q_D(PlatformFeature);
    if (event->type() == QEvent::MouseMove) {
        if (auto mouseEvent = static_cast<QMouseEvent* >(event)) {
            if (mouseEvent->buttons().testFlag(Qt::LeftButton)) {
                if (d->canMove) {
                    XUtils::MoveWindow(widget, Qt::LeftButton);
                    d->needXMouseButtonRelease = true;
                    //return true;
                }
            }
        }
    } else if (event->type() == QEvent::MouseButtonRelease) {
        if (d->canMove) {
            XUtils::CancelMoveWindow(widget, Qt::LeftButton);
            d->canMove = false;
        }
    }
#endif
    return false;
}
#endif
