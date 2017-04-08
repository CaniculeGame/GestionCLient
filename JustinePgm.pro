#-------------------------------------------------
#
# Project created by QtCreator 2015-02-04T11:24:19
#
#-------------------------------------------------

QT       += core gui sql widgets webkitwidgets network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = GestionClient
TEMPLATE = app

RC_ICONS = icon.ico

QTPLUGIN += QSQLMYSQL


SOURCES += main.cpp\
        mainwindow.cpp \
    dialog.cpp \
    dialogprestation.cpp \
    listesoins.cpp \
    mdpdialog.cpp \
    prixconfigw.cpp \
    fidelitedessin.cpp \
    graphfidelite.cpp \
    gestion.cpp \
    ../../qcustomplot/qcustomplot.cpp \
    qcustomplot.cpp \
    argentio.cpp \
    uistock.cpp \
    infoperson.cpp \
    windowpromo.cpp \
    planningui.cpp

HEADERS  += mainwindow.h \
    dialog.h \
    dialogprestation.h \
    listesoins.h \
    mdpdialog.h \
    prixconfigw.h \
    fidelitedessin.h \
    graphfidelite.h \
    gestion.h \
    ../../qcustomplot/qcustomplot.h \
    qcustomplot.h \
    argentio.h \
    uistock.h \
    infoperson.h \
    windowpromo.h \
    planningui.h

FORMS    += mainwindow.ui \
    dialog.ui \
    dialogprestation.ui \
    mdpdialog.ui \
    prixconfigw.ui \
    gestion.ui \
    argentio.ui \
    uistock.ui \
    windowpromo.ui \
    planningui.ui

