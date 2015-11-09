#-------------------------------------------------
#
# Project created by QtCreator 2015-10-27T16:00:31
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

INCLUDEPATH += /Users/lizhimin/Desktop/QT/eigen

TARGET = GrandMap
TEMPLATE = app


SOURCES += main.cpp\
        window.cpp \
    renderarea.cpp \
    DataNavigator/projectionpanel.cpp \
    fileloader.cpp \
    storage.cpp \
    Map/grandmap.cpp \
    Map/projectionview.cpp \
    DataNavigator/point.cpp \
    DataNavigator/ndpad.cpp \
    DataNavigator/scatterplot.cpp \
    ../Scagnostics/Binner.cpp \
    ../Scagnostics/GraphMeasures.cpp \
    ../Scagnostics/GraphTheoreticScagnosticsMetric.cpp \
    ../Scagnostics/scag.cpp \
    ../Scagnostics/Scagnostics.cpp \
    ../Scagnostics/StringTokenizer.cpp \
    ../ALGLIB/src/alglibinternal.cpp \
    ../ALGLIB/src/alglibmisc.cpp \
    ../ALGLIB/src/ap.cpp \
    ../ALGLIB/src/dataanalysis.cpp \
    ../ALGLIB/src/diffequations.cpp \
    ../ALGLIB/src/fasttransforms.cpp \
    ../ALGLIB/src/integration.cpp \
    ../ALGLIB/src/interpolation.cpp \
    ../ALGLIB/src/linalg.cpp \
    ../ALGLIB/src/optimization.cpp \
    ../ALGLIB/src/solvers.cpp \
    ../ALGLIB/src/specialfunctions.cpp \
    ../ALGLIB/src/statistics.cpp \
    Library/kmeans++/KMeans.cpp \
    Library/kmeans++/KmTree.cpp \
    Library/kmeans++/KmUtils.cpp \
    Map/parallelcoordinate.cpp \
    Map/infopanel.cpp \
    DataNavigator/scatterplotinfopanel.cpp \
    DataNavigator/painterpanel.cpp

HEADERS  += window.h \
    renderarea.h \
    DataNavigator/projectionpanel.h \
    fileloader.h \
    storage.h \
    Map/grandmap.h \
    Map/projectionview.h \
    DataNavigator/point.h \
    DataNavigator/ndpad.h \
    DataNavigator/scatterplot.h \
    ../Scagnostics/Binner.h \
    ../Scagnostics/GraphMeasures.h \
    ../Scagnostics/GraphTheoreticScagnosticsMetric.h \
    ../Scagnostics/Scagnostics.h \
    ../Scagnostics/StringTokenizer.h \
    ../ALGLIB/src/alglibinternal.h \
    ../ALGLIB/src/alglibmisc.h \
    ../ALGLIB/src/ap.h \
    ../ALGLIB/src/dataanalysis.h \
    ../ALGLIB/src/diffequations.h \
    ../ALGLIB/src/fasttransforms.h \
    ../ALGLIB/src/integration.h \
    ../ALGLIB/src/interpolation.h \
    ../ALGLIB/src/linalg.h \
    ../ALGLIB/src/optimization.h \
    ../ALGLIB/src/solvers.h \
    ../ALGLIB/src/specialfunctions.h \
    ../ALGLIB/src/statistics.h \
    ../ALGLIB/src/stdafx.h \
    Library/kmeans++/KMeans.h \
    Library/kmeans++/KmTree.h \
    Library/kmeans++/KmUtils.h \
    Map/parallelcoordinate.h \
    Map/infopanel.h \
    DataNavigator/scatterplotinfopanel.h \
    DataNavigator/painterpanel.h

FORMS    += window.ui
