/**
 * @file    main.cpp
 * @author  Myth
 * @version 1.0
 * @date    2021.10.23
 */

#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
