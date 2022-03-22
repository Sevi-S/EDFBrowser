#include "import_gyrodata.h"

UI_Import_Data::UI_Import_Data(QWidget *w_parent) {

    listlen = 0;

    mainwindow = (UI_Mainwindow *) w_parent;

    ImportDialog = new QDialog(w_parent);
    LoadDialog = new QDialog(w_parent);
    PatientDialog = new QDialog(w_parent);

    //Import Dialog (when you click new)
    ImportDialog->setMinimumSize(600, 400);
    ImportDialog->setMaximumSize(600, 400);
    ImportDialog->setWindowTitle("Configuring Gyroscopic Data");
    ImportDialog->setModal(true);
    //   ImportDialog->setAttribute(Qt::WA_DeleteOnClose, true);

    SelectWear = new QLabel(ImportDialog);
    SelectWear->setObjectName(QString::fromUtf8("SelectWear"));
    SelectWear->setGeometry(QRect(20, 70, 170, 15));
    LeftButt = new QPushButton(ImportDialog);
    LeftButt->setObjectName(QString::fromUtf8("LeftButt"));
    LeftButt->setGeometry(QRect(10, 90, 110, 30));
    Right = new QPushButton(ImportDialog);
    Right->setObjectName(QString::fromUtf8("Right"));
    Right->setGeometry(QRect(140, 90, 110, 30));
    HeadButt = new QPushButton(ImportDialog);
    HeadButt->setObjectName(QString::fromUtf8("HeadButt"));
    HeadButt->setGeometry(QRect(270, 90, 110, 30));
    SelectEMG = new QLabel(ImportDialog);
    SelectEMG->setObjectName(QString::fromUtf8("SelectEMG"));
    SelectEMG->setGeometry(QRect(20, 10, 170, 15));
    EmgButt = new QPushButton(ImportDialog);
    EmgButt->setObjectName(QString::fromUtf8("EmgButt"));
    EmgButt->setGeometry(QRect(10, 30, 110, 30));
    SelectVid = new QLabel(ImportDialog);
    SelectVid->setObjectName(QString::fromUtf8("SelectVid"));
    SelectVid->setGeometry(QRect(20, 120, 170, 15));
    VidButt = new QPushButton(ImportDialog);
    VidButt->setObjectName(QString::fromUtf8("VidButt"));
    VidButt->setGeometry(QRect(10, 140, 110, 30));
    ViewFIles = new QLabel(ImportDialog);
    ViewFIles->setObjectName(QString::fromUtf8("ViewFIles"));
    ViewFIles->setGeometry(QRect(10, 175, 170, 15));
    filelist = new QListWidget(ImportDialog);
    filelist->setGeometry(10, 200, 380, 75);
    fileList();
    SyncButt = new QPushButton(ImportDialog);
    SyncButt->setObjectName(QString::fromUtf8("SyncButt"));
    SyncButt->setGeometry(QRect(10, 310, 140, 30));
    CancelButt = new QPushButton(ImportDialog);
    CancelButt->setObjectName(QString::fromUtf8("CancelButt"));
    CancelButt->setGeometry(QRect(480, 350, 110, 30));
    OkButt = new QPushButton(ImportDialog);
    OkButt->setObjectName(QString::fromUtf8("OkButt"));
    OkButt->setGeometry(QRect(380, 350, 110, 30));
    BackButt = new QPushButton(ImportDialog);
    BackButt->setObjectName(QString::fromUtf8("BackButt"));
    BackButt->setGeometry(QRect(280, 350, 110, 30));

    SelectWear->setText("Select Wearables Data");
    LeftButt->setText("Left");
    Right->setText("Right");
    HeadButt->setText("Other");
    SelectEMG->setText("Select EMG Data");
    EmgButt->setText("EMG");
    SelectVid->setText("Select Video File");
    VidButt->setText("Video");
    ViewFIles->setText("Selected Files:");
    SyncButt->setText("Synchronize Data");
    CancelButt->setText("Cancel");
    OkButt->setText("Next");
    BackButt->setText("Back");


    //Load Dialog
    LoadDialog->setMinimumSize(400, 200);
    LoadDialog->setMaximumSize(600, 300);
    LoadDialog->setWindowTitle("Configuring Gyroscopic Data");
    LoadDialog->setModal(true);
    //  LoadDialog->setAttribute(Qt::WA_DeleteOnClose, true);

    LoadExsisting = new QLabel(LoadDialog);
    LoadExsisting->setObjectName(QString::fromUtf8("LoadExsisting"));
    LoadExsisting->setGeometry(QRect(190, 100, 191, 20));
    LoadExsisting->setAlignment(Qt::AlignCenter);
    NewPatient_2 = new QPushButton(LoadDialog);
    NewPatient_2->setObjectName(QString::fromUtf8("NewPatient_2"));
    NewPatient_2->setGeometry(QRect(30, 50, 113, 32));
    Load_2 = new QPushButton(LoadDialog);
    Load_2->setObjectName(QString::fromUtf8("Load_2"));
    Load_2->setGeometry(QRect(30, 100, 113, 32));
    Cancel = new QPushButton(LoadDialog);
    Cancel->setObjectName(QString::fromUtf8("Cancel"));
    Cancel->setGeometry(QRect(240, 170, 113, 32));
    NewPatient = new QLabel(LoadDialog);
    NewPatient->setObjectName(QString::fromUtf8("NewPatient"));
    NewPatient->setGeometry(QRect(190, 60, 191, 20));
    NewPatient->setAlignment(Qt::AlignCenter);

    LoadExsisting->setText("Load Exsiting Run");
    Load_2->setText("Load");
    Cancel->setText("Cancel");
    NewPatient_2->setText( "New");
    NewPatient->setText("New Patient");

    //patientDialog
    ImportDialog->setMinimumSize(600, 400);
    ImportDialog->setMaximumSize(600, 400);
    ImportDialog->setWindowTitle("Enter Patient Information");
    ImportDialog->setModal(true);

    PatientDialog->resize(510, 400);
    EnterDateTime = new QDateTimeEdit(PatientDialog);
    EnterDateTime->setObjectName(QString::fromUtf8("EnterDateTime"));
    EnterDateTime->setGeometry(QRect(20, 80, 200, 25));
    EnterDate = new QLabel(PatientDialog);
    EnterDate->setObjectName(QString::fromUtf8("EnterDate"));
    EnterDate->setGeometry(QRect(20, 50, 200, 25));
    EnterName = new QTextEdit(PatientDialog);
    EnterName->setObjectName(QString::fromUtf8("EnterName"));
    EnterName->setGeometry(QRect(20, 20, 200, 30));
    EnterName->setTabChangesFocus(true);
    EnterPatient = new QTextEdit(PatientDialog);
    EnterPatient->setObjectName(QString::fromUtf8("EnterPatient"));
    EnterPatient->setGeometry(QRect(20, 110, 200, 30));
    EnterPatient->setTabChangesFocus(true);
    EnterPatient2 = new QTextEdit(PatientDialog);
    EnterPatient2->setObjectName(QString::fromUtf8("EnterPatient2"));
    EnterPatient2->setGeometry(QRect(20, 150, 200, 80));
    EnterPatient2->setTabChangesFocus(true);
    EnterSaveName = new QTextEdit(PatientDialog);
    EnterSaveName->setObjectName(QString::fromUtf8("EnterName"));
    EnterSaveName->setGeometry(QRect(20, 240, 200, 30));
    EnterSaveName->setTabChangesFocus(true);

  //  DictButt = new QPushButton(PatientDialog);
  //  DictButt->setObjectName(QString::fromUtf8("DictButt"));
  //  DictButt->setGeometry(QRect(170, 240, 115, 30));
  //  SelectDict = new QLabel(PatientDialog);
  //  SelectDict->setObjectName(QString::fromUtf8("SelectDict"));
  //  SelectDict->setGeometry(QRect(20, 250, 140, 15));
    SelectPyth = new QLabel(ImportDialog);
    SelectPyth->setObjectName(QString::fromUtf8("SelectPyth"));
    SelectPyth->setGeometry(QRect(21, 280, 190, 20));

    PythButt = new QPushButton(ImportDialog);
    PythButt->setObjectName(QString::fromUtf8("PythButt"));
    PythButt->setGeometry(QRect(160, 275, 115, 30));
    DictButt_2 = new QPushButton(PatientDialog);
    DictButt_2->setObjectName(QString::fromUtf8("DictButt_2"));
    DictButt_2->setGeometry(QRect(175, 320, 115, 30));
    DictButt_3 = new QPushButton(PatientDialog);
    DictButt_3->setObjectName(QString::fromUtf8("DictButt_3"));
    DictButt_3->setGeometry(QRect(400, 320, 115, 30));
    DictButt_4 = new QPushButton(PatientDialog);
    DictButt_4->setObjectName(QString::fromUtf8("DictButt_4"));
    DictButt_4->setGeometry(QRect(285, 320, 115, 30));

    EnterDate->setText("Enter Date and time");
    EnterName->setText("Enter Name");
    EnterPatient->setText("Enter Patient Number");
    EnterPatient2->setText("Enter Additional Patient Information");
    EnterSaveName->setText("Enter Name of Save File");
  //  DictButt->setText("Go");
  //  SelectDict->setText("Select Working Folder");
    SelectPyth->setText("Select Python File");
    PythButt->setText("Load");
    DictButt_2->setText("Back");
    DictButt_3->setText("Cancel");
    DictButt_4->setText("Next");


    //Slots
    //QObject::connect(AddButtonPy,SIGNAL(clicked()),this,SLOT(testFunc()));
    QObject::connect(Cancel,SIGNAL(clicked()),LoadDialog,SLOT(close()));
    QObject::connect(Load_2,SIGNAL(clicked()),this,SLOT(openLoadDock()));
    QObject::connect(CancelButt,SIGNAL(clicked()),ImportDialog,SLOT(close()));
    // QObject::connect(Load_2,SIGNAL(clicked()),this,SLOT());   //slot for loading exsiting run
    QObject::connect(NewPatient_2,SIGNAL(clicked()),this,SLOT(closeIntroOpenNew()));

    QObject::connect(BackButt,SIGNAL(clicked()),this,SLOT(backIntro()));
    QObject::connect(LeftButt,SIGNAL(clicked()),this, SLOT(importFunc()));
    QObject::connect(Right,SIGNAL(clicked()),this, SLOT(importFunc()));
    QObject::connect(EmgButt,SIGNAL(clicked()),this, SLOT(importFunc()));
    QObject::connect(HeadButt,SIGNAL(clicked()),this, SLOT(importFunc()));
    QObject::connect(VidButt,SIGNAL(clicked()),this, SLOT(handleVideo()));
    QObject::connect(OkButt,SIGNAL(clicked()),this, SLOT(PatientInfo()));
    QObject::connect(DictButt_3,SIGNAL(clicked()),PatientDialog, SLOT(close()));
    QObject::connect(DictButt_2,SIGNAL(clicked()),this, SLOT(backFromPatient()));
    QObject::connect(DictButt_4,SIGNAL(clicked()),this, SLOT(savePatient()));
    QObject::connect(PythButt,SIGNAL(clicked()),this, SLOT(SelectPythonFile()));
   // QObject::connect(DictButt,SIGNAL(clicked()),this, SLOT(testFunc()));
    connect(mainwindow, SIGNAL(importFileZ()), this, SLOT(newFiles()));
    //  connect(mainwindow, SIGNAL(python()), this, SLOT(testFunc()));

    //Patinet slots:
    //DictButt ->choose directory
    //    DictButt_4->setText("");
    // PythButt

    QObject::connect(SyncButt,SIGNAL(clicked()),this,SLOT(handleSync()));


    //struct tremor;

    //struct TremorData saveTremor;


    if (isNew == false)
    {
        LoadDialog->exec();
    }
    else
    {
        PatientDialog->exec();
    }
    filecount = 0;
}

void UI_Import_Data::closeIntroOpenNew()
{
    LoadDialog->hide();
    // delete LoadDialog;
    PatientDialog->show();

}
void UI_Import_Data::backIntro()
{
    ImportDialog->hide();
    //  delete ImportDialog;
    PatientDialog->show();
}
void UI_Import_Data::openLoadDock()
{
    ImportDialog->close();
    //  delete ImportDialog;
    LoadDialog->close();
    PatientDialog->close();

    //jumphere

    //  strlcpy(path, QFileDialog::getOpenFileName(mainwindow, "Open file", QString::fromLocal8Bit(mainwindow->recent_opendir), "*.txt").toLocal8Bit().data(), MAX_PATH_LENGTH);

    //   if(!strcmp(path, ""))
    //   {
//        return;
//    }

    mainwindow->startgyroanalysis = new startGyroAnalysis(mainwindow);
  //  mainwindow->startgyroanalysis->loadData();
    mainwindow->startgyroanalysis->loadStruct();//todo

    strlcpy(saveTremor.patientName,mainwindow->startgyroanalysis->loadTremor.patientName,strlen(mainwindow->startgyroanalysis->loadTremor.patientName)+2);
    strlcpy(saveTremor.saveFileName,mainwindow->startgyroanalysis->loadTremor.saveFileName,strlen(mainwindow->startgyroanalysis->loadTremor.saveFileName)+2);
    strlcpy(saveTremor.patientData,mainwindow->startgyroanalysis->loadTremor.patientData,strlen(mainwindow->startgyroanalysis->loadTremor.patientData)+2);
    strlcpy(saveTremor.patientNumber,mainwindow->startgyroanalysis->loadTremor.patientNumber,strlen(mainwindow->startgyroanalysis->loadTremor.patientNumber)+2);
    strlcpy(saveTremor.patientDate,mainwindow->startgyroanalysis->loadTremor.patientDate,strlen(mainwindow->startgyroanalysis->loadTremor.patientDate)+2);

    strlcpy(saveTremor.dataFilesEMG,mainwindow->startgyroanalysis->loadTremor.dataFilesEMG,strlen(mainwindow->startgyroanalysis->loadTremor.dataFilesEMG)+2);
    strlcpy(saveTremor.dataFilesgyro1,mainwindow->startgyroanalysis->loadTremor.dataFilesgyro1,strlen(mainwindow->startgyroanalysis->loadTremor.dataFilesgyro1)+2);
    strlcpy(saveTremor.dataFilesgyro2,mainwindow->startgyroanalysis->loadTremor.dataFilesgyro2,strlen(mainwindow->startgyroanalysis->loadTremor.dataFilesgyro2)+2);
    strlcpy(saveTremor.dataFilesgyro3,mainwindow->startgyroanalysis->loadTremor.dataFilesgyro3,strlen(mainwindow->startgyroanalysis->loadTremor.dataFilesgyro3)+2);

    strlcpy(saveTremor.savePath1,mainwindow->startgyroanalysis->loadTremor.savePath1,strlen(mainwindow->startgyroanalysis->loadTremor.savePath1)+2);
    strlcpy(saveTremor.pyPath1,mainwindow->startgyroanalysis->loadTremor.pyPath1,strlen(mainwindow->startgyroanalysis->loadTremor.pyPath1)+2);

    saveTremor.syncOffset1 = mainwindow->startgyroanalysis->loadTremor.syncOffset1;
    saveTremor.syncOffset2 = mainwindow->startgyroanalysis->loadTremor.syncOffset2;
    saveTremor.syncOffset3 = mainwindow->startgyroanalysis->loadTremor.syncOffset3;
    saveTremor.tremorNum = mainwindow->startgyroanalysis->loadTremor.tremorNum;

    for (int i = 0; i < 23; i++)
    {
        saveTremor.TremorS[i].TremorNum = mainwindow->startgyroanalysis->loadTremor.TremorS[i].TremorNum;
        saveTremor.TremorS[i].StartTime = mainwindow->startgyroanalysis->loadTremor.TremorS[i].StartTime;
        saveTremor.TremorS[i].EndTime = mainwindow->startgyroanalysis->loadTremor.TremorS[i].EndTime;
        saveTremor.TremorS[i].EMGStart = mainwindow->startgyroanalysis->loadTremor.TremorS[i].EMGStart;
        saveTremor.TremorS[i].EMGEnd = mainwindow->startgyroanalysis->loadTremor.TremorS[i].EMGEnd;
    }
    loaded = true;


}

void UI_Import_Data::newFiles()
{
 if(filecount == 0)
 {
     strlcpy(saveTremor.dataFilesEMG,mainwindow->path,strlen(mainwindow->path)+2);
    // saveTremor.dataFilesEMG = temp;
  //   printf("%s\n",saveTremor.dataFilesEMG.c_str());
 }
 else if(filecount== 1)
 {
     strlcpy(saveTremor.dataFilesgyro1,mainwindow->path,strlen(mainwindow->path)+2);
   //  printf("%s\n",saveTremor.dataFilesgyro1.c_str());
 }
 else if(filecount== 2)
 {
     strlcpy(saveTremor.dataFilesgyro2,mainwindow->path,strlen(mainwindow->path)+2);
  //   printf("%s\n",saveTremor.dataFilesgyro2.c_str());
 }
 else if(filecount== 3)
 {
     strlcpy(saveTremor.dataFilesgyro3,mainwindow->path,strlen(mainwindow->path)+2);
  //   printf("%s\n",saveTremor.dataFilesgyro3.c_str());
 }

   filecount = filecount +1;
}

void UI_Import_Data::PatientInfo()
{
   LoadDialog->close();
   PatientDialog->close();
   ImportDialog->close();

   mainwindow->creategraphs = new createGraphs(mainwindow);

   //strlcpy(saveFile, patientSave.toUtf8().constData(),MAX_PATH_LENGTH);

//   std::ofstream myfile;
//   myfile.open (saveFile, std::ios::out|std::ios::trunc);
//   myfile.close();

 //  writeToFile(saveFile,saveTremor);


   loaded = false;
}

void UI_Import_Data::writeToFile(char* filename,TremorData tempTre)
{
   FILE *outfile;

   // open file for writing
   outfile = fopen (filename, "wb");
   if (outfile == NULL)
   {
       fprintf(stderr, "\nError opend file\n");
       exit (1);
   }

   // write struct to file
   fwrite (&tempTre, sizeof(struct TremorData), 1, outfile);
 //  fwrite (&input2, sizeof(struct person), 1, outfile);

   if(fwrite != 0)
       printf("contents to file written successfully !\n");
   else
       printf("error writing file !\n");

   // close file
   fclose (outfile);

   /*
   TremorData item = tempTre;

   std::ofstream fout(filename);
   fout << item.patientName << '\n' << item.saveFileName << '\n' << item.patientData << '\n' << item.patientNumber << '\n' << item.patientDate << '\n'
   << item.dataFiles[0] << '\n' << item.dataFiles[1] << '\n' << item.dataFiles[2] << '\n' << item.dataFiles[3] << '\n' << item.savePath1 << '\n' << item.pyPath1
   << '\n' << item.syncOffset1 << '\n' << item.syncOffset2 << '\n' << item.syncOffset3 << '\n' << item.tremorNum << '\n';


   for (int i = 0; i < item.tremorNum; i++)
   {
       fout << item.TremorS[i].TremorNum << '\n' << item.TremorS[i].StartTime << '\n' << item.TremorS[i].EndTime << '\n' << item.TremorS[i].EMGStart << '\n' << item.TremorS[i].EMGEnd;
   }
   fout.close();

    */
}

void UI_Import_Data::readFromFile(char* filename)
{
   // TremorData tempTremor;

    FILE *infile;
    //struct person input;

    // Open person.dat for reading
    infile = fopen (filename, "rb");
    if (infile == NULL)
    {
        fprintf(stderr, "\nError opening file\n");
        exit (1);
    }

    // read file contents till end of file
    while(fread(&loadTremor, sizeof(struct TremorData), 1, infile));

    // close file
    fclose (infile);

   // loadTremor = tempTremor;


    // std::string line;

   // PointFull item_from_disk;
  //  std::ifstream fin(filename);



 /*   fin >> loadTremor.patientName >> loadTremor.saveFileName >> loadTremor.patientData >>  loadTremor.patientNumber >> loadTremor.patientDate
    >> loadTremor.dataFiles[0] >> loadTremor.dataFiles[1]  >> loadTremor.dataFiles[2]  >> loadTremor.dataFiles[3] >> loadTremor.savePath1 >> loadTremor.pyPath1
    >> loadTremor.syncOffset1  >> loadTremor.syncOffset2 >> loadTremor.syncOffset3 >> loadTremor.tremorNum;

    for (int i = 0; i < loadTremor.tremorNum +1; i++)
    {
       fin >> loadTremor.TremorS[i].TremorNum >> loadTremor.TremorS[i].StartTime >> loadTremor.TremorS[i].EndTime >> loadTremor.TremorS[i].EMGStart >> loadTremor.TremorS[i].EMGEnd;
    }*/

   // fin.close();
  //  fin.close();



  //saveTremor = loadTremor;

  /*
   loadTremor.patientName = tempTremor.patientName;
    loadTremor.saveFileName = tempTremor.saveFileName;
    loadTremor.patientData = tempTremor.patientData;
    loadTremor.patientNumber = tempTremor.patientNumber;
    loadTremor.patientDate = tempTremor.patientDate;

    loadTremor.dataFiles[0] = tempTremor.dataFiles[0];
    loadTremor.dataFiles[1] = tempTremor.dataFiles[1];
    loadTremor.dataFiles[2] = tempTremor.dataFiles[2];
    loadTremor.dataFiles[3] = tempTremor.dataFiles[3];

    loadTremor.savePath1 = tempTremor.savePath1;
    loadTremor.pyPath1 = tempTremor.pyPath1;

    loadTremor.syncOffset1 = tempTremor.syncOffset1;
    loadTremor.syncOffset2 = tempTremor.syncOffset2;
    loadTremor.syncOffset3 = tempTremor.syncOffset3;
    loadTremor.tremorNum = tempTremor.tremorNum;

    for (int i = 0; i < tempTremor.tremorNum +1; i++)
    {
        loadTremor.TremorS[i].TremorNum = tempTremor.TremorS[i].TremorNum;
        loadTremor.TremorS[i].StartTime = tempTremor.TremorS[i].StartTime;
        loadTremor.TremorS[i].EndTime = tempTremor.TremorS[i].EndTime;
        loadTremor.TremorS[i].EMGStart = tempTremor.TremorS[i].EMGStart;
        loadTremor.TremorS[i].EMGEnd = tempTremor.TremorS[i].EMGEnd;
    }
*/
}


void UI_Import_Data::backFromPatient()
{
    ImportDialog->hide();
    PatientDialog->hide();//432 in exe, 431, in py file
    LoadDialog->show();
}

void UI_Import_Data::handleVideo()
{
    //   QMessageBox messagewindow(QMessageBox::Critical, "Error", "Rename video file accordingly");
    // messagewindow.exec();

    ImportDialog->hide();
    mainwindow->start_stop_video();
    ImportDialog->show();
}

void UI_Import_Data::handleGraphs()
{
    mainwindow->creategraphs =  new createGraphs(mainwindow);
    ImportDialog->close();

}

void UI_Import_Data::testFunc()//choose the main func
{
    //char *result;//This char will receive the return value.

// setenv("PYTHONPATH",".",1);
    /* FILE *Pyfile;
     Pyfile = fopen(pyfile, "r");

     //i wonder if python path has to be set

     PyObject *module, *func, *prm, *ret;
     // Initialize the Python Interpreter
     //const char *scriptDirectoryName = "/Users/Sevi_Pro/Desktop/seviEDFbrow/edfbrowser_170_source/"; /Users/Sevi_Pro/Desktop/EDF177/EDFbrowser-master/test1.py

     // std::string pypathStr = pypath;
     const char *scriptDirectoryName = pypath;
     // const char *scriptDirectoryName = "/Users/Sevi_Pro/Desktop/EDF177/EDFbrowser-master/";

     Py_Initialize();

     PyObject *sysPath = PySys_GetObject("path");
     PyObject *path2 = PyUnicode_FromString(scriptDirectoryName);
     int result = PyList_Insert(sysPath, 0, path2);



     //"/Users/Sevi_Pro/Desktop/ideEDFBROW/edfbrowser_170_source/cmake-build-debug/graphs.py"
     // Load the module object
     module = PyImport_ImportModule(pyfile);
     // std::string pyfileStr = pyfile;
     //  module = PyImport_ImportModule("test1");
     //   module = PyImport_ImportModule("test1"); //this one right here chief


     if (module != 0)//Asks if the script was loaded at all.
     {
          func = PyObject_GetAttrString(module,(char*) "someFunction");//Opens a function within the python script. Notice that you must use a function within the python script, because otherwise you can't return anything.
          prm = Py_BuildValue("(s)", "Hello");//The "(ss)" means two strings are passed (replace with "i" for integer for instance), the "Hello" and "Mars" are the strings i pass to the script.
          ret = PyObject_CallObject(func, prm);//Returns some python object i have literally no idea about ...


      //   PyRun_AnyFile(Pyfile,pyfile);

         // PyRun_SimpleString(module);
         //result = PyUnicode_FromString(ret);// ... but luckily there's a function to cast it back to a c-compatible char*!
         //printf("The Script decdided that '%s' is longer!",result);

         Py_DECREF(module);//cleanup?
         Py_DECREF(func);//cleanup?
         Py_DECREF(prm);//cleanup?
         Py_DECREF(ret);//cleanup?
         Py_Finalize();
     }

         //https://stackoverflow.com/questions/3286448/calling-a-python-method-from-c-c-and-extracting-its-return-value source 1
         //https://stackoverflow.com/questions/28438585/how-to-return-output-from-pyrun-simplefile-in-c-code

         //https://docs.python.org/3/c-api/import.html imports of modules

         //example implementation https://github.com/python/cpython
         //https://github.com/hrldcpr/hungarian/blob/master/example.py
     else//No script found
     {
         printf("Error: No script file named \"script.py\" was found!\n");
         Py_Finalize();
     }
     fclose(Pyfile);
     //Py_Finalize(); */

}

void UI_Import_Data::SelectPythonFile()
{
    FILE *newfile;

    strlcpy(path, QFileDialog::getOpenFileName(mainwindow, "Open file", QString::fromLocal8Bit(mainwindow->recent_opendir)).toLocal8Bit().data(), MAX_PATH_LENGTH);

    if(!strcmp(path, ""))
    {
        return;
    }
    char ch = '\"';
    char ch1 = '/';


    get_directory_from_path(recent_opendir, path, MAX_PATH_LENGTH);
    get_filename_from_path(outputfilename, path, MAX_PATH_LENGTH);

    printf("%s\n", recent_opendir);
    printf("%s\n", outputfilename);


    strlcpy(pypathFin, recent_opendir,strlen(recent_opendir)+2);
    strlcpy(pyfilefin, outputfilename, strlen(outputfilename)+2);

    std::string filepy(pypathFin);

    std::string filename = (filepy + "/");
//#if defined (Q_OS_MAC)
//    std::string filename = (filepy + "/");
//#elif defined (Q_OS_WIN32)
//    std::string filename = (filepy + "\\");
//#endif
    filename += pyfilefin;
    //printf("%s\n", filename.c_str());

 //   std::ofstream myfile;
 //   myfile.open (saveFile, std::ios::out|std::ios::app);
 //   myfile<<"#" << filename << "\n";
 //   myfile.close();

  //  std::string pyPathTemp(saveFile);

    strlcpy(saveTremor.pyPath1,filename.c_str(),strlen(filename.c_str())+2);
    printf("%s\n", saveTremor.pyPath1);

   // strlcpy(saveTremor.pyPath1,saveFile,strlen(saveFile)+2);
    //saveTremor.pyPath1 = filename;


    //to remove .py from end of file, depends on how python is embeded
 /*   int q = strlen(recent_opendir);
    //all this to add quotation marks to end of the path name
    strlcat(recent_opendir,&ch1,MAX_PATH_LENGTH);
    strlcpy(pypath,recent_opendir,q+2);
    // printf("%s\n", temppath);
    //   strlcat(recent_opendir,&ch,MAX_PATH_LENGTH);
    //   strlcat(&ch,recent_opendir,MAX_PATH_LENGTH);
    //   strlcpy(pypath,&ch,q+4);
    //   printf("%s\n", pypath);

    char ch2 =  '\"';
    char temp = '\"';

    int p = strlen(outputfilename);
    printf("%s\n",outputfilename);
    //add quotation marks to file name
    // strlcat(&ch2,outputfilename,p);
    //  strlcat(outputfilename,&ch2,MAX_PATH_LENGTH);
    //  strlcpy(pyfile,&ch2,p+1);

    strlcpy(pyfile,outputfilename,p-2);
    printf("%s\n",pyfile);
*/
    /*   for (int i = 1; i < strlen(pyfile); i++)
       {
          pyfile[i] = pyfile[i+1];
       }
       strlcat(pyfile,&temp,p);
       printf("%s\n",pyfile);
       printf("%s\n",pypath);*/
}



void UI_Import_Data::RefreshMain()
{
    ImportDialog->update();
    fileList();
}

void UI_Import_Data::savePatient()
{
    QString patientNameZ = EnterName->toPlainText();
    QString patientNumZ = EnterPatient->toPlainText();
    QString patientDetZ = EnterPatient2->toPlainText();
    QString patientSave = EnterSaveName->toPlainText();

    QString time_format = "yyyy-MM-dd  HH:mm:ss";
    QDateTime myDate = EnterDateTime->dateTime();
    QString dateString = myDate.toString(time_format);

    std::string patientNam = patientNameZ.toUtf8().constData();
    std::string patientNum = patientNumZ.toUtf8().constData();
    std::string patientDet = patientDetZ.toUtf8().constData();
    std::string date = dateString.toUtf8().constData();

    strlcpy(saveFile, patientSave.toUtf8().constData(),strlen(patientSave.toUtf8().constData())+2);
   // printf("%s\n",saveFile);

    strlcpy(saveTremor.patientName,patientNameZ.toUtf8().constData(),strlen(patientNameZ.toUtf8().constData())+2);
    strlcpy(saveTremor.saveFileName,patientSave.toUtf8().constData(),strlen(patientSave.toUtf8().constData())+2);
    strlcpy(saveTremor.patientNumber,patientNumZ.toUtf8().constData(),strlen(patientNumZ.toUtf8().constData())+2);
    strlcpy(saveTremor.patientData,patientDetZ.toUtf8().constData(),strlen(patientDetZ.toUtf8().constData())+2);
    strlcpy(saveTremor.patientDate,dateString.toUtf8().constData(),strlen(dateString.toUtf8().constData())+2);

   // saveTremor.patientName = patientNam;
    //saveTremor.patientData = patientDet;
   // saveTremor.patientNumber = patientNum;
   // saveTremor.patientDate = date;
   // saveTremor.saveFileName = patientSave.toUtf8().constData();


    for(i=0; i<24; i++)
    {
        saveTremor.TremorS[i] ={0,0,0,0,0};
    }

    char testTemp = '0';

  //  strlcpy(saveTremor.dataFilesEMG,mainwindow->path,strlen(mainwindow->path)+2);
  //  strlcpy(saveTremor.dataFilesgyro1,mainwindow->path,strlen(mainwindow->path)+2);
    strlcpy(saveTremor.dataFilesgyro2,&testTemp,strlen(&testTemp)+2);
    strlcpy(saveTremor.dataFilesgyro3,&testTemp,strlen(&testTemp)+2);


    PatientDialog->close();
    LoadDialog->close();
    ImportDialog->show();

}

void UI_Import_Data::fileList()
{

    filelist->clear();
    for(i=0; i<mainwindow->files_open; i++){
        new QListWidgetItem(QString::fromLocal8Bit(mainwindow->edfheaderlist[i]->filename), filelist);}
}

void UI_Import_Data::importFunc()
{
    listlen++;

    mainwindow->open_new_file();
    RefreshMain();
}

void UI_Import_Data::handleSync()
{
    ImportDialog->hide();
    LoadDialog->hide();

    isNew = true;
      mainwindow->syncManager = new sync_manager(mainwindow);//mainwindow here is important - NOT "this"
}

UI_Import_Data::~UI_Import_Data()
{
    delete ImportDialog;
    ImportDialog->close();

    delete LoadDialog;
    LoadDialog->close();

    delete PatientDialog;
    PatientDialog->close();
}