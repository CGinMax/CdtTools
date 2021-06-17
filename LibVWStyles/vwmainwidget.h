#ifndef VWMAINWIDGET_H
#define VWMAINWIDGET_H

#include <QWidget>
#include "platformfeature/platformfeature.h"
#include "libvwstyles_global.h"

class LIBVWSTYLES_EXPORT VWMainWidget : public QWidget, public PlatformFeature
{

public:
    /*!
     * 在使用 setUserData 时，ID=1已被默认使用
     */
    enum UserDataID {
        InternalData = 1
    };
    explicit VWMainWidget(QWidget* parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags());
    ~VWMainWidget() override;

    void setTitleWidget(QWidget* titleWidget);
protected:
    void resizeEvent(QResizeEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void moveEvent(QMoveEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    bool event(QEvent* event) override;
    bool nativeEvent(const QByteArray &eventType, void *message, long *result) override;
    bool eventFilter(QObject* watched, QEvent* event) override;

};

#endif // VWMAINWIDGET_H
