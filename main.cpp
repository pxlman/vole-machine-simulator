#include "mainwindow.h"

#include <QApplication>
#include <QTextStream>
#include "qfile.h"
#include "machine/Machine.h"
#include <iostream>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    // Get Style sheet
    QFile file(":/styles/stylesheet.qss");
    if(file.open(QFile::ReadOnly | QFile::Text)){
        QTextStream textStream(&file);
        QString styleSheet = textStream.readAll();
        file.close();
        a.setStyleSheet(styleSheet);
    }
    MainWindow w;
    w.show();
    return a.exec();
}
