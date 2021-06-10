#include "sensoradddialog.h"
#include "ui_sensoradddialog.h"
#include "../buttons/outlinebutton.h"
#include "../buttons/raisebutton.h"

SensorAddDialog::SensorAddDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SensorAddDialog)
{
    ui->setupUi(this);

    auto okBtn = new Ui::RaiseButton(tr("OK"), this);
    okBtn->setForegroundColor(Qt::white);
    okBtn->setEnabledShadow(false);

    auto cancelBtn = new Ui::RaiseButton(tr("Cancel"), this);
    cancelBtn->setForegroundColor(cancelBtn->backgroundColor());
    cancelBtn->setEnabledShadow(false);
    cancelBtn->setBackgroundColor(Qt::transparent);

    ui->btnLayout->addWidget(okBtn);
    ui->btnLayout->addWidget(cancelBtn);

    connect(okBtn, &QAbstractButton::clicked, this, &SensorAddDialog::accept);
    connect(cancelBtn, &QAbstractButton::clicked, this, &SensorAddDialog::reject);
}

SensorAddDialog::~SensorAddDialog()
{
    delete ui;
}

int SensorAddDialog::getCount()
{
    return ui->editCount->value();
}
