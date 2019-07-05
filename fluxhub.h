#ifndef FLUXHUB_H
#define FLUXHUB_H

#include <QObject>
#include <QQuickItem>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QQmlEngine>
#include <QQmlProperty>
#include <QList>
#include <QColor>

class FluxHub : public QObject
{
    Q_OBJECT
public:
    Q_PROPERTY(QString demoJson READ demoJson CONSTANT)
    explicit FluxHub(QQmlEngine* engine, QObject *parent = nullptr);

    QString demoJson() const;

    Q_INVOKABLE QQuickItem *visualize(QQuickItem *rootItem, const QVariantMap &qmlJsonObj);
signals:

public slots:
    void destroyNode(QQuickItem *rootNode){
        delete rootNode;
    }
private:
    QQuickItem *createObjectNode(QQuickItem *currentItem, const QString &key, const QJsonObject &jsonObj, int level);
    QQuickItem * createLeafNode(QQuickItem *currentItem, const QString &key, const QJsonValue &jsonVal, int level);
    QQuickItem * createArrayNode(QQuickItem *currentItem, const QString &key, const QJsonArray &jsonArray, int level);
    inline QQuickItem *createLevelItem(int level, QQuickItem *parentItem);

    inline void setNodeText(QQuickItem *item, const QString &text){
        QQmlProperty nodeTextProp(item, "text");
        nodeTextProp.write(text);
    }
    inline QString levelColor(int level){
        return level >= m_levelColors.size()? m_levelColors.back() : m_levelColors[level];
    }
private:
    QQmlEngine *m_engine = nullptr;
    QList<QQmlComponent*> m_levelComponents;
    QList<QString> m_levelColors;
};

#endif // FLUXHUB_H
