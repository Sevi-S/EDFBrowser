//
// Created by Sevi Suonenlahti on 20.12.2019.
//

#ifndef EDFBROWSER_170_SOURCE_CREATEGRAPHS_H
#define EDFBROWSER_170_SOURCE_CREATEGRAPHS_H

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
#include "import_Data.h"


#include "third_party/fidlib/fidlib.h"
class UI_Mainwindow;

class createGraphs : public QObject {
    Q_OBJECT
    public:
        createGraphs(QWidget *w_parent);


        UI_Mainwindow *mainwindow;

        ~createGraphs();

private:

    QDockWidget* dock;
    QPushButton *AddButtonCreate, *AddButtonCross, *AddButtonGet;

    int counter;

private slots:
    void testFunc();
    void selectArea();
    void getArea();

public slots:


};


#endif //EDFBROWSER_170_SOURCE_CREATEGRAPHS_H
