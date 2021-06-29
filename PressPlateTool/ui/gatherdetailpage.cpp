#include "gatherdetailpage.h"
#include "ui_gatherdetailpage.h"
#include "tables/listdelegate.h"
#include "tables/listheader.h"
#include "expand/gathercontroller.h"
#include "../protocol/ybprotocolchannel.h"

#include <QFont>

GatherDetailPage::GatherDetailPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GatherDetailPage)
{
    ui->setupUi(this);
    auto font = ui->txtName->font();
    font.setBold(true);
    font.setPixelSize(16);
    ui->txtName->setFont(font);

    m_tablePage = new TablePage(ui->tabWidget);
    m_confPage = new ConfigurationPage(ui->tabWidget);
    ui->tabWidget->addTab(m_tablePage, tr("Sensors"));
    ui->tabWidget->addTab(m_confPage, tr("Configuration"));

    connect(m_confPage, &ConfigurationPage::notifySave, this, &GatherDetailPage::onSave);

}

GatherDetailPage::~GatherDetailPage()
{
    delete ui;
}

void GatherDetailPage::onItemChanged(GatherController *controller)
{
    // disconnect invalid signal
    if (!m_gatherData.isNull()) {
        disconnect(m_gatherData.data(), &GatherData::nameChanged, this, &GatherDetailPage::onNameChanged);
        disconnect(m_gatherData.data(), &GatherData::addrChanged, this, &GatherDetailPage::onAddrChanged);
    }

    m_tablePage->setGatherController(controller);
    if (controller != nullptr) {
        // init configure page data if controller is valid
        m_gatherData = controller->gatherData();
        ui->txtName->setText(m_gatherData->name());
        ui->txtAddress->setText(QString::number(m_gatherData->addr()));
        m_confPage->setPortParam(m_gatherData->portParam());
        m_confPage->setGatherTime(m_gatherData->gatherTimeout());
        m_confPage->setSensorTime(m_gatherData->sensorTimeout());

        connect(m_gatherData.data(), &GatherData::nameChanged, this, &GatherDetailPage::onNameChanged);
        connect(m_gatherData.data(), &GatherData::addrChanged, this, &GatherDetailPage::onAddrChanged);

        ui->stackedWidget->setCurrentIndex(1);
        return ;
    }
    m_gatherData.reset(nullptr);
    ui->stackedWidget->setCurrentIndex(0);
}

void GatherDetailPage::onNameChanged(const QString &name)
{
    ui->txtName->setText(name);
}

void GatherDetailPage::onAddrChanged(int addr)
{
    ui->txtAddress->setText(QString::number(addr));
}

void GatherDetailPage::onSave()
{
    if (m_gatherData.isNull()) {
        qDebug("error gather controller is empty!");
        //TODO(shijm): not_null
        return;
    }

    const PortParam param = m_confPage->portParam();
    m_gatherData->setPortParam(param);
    m_gatherData->setGatherTimeout(m_confPage->gatherTime());
    m_gatherData->setSensorTimeout(m_confPage->sensorTime());
}
