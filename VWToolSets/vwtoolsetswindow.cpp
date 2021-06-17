#include "vwtoolsetswindow.h"
#include "ui_vwtoolsetswindow.h"

#include <QApplication>
#include <QPainter>
#include <QScreen>

#include "shadoweffect.h"

class VWToolSetsWindowPrivate final
{
    Q_DISABLE_COPY(VWToolSetsWindowPrivate)
    Q_DECLARE_PUBLIC(VWToolSetsWindow)
public:
    explicit VWToolSetsWindowPrivate(VWToolSetsWindow* q);
    ~VWToolSetsWindowPrivate();

    void initWidgets();

    VWToolSetsWindow* const q_ptr;
    Ui::VWToolSetsWindow* ui;
};

VWToolSetsWindowPrivate::VWToolSetsWindowPrivate(VWToolSetsWindow* q)
    : q_ptr(q)
    , ui(new Ui::VWToolSetsWindow)
{
    ui->setupUi(q);
}

VWToolSetsWindowPrivate::~VWToolSetsWindowPrivate()
{
    delete ui;
}

void VWToolSetsWindowPrivate::initWidgets()
{
    Q_Q(VWToolSetsWindow);
    q->resize(960, 720);
    q->setTitleWidget(ui->textTitle);

//    auto effect = new Ui::ShadowEffect(q);
//    q->setGraphicsEffect(effect);

    QObject::connect(ui->btnMaxmum, &QPushButton::clicked, q, [q] {
        q->showMinimized();
    });
    QObject::connect(ui->btnMinimum, &QPushButton::clicked, q, [q] {
        q->showMinimized();
    });
    QObject::connect(ui->btnClose, &QPushButton::clicked, q, [q] {
        q->close();
    });

    auto screenSize = qApp->primaryScreen()->availableSize();
    q->setGeometry((screenSize.width() - q->width()) / 2, (screenSize.height() - q->height()) / 2, q->width(), q->height());

}


VWToolSetsWindow::VWToolSetsWindow(QWidget *parent)
    : VWMainWidget(parent)
    , d_ptr(new VWToolSetsWindowPrivate(this))
{
    d_ptr->initWidgets();

}

VWToolSetsWindow::~VWToolSetsWindow()
{

}


//void VWToolSetsWindow::paintEvent(QPaintEvent *event)
//{
//    Q_UNUSED(event)
//    QPainter painter(this);
//    painter.setRenderHint(QPainter::Antialiasing);
//    painter.setBrush(Qt::white);
//    painter.setPen(Qt::NoPen);
//    painter.drawRoundedRect(this->rect(), 10, 10);
//}
