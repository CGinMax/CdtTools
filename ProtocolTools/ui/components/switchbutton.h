#ifndef SWITCHBUTTON_H
#define SWITCHBUTTON_H

#include <QAbstractButton>
#include <QScopedPointer>

class SwitchButtonPrivate;

class SwitchButton : public QAbstractButton
{
    Q_OBJECT

    Q_PROPERTY(QColor disabledColor WRITE setDisabledColor READ disabledColor)
    Q_PROPERTY(QColor activeColor WRITE setActiveColor READ activeColor)
    Q_PROPERTY(QColor inactiveColor WRITE setInactiveColor READ inactiveColor)
    Q_PROPERTY(QColor trackColor WRITE setTrackColor READ trackColor)
public:
    explicit SwitchButton(QWidget *parent = nullptr);
    ~SwitchButton() override;

    void setDisabledColor(const QColor &color);
    QColor disabledColor() const;

    void setActiveColor(const QColor &color);
    QColor activeColor() const;

    void setInactiveColor(const QColor &color);
    QColor inactiveColor() const;

    void setTrackColor(const QColor &color);
    QColor trackColor() const;

    void setOrientation(Qt::Orientation orientation);
    Qt::Orientation orientation() const;

    void setThumbMargins(const QMargins& margins);
    QMargins thumbMargins() const;

    void setTrackMargins(const QMargins& margins);
    QMargins trackMargins() const;

    void setPenWidth(int penWidth);
    int penWidth() const;

    QRect thumbRect();

    QRect trackRect();

    void setOffStateWidth(int offWidth);

    QSize sizeHint() const Q_DECL_OVERRIDE;

protected:
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;
signals:

public slots:

private:
    Q_DECLARE_PRIVATE(SwitchButton)
    const QScopedPointer<SwitchButtonPrivate> d_ptr;

};

#endif // SWITCHBUTTON_H
