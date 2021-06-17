#include "shadoweffect.h"
#include <QPainter>
#include <QImage>

Ui::ShadowEffect::ShadowEffect(QObject *parent)
    : QGraphicsEffect(parent)
    , m_blurRadius(5.0)
    , m_distance(1.0)
    , m_color(QColor(0, 0, 0, 80))
{

}

QT_BEGIN_NAMESPACE
  extern Q_WIDGETS_EXPORT void qt_blurImage(QPainter *p, QImage &blurImage, qreal radius, bool quality, bool alphaOnly, int transposed = 0 );
QT_END_NAMESPACE

QRectF Ui::ShadowEffect::boundingRectFor(const QRectF &sourceRect) const
{
    qreal delta = blurRadius() + distance();
    return sourceRect.united(sourceRect.adjusted(-delta, -delta, delta, delta));
}

void Ui::ShadowEffect::draw(QPainter *painter)
{
    // if nothing to show outside the item, just draw source
       if ((blurRadius() + distance()) <= 0) {
           drawSource(painter);
           return;
       }

       PixmapPadMode mode = QGraphicsEffect::PadToEffectiveBoundingRect;
       QPoint offset;
       const QPixmap px = sourcePixmap(Qt::DeviceCoordinates, &offset, mode);

       // return if no source
       if (px.isNull()) {
           return;
       }

       // save world transform
       QTransform restoreTransform = painter->worldTransform();
       painter->setWorldTransform(QTransform());

       // Calculate size for the background image
       QSize szi(px.size().width() + 2 * distance(), px.size().height() + 2 * distance());

       QImage tmp(szi, QImage::Format_ARGB32_Premultiplied);
       QPixmap scaled = px.scaled(szi);
       tmp.fill(0);
       QPainter tmpPainter(&tmp);
       tmpPainter.setCompositionMode(QPainter::CompositionMode_Source);
       tmpPainter.drawPixmap(QPointF(-distance(), -distance()), scaled);
       tmpPainter.end();

       // blur the alpha channel
       QImage blurred(tmp.size(), QImage::Format_ARGB32_Premultiplied);
       blurred.fill(0);
       QPainter blurPainter(&blurred);
       qt_blurImage(&blurPainter, tmp, blurRadius(), false, true);
       blurPainter.end();

       tmp = blurred;

       // blacken the image...
       tmpPainter.begin(&tmp);
       tmpPainter.setCompositionMode(QPainter::CompositionMode_SourceIn);
       tmpPainter.fillRect(tmp.rect(), color());
       tmpPainter.end();

       // draw the blurred shadow...
       painter->drawImage(offset, tmp);

       // draw the actual pixmap...
       painter->drawPixmap(offset, px, QRectF());

       // restore world transform
       painter->setWorldTransform(restoreTransform);
}

QColor Ui::ShadowEffect::color() const
{
    return m_color;
}

void Ui::ShadowEffect::setColor(const QColor &color)
{
    m_color = color;
}

qreal Ui::ShadowEffect::distance() const
{
    return m_distance;
}

void Ui::ShadowEffect::setDistance(const qreal &distance)
{
    m_distance = distance;
    updateBoundingRect();
}

qreal Ui::ShadowEffect::blurRadius() const
{
    return m_blurRadius;
}

void Ui::ShadowEffect::setBlurRadius(const qreal &blurRadius)
{
    m_blurRadius = blurRadius;
    updateBoundingRect();
}
