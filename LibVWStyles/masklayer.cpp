#include <QPainter>
#include <QMovie>
#include "masklayer.h"
#include "ui_masklayer.h"
#include "private/qwidget_p.h"


// *************************** MaskLayerData ********************************************
namespace MaskLayerData
{
    static MaskLayer* s_maskLayer = nullptr;
    static QWidget* s_parentWidget = nullptr;
}


// *************************** MaskLayerPrivate *****************************************
class MaskLayerPrivate final : public QWidgetPrivate
{
public:
    MaskLayerPrivate();
    ~MaskLayerPrivate();

    Ui::MaskLayer ui;
};

MaskLayerPrivate::MaskLayerPrivate() : QWidgetPrivate()
{

}

MaskLayerPrivate::~MaskLayerPrivate()
{

}


// *************************** MaskLayer ************************************************
MaskLayer::MaskLayer(const QString& text, QWidget *parent) : QWidget(*(new MaskLayerPrivate), parent, Qt::WindowFlags())
{
    Q_D(MaskLayer);
    d->ui.setupUi(this);
    d->ui.text->setText(text);

    setStyleSheet(QLatin1String("QWidget#MaskLayer{border:none;}"));
    setWindowModality(Qt::ApplicationModal);
    setAttribute(Qt::WA_TranslucentBackground, true);

    d->ui.cancel->setFixedWidth(150);

    if (parent) {
        setParent(parent);
        auto geometry = parent->geometry();
        resize(geometry.size());
    }

    QMovie* movie = new QMovie(QLatin1String("://resources/loading.gif"));
    d->ui.pixmap->setMovie(movie);
    movie->start();

    connect(d->ui.cancel, &QPushButton::clicked, [=]() {
        hide();

        this->deleteLater();
        MaskLayerData::s_maskLayer = nullptr;
    });
}

MaskLayer::~MaskLayer()
{

}

void MaskLayer::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)

    QPainter painter(this);
    static QColor fileColor = QColor(255, 255, 255, 100);
    painter.fillRect(this->rect(), fileColor);
}

void MaskLayer::setMaskLayerParentWidget(QWidget* widget)
{
    MaskLayerData::s_parentWidget = widget;
}

MaskLayer* MaskLayer::showMaskLayer(QWidget* parent, const QString& text)
{
    if (!parent)
        return nullptr;

    if (MaskLayerData::s_maskLayer)
        return nullptr;

    MaskLayerData::s_maskLayer = new MaskLayer(text, parent);
    MaskLayerData::s_maskLayer->show();
    return MaskLayerData::s_maskLayer;
}

MaskLayer* MaskLayer::showMaskLayer(const QString& text)
{
    return MaskLayer::showMaskLayer(MaskLayerData::s_parentWidget, text);
}

void MaskLayer::hideMaskLayer(MaskLayer* maskLayer)
{
    if (!maskLayer)
        return;

    if (MaskLayerData::s_maskLayer) {
        if (maskLayer == MaskLayerData::s_maskLayer) {
            MaskLayerData::s_maskLayer->hide();
            MaskLayerData::s_maskLayer->deleteLater();
            MaskLayerData::s_maskLayer = nullptr;
        }
    }
}

void MaskLayer::resizeMaskLayer(const QSize& size)
{
    if (MaskLayerData::s_maskLayer) {
        MaskLayerData::s_maskLayer->resize(size);
    }
}

bool MaskLayer::isShowing()
{
    if (MaskLayerData::s_maskLayer)
        return true;
    return false;
}
