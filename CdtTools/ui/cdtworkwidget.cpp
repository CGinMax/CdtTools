#include "cdtworkwidget.h"
#include "ui_cdtworkwidget.h"
#include "tables/ditablemodel.h"
#include "tables/aitablemodel.h"
#include "tables/delegates/comboboxdelegate.h"
#include "tables/delegates/digitlimitedelegate.h"
#include "../protocol/standard/cdtprotocol.h"
#include "../protocol/interacte/cdtinteracte.h"
#include "../protocol/cycle/cdtcycle.h"
#include "../protocol/nr/cdtexnr.h"
#include "../protocol/ut/cdtexut.h"
#include "../common/threadpool.h"
#include "./dialog/ykdialog.h"
#include <QDebug>
#include <QMenu>
#include <QThread>

CDTWorkWidget::CDTWorkWidget(const QSharedPointer<NetworkBase> &network, const QSharedPointer<SettingData> &settingData, QWidget *parent)
    : QWidget(parent)
    , m_protocol(nullptr)
    , m_network(network)
    , m_floatBtnGroup(new FloatButtonGroup(this))
    , ui(new Ui::CDTWorkWidget)
{
    ui->setupUi(this);

    ui->horSplitter->setCollapsible(0, false);
    ui->horSplitter->setCollapsible(1, false);
    ui->vecSplitter->setCollapsible(0, false);
    ui->vecSplitter->setCollapsible(1, false);

    m_diModel = new DiTableModel({"Id", "Name", "Value"}, settingData->m_ptCfg->m_globalDiList, ui->viewDi);
    ui->viewDi->setModel(m_diModel);
    ui->viewDi->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->viewDi->verticalHeader()->setVisible(false);
    ui->viewDi->setEditTriggers(QAbstractItemView::DoubleClicked);
    ui->viewDi->setSelectionBehavior(QAbstractItemView::SelectItems);
    auto diDelegate = new ComboBoxDelegate(ui->viewDi);
    diDelegate->setItems({QStringLiteral("分"), QStringLiteral("合")});
    ui->viewDi->setItemDelegateForColumn(2, diDelegate);

    m_aiModel = new AiTableModel({"Id", "Name", "Value"}, settingData->m_ptCfg->m_globalAiList, ui->viewAi);
    ui->viewAi->setModel(m_aiModel);
    ui->viewAi->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->viewAi->verticalHeader()->setVisible(false);
    ui->viewAi->setEditTriggers(QAbstractItemView::DoubleClicked);
    ui->viewAi->setSelectionBehavior(QAbstractItemView::SelectItems);
    auto aiDelegate = new DigitLimiteDelegate();
    ui->viewAi->setItemDelegateForColumn(2, aiDelegate);

    m_vyxModel = new DiTableModel({"Id", "Name", "Value"}, settingData->m_ptCfg->m_globalVDiList, ui->viewVDi);
    ui->viewVDi->setModel(m_vyxModel);
    ui->viewVDi->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->viewVDi->verticalHeader()->setVisible(false);
    ui->viewVDi->setEditTriggers(QAbstractItemView::DoubleClicked);
    ui->viewVDi->setSelectionBehavior(QAbstractItemView::SelectItems);
    auto vyxDelegate = new ComboBoxDelegate(ui->viewVDi);
    vyxDelegate->setItems({QStringLiteral("分"), QStringLiteral("合")});
    ui->viewVDi->setItemDelegateForColumn(2, vyxDelegate);

    bool isRandom = settingData->m_ptCfg->m_isRandom;
    connect(&m_viewTimer, &QTimer::timeout, [this, isRandom]{
        if (isRandom)
            m_aiModel->randomNumber();
        ui->viewAi->viewport()->update();
        ui->viewDi->viewport()->update();
    });
    m_viewTimer.start(2000);

    ui->edPtId->setMinimum(settingData->m_ptCfg->m_globalDiList->first()->io());

    ui->textYkInfo->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->textYkInfo, &QPlainTextEdit::customContextMenuRequested, this, &CDTWorkWidget::onPlainTextContextMenuRequested);
    ui->textBrowser->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->textBrowser, &QTextBrowser::customContextMenuRequested, this, &CDTWorkWidget::onTextBrowserContextMenuRequested);

    auto btn = new QPushButton(tr("clear"), this);
    connect(btn, &QPushButton::clicked, [this](){
        this->ui->textBrowser->clear();
    });
    m_floatBtnGroup->addButton(btn);
    m_floatBtnGroup->move(ui->textBrowser->x() + width() - 18 - m_floatBtnGroup->width(), ui->textBrowser->y() + 9);
    m_floatBtnGroup->raise();


}

CDTWorkWidget::~CDTWorkWidget()
{
    if (m_protocol) {
        delete m_protocol;
        m_protocol = nullptr;
    }
    m_viewTimer.stop();
}

void CDTWorkWidget::startCommunication(const QSharedPointer<SettingData> &settingData)
{
    ThreadPool::instance()->run([&settingData, this](){
        switch (settingData->m_ptCfg->m_protocol) {
        case eProtocol::CDTStandard:
            this->m_protocol = new CDTProtocol(this->m_network, settingData);
            break;
        case eProtocol::CDTGcInterace:
            this->m_protocol = new CDTInteracte(this->m_network, settingData);
            break;
        case eProtocol::CDTGcCycle:
            this->m_protocol = new CDTCycle(this->m_network, settingData);
            break;
        case eProtocol::CDTUt:
            this->m_protocol = new CDTExUt(this->m_network, settingData);
            break;
        case eProtocol::CDTNr:
            this->m_protocol = new CDTExNr(this->m_network, settingData);
            break;

        default:
            break;
        }
        connect(this->m_protocol, &ProtocolBase::sendProtocolContent, this, &CDTWorkWidget::recvProtocolContent);
        connect(this->m_protocol, &ProtocolBase::sendYKMsg, this, &CDTWorkWidget::recvYKMsg);
        connect(this, &CDTWorkWidget::stop, this->m_protocol, &ProtocolBase::stop, Qt::BlockingQueuedConnection);
        connect(this, &CDTWorkWidget::startYK, this->m_protocol, &ProtocolBase::sendYk);
        connect(this, &CDTWorkWidget::reverseYx, this->m_protocol, &ProtocolBase::onReverseYx);
        connect(this->m_protocol, &ProtocolBase::notifyYK, this, &CDTWorkWidget::onNotifyYK);
        connect(this->m_network.data(), &NetworkBase::disconnected, this, &CDTWorkWidget::disconnected);
        this->m_protocol->initStrategy();
        this->m_protocol->start();
    });
}

void CDTWorkWidget::resetAiRandom(bool start)
{
    if (start) {
        m_viewTimer.start(1000);
    }
    else {
        m_viewTimer.stop();
    }
}

void CDTWorkWidget::stopCommunication()
{
    emit stop();
    m_network->close();
}

bool CDTWorkWidget::isConnection()
{
    return m_protocol->isConnection();
}

void CDTWorkWidget::resizeEvent(QResizeEvent *event)
{
    if ((width() + 36) <= m_floatBtnGroup->width() || height() <= m_floatBtnGroup->height()) {
        return ;
    }

    m_floatBtnGroup->move(ui->textBrowser->x() + width() - 18 - m_floatBtnGroup->width(), ui->textBrowser->y() + 9);
    QWidget::resizeEvent(event);
}

void CDTWorkWidget::recvProtocolContent(const QString &msg)
{
    ui->textBrowser->append(msg);
}

void CDTWorkWidget::recvYKMsg(const QString &msg)
{
    ui->textYkInfo->appendPlainText(msg);
}

void CDTWorkWidget::onNotifyYK(int ptId)
{
    QString info = QStringLiteral("是否对点%1进行变位").arg(ptId);
    YKDialog dialog(info, "Dialog");
    auto globalPos = mapToGlobal(QPoint(x(), y()));
    dialog.move(globalPos.x() + width() / 2 - dialog.width() / 2, globalPos.y() + height() / 2 - dialog.height() / 2);
    int ret = dialog.exec();

    emit reverseYx(ptId, ret == QDialog::Accepted);
}

void CDTWorkWidget::on_btnExecute_clicked()
{
    emit startYK(ui->edPtId->value(), ui->cbbYKOper->currentIndex() > 0);

}

void CDTWorkWidget::onPlainTextContextMenuRequested(const QPoint &pos)
{
    Q_UNUSED(pos);
    QMenu menu;
    menu.addAction(tr("清空"), this, [this]{
        this->ui->textYkInfo->clear();
    });
    menu.exec(QCursor::pos());
}

void CDTWorkWidget::onTextBrowserContextMenuRequested(const QPoint &pos)
{
    Q_UNUSED(pos);
    QMenu menu;
    menu.addAction(tr("清空"), this, [this]{
        this->ui->textBrowser->clear();
    });
    menu.exec(QCursor::pos());
}
