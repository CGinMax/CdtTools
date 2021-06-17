#ifndef PLATFORMFEATURE_H
#define PLATFORMFEATURE_H


#include <QtGlobal>
#include <QScopedPointer>
#include <functional>

QT_FORWARD_DECLARE_CLASS(QWidget)
QT_FORWARD_DECLARE_CLASS(QPoint)
QT_FORWARD_DECLARE_CLASS(PlatformFeaturePrivate)
QT_FORWARD_DECLARE_CLASS(QMouseEvent)
QT_FORWARD_DECLARE_CLASS(QEvent)
class PlatformFeature
{
public:
    virtual ~PlatformFeature();
protected:
    enum {
        BorderWidth = 5
    };

    explicit PlatformFeature();

    void initialize(QWidget* widget);
    bool nativeEventWrapper(QWidget* widget, const QByteArray &eventType, void *message, long *result, QWidget* titleWidget, std::function<bool (QWidget*)> jujdeMoveFunctor);
    void resizeEventWrapper(QWidget* widget);
    void mousePressEventWrapper(QWidget* widget, QMouseEvent* event, QWidget* titleWidget, std::function<bool (QWidget*)> jujdeMoveFunctor);
    void mouseReleaseEventWrapper();
    void eventWrapper(QWidget* widget, QEvent* event);
    bool eventFilterWrapper(QWidget* widget, QEvent* event);
private:
    QScopedPointer<PlatformFeaturePrivate> pfp;
    Q_DECLARE_PRIVATE_D(pfp, PlatformFeature)
};

#endif // PLATFORMFEATURE_H
