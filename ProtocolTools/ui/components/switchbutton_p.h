#ifndef SWITCHBUTTON_P_H
#define SWITCHBUTTON_P_H

#include <Qt>
#include <QColor>
#include <QMargins>
#include <QRect>
class QStateMachine;
class QState;
class SwitchButton;
class SwitchButtonThumb;
class SwitchButtonTrack;

class SwitchButtonPrivate
{
    Q_DISABLE_COPY(SwitchButtonPrivate)
    Q_DECLARE_PUBLIC(SwitchButton)
public:
    explicit SwitchButtonPrivate(SwitchButton *q);
    ~SwitchButtonPrivate();

    void init();
    void setupProperties();

    SwitchButton              *const q_ptr;
    SwitchButtonThumb         *thumb;
    SwitchButtonTrack         *track;
    QMargins                   trackMargins;
    QRect                      trackRect;
    QMargins                   thumbMargins;
    QStateMachine             *stateMachine;
    QState                    *offState;
    QState                    *onState;
    Qt::Orientation            orientation;
    QColor                     disabledColor;
    QColor                     activeColor;
    QColor                     inactiveColor;
    QColor                     trackColor;
    int                        penWidth;
};

#endif // SWITCHBUTTON_P_H
