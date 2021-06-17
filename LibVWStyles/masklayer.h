#ifndef MASKLAYER_H
#define MASKLAYER_H


#include <QWidget>
#include "libvwstyles_global.h"

QT_FORWARD_DECLARE_CLASS(MaskLayerPrivate)
class LIBVWSTYLES_EXPORT MaskLayer final : public QWidget
{
    Q_OBJECT
public:
    ~MaskLayer() override;

    static void setMaskLayerParentWidget(QWidget* widget);
    static MaskLayer* showMaskLayer(QWidget* parent, const QString& text);
    /*!
     * 调用前先确保 setMaskLayerParentWidget 已被调用
     */
    static MaskLayer* showMaskLayer(const QString& text);
    static void hideMaskLayer(MaskLayer* maskLayer);
    static void resizeMaskLayer(const QSize& size);
    static bool isShowing();
protected:
    explicit MaskLayer(const QString& text, QWidget *parent = nullptr);

    virtual void paintEvent(QPaintEvent *event) override;
private:
    Q_DECLARE_PRIVATE(MaskLayer)
};

#endif // MASKLAYER_H
