#include "configurationpage.h"
#include "ui_configurationpage.h"
#include "../../Communication/portparam.h"
#include <climits>

ConfigurationPage::ConfigurationPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ConfigurationPage)
{
    ui->setupUi(this);
    ui->editGatherTime->setRange(1, INT_MAX);
    ui->editGatherTime->setValue(5000);
    ui->editSensorTime->setRange(1, INT_MAX);
    ui->editSensorTime->setValue(5000);
    ui->serialportWidget->setCountEditHidden(true);
    ui->serialportWidget->setBtnGroupHidden(true);
    connect(ui->btnSave, &QPushButton::clicked, this, &ConfigurationPage::notifySave);
}

ConfigurationPage::~ConfigurationPage()
{
    delete ui;
}

int ConfigurationPage::gatherTime() const
{
    return ui->editGatherTime->value();
}

void ConfigurationPage::setGatherTime(int time)
{
    ui->editGatherTime->setValue(time);
}

int ConfigurationPage::sensorTime() const
{
    return ui->editSensorTime->value();
}

void ConfigurationPage::setSensorTime(int time)
{
    ui->editSensorTime->setValue(time);
}

void ConfigurationPage::setPortParam(const PortParam &param)
{
    ui->serialportWidget->setPortParam(param);
}

PortParam ConfigurationPage::portParam() const
{
    return ui->serialportWidget->getPortParam();
}
