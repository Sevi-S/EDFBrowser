//
// Created by Sevi Suonenlahti on 24.8.2020.
//

#ifndef EDFBROWSER_MASTER_SYNC_MANAGER_H
#define EDFBROWSER_MASTER_SYNC_MANAGER_H

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

#include <stdio.h>
#include <string.h>
#include <string>
#include <stdlib.h>

#include <iostream>
#include <fstream>

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

class UI_Mainwindow;

class sync_manager : public QObject {
Q_OBJECT
public:
    sync_manager(QWidget *w_parent);

    UI_Mainwindow *mainwindow;

    int i,p,totOffset1, totOffset2, totOffset3;

    ~sync_manager();

    QDialog *SyncDialog;

    int clickedfile;


private:
    //   int offset1, offset2, offset3;
   // QListWidget* filelist;
    QDockWidget* dock;

    QString *printOffset1,*printOffset2,*printOffset3;

    QLabel *label1,*label2,*label3,*label4,*totOffSet1, *totOffSet2, *totOffSet3;

    QVBoxLayout *vlayout1;

    QDoubleSpinBox *gyrooffset1,*gyrooffset2,*gyrooffset3;

    //  QTextEdit *totOffSet1, *totOffSet2, *totOffSet3;

    double crosshairOffset, initialOffset, totalOffset, printoffset,offset1, offset2, offset3;

    QPushButton *AddButtonClose,*AddButtonSync, *AddButtonBack, *AddButtonCross, *AddbuttonGyr1, *AddbuttonGyr2, *AddbuttonGyr3;
    // SignalCurve *curve1, curve2, curve3;

    void emitSimulateClick();

public slots:
   // void fileList();
    // void timeControl();
    void openImporter();
    void crossHairs();
    void itsHappening();
    //  void refreshTimes();
    void doSyncStuff();
    void file1();
    void file2();
    void file3();

    //signals:

};

#endif //EDFBROWSER_MASTER_SYNC_MANAGER_H