//
// Created by Sevi Suonenlahti on 3.9.2020.
//

#ifndef EDFBROWSER_MASTER_STARTGYROANALYSIS_H
#define EDFBROWSER_MASTER_STARTGYROANALYSIS_H

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
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <time.h>
#include <limits.h>
#include <string>
#include <cstring>

#include <fstream>
#include <iostream>

#include <map>
#include <vector>
#include <algorithm>
#include <iterator>

#include "global.h"
#include "mainwindow.h"
#include "signalcurve.h"
#include "utils.h"
#include "filter.h"
#include "spike_filter.h"
#include "ravg_filter.h"
#include "flywheel.h"
#include "fft_wrap.h"

#include <fstream>

#include "third_party/fidlib/fidlib.h"

#include "sync_manager.h"
#include "createGraphs.h"

class UI_Mainwindow;

class startGyroAnalysis : public QObject {
Q_OBJECT
public:
    startGyroAnalysis(QWidget *w_parent);

    QDockWidget* dock;

    UI_Mainwindow *mainwindow;

    ~startGyroAnalysis();

    //void loadData();
    void ParseSave(char* filename,int load);
    void loadStruct();

    QComboBox *combobox;

    struct tremor {

        int TremorNum;
        double StartTime;
        double EndTime;
        double EMGStart;
        double EMGEnd;

        tremor():StartTime(0),EndTime(0),EMGStart(0),EMGEnd(0),TremorNum(0) { };
    };

    struct TremorData {

        char patientName[MAX_PATH_LENGTH];
        char saveFileName[MAX_PATH_LENGTH];
        char patientData[MAX_PATH_LENGTH];//DATA
        char patientNumber[MAX_PATH_LENGTH];
        char patientDate[MAX_PATH_LENGTH];//Date!!

        char dataFilesEMG[MAX_PATH_LENGTH];
        char dataFilesgyro1[MAX_PATH_LENGTH];
        char dataFilesgyro2[MAX_PATH_LENGTH];
        char dataFilesgyro3[MAX_PATH_LENGTH];
        char savePath1[MAX_PATH_LENGTH];//changed to strings
        char pyPath1[MAX_PATH_LENGTH];


        double syncOffset1;
        double syncOffset2;
        double syncOffset3;
        int tremorNum;

        struct tremor TremorS[24];    //24 max tremors saved for now


        TremorData():syncOffset1(0),syncOffset2(0),syncOffset3(0),tremorNum(0) { };
    };

    TremorData loadTremor;//these names are backwards i know
    TremorData newTremor;



signals:
    void python();
   // void ParseStruct(TremorDatas tempTrm);
private:

    QWidget *dockWidgetContents;
    //QDockWidget* dock;
    QLabel *PateintName;
    QLabel *PatientNumber;
    QLabel *PatientNumber_2;
    QLabel *dateTimeEdit;
    QPushButton *pushButton;
    QPushButton *pushButton1;
    QPushButton *saveButton;
   // QPushButton *pushButton1;
    //QComboBox *TremorViewer;
    QLabel *startText;
    QLabel *endText;

    int counter;

    std::string patientData;
    std::string patientName;
    std::string patientNumber;
    std::string patientinfo;
    std::string patientDate;
    std::string tremorNum;
    std::string tremorTime;
    std::string tremorStartTime;
    std::string tremorendTime;

    std::string loadFilename;
    char pyfileName[MAX_PATH_LENGTH];
    char pypath[MAX_PATH_LENGTH],pyfile[MAX_PATH_LENGTH],path[MAX_PATH_LENGTH],recent_opendir[MAX_PATH_LENGTH],outputfilename[MAX_PATH_LENGTH],fullSavePath[MAX_PATH_LENGTH];


private slots:

    void sendPythonSig();
    void printGraphs();
    void backToCrosshairs();
    //  void selectArea();
    //   void getArea();
    void printTrm();
    void save();
    void loadData();

public slots:

    void ParseStructs();
    void convertStruct();
    void onActivated();
};


#endif//EDFBROWSER_MASTER_STARTGYROANALYSIS_H