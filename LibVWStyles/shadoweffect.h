#ifndef SHADOWEFFECT_H
#define SHADOWEFFECT_H

#include <QGraphicsEffect>

namespace Ui {
class ShadowEffect : public QGraphicsEffect
{
    Q_OBJECT
public:
    explicit ShadowEffect(QObject *parent = nullptr);

    QRectF boundingRectFor(const QRectF &sourceRect) const override;
    qreal blurRadius() const;
    void setBlurRadius(const qreal &blurRadius);

    qreal distance() const;
    void setDistance(const qreal &distance);

    QColor color() const;
    void setColor(const QColor &color);

protected:
    void draw(QPainter *painter) override;
signals:

public slots:

private:
    qreal m_blurRadius;
    qreal m_distance;
    QColor m_color;
};
}  // namespace Ui


#endif // SHADOWEFFECT_H
