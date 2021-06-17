#ifndef VWTOOLSETSWINDOW_H
#define VWTOOLSETSWINDOW_H

#include "vwmainwindow.h"
#include "vwmainwidget.h"

QT_FORWARD_DECLARE_CLASS(VWToolSetsWindowPrivate)

class VWToolSetsWindow final : public VWMainWidget
{
    Q_OBJECT
public:
    explicit VWToolSetsWindow(QWidget *parent = nullptr);
    ~VWToolSetsWindow() override;


protected:
//    void paintEvent(QPaintEvent *event) override;

signals:

public slots:

private:
    QScopedPointer<VWToolSetsWindowPrivate> d_ptr;
    Q_DECLARE_PRIVATE(VWToolSetsWindow)
};

#endif // VWTOOLSETSWINDOW_H
