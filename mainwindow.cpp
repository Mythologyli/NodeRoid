/**
 * @file    mainwindows.cpp
 * @author  Myth
 * @version 1.0
 * @date    2021.10.23
 */

#include "ui_mainwindow.h"

#include "notificationclient.h"
#include "syncdata.h"

#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    syncdata = new SyncData(this);
    connect(syncdata, SIGNAL(timeUpdated(QString)),
            this, SLOT(dispUpdateTime(QString)));
    connect(syncdata, SIGNAL(nodeDataUpdated(int, float, float, float)),
            this, SLOT(dispNodeData(int, float, float, float)));
    connect(syncdata, SIGNAL(nodeDataUpdated(int, float, float, float)),
            this, SLOT(sendNodeWarn(int, float, float, float)));

    temp_max = 27.0;
    connect(this->ui->doubleSpinBox, SIGNAL(valueChanged(double)),
            this, SLOT(updateTempMax(double)));
}

/**
 * @brief  槽函数：更新高温报警温度
 */
void MainWindow::updateTempMax(double temp_value)
{
    temp_max = temp_value;
}

/**
 * @brief  槽函数：显示节点数据
 */
void MainWindow::dispNodeData(int seq, float humi, float temp, float light)
{
    QString line_str;

    line_str = "节点" +
               QString::number(seq) +
               " 湿度：" +
               QString::number((double)humi) +
               "% 温度：" +
               QString::number((double)temp) +
               "°C\n           光照度：" +
               QString::number((double)light) +
               "lx";

    switch (seq)
    {
    case 2:
        ui->label_node2->setText(line_str);
        break;
    case 3:
        ui->label_node3->setText(line_str);
        break;
    case 4:
        ui->label_node4->setText(line_str);
        break;
    case 5:
        ui->label_node5->setText(line_str);
    }
}

/**
 * @brief  槽函数：显示更新时间
 */
void MainWindow::dispUpdateTime(QString time)
{
    QString line_str;

    line_str = "更新时间：" + time;
    ui->label_time->setText(line_str);
}

/**
 * @brief  槽函数：发送系统通知
 */
void MainWindow::sendNodeWarn(int seq, float humi, float temp, float light)
{
    static bool is_first_run = true;
    static QString last_warn_str;
    if (is_first_run)
    {
        NotificationClient().setNotification("成功获取数据！");
        is_first_run = false;
    }

    if (temp > temp_max)
    {
        QString warn_str = "节点" +
                           QString::number(seq) +
                           "温度过高：" +
                           QString::number((double)temp) +
                           "°C";

        if (QString::compare(warn_str, last_warn_str) != 0)
        {
            NotificationClient().setNotification(warn_str);
            last_warn_str.clear();
            last_warn_str.append(warn_str);
        }
    }
}

MainWindow::~MainWindow()
{
    delete ui;
    delete syncdata;
}
