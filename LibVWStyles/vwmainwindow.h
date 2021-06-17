#ifndef VWMAINWINDOW_H
#define VWMAINWINDOW_H


#include <QMainWindow>
#include "platformfeature/platformfeature.h"
#include "libvwstyles_global.h"

class LIBVWSTYLES_EXPORT VWMainWindow : public QMainWindow, public PlatformFeature
{
public:
    /*!
     * 在使用 setUserData 时，ID=1已被默认使用
     */
    enum UserDataID {
        InternalData = 1
    };

    explicit VWMainWindow(QWidget *parent = nullptr, Qt::WindowFlags flags = Qt::WindowFlags());
    virtual ~VWMainWindow();

    void setTitleWidget(QWidget* titleWidget);
protected:
    virtual void resizeEvent(QResizeEvent* event) override;
    virtual void mousePressEvent(QMouseEvent* event) override;
    virtual void moveEvent(QMoveEvent* event) override;
    virtual void mouseReleaseEvent(QMouseEvent* event) override;
    virtual bool event(QEvent* event) override;
    virtual bool nativeEvent(const QByteArray &eventType, void *message, long *result) override;
    virtual bool eventFilter(QObject* watched, QEvent* event) override;
};

#endif // VWMAINWINDOW_H
