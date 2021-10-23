/**
 * @file    syncdata.cpp
 * @author  Myth
 * @version 1.0
 * @date    2021.10.23
 */

#include <QByteArray>
#include <QJsonDocument>
#include <QJsonParseError>
#include <QJsonObject>
#include <QJsonValue>

#include "syncdata.h"

//数据源链接，数据由其他程序生成
#define DATA_URL "http://node.akashic.cc/data.json"

//每 100ms 更新一次数据
#define UPDATE_INTERVAL 100

SyncData::SyncData(QObject *parent) : QObject(parent)
{
    data_manager = new QNetworkAccessManager(this);
    //每次请求返回结果后调用槽函数 dataReplyFinished
    connect(data_manager, SIGNAL(finished(QNetworkReply *)),
            this, SLOT(dataReplyFinished(QNetworkReply *)));

    timer = new QTimer(this);
    //定时器触发后调用槽函数 updateNodeData
    connect(timer, SIGNAL(timeout()),
            this, SLOT(updateNodeData()));
    timer->start(UPDATE_INTERVAL);
}

/**
 * @brief  槽函数：请求新的数据
 */
void SyncData::updateNodeData(void)
{
    data_manager->get(QNetworkRequest(QUrl(DATA_URL)));
}

/**
 * @brief  槽函数：处理收到的数据
 */
void SyncData::dataReplyFinished(QNetworkReply *reply)
{
    QNetworkReply::NetworkError networkerror = reply->error();

    if (networkerror == QNetworkReply::NoError)
    {
        // JSON 解析
        QByteArray databuff = reply->readAll();
        QString result = QString::fromStdString(databuff.toStdString()).toUtf8();
        QJsonDocument document;
        QJsonParseError parseerror;
        document = QJsonDocument::fromJson(result.toUtf8(), &parseerror);

        QString time;
        double humi;
        double temp;
        double light;

        if (!document.isNull() && parseerror.error == QJsonParseError::NoError)
        {
            if (document.isObject())
            {
                QJsonObject Object = document.object();
                if (!Object.isEmpty())
                {
                    if (Object.contains("time"))
                    {
                        QJsonValue time_val = Object.value("time");
                        if (time_val.isString())
                        {
                            time = time_val.toString();
                            emit timeUpdated(time); //信号：时间更新
                        }
                    }

                    //遍历 2 ~ 5 节点数据
                    for (int seq = 2; seq <= 5; seq++)
                    {
                        if (Object.contains("node" + QString::number(seq)))
                        {
                            QJsonValue dataObj = Object.value("node" + QString::number(seq));
                            if (dataObj.isObject())
                            {
                                QJsonObject ObjectData = dataObj.toObject();
                                if (ObjectData.contains("humi"))
                                {
                                    QJsonValue humi_val = ObjectData.value("humi");
                                    if (humi_val.isDouble())
                                    {
                                        humi = humi_val.toDouble();
                                    }
                                }

                                if (ObjectData.contains("temp"))
                                {
                                    QJsonValue temp_val = ObjectData.value("temp");
                                    if (temp_val.isDouble())
                                    {
                                        temp = temp_val.toDouble();
                                    }
                                }

                                if (ObjectData.contains("light"))
                                {
                                    QJsonValue light_val = ObjectData.value("light");
                                    if (light_val.isDouble())
                                    {
                                        light = light_val.toDouble();
                                    }
                                }

                                emit nodeDataUpdated(seq, humi, temp, light); //信号：数据更新
                            }
                        }
                    }
                }
            }
        }
    }
}

SyncData::~SyncData()
{
    delete data_manager;
    delete timer;
}
