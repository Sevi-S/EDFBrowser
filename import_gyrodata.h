#ifndef EDFBROWSER_MASTER_IMPORT_GYRODATA_H
#define EDFBROWSER_MASTER_IMPORT_GYRODATA_H


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
//#include <iostream>
//#include <fstream>


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

#include "sync_manager.h"
#include "createGraphs.h"
#include "startGyroAnalysis.h"


class UI_Mainwindow;

class UI_Import_Data : public QObject {
Q_OBJECT
public:
    UI_Import_Data(QWidget *w_parent);

    UI_Mainwindow *mainwindow;

    //  QDockWidget *dock;

//public:

    //UI_Import_Data(QWidget *parent);

    ~UI_Import_Data();


    QDialog *ImportDialog;
    QDialog *LoadDialog;
    QDialog *PatientDialog;

    int listlen,i{};
    char saveFile[MAX_PATH_LENGTH];

    bool isNew = false;
    bool loaded =false;


    //load Dialog
    QLabel *LoadExsisting;
    QPushButton *Load_2;
    QPushButton *Cancel;
    QPushButton *NewPatient_2;
    QLabel *NewPatient;

    //Import New Dialog
    QLabel *SelectWear;
    QPushButton *LeftButt;
    QScrollArea *scrollArea{};
    QWidget *scrollAreaWidgetContents{};
    QPushButton *Right;
    QPushButton *HeadButt;
    QLabel *SelectEMG;
    QPushButton *EmgButt;
    QLabel *SelectVid;
    QPushButton *VidButt;
    QLabel *ViewFIles;
    QPushButton *SyncButt;
    QPushButton *CancelButt;
    QPushButton *OkButt;
    QPushButton *BackButt;

    QListWidget* filelist;

    //patient dialog
    QDateTimeEdit *EnterDateTime;
    QTextEdit *EnterName;
    QTextEdit *EnterPatient;
    QTextEdit *EnterPatient2;
    QTextEdit *EnterSaveName;
    QPushButton *DictButt;
    QLabel *SelectDict;
    QLabel *EnterDate;
    QLabel *SelectPyth;
    QPushButton *PythButt;
    QPushButton *DictButt_2;
    QPushButton *DictButt_3;
    QPushButton *DictButt_4;

    void fileList();
    void RefreshMain();

    // char path[MAX_PATH_LENGTH],pypath[MAX_PATH_LENGTH];

    char pypathFin[MAX_PATH_LENGTH],pyfilefin[MAX_PATH_LENGTH];

    QPushButton *AddButtonPy;

    struct tremor {
        int TremorNum;
        double StartTime;
        double EndTime;
        double EMGStart;
        double EMGEnd;
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
  //  struct TremorData tempTremor;

    struct TremorData saveTremor;
    struct TremorData loadTremor;


public slots:
    void writeToFile(char* filename,TremorData tempTre);
    void readFromFile(char* filename);

private:

    char path[MAX_PATH_LENGTH]{},
            pypath[MAX_PATH_LENGTH]{},
            outputfilename[MAX_PATH_LENGTH]{},
            recent_opendir[MAX_PATH_LENGTH]{},
            pyfile[MAX_PATH_LENGTH]{};
    //   temppath[MAX_PATH_LENGTH];

    int filecount;



private slots:
    void testFunc();
    void closeIntroOpenNew();
    void backIntro();
    void importFunc();
    void handleVideo();
    void handleSync();
    void PatientInfo();
    void backFromPatient();
    void savePatient();
    void SelectPythonFile();
    void handleGraphs();
    void openLoadDock();
    void newFiles();

};

#endif //EDFBROWSER_MASTER_IMPORT_GYRODATA_H