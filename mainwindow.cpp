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

    if (temp >= temp_max)
    {
        line_str = "节点<font color=\"#FF00FF\">" +
                   QString::number(seq) +
                   "</font>&nbsp;湿度：<font color=\"#0000FF\">" +
                   QString::number((double)humi) +
                   "%</font>&nbsp;温度：<font color=\"#FF0000\">" +
                   QString::number((double)temp) +
                   "°C</font><br>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;光照度：<font color=\"#0000FF\">" +
                   QString::number((double)light) +
                   "lx</font>";
    }
    else
    {
        line_str = "节点<font color=\"#FF00FF\">" +
                   QString::number(seq) +
                   "</font>&nbsp;湿度：<font color=\"#0000FF\">" +
                   QString::number((double)humi) +
                   "%</font>&nbsp;温度：<font color=\"#0000FF\">" +
                   QString::number((double)temp) +
                   "°C</font><br>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;光照度：<font color=\"#0000FF\">" +
                   QString::number((double)light) +
                   "lx</font>";
    }

    switch (seq)
    {
    case 2:
        ui->textBrowser_node2->setHtml("<b><font size=\"60\">" + line_str + "</font></b>");
        break;
    case 3:
        ui->textBrowser_node3->setHtml("<b><font size=\"60\">" + line_str + "</font></b>");
        break;
    case 4:
        ui->textBrowser_node4->setHtml("<b><font size=\"60\">" + line_str + "</font></b>");
        break;
    case 5:
        ui->textBrowser_node5->setHtml("<b><font size=\"60\">" + line_str + "</font></b>");
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

    if (temp >= temp_max)
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
