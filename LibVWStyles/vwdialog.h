#ifndef VWDIALOG_H
#define VWDIALOG_H


#include <QDialog>
#include "platformfeature/platformfeature.h"
#include "libvwstyles_global.h"

class LIBVWSTYLES_EXPORT VWDialog : public QDialog, public PlatformFeature
{
public:
    enum UserDataID {
        InternalData = 1
    };

    explicit VWDialog(QWidget *parent = nullptr, Qt::WindowFlags flags = Qt::WindowFlags());
    explicit VWDialog(QDialogPrivate& dd, QWidget* parent, Qt::WindowFlags f = Qt::WindowFlags());
    virtual ~VWDialog();

    void setTitleWidget(QWidget* titleWidget);

    /*!
     * 在使用 setUserData 时，ID=1已被默认使用
     */
    using QDialog::setUserData;
protected:
    virtual void resizeEvent(QResizeEvent* event) override;
    virtual void mousePressEvent(QMouseEvent* event) override;
    virtual void moveEvent(QMoveEvent* event) override;
    virtual void mouseReleaseEvent(QMouseEvent* event) override;
    virtual bool event(QEvent* event) override;
    virtual bool nativeEvent(const QByteArray &eventType, void *message, long *result) override;
    virtual bool eventFilter(QObject* watched, QEvent* event) override;
};

#endif // VWDIALOG_H
