/**
 * @file    syncdata.h
 * @author  Myth
 * @version 1.0
 * @date    2021.10.23
 */

#ifndef SYNCDATA_H
#define SYNCDATA_H

#include <QObject>
#include <QtNetwork>
#include <QTimer>

class SyncData : public QObject
{
    Q_OBJECT

public:
    SyncData(QObject *parent = nullptr);
    ~SyncData();

private:
    QNetworkAccessManager *data_manager;
    QTimer *timer;

signals:
    void timeUpdated(QString time);
    void nodeDataUpdated(int seq, float humi, float temp, float light);

private slots:
    void updateNodeData(void);
    void dataReplyFinished(QNetworkReply *reply);
};

#endif // SYNCDATA_H
