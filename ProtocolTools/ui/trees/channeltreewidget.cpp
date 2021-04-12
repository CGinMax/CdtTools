#include "channeltreewidget.h"
#include <QCursor>
#include <QAction>
#include <QMap>
#include <QSharedPointer>
#include <QStyleFactory>
#include <QIcon>
#include <QDebug>
#include "../dialog/nameddialog.h"

ChannelTreeWidget::ChannelTreeWidget(QWidget *parent)
    : QTreeWidget(parent)
    , m_tcpClientParent(new QTreeWidgetItem(this))
    , m_tcpServerParent(new QTreeWidgetItem(this))
    , m_udpParent(new QTreeWidgetItem(this))
    , m_currentPopupItem(nullptr)
{
    m_networkIcons.append(QIcon(":/icon/resources/server.png"));
    m_networkIcons.append(QIcon(":/icon/resources/client.png"));
    m_networkIcons.append(QIcon(":/icon/resources/udp.png"));

    setStyle(QStyleFactory::create(QLatin1String("Windows")));
    initMenuAction();
    m_tcpClientParent->setText(0, tr("Tcp Client"));
    m_tcpServerParent->setText(0, tr("Tcp Server"));
    m_udpParent->setText(0, tr("Udp"));

    m_tcpClientParent->setExpanded(true);
    m_tcpServerParent->setExpanded(true);
    m_udpParent->setExpanded(true);

    setHeaderLabel(tr("Channel"));
    insertTopLevelItem(0, m_tcpClientParent);
    insertTopLevelItem(1, m_tcpServerParent);
    insertTopLevelItem(2, m_udpParent);

    setAnimated(true);
    connect(this, &QTreeWidget::itemClicked, this, &ChannelTreeWidget::onItemClicked);

    setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, &QWidget::customContextMenuRequested, this, &ChannelTreeWidget::onCustomContextMenuRequested);
}

ChannelTreeWidget::~ChannelTreeWidget()
{

}

void ChannelTreeWidget::initMenuAction()
{
    m_parentNodeMenu.addAction(tr("Add Channel"), [this]{
        static int pageNum = 1;
        auto defaultName = tr("Page%1").arg(pageNum);
        NamedDialog dlg(defaultName);
        if (dlg.exec() == QDialog::Accepted) {
            auto item = new QTreeWidgetItem(m_currentPopupItem, {dlg.getNameString()});
            item->setIcon(0, createNetworkIcon(selectNetworkType(m_currentPopupItem)));
            QSharedPointer<SettingData> settingData(new SettingData());
            emit notifyAddNewChannel(item, settingData);

            pageNum++;
        }
    });
    m_parentNodeMenu.addAction(tr("Delete Child Channel"), [this]{
        auto childrenChannel = m_currentPopupItem->takeChildren();
        if (childrenChannel.isEmpty()) {
            return;
        }
        for (auto& child : childrenChannel) {
            emit notifyDeleteChannel(child);
        }
    });

    m_childNodeMenu.addAction(tr("Change Name"), [this]{
        NamedDialog dlg(m_currentPopupItem->text(0));
        if (dlg.exec() == QDialog::Accepted) {
            m_currentPopupItem->setText(0, dlg.getNameString());
            emit notifyChangeName(m_currentPopupItem);
        }
    });
    m_childNodeMenu.addAction(tr("Start"), [this]{
        emit notifyChannelStart(m_currentPopupItem);
    });
    m_childNodeMenu.addAction(tr("Stop"), [this]{
        emit notifyChannelStop(m_currentPopupItem);
    });
    m_childNodeMenu.addAction(tr("Delete"), [this]{
        emit notifyDeleteChannel(m_currentPopupItem);
        auto parentItem = m_currentPopupItem->parent();
        parentItem->removeChild(m_currentPopupItem);
        m_currentPopupItem = parentItem;
        this->setCurrentItem(m_currentPopupItem);
    });
}

QTreeWidgetItem *ChannelTreeWidget::addChannel(const QString &name, eNetworkType type)
{
    QTreeWidgetItem* item = nullptr;
    switch (type) {
    case eNetworkType::eTcpClient:
        item = new QTreeWidgetItem(m_tcpClientParent, {name});
        item->setIcon(0, createNetworkIcon(eNetworkType::eTcpClient));
        break;
    case eNetworkType::eTcpServer:
        item = new QTreeWidgetItem(m_tcpServerParent, {name});
        item->setIcon(0, createNetworkIcon(eNetworkType::eTcpServer));
        break;
    case eNetworkType::eUdp:
        item = new QTreeWidgetItem(m_udpParent, {name});
        item->setIcon(0, createNetworkIcon(eNetworkType::eUdp));
        break;
    default:
        break;
    }

    return item;
}

QTreeWidgetItem *ChannelTreeWidget::tcpClientItem()
{
    return m_tcpClientParent;
}

QTreeWidgetItem *ChannelTreeWidget::tcpServerItem()
{
    return m_tcpServerParent;
}

QTreeWidgetItem *ChannelTreeWidget::udpItem()
{
    return m_udpParent;
}

void ChannelTreeWidget::onCurrentItemChanged(QTreeWidgetItem *item)
{
    this->setCurrentItem(item);
}

void ChannelTreeWidget::onItemClicked(QTreeWidgetItem *item)
{
    if (item == m_tcpClientParent
       || item == m_tcpServerParent
       || item == m_udpParent) {
        return ;
    }

    emit notifyItemSelected(item);
}

void ChannelTreeWidget::onCustomContextMenuRequested(const QPoint &pos)
{
    auto popupItem = itemAt(pos);
    if (popupItem == nullptr) {
        return ;
    }

    m_currentPopupItem = popupItem;
    if (popupItem == m_tcpClientParent
       || popupItem == m_tcpServerParent
       || popupItem == m_udpParent) {
        // 显示菜单

        m_parentNodeMenu.exec(QCursor::pos());
    }
    else {
        m_childNodeMenu.exec(QCursor::pos());
    }
}

eNetworkType ChannelTreeWidget::selectNetworkType(QTreeWidgetItem *item)
{
    if (item == m_tcpClientParent) {
        return eNetworkType::eTcpClient;
    } else if (item == m_tcpServerParent) {
        return eNetworkType::eTcpServer;
    } else if (item == m_udpParent) {
        return eNetworkType::eUdp;
    } else {
        return eNetworkType::eErrorNetworkType;
    }
}

QIcon ChannelTreeWidget::createNetworkIcon(eNetworkType type)
{
    return m_networkIcons.at(static_cast<int>(type));
}
