//
// Created by Sevi Suonenlahti on 3.9.2020.
//

#ifndef EDFBROWSER_MASTER_CREATEGRAPHS_H
#define EDFBROWSER_MASTER_CREATEGRAPHS_H

#include <QtGlobal>
#include <QApplication>
#include <QDialog>
#include <QDockWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QTimer>
#include <QSlider>
#include <QLabel>
#include <QProgressDialog>
#include <QCheckBox>
#include <QString>
#include <QStringList>
#include <QMessageBox>
#include <QSpinBox>
#include <QComboBox>
#include <QToolTip>
#include <QPalette>
#include <QTextEdit>

#include <iostream>
#include <fstream>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "global.h"
#include "mainwindow.h"
#include "signalcurve.h"
#include "utils.h"
#include "filter.h"
#include "spike_filter.h"
#include "ravg_filter.h"
#include "flywheel.h"
#include "fft_wrap.h"
#include "import_gyrodata.h"
#include "third_party/fidlib/fidlib.h"

#include "startGyroAnalysis.h"
#include "sync_manager.h"


class UI_Mainwindow;

class createGraphs : public QObject {
Q_OBJECT
public:
    createGraphs(QWidget *w_parent);


    UI_Mainwindow *mainwindow;

    ~createGraphs();
    int counter = 0;


    QDockWidget* dock;

    struct tremor {
        int TremorNum;
        long double StartTime;
        long double EndTime;
        long double EMGStart;
        long double EMGEnd;
    };

private:

  //  QDockWidget* dock;
    QPushButton *AddButtonCreate, *AddButtonCross, *AddButtonGet;
    QLabel *tremNumba;

    tremor Tremor[24];

private slots:
    void selectArea();
    void getArea();
    void gotToAnalysis();

public slots:


};


#endif //EDFBROWSER_MASTER_CREATEGRAPHS_H
