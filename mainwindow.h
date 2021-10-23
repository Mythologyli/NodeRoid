/**
 * @file    mainwindows.h
 * @author  Myth
 * @version 1.0
 * @date    2021.10.23
 */

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "syncdata.h"

QT_BEGIN_NAMESPACE
namespace Ui
{
    class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    SyncData *syncdata;
    double temp_max;

private slots:
    void updateTempMax(double temp_value);
    void dispNodeData(int seq, float humi, float temp, float light);
    void dispUpdateTime(QString time);
    void sendNodeWarn(int seq, float humi, float temp, float light);
};
#endif // MAINWINDOW_H
