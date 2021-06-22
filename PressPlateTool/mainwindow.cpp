#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QMessageBox>
#include <QFileDialog>
#include <QScreen>
#include <QJsonDocument>
#include <QTextStream>
#include <QFutureWatcher>
#include <QtConcurrent>

#include <tuple>

#include "ui/expand/expandwidgetitem.h"
#include "ui/expand/expandwidget.h"
#include "ui/dialogs/serialportdialog.h"
#include "ui/buttons/iconbutton.h"
#include "ui/base/faicon.h"

int MainWindow::createNo = 1;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setStyleSheet("QWidget#centralWidget{background-color:#F0F3F4;}");
    ui->mainSplitter->setStretchFactor(0, 2);
    ui->mainSplitter->setStretchFactor(1, 3);
    ui->mainSplitter->setStyleSheet("QSplitter::handle{background-color:lightgray;}");

    ui->mainToolBar->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    ui->mainToolBar->addAction(FAIcon::instance()->icon(QString("save")), tr("Save"), this, &MainWindow::onSave);
    ui->mainToolBar->addAction(FAIcon::instance()->icon("folderopen"), tr("Open"), this, &MainWindow::onLoad);

    connect(ui->gatherPage, &GatherPage::itemChanged, ui->gatherDetailPage, &GatherDetailPage::onItemChanged);

    m_fabMenu = new FabCircularMenu(this);
    auto oneBtn = new Ui::IconButton(FAIcon::instance()->icon("plus"), this);
    oneBtn->setIconSize(QSize(16, 16));
    oneBtn->setCheckable(false);
    oneBtn->resize(36, 36);
    oneBtn->setXRadius(oneBtn->width())->setYRadius(oneBtn->height());
    oneBtn->setForegroundColor(Qt::white);
    auto multiBtn = new Ui::IconButton(QIcon(":/icons/add-multi.png"), this);
    multiBtn->setIconSize(QSize(16, 16));
    multiBtn->setCheckable(false);
    multiBtn->resize(36, 36);
    multiBtn->setXRadius(multiBtn->width())->setYRadius(multiBtn->height());
    multiBtn->setForegroundColor(Qt::white);

    m_fabMenu->addButton(oneBtn);
    m_fabMenu->addButton(multiBtn);
    connect(oneBtn, &QAbstractButton::clicked, this, &MainWindow::onNotifyAddOne);
    connect(multiBtn, &QAbstractButton::clicked, this, &MainWindow::onNotifyAddMulti);


    auto screenSize = qApp->primaryScreen()->availableSize();
    setGeometry((screenSize.width() - width()) / 2, (screenSize.height() - height()) / 2, width(), height());
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onNotifyAddOne()
{
    SerialPortDialog dialog(false, this);
    if(dialog.exec() == QDialog::Accepted) {
        m_fabMenu->setChecked(false);
        ui->gatherPage->addExpandItem(ExpandWidget::createExpandWidget(dialog.portParam(), createNo++, 8));
    }
}

void MainWindow::onNotifyAddMulti()
{
    SerialPortDialog dialog(true, this);
    if (dialog.exec() == QDialog::Accepted) {
        int num = dialog.gatherNum();
        for (int i = 0; i < num; i++) {
            m_fabMenu->setChecked(false);
            ui->gatherPage->addExpandItem(ExpandWidget::createExpandWidget(dialog.portParam(), createNo++, 8));
        }
    }
}

void MainWindow::onSave()
{
    auto filename = getSaveFileName();
    if (filename.isEmpty()) {
        return;
    }
    QJsonDocument jsonDoc;
    QJsonArray dataArray;
    for (auto& item : ui->gatherPage->itemList()) {
        dataArray.append(item->getController()->gatherData()->save());
    }
    jsonDoc.setArray(dataArray);

    auto future = QtConcurrent::run([=](){
        QFile file(filename);
        if (!file.open(QFile::WriteOnly)) {
            return qMakePair(false, file.errorString());
        }
        QTextStream ts(&file);
        ts << jsonDoc.toJson(QJsonDocument::Indented);
        ts.flush();
        file.close();
        return qMakePair(true, QString());
    });
    auto watcher = new QFutureWatcher<QPair<bool, QString>>();
    watcher->setFuture(future);
    connect(watcher, &QFutureWatcher<QPair<bool, QString>>::finished, this, [=](){
        auto result = watcher->result();
        QMessageBox::information(this, tr("Save Result"),
                                 tr("Save file %1 %2.%3").arg(filename).arg(result.first ? tr("Success") : tr("Failed")).arg(result.second),
                                 QMessageBox::Ok);
        delete watcher;
    });
}

void MainWindow::onLoad()
{
    using ReadRet = std::tuple<bool, QString, QList<QSharedPointer<GatherData>>>;
    auto filename = getOpenFileName();
    if (filename.isEmpty()) {
        return ;
    }
    auto future = QtConcurrent::run([=]{
        QFile file(filename);
        if (!file.open(QFile::ReadOnly)) {
            return ReadRet(false, file.errorString(), QList<QSharedPointer<GatherData>>());
        }
        QTextStream ts(&file);
        QByteArray jsonData = ts.readAll().toUtf8();
        QJsonParseError error;
        QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonData, &error);
        if (jsonDoc.isNull()) {
            return ReadRet(false, error.errorString(), QList<QSharedPointer<GatherData>>());
        }
        auto rootArray = jsonDoc.array();
        QList<QSharedPointer<GatherData>> dataList;
        for (int i = 0; i < rootArray.count(); i++) {
            auto data = new GatherData();
            data->load(rootArray.at(i).toObject());
            dataList.append(QSharedPointer<GatherData>(data));
        }

        return ReadRet(true, QString(), dataList);
    });
    auto watcher = new QFutureWatcher<ReadRet>();
    watcher->setFuture(future);
    connect(watcher, &QFutureWatcher<ReadRet>::finished, this, [=]{
        auto result = watcher->result();
        bool success = std::get<0>(result);
        auto dataList = std::get<2>(result);
        if (success) {
            this->ui->gatherPage->addExpandItems(dataList);
        }
        QMessageBox::information(this, tr("Open Result"),
                                 tr("Open file %1 %2.%3").arg(filename).arg(success ? tr("Success") : tr("Failed")).arg(std::get<1>(result)),
                                 QMessageBox::Ok);
        delete watcher;
    });
}

QString MainWindow::getSaveFileName()
{
    auto docPaths = QStandardPaths::standardLocations(QStandardPaths::DocumentsLocation);
    QString path = QApplication::applicationDirPath();
    if (!docPaths.isEmpty()) {
        path = docPaths.first();
    }
    QString name = tr("untitled");
    auto filename = QFileDialog::getSaveFileName(this, tr("Save"), path, QLatin1String("Json (*.json)"), &name);
    if (filename.contains(QString(".json"), Qt::CaseSensitive) || filename.isEmpty()) {
        return filename;
    }
    return filename += QLatin1String(".json");
}

QString MainWindow::getOpenFileName()
{
    auto docPaths = QStandardPaths::standardLocations(QStandardPaths::DocumentsLocation);
    QString path = QApplication::applicationDirPath();
    if (!docPaths.isEmpty()) {
        path = docPaths.first();
    }
    return QFileDialog::getOpenFileName(this, tr("Open"), path, QLatin1String("Json (*.json)"));

}
