#include "fluxhub.h"
#include <QFile>
#include <QtDebug>
#include <QQmlProperty>

FluxHub::FluxHub(QQmlEngine *engine, QObject *parent)
    : QObject(parent),
      m_engine(engine)
{

}

QString FluxHub::demoJson() const
{
    QFile inf(":/demo.json");
    if(!inf.open(QIODevice::ReadOnly)){
        qWarning()<<"Cannot open demo json file!";
        return "";
    }
    auto content = inf.readAll();
    inf.close();
    return QString(content);
}

QQuickItem * FluxHub::visualize(QQuickItem *rootItem, const QVariantMap &qmlJsonObj)
{
    auto component = new QQmlComponent(m_engine, QUrl::fromLocalFile(":/Node.qml"));
    if(component->status() == QQmlComponent::Error){
        qWarning()<<"Component create error!"<<component->errorString();
        return nullptr;
    }
    m_levelComponents.push_back(component);
//    m_levelColors.push_back("#1f4e5a");
//    m_levelColors.push_back("#009c8e");
//    m_levelColors.push_back("#ffdb6a");
//    m_levelColors.push_back("#ffa658");
//    m_levelColors.push_back("#ea5f40");
    m_levelColors.push_back("#205374");
    m_levelColors.push_back("#27a09e");
    m_levelColors.push_back("#30ce88");
    m_levelColors.push_back("#7de393");
    m_levelColors.push_back("#d3f5ce");

    auto jsonObj = QJsonObject::fromVariantMap(qmlJsonObj);
    auto rootNodeName = jsonObj[QStringLiteral("name")].toString();
    jsonObj.remove(QStringLiteral("name"));

    return createObjectNode(rootItem, rootNodeName, jsonObj, 0);
}

QQuickItem *FluxHub::createLevelItem(int level, QQuickItem *parentItem)
{
    auto *component = level >= m_levelComponents.count()?
                m_levelComponents.back() : m_levelComponents[level];
    auto item = qobject_cast<QQuickItem*>(component->create());

    if(item == nullptr){
        qWarning()<<"Create level item error!"<<component->errorString();
        return nullptr;
    }
    QQmlProperty layoutProp(parentItem, "childrenLayout");
    if(layoutProp.type() == QQmlProperty::Invalid){
        item->setParentItem(parentItem);
    }else{
        auto layout = layoutProp.read().value<QQuickItem*>();
        Q_ASSERT(layout);
        item->setParentItem(layout);
    }

    QQmlProperty colorProp(item, "color");
    colorProp.write(levelColor(level));
    return item;
}


QQuickItem * FluxHub::createObjectNode(QQuickItem *currentItem, const QString &key, const QJsonObject &jsonObj, int level)
{
    auto objectNode = createLevelItem(level, currentItem);
    setNodeText(objectNode, key);

    for(auto it = jsonObj.begin(); it != jsonObj.end(); ++it){
        auto child = it.value();
        auto childName = it.key();
        auto childType = it.value().type();

        QQuickItem *rt = nullptr;
        if(childType == QJsonValue::Object){
            rt = createObjectNode(objectNode, childName, child.toObject(), level + 1);

        }
        else if(childType == QJsonValue::Array){
            rt = createArrayNode(objectNode, childName, child.toArray(), level + 1);
        }
        else{
            rt = createLeafNode(objectNode, childName, child, level + 1);
        }
        if(rt == nullptr){
            qWarning()<<"Create child node error!"<<childName<<childType;
            return nullptr;
        }
    }
    return objectNode;
}

QQuickItem * FluxHub::createArrayNode(QQuickItem *currentItem, const QString &key, const QJsonArray &jsonArray, int level)
{
    auto arrayNode = createLevelItem(level, currentItem);
    setNodeText(arrayNode, key);

    for(int i = 0; i != jsonArray.size(); ++i){
        auto element = jsonArray[i];
        auto type = element.type();
        QQuickItem *rt = nullptr;
        auto elementKey = QString("[%1]").arg(i);
        if(type == QJsonValue::Object){
            rt = createObjectNode(arrayNode, elementKey, element.toObject(), level + 1);
        }
        else if(type == QJsonValue::Array){
            rt = createArrayNode(arrayNode, elementKey, element.toArray(), level + 1);
        }
        else{
            rt = createLeafNode(arrayNode, elementKey, element, level + 1);
        }
        if(rt == nullptr){
            qWarning()<<"Create array element error!"<<elementKey<<type;
            return nullptr;
        }
    }
    return arrayNode;
}

QQuickItem * FluxHub::createLeafNode(QQuickItem *currentItem, const QString &key, const QJsonValue &jsonVal, int level)
{
    QQuickItem * leafNode = nullptr;
    auto leafType = jsonVal.type();
    switch (leafType) {
    case QJsonValue::Bool:
        leafNode = createLevelItem(level, currentItem);
        setNodeText(leafNode, key + " : " + QString(jsonVal.toBool()? "true" : "false"));
        break;
    case QJsonValue::Double:
        leafNode = createLevelItem(level, currentItem);
        setNodeText(leafNode, key + " : " + QString::number(jsonVal.toDouble()));
        break;
    case QJsonValue::String:
        leafNode = createLevelItem(level, currentItem);
        setNodeText(leafNode, key + " : " + jsonVal.toString());
        break;
    default:
        break;
    }
    return leafNode;
}
