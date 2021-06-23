#include "tablepage.h"
#include "ui_tablepage.h"
#include "../expand/gathercontroller.h"
#include "../notification/snackbar.h"
#include "../dialogs/sensoradddialog.h"
#include "../dialogs/sensoroperationdlg.h"
#include <QMessageBox>
#include <QMenu>
#include <QApplication>

TablePage::TablePage(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::TablePage)
    , m_currentIndex(0)
    , m_currentConfAddr(1)
    , m_controller(nullptr)
{
    ui->setupUi(this);

    m_table = new YBTableView(this);
    ui->scrollArea->setWidget(m_table);
    ui->scrollArea->setWidgetResizable(true);
    ui->scrollArea->setStyleSheet(QLatin1String("QScrollArea#scrollArea{border: transparent; background-color: white;}"));

    ui->splitter->setStretchFactor(0, 7);
    ui->splitter->setStretchFactor(1 ,1);

    ui->editBegin->setRange(0x01, 0x7E);
    ui->editEnd->setRange(0x01, 0x7E);

    ui->textMsg->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->textMsg, &QWidget::customContextMenuRequested, this, [=](const QPoint& pos){
        QMenu menu;
        menu.addAction(tr("Clear"), this, [=]{ ui->textMsg->clear(); });
        menu.exec(QCursor::pos());
    });


    connect(ui->btnAddOne, &QAbstractButton::clicked, this, [=]{
        auto data = new YBSensorData();// delete in GatherData
        this->m_table->addYBSensor(data);
        m_controller->appendSensorData(data);
    });
    connect(ui->btnAddMulti, &QAbstractButton::clicked, this, [=]{
        SensorAddDialog dlg(this);
        if (dlg.exec() == QDialog::Accepted) {
            int inputCount = dlg.getCount();
            if (inputCount > (100 - this->m_table->sensorCount())) {
                inputCount = 100 - this->m_table->sensorCount();
            }
            for (int i = 0; i < inputCount; i++) {
                auto data = new YBSensorData();// delete in GatherData
                this->m_table->addYBSensor(data);
                m_controller->appendSensorData(data);
            }
        }
    });
    connect(ui->btnDeleteAll, &QAbstractButton::clicked, this, [=]{
        this->m_table->deleteAllYBSensors();
    });

    connect(m_table, &YBTableView::setSensorAddr, this, &TablePage::onSetSensorAddr);
    connect(m_table, &YBTableView::querySensorStatus, this, &TablePage::onQuerySensorStatus);
    connect(m_table, &YBTableView::querySensorVersion, this, &TablePage::onQuerySensorVersion);
    connect(m_table, &YBTableView::changeSensorStatus, this, &TablePage::onChangeSensorStatus);
}

TablePage::~TablePage()
{
    delete ui;
}

void TablePage::setGatherController(GatherController *controller)
{
    if (controller != nullptr) {
        if (m_controller != nullptr) {
            disconnect(m_controller->protocol(), &ProtocolChannelBase::showProtocolMsg, this, &TablePage::onShowProtocolMsg);
        }
        connect(controller->protocol(), &ProtocolChannelBase::showProtocolMsg, this, &TablePage::onShowProtocolMsg, Qt::QueuedConnection);
        m_table->resetYBSensors(controller->gatherData()->sensorDataList());
    }
    m_controller = controller;

}

void TablePage::confAddrRecursion()
{
    if (m_currentConfAddr > ui->editEnd->value()) {
        Ui::SnackBar::showSnackBar(this, tr("Automatically configure sensor address finished!"));
        qDebug("set address finish");
        return;
    }
    auto dlg = new SensorOperationDlg(tr("Set sensor address dialog"), tr("Set sensor address = %1").arg(m_currentConfAddr), this);
    auto reply = m_controller->protocol()->setAddress(eYBFrameType::YBSensor, static_cast<uint8_t>(m_currentConfAddr), m_controller->gatherData()->sensorTimeout());
    reply->subscribe([=](std::shared_ptr<IContent> result){
        if (result == nullptr) {
            Ui::SnackBar::showSnackBar(this, tr("Info:Configure sensor address receive unkonw frame response!"));
            qDebug("Unknow frame data");
            dlg->accept();
            delete dlg;
            return ;
        }
        if (result->functionCode() == eYBFunCode::NAKCode) {
            Ui::SnackBar::showSnackBar(this, tr("Recevie NAK Frame!"));
            qDebug("NAK Error");
            dlg->accept();
            delete dlg;
            return ;
        }
        if (result->success()) {
//            this->onQuerySensorStatus(this->m_currentIndex, this->m_currentConfAddr);
            this->queryStatusImpl(this->m_currentIndex, this->m_currentConfAddr, [=](std::shared_ptr<IContent> result){
                dlg->accept();
                delete dlg;

                if (result == nullptr) {
                    qDebug("Unknow frame data");
                    return ;
                }
                if (result->functionCode() == eYBFunCode::NAKCode) {
                    qDebug("NAK Error");
                    return ;
                }
                if (result->success()) {
                    this->m_table->setListItemAddr(this->m_currentIndex, this->m_currentConfAddr);
                    this->m_currentIndex++;
                    this->m_currentConfAddr++;
                    this->confAddrRecursion();
                } else {
                    qDebug("set sensor address failed!");
                }
            }, [=](){
                Ui::SnackBar::showSnackBar(this, tr("Configure sensor address failed!"));
                qDebug("set sensor address failed!");
            });

        } else {
            dlg->accept();
            delete dlg;
            Ui::SnackBar::showSnackBar(this, tr("Configure sensor address failed!"));
            qDebug("sensor set address failed");
        }
    },[=](){
        dlg->reject();
        delete dlg;
        QMessageBox::critical(this, tr("Error"), tr("Query sensor address = %1 timeout!").arg(this->m_currentConfAddr), QMessageBox::Ok);
        qDebug("sensor set address timeout cancel");
        this->m_currentIndex++;
        this->m_currentConfAddr++;
        this->confAddrRecursion();
    });
    dlg->exec();
}

void TablePage::queryStatusRecursion()
{
    if (m_currentIndex >= m_table->sensorCount()) {
        Ui::SnackBar::showSnackBar(this, tr("Automatically query sensor status finished!"));
        qDebug("query status finish");
        return ;
    }
    const auto addr = static_cast<uint16_t>(m_table->getListItemAddr(m_currentIndex));
    auto dlg = new SensorOperationDlg(tr("Query sensor status dialog"), tr("Query sensor(address = %1) status").arg(addr), this);
    auto reply = m_controller->protocol()->queryStatus(addr, m_controller->gatherData()->sensorTimeout());
    reply->subscribe([=](std::shared_ptr<IContent> result){
        if (result == nullptr) {
            Ui::SnackBar::showSnackBar(this, tr("Info:Configure sensor address receive unkonw frame response!"));
            qDebug("Unknow frame data");
            return ;
        }
        if (result->functionCode() == eYBFunCode::NAKCode) {
            qDebug("NAK Error");
            return ;
        }
        dlg->accept();
        delete dlg;
        this->m_table->setListItemStatus(m_currentIndex, result->currentStatusCode(), result->configedStatusCode());
        this->m_currentIndex++;
        this->queryStatusRecursion();
    },
    [=](){
        dlg->reject();
        delete dlg;
        QMessageBox::critical(this, tr("Error"), tr("Query sensor(address = %1) status timeout!").arg(addr), QMessageBox::Ok);
        qDebug("sensor %d query status timeout cancel", addr);
        this->m_currentIndex++;
        this->queryStatusRecursion();
    });
    dlg->exec();
}

void TablePage::queryVersionRecursion()
{
    if (m_currentIndex >= m_table->sensorCount()) {
        Ui::SnackBar::showSnackBar(this, tr("Automatically query sensor version finished!"));
        qDebug("query version finish");
        return ;
    }
    const auto addr = static_cast<uint8_t>(m_table->getListItemAddr(m_currentIndex));
    auto dlg = new SensorOperationDlg(tr("Query Sensor Dialog"), tr("Query sensor version, the address=%1").arg(addr), this);
    auto reply = m_controller->protocol()->queryVersion(eYBFrameType::YBSensor, addr, m_controller->gatherData()->sensorTimeout());
    reply->subscribe([=](std::shared_ptr<IContent> result){
        if (result == nullptr) {
            Ui::SnackBar::showSnackBar(this, tr("Info:Configure sensor address receive unkonw frame response!"));
            qDebug("Unknow frame data");
            return ;
        }
        if (result->functionCode() == eYBFunCode::NAKCode) {
            qDebug("NAK Error");
            return ;
        }
        this->m_table->setListItemVersion(this->m_currentIndex
                                    , QString::fromStdString(result->hardwareVersion())
                                    , QString::fromStdString(result->softwareVersion())
                                    , QString::fromStdString(result->productDescript()));
        dlg->accept();
        delete dlg;
        this->m_currentIndex++;
        this->queryVersionRecursion();
    },
    [=](){
        dlg->reject();
        delete dlg;
        QMessageBox::critical(this, tr("Error"), tr("Query sensor(address = %1) version timeout!").arg(addr), QMessageBox::Ok);
        qDebug("sensor %d query version timeout cancel", addr);
        this->m_currentIndex++;
        this->queryStatusRecursion();
    });
    dlg->exec();
}

bool TablePage::canDoOperate()
{
    bool active = m_controller->isCommunicationActive();
    if (!active) {
        Ui::SnackBar::showSnackBar(this, tr("Communication No Open!"));
        qDebug("not open communication");
    }
    return active;
}

void TablePage::queryStatusImpl(int index, int addr, Fn<void (std::shared_ptr<IContent>)> success, Fn<void ()> error)
{
    auto reply = m_controller->protocol()->setAddress(eYBFrameType::YBSensor, static_cast<uint8_t>(addr), m_controller->gatherData()->sensorTimeout());
    reply->subscribe(success, error);
}

void TablePage::onSetSensorAddr(int index, int addr)
{
    if (!canDoOperate()) {
        return;
    }

    auto reply = m_controller->protocol()->setAddress(eYBFrameType::YBSensor, static_cast<uint8_t>(addr), m_controller->gatherData()->sensorTimeout());
    auto dlg = new SensorOperationDlg(tr("Set sensor address dialog"), tr("Set sensor address = %1").arg(addr), this);
    reply->subscribe([=](std::shared_ptr<IContent> result){
        if (result == nullptr) {
            Ui::SnackBar::showSnackBar(this, tr("Info:Configure sensor address receive unkonw frame response!"));
            qDebug("Unknow frame data");
            dlg->accept();
            delete dlg;
            return ;
        }
        if (result->functionCode() == eYBFunCode::NAKCode) {
            qDebug("NAK Error");
            dlg->accept();
            delete dlg;
            return ;
        }
        if (result->success()) {
            this->m_table->setListItemAddr(index, addr);
            this->onQuerySensorStatus(index, addr);
            Ui::SnackBar::showSnackBar(this, tr("Configure sensor address success!"));
        } else {
            Ui::SnackBar::showSnackBar(this, tr("Configure sensor address failed!"));
            qDebug("set sensor address failed!");
        }
        dlg->accept();
        delete dlg;
    }, [=](){
        Ui::SnackBar::showSnackBar(this, tr("Configure sensor address timeout!"));
        qDebug("set sensor address timeout!");
        dlg->reject();
        delete dlg;
    });
    dlg->exec();
}

void TablePage::onQuerySensorStatus(int index, int addr)
{
    if (!canDoOperate()) {
        return;
    }

    auto reply = m_controller->protocol()->queryStatus(static_cast<uint16_t>(addr), m_controller->gatherData()->sensorTimeout());
    reply->subscribe([=](std::shared_ptr<IContent> result){
        if (result == nullptr) {
            Ui::SnackBar::showSnackBar(this, tr("Info:Configure sensor address receive unkonw frame response!"));

            qDebug("Unknow frame data");
            return ;
        }
        if (result->functionCode() == eYBFunCode::NAKCode) {
            qDebug("NAK Error");
            return ;
        }
        this->m_table->setListItemStatus(index, result->currentStatusCode(), result->configedStatusCode());
    }, [=](){
        Ui::SnackBar::showSnackBar(this, tr("Query sensor status timeout!"));
        qDebug("query sensor status timeout!");
    });
}

void TablePage::onQuerySensorVersion(int index, int addr)
{
    if (!canDoOperate()) {
        return;
    }

    auto reply = m_controller->protocol()->queryVersion(eYBFrameType::YBSensor, static_cast<uint16_t>(addr), m_controller->gatherData()->sensorTimeout());
    reply->subscribe([=](std::shared_ptr<IContent> result){
        if (result == nullptr) {
            Ui::SnackBar::showSnackBar(this, tr("Info:Configure sensor address receive unkonw frame response!"));

            qDebug("Unknow frame data");
            return ;
        }
        if (result->functionCode() == eYBFunCode::NAKCode) {
            qDebug("NAK Error");
            return ;
        }
        this->m_table->setListItemVersion(index
                                          , QString::fromStdString(result->hardwareVersion())
                                          , QString::fromStdString(result->softwareVersion())
                                          , QString::fromStdString(result->productDescript()));
    }, [this](){

        Ui::SnackBar::showSnackBar(this, tr("Query sensor version timeout!"));
        qDebug("query sensor version timeout!");
    });
}

void TablePage::onChangeSensorStatus(int index, int addr, int status)
{
    if (!canDoOperate()) {
        return;
    }

    auto reply = m_controller->protocol()->setStatus(static_cast<uint8_t>(status & 0xFF), static_cast<uint16_t>(addr), m_controller->gatherData()->sensorTimeout());
    reply->subscribe([=](std::shared_ptr<IContent> result){
        if (result == nullptr) {
            Ui::SnackBar::showSnackBar(this, tr("Info:Configure sensor address receive unkonw frame response!"));

            qDebug("Unknow frame data");
            return ;
        }
        if (result->functionCode() == eYBFunCode::NAKCode) {
            qDebug("NAK Error");
            return ;
        }
        if (result->success()) {
            this->m_table->setListItemConfigedStatus(index, static_cast<uint8_t>(status & 0xFF));
        } else {
            Ui::SnackBar::showSnackBar(this, tr("Failed:Change sensor status failed!"));

            qDebug("change status failed!");
        }
    }, [this](){

        Ui::SnackBar::showSnackBar(this, tr("Change sensor status timeout!"));
        qDebug("change status timeout!");
    });
}

void TablePage::onShowProtocolMsg(const QString &msg)
{
    ui->textMsg->appendHtml(msg);
    ui->textMsg->appendHtml("");
}

void TablePage::on_btnConfAllAddr_clicked()
{
    if (!canDoOperate()) {
        return;
    }

    m_currentIndex = 0;
    m_currentConfAddr = ui->editBegin->value();
    confAddrRecursion();
}

void TablePage::on_btnQueryAllStatus_clicked()
{
    if (!canDoOperate()) {
        return;
    }

    m_currentIndex = 0;
    queryStatusRecursion();
}

void TablePage::on_btnQueryAllVer_clicked()
{
    if (!canDoOperate()) {
        return;
    }

    m_currentIndex = 0;
    queryVersionRecursion();
}

void TablePage::on_editBegin_valueChanged(int value)
{
    ui->editEnd->setRange(value + 1, value + m_table->sensorCount());
}
