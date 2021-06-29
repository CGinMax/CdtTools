#include "expandtile.h"
#include "ui_expandtile.h"
#include "../buttons/iconbutton.h"
#include "../base/faicon.h"

#include <QGridLayout>
#include <QFontMetrics>
#include <QMouseEvent>
ExpandTile::ExpandTile(const QString &title, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ExpandTile)
    , m_checked(false)
    , m_editFilter(new EditableEventFilter(this))
{
    ui->setupUi(this);

    setLabelStyle(ui->txtSVTile);
    setLabelStyle(ui->txtHVTitle);
    auto font = ui->editTitle->font();
    font.setBold(true);
    font.setPixelSize(20);
    ui->editTitle->setFont(font);
    ui->editTitle->setText(title);
    auto delBtn = new Ui::IconButton(FAIcon::instance()->icon(QString("trash-alt")), this);
    delBtn->setIconSize(QSize(20, 20));
    delBtn->setBackgroundEnabled(false);
    delBtn->setMaximumSize(20, 20);
    delBtn->setXRadius(delBtn->width())->setYRadius(delBtn->height());
    ui->mainLayout->addWidget(delBtn, 0, 1);

    auto queryVerBtn = new Ui::IconButton(FAIcon::instance()->icon(QString("redo")), this);
    queryVerBtn->setIconSize(QSize(20, 20));
    queryVerBtn->setBackgroundEnabled(false);
    queryVerBtn->setMaximumSize(20, 20);
    ui->mainLayout->addWidget(queryVerBtn, 2, 1);
    ui->mainLayout->setColumnStretch(0, 1);

    setEventFilter(ui->editTitle);
    connect(queryVerBtn, &QAbstractButton::clicked, this, &ExpandTile::queryVersion);
    connect(delBtn, &QAbstractButton::clicked, this, &ExpandTile::deleteItem);
}

ExpandTile::~ExpandTile()
{

}

void ExpandTile::setTitle(const QString &title)
{
    ui->editTitle->setText(title);
}

void ExpandTile::setHardwareVersion(const QString &ver)
{
    ui->txtHardwareVer->setText(ver);
}

void ExpandTile::setSoftwareVersion(const QString &ver)
{
    ui->txtSoftwareVer->setText(ver);
}

void ExpandTile::setProductDescript(const QString &desc)
{
    ui->txtProductDesc->setText(desc);
}

void ExpandTile::setChecked(bool checked)
{
    if (m_checked == checked){
        return ;
    }
    m_checked = checked;
    emit toggled(checked);
}

bool ExpandTile::checked() const
{
    return m_checked;
}

void ExpandTile::setEventFilter(QLineEdit *edit)
{
    connect(edit, &QLineEdit::editingFinished, this, [=]{
        edit->setStyleSheet("background-color:transparent;");
        edit->setFrame(false);
        edit->setReadOnly(true);
        if (edit == ui->editTitle) {
            emit this->titleChanged(edit->text());
        }
    });
    emit edit->editingFinished();
    edit->installEventFilter(m_editFilter);
}

void ExpandTile::setLabelStyle(QLabel *label)
{
    label->setStyleSheet("color:gray;font: normal 11px");
}

void ExpandTile::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        setChecked(!m_checked);
    }
    QWidget::mousePressEvent(event);
}
