#include "gatherdata.h"

GatherData::GatherData(const QString &name, QObject *parent)
    : QObject(parent)
    , m_name(name)
    , m_addr(-1)
    , m_hardwareVer(QString())
    , m_softwareVer(QString())
    , m_productDesc(QString())
    , m_sensorCount(0)
    , m_gatherTimeout(12000)
    , m_sensorTimeout(12000)
{

}

GatherData::~GatherData()
{
    qDeleteAll(m_ybDataList);
    m_ybDataList.clear();
}

QString GatherData::name() const
{
    return m_name;
}

void GatherData::setName(const QString &name)
{
    if (m_name == name) {
        return ;
    }
    m_name = name;
    emit nameChanged(name);
}

int GatherData::addr() const
{
    return m_addr;
}

void GatherData::setAddr(int addr)
{
    if (m_addr == addr) {
        return ;
    }
    m_addr = addr;
    emit addrChanged(addr);
}

QString GatherData::hardwareVersion() const
{
    return m_hardwareVer;
}

void GatherData::setHardwareVerion(const QString &version)
{
    m_hardwareVer = version;
}

QString GatherData::softwareVersion() const
{
    return m_softwareVer;
}

void GatherData::setSoftwareVersion(const QString &softwareVersion)
{
    m_softwareVer = softwareVersion;
}

QString GatherData::productDescription() const
{
    return m_productDesc;
}

void GatherData::setProductDesc(const QString &desc)
{
    m_productDesc = desc;
}

int GatherData::sensorCount() const
{
    return m_sensorCount;
}

void GatherData::setSensorCount(int count)
{
    m_sensorCount = count;
}

void GatherData::appendSensorData(YBSensorData *data)
{
    m_ybDataList.append(data);
}

QList<YBSensorData *> GatherData::sensorDataList() const
{
    return m_ybDataList;
}

PortParam GatherData::portParam() const
{
    return m_portParam;
}

void GatherData::setPortParam(const PortParam &portParam)
{
    m_portParam = portParam;
}

int GatherData::gatherTimeout() const
{
    return m_gatherTimeout;
}

void GatherData::setGatherTimeout(int msecTimeout)
{
    m_gatherTimeout = msecTimeout;
}

int GatherData::sensorTimeout() const
{
    return m_sensorTimeout;
}

void GatherData::setSensorTimeout(int msecTimeout)
{
    m_sensorTimeout = msecTimeout;
}

QJsonObject GatherData::save()
{
    QJsonObject root;
    root.insert(QLatin1String("name"), name());
    root.insert(QLatin1String("address"), addr());
    root.insert(QLatin1String("hardwareVer"), hardwareVersion());
    root.insert(QLatin1String("softwareVer"), softwareVersion());
    root.insert(QLatin1String("productDescription"), productDescription());
    root.insert(QLatin1String("sensorCount"), sensorCount());
    root.insert(QLatin1String("portParam"), m_portParam.saveJson());
    QJsonArray sensorArr;
    for (int i = 0; i < m_ybDataList.count(); i++) {
        sensorArr.append(m_ybDataList[i]->save());
    }
    root.insert(QLatin1String("YBDatas"), sensorArr);

    root.insert(QLatin1String("gatherTimeout"), m_gatherTimeout);
    root.insert(QLatin1String("sensorTimeout"), m_sensorTimeout);
    return root;
}

void GatherData::load(const QJsonObject &root)
{
    setName(root.value(QLatin1String("name")).toString());
    setAddr(root.value(QLatin1String("address")).toInt());
    setHardwareVerion(root.value(QLatin1String("hardwareVer")).toString());
    setSoftwareVersion(root.value(QLatin1String("softwareVer")).toString());
    setProductDesc(root.value(QLatin1String("productDescription")).toString());
    setSensorCount(root.value(QLatin1String("sensorCount")).toInt());
    QJsonObject paramObj = root.value(QLatin1String("portParam")).toObject();
    m_portParam.loadJson(paramObj);

    QJsonArray sensorArr = root.value(QLatin1String("YBDatas")).toArray();
    for (int i = 0; i < sensorArr.count(); i++) {
        auto sensorData = new YBSensorData();
        sensorData->load(sensorArr.at(i).toObject());
        m_ybDataList.append(sensorData);
    }

    m_gatherTimeout = root.value(QLatin1String("gatherTimeout")).toInt();
    m_sensorTimeout = root.value(QLatin1String("sensorTimeout")).toInt();
}
