#include "mainwindow.h"
#include <QApplication>
#include <QCoreApplication>
#include <QtSql>
#include <QTranslator>
#include <QString>
#include <iostream>


#define q2c(string) string.toStdString()

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w; //fenetre + gestionnaire

    QTranslator translator;
    QString locationPath  = QLibraryInfo::location (QLibraryInfo::TranslationsPath);
    translator.load ("qt_" + QLocale::system ().name (), locationPath);
    a.installTranslator (&translator);

    w.show();

    return a.exec();
}
