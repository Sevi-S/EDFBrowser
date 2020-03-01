//
// Created by Sevi Suonenlahti on 27.11.2019.
//

#ifndef EDFBROWSER_170_SOURCE_HILBERTDOCK_H
#define EDFBROWSER_170_SOURCE_HILBERTDOCK_H

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

#include "third_party/fidlib/fidlib.h"

class UI_HilbertDockWindow : public QObject {
Q_OBJECT

public:
    UI_HilbertDockWindow(QWidget *parent);

   //UI_HilbertDockWindow();
   ~UI_HilbertDockWindow();

    UI_Mainwindow *mainwindow;

    QDialog *HilbertDialog;
   QDockWidget *dock;

    struct signalcompblock *signalcomp;

    void init(int);
    void DoTransform();
    void GenerateGraphs();
    void GetTimes();
    void GetInfo();
    void setDashboard();

private:
   // QDialog *HilbertDialog;
   QHBoxLayout *vlayout1;

   int dashboard;
};

//UI_HilbertDockWindow::~UI_HilbertDockWindow()
//{
    //from spectrum dock

 //   free(buf1);
  //  buf1 = NULL;
  //  free(buf2);
 //   buf2 = NULL;
 //   free(buf3);
  //  buf3 = NULL;
  //  free(buf4);
  //  buf4 = NULL;
  //  free(buf5);
 //   buf5 = NULL;

   // free_fft_wrap(fft_data);
   // fft_data = NULL;

   // delete HilbertDialog;
//}

#endif //EDFBROWSER_170_SOURCE_HILBERTDOCK_H


