//
// Created by Sevi Suonenlahti on 3.9.2020.
//

#include "startGyroAnalysis.h"


startGyroAnalysis::startGyroAnalysis(QWidget *w_parent) {
    QPalette palette;

    mainwindow = (UI_Mainwindow *) w_parent;

    //temp stuff for now
    dock = new QDockWidget("Tremor Data", w_parent);
    dock->setAllowedAreas(Qt::RightDockWidgetArea);
    dock->setFeatures(
            QDockWidget::DockWidgetClosable | QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetFloatable);
    //Qt::LeftDockWidgetArea |
    //   dock->setAttribute(Qt::WA_TranslucentBackground);
    //  dock->setWindowFlags(Qt::FramelessWindowHint);
    dock->setMinimumHeight(250);
    dock->setMaximumHeight(300);
    dock->setMinimumWidth(800);


    auto color = mainwindow->maincurve->backgroundcolor;
    // color.setAlpha(10);
    palette.setColor(QPalette::Text, mainwindow->maincurve->text_color);
    palette.setColor(QPalette::Base, color);// try to make the window transparent
    dock->setAutoFillBackground(true);//this is the only thing that makes the background appeat at all for somereason
    dock->setPalette(palette);

    PateintName = new QLabel(dock);
    PateintName->setObjectName(QString::fromUtf8("PateintName"));
    PateintName->setGeometry(QRect(430, 30, 121, 15));
    PatientNumber = new QLabel(dock);
    PatientNumber->setObjectName(QString::fromUtf8("PatientNumber"));
    PatientNumber->setGeometry(QRect(430, 50, 121, 15));
    PatientNumber_2 = new QLabel(dock);
    PatientNumber_2->setObjectName(QString::fromUtf8("PatientNumber_2"));
    PatientNumber_2->setGeometry(QRect(430, 20, 200, 150));
    dateTimeEdit = new QLabel(dock);
    dateTimeEdit->setObjectName(QString::fromUtf8("dateTimeEdit"));
    dateTimeEdit->setGeometry(QRect(430, 150, 190, 25));
    pushButton = new QPushButton(dock);
    pushButton->setObjectName(QString::fromUtf8("pushButton"));
    pushButton->setGeometry(QRect(430, 170, 150, 30));
    pushButton1 = new QPushButton(dock);
    pushButton1->setObjectName(QString::fromUtf8("pushButton1"));
    pushButton1->setGeometry(QRect(430, 210, 150, 30));

    saveButton = new QPushButton(dock);
    saveButton->setObjectName(QString::fromUtf8("saveButton"));
    saveButton->setGeometry(QRect(600, 210, 150, 30));


    combobox = new QComboBox(dock);
    combobox->setObjectName(QString::fromUtf8("combobox"));
    combobox->setGeometry(QRect(220, 30, 170, 30));

    startText = new QLabel(dock);
    startText->setObjectName(QString::fromUtf8("startText"));
    startText->setGeometry(QRect(230, 60, 150, 16));
    endText = new QLabel(dock);
    endText->setObjectName(QString::fromUtf8("endText"));
    endText->setGeometry(QRect(230, 100, 150, 16));

    PateintName->setText("PatientName");
    PatientNumber->setText("PatientNum");
    PatientNumber_2->setText("Patient INFO");
    pushButton->setText("Print Graphs");
    pushButton1->setText("Select Area");
    saveButton->setText("Save Session");


    QObject::connect(pushButton,SIGNAL(clicked()),this, SLOT(printTrm()));
//    QObject::connect(pushButton,SIGNAL(clicked()),this, SLOT(loadData()));//syncteststuff
    //loadData()
    QObject::connect(pushButton1,SIGNAL(clicked()),this, SLOT(backToCrosshairs()));
    QObject::connect(saveButton,SIGNAL(clicked()),this, SLOT(save()));
   QObject::connect(combobox,SIGNAL(activated(int)),this,SLOT(onActivated()));

    dock->show();

   // onActivated();

    startText->setText(QString::fromStdString("Start Time = " + std::to_string(loadTremor.TremorS[0].EMGStart)));
    endText->setText(QString::fromStdString("End Time = " + std::to_string(loadTremor.TremorS[0].EMGEnd)));

}

void startGyroAnalysis::onActivated()
{
    int index = combobox->currentIndex();
    startText->setText(QString::fromStdString("Start Time = " + std::to_string(loadTremor.TremorS[index].EMGStart)));
    endText->setText(QString::fromStdString("End Time = " + std::to_string(loadTremor.TremorS[index].EMGEnd)));

    dock->update();
}


void startGyroAnalysis::sendPythonSig()
{
    emit mainwindow->python();
}

void startGyroAnalysis::ParseStructs()
{
    PateintName->setText(QString::fromUtf8(loadTremor.patientName));
    PatientNumber->setText(QString::fromUtf8(loadTremor.patientNumber));
    PatientNumber_2->setText(QString::fromUtf8(loadTremor.patientData));
    dateTimeEdit->setText(QString::fromUtf8(loadTremor.patientDate));

   // printf("%i\n",loadTremor.tremorNum);
    for (int i = 0; i < loadTremor.tremorNum; i++)
    {
        combobox->insertItem(i,QString::fromStdString("Tremor Number " + std::to_string(loadTremor.TremorS[i].TremorNum +1)));
      // TremorViewer->insertItem(loadTremor.TremorS[i].TremorNum,QString::fromStdString("Tremor Number " + std::to_string(loadTremor.TremorS[i].TremorNum +1)));
     //   printf("%i\n",loadTremor.TremorS[i].TremorNum); //QString::fromStdString("Number of Tremors selected = " + std::to_string(counter +1))

    }

    printf("%i\n",loadTremor.tremorNum);

}

void startGyroAnalysis::loadStruct()
{
    strlcpy(path, QFileDialog::getOpenFileName(mainwindow, "Open file", QString::fromLocal8Bit(mainwindow->recent_opendir)).toLocal8Bit().data(), MAX_PATH_LENGTH);

    if(!strcmp(path, ""))
    {
        return;
    }
    //todo slow scrolling during selecting the tremors
    get_directory_from_path(recent_opendir, path, MAX_PATH_LENGTH);
    get_filename_from_path(outputfilename, path, MAX_PATH_LENGTH);

    char ch1 = '/';
    int q = strlen(recent_opendir);
    int p = strlen(outputfilename);
    strlcat(recent_opendir,&ch1,MAX_PATH_LENGTH);
    strlcpy(fullSavePath,recent_opendir,q+2);
    strlcat(fullSavePath, outputfilename, p+q +2);


    FILE *infile;

    infile = fopen (fullSavePath, "rb");

    // read file contents till end of file
    while(fread(&newTremor, sizeof(struct TremorData), 1, infile));

    // close file
    fclose (infile);

    loadTremor = newTremor;
   // mainwindow->UI_import_data->saveTremor ;
    ParseStructs();

    mainwindow->load_file_from_save(loadTremor.dataFilesEMG);
   // printf("%s\n",loadTremor.dataFilesgyro1);
    mainwindow->load_file_from_save(loadTremor.dataFilesgyro1);
    mainwindow->load_file_from_save(loadTremor.dataFilesgyro2);
    mainwindow->load_file_from_save(loadTremor.dataFilesgyro3);

    startText->setText(QString::fromStdString("Start Time = " + std::to_string(loadTremor.TremorS[0].EMGStart)));
    endText->setText(QString::fromStdString("End Time = " + std::to_string(loadTremor.TremorS[0].EMGEnd)));

    for(int i=0; i<mainwindow->files_open; i++)
    {
        if(i == 0)
        { continue;}
        else if (i == 1)
        {
            mainwindow->edfheaderlist[i]->viewtime = mainwindow->edfheaderlist[i]->viewtime + ((loadTremor.syncOffset1)/ 0.0000001);
            printf("%f\n",loadTremor.syncOffset1);
            printf("%f\n",((loadTremor.syncOffset1)* 0.0000001));
        }

        else if (i == 2 )
        {
            mainwindow->edfheaderlist[i]->viewtime = mainwindow->edfheaderlist[i]->viewtime + ((loadTremor.syncOffset2)/ 0.0000001);
        }
        else if (i == 3 )
        {
            mainwindow->edfheaderlist[i]->viewtime = mainwindow->edfheaderlist[i]->viewtime + ((loadTremor.syncOffset3)/ 0.0000001);
        }
      //  mainwindow->edfheaderlist[i]->viewtime = mainwindow->edfheaderlist[i]->viewtime +   ;

    }
    mainwindow->setup_viewbuf();
}

void startGyroAnalysis::convertStruct()
{

    strlcpy(loadTremor.patientName,mainwindow->UI_import_data->saveTremor.patientName,strlen(mainwindow->UI_import_data->saveTremor.patientName)+2);
    strlcpy(loadTremor.saveFileName,mainwindow->UI_import_data->saveTremor.saveFileName,strlen(mainwindow->UI_import_data->saveTremor.saveFileName)+2);
    strlcpy(loadTremor.patientData,mainwindow->UI_import_data->saveTremor.patientData,strlen(mainwindow->UI_import_data->saveTremor.patientData)+2);
    strlcpy(loadTremor.patientNumber,mainwindow->UI_import_data->saveTremor.patientNumber,strlen(mainwindow->UI_import_data->saveTremor.patientNumber)+2);
    strlcpy(loadTremor.patientDate,mainwindow->UI_import_data->saveTremor.patientDate,strlen(mainwindow->UI_import_data->saveTremor.patientDate)+2);

    strlcpy(loadTremor.dataFilesEMG,mainwindow->UI_import_data->saveTremor.dataFilesEMG,strlen(mainwindow->UI_import_data->saveTremor.dataFilesEMG)+2);
    strlcpy(loadTremor.dataFilesgyro1,mainwindow->UI_import_data->saveTremor.dataFilesgyro1,strlen(mainwindow->UI_import_data->saveTremor.dataFilesgyro1)+2);
    strlcpy(loadTremor.dataFilesgyro2,mainwindow->UI_import_data->saveTremor.dataFilesgyro2,strlen(mainwindow->UI_import_data->saveTremor.dataFilesgyro2)+2);
    strlcpy(loadTremor.dataFilesgyro3,mainwindow->UI_import_data->saveTremor.dataFilesgyro3,strlen(mainwindow->UI_import_data->saveTremor.dataFilesgyro3)+2);

    strlcpy(loadTremor.savePath1,mainwindow->UI_import_data->saveTremor.savePath1,strlen(mainwindow->UI_import_data->saveTremor.savePath1)+2);
    strlcpy(loadTremor.pyPath1,mainwindow->UI_import_data->saveTremor.pyPath1,strlen(mainwindow->UI_import_data->saveTremor.pyPath1)+2);

    loadTremor.syncOffset1 = mainwindow->UI_import_data->saveTremor.syncOffset1;
    loadTremor.syncOffset2 = mainwindow->UI_import_data->saveTremor.syncOffset2;
    loadTremor.syncOffset3 = mainwindow->UI_import_data->saveTremor.syncOffset3;
    loadTremor.tremorNum = mainwindow->UI_import_data->saveTremor.tremorNum;


    for (int i = 0; i < 24; i++)
    {
        loadTremor.TremorS[i].TremorNum = mainwindow->UI_import_data->saveTremor.TremorS[i].TremorNum;
        loadTremor.TremorS[i].StartTime = mainwindow->UI_import_data->saveTremor.TremorS[i].StartTime;
        loadTremor.TremorS[i].EndTime = mainwindow->UI_import_data->saveTremor.TremorS[i].EndTime;
        loadTremor.TremorS[i].EMGStart = mainwindow->UI_import_data->saveTremor.TremorS[i].EMGStart;
        loadTremor.TremorS[i].EMGEnd = mainwindow->UI_import_data->saveTremor.TremorS[i].EMGEnd;
    }
}

void startGyroAnalysis::ParseSave(char* filename,int load)
{
    if(!loadFilename.size())
    {
        loadFilename = "testDoc1.txt";
    }

   // std::ifstream inFile;

 //   if(!loadFilename.size())
    //{
   //     std::ifstream inFile("testDoc1.txt");
  //  }
   // else
    //    {
            std::ifstream inFile(filename);
    //    }

    if (inFile.is_open()) {
        std::string line;
        int namecount = 0;
        int numcount = 0;
        int infocount = 0;
        int tremorNumCont = 0;
        int timeCont = 0;
        int sync1 = 0;
        int sync2 = 0;
        int sync3 = 0;
        int tremornum;
        std::string pathsave;
        std::string pathpytfile;
        std::string temp;
        while (std::getline(inFile, line))
        {
            if(line.at(0)=='&')
            {
                patientData = line;
                for (int i = 0; i < patientData.length(); i++)
                {
                    patientData.at(i) = patientData.at(i+1);//remove the delimter

                    if(patientData.at(i)==',')
                    {
                        patientName.assign(patientData,0,namecount);
                        break;
                    }
                    else
                    {
                        namecount++;
                    }
                }
                for (int i = namecount + 1; i < patientData.length(); i++)
                {
                    patientData.at(i) = patientData.at(i+1);//remove the delimter

                    if(patientData.at(i)==',')
                    {
                        patientNumber.assign(patientData,namecount+2,numcount-1);
                        break;
                    }
                    else
                    {
                        numcount++;
                    }
                }
                for (int i = numcount + namecount + 2; i < patientData.length(); i++)
                {
                    patientData.at(i) = patientData.at(i+1);//remove the delimter

                    if(patientData.at(i)==',')
                    {
                        patientinfo.assign(patientData,namecount + numcount+3,infocount-1);
                        break;
                    }
                    else
                    {
                        infocount++;
                    }
                }
                patientDate.assign(patientData,namecount + numcount+ infocount +5,patientData.length()-(namecount + numcount+ infocount +3));
            }
            else if(line.at(0)=='@')
            {
                //  TremorViewer->addItem(QString());
                tremorNum = line;
                for (int i = 0; i < tremorNum.length(); i++)
                {
                    tremorNum[i] = tremorNum[i+1];
                }
                combobox->addItem(QString(QString::fromStdString(tremorNum)));
                //   TremorViewer->setItemText(tremorNumCont+1,QString::fromStdString(tremorNum));
                //    TremorViewer->setItemText(tremorNumCont+1,"apples");
                tremorNumCont++;
            }
            else if(line.at(0)=='$')//needs to be ! in the future
            {
                tremorTime = line;
                // tremorStartTime = std::stoi(tremorTime[1]) when i make int list
                for (int i = 0; i < tremorTime.length(); i++)
                {
                    if(tremorTime.at(i)==',')
                    {
                        break;
                    }
                    timeCont++;
                }
                tremorStartTime.assign(tremorTime,1,timeCont-1);
                tremorendTime.assign(tremorTime,timeCont+1,tremorTime.length()-timeCont);
            }
           else if(line.at(0)=='%')//load the path files
            {
                pathsave = line;
                for (int i = 0; i < pathsave.length(); i++) {
                    pathsave[i] = pathsave[i+1];//remove the delimter
                }

                if(load == 1)
                {mainwindow->load_file_from_save(pathsave.c_str());}
            }
            else if(line.at(0)=='#')//load the path files
            {
                pathpytfile = line;
                for (int i = 0; i < pathpytfile.length(); i++) {
                    pathpytfile[i] = pathpytfile[i+1];//remove the delimter
                }

               strlcat(pyfileName,pathpytfile.c_str(),pathpytfile.length()+1); //pyfileName.assign(pathpytfile,1,pathpytfile.length()-1);
            }
            else if(line.at(0)=='?')//load the path files
            {
                temp = line;
                //sync1 = std::stoi(line);
                for (int i = 0; i < temp.length(); i++) {
                    temp[i] = temp[i+1];//remove the delimter
                }

                sync1 = std::stoi(temp);
            }
            else if(line.at(0)=='!')//load the path files
            {
                temp = line;
                //sync1 = std::stoi(line);
                for (int i = 0; i < temp.length(); i++) {
                    temp[i] = temp[i+1];//remove the delimter
                }

                sync2 = std::stoi(temp);
            }
            else if(line.at(0)=='<')//load the path files
            {
                temp = line;
                //sync1 = std::stoi(line);
                for (int i = 0; i < temp.length(); i++) {
                    temp[i] = temp[i+1];//remove the delimter
                }

                sync3 = std::stoi(temp);
            } else{ continue;}


            // using printf() in all tests for consistency
            // printf("%s\n", line.c_str());
        }

        inFile.close();
    }

    PateintName->setText(QString::fromStdString(patientName));
    PatientNumber->setText(QString::fromStdString(patientNumber));
    PatientNumber_2->setText(QString::fromStdString(patientinfo));
    dateTimeEdit->setText(QString::fromStdString(patientDate));



    //  auto now = QtCore.QDate.fromString(QString::fromStdString(patientDate));
    //  dateTimeEdit.setDate(now);

    //  QString time_format = "yyyy-MM-dd  HH:mm:ss";
    //  QDateTime a = QDateTime::currentDateTime();
    //  QString as = a.toString(time_format);

    dock->update();
}

void startGyroAnalysis::loadData()
{


    mainwindow->edfheaderlist[1]->viewtime += 10000;

    mainwindow->edfheaderlist[2]->viewtime += 10000000;

    //mainwindow->setup_viewbuf();

    printf("%s\n","synctest");
    //todo temp sync test

}

void startGyroAnalysis::printGraphs()
{

    strlcpy(pyfile,mainwindow->UI_import_data->pyfilefin,strlen(mainwindow->UI_import_data->pyfilefin)+1);
    strlcpy(pypath,mainwindow->UI_import_data->pypathFin,strlen(mainwindow->UI_import_data->pypathFin)+1);

    //read through margots code these days and figure out the formating
    //these super ghetto methods need to be cleaned up
    //https://www.tutorialspoint.com/cplusplus/cpp_files_streams.htm - to figure out how to formating needs to be done


    //then find a way to read in

    //std::string thisDir= qApp->applicationDirPath();

    std::string filepy(pypath);

    if(strlen(pypath)<2)
    {
        //std::string saveFileTxt(fullSavePath);
        //std::string saveFileTxtwin = std::string(" ") + " \""+saveFileTxt+".txt"+"\"";
       // saveFileTxt = std::string(" ") + saveFileTxt;

       //int x = strlen(pyfileName)+strlen(fullSavePath);

      // char tem = '  ';

       std::string macpath = std::string(pyfileName) + " " + std::string(fullSavePath);
       printf("%s\n",macpath.c_str());

       std::string winpath = " \""+ std::string(pyfileName) +"\" \""+ std::string(fullSavePath) +"\"";
        printf("%s,\n",winpath.c_str());
      //  std::string winpy(pyfileName);
      //  winpy = " \"" + winpy + "\" ";
      //  printf("%s\n",winpy.c_str());
     //   printf("%s\n",saveFileTxtwin.c_str());
   //     strlcat(pyfileName,&tem,strlen(pyfileName)+1);
    //    strlcat(pyfileName,fullSavePath,x+1);

     //   std::string filename(pyfileName);

       // std::string tempfilename = filename;
       // int spacecount;
      /*  for (int i = 0; i < filename.length(); i++)
        {
            if(filename.at(i)=='p')
            {
                if(filename.at(i+1)=='y')
                {
                    filename[i+2] = ' ';
                    for (int i = spacecount; i < filename.length(); i++)
                    {
                        filename[i+3] =tempfilename[i+3];
                    }
                    printf("%s\n",filename.c_str());
                    break;
                }

            }
           spacecount++;



        }
        */
#if defined (Q_OS_MAC)
        std::string command = "python3 ";//use python 3 instead of python.................
        command += macpath;//this right here is key+
    //
        system(command.c_str());

#elif defined (Q_OS_WIN32)
        std::string commands;
        if(pyfile[strlen(pyfile)] == 'x' || pyfile[strlen(pyfile)] == 'e')
                {
                commands = "";
                }
            else
                {
                commands = "python ";
                }
          commands += winpath;

          system(commands.c_str());
#endif
    }
    else
        {
            get_directory_from_path(recent_opendir, mainwindow->UI_import_data->saveFile, MAX_PATH_LENGTH);

            int q = strlen(recent_opendir);
        //    strlcpy(fullSavePath,recent_opendir,q+2);
            // strlcat(fullSavePath, outputfilename, p+q +2);

            std::string saveFileTxt(recent_opendir);

          //  saveFileTxt = std::string(" ") + saveFileTxt;

#if defined (Q_OS_MAC)

        std::string filename = (filepy + "/");

        filename += pyfile;//find a way to run margots code on my own machine

        //std::string filename = "\"C:/Users/Margot Heijmans/windostest3/graphs.py\"";

        //std::string command = "python ";
        //need to import boost::filesystem library to make this work on windows
        // std::string filename = "/Users/Sevi_Pro/Desktop/seviEDFbrow/edfbrowser_170_source/cmake-build-debug/graphs.py";
        std::string command = "python3 ";//use python 3 instead of python.................

        command += filename;        //this right here is key+
        command += " " + saveFileTxt;
        system(command.c_str());

#elif defined (Q_OS_WIN32)
          std::string filenames = (filepy + "\\");
          filenames += pyfile;//find a way to run margots code on my own machine
            std::string commands;
            if(pyfile[strlen(pyfile)] == 'x' ||pyfile[strlen(pyfile)] == 'e')
                {
                commands = "";
                }
            else
                {
                commands = "python ";
                }
          filenames = "\"" + filenames + "\"";



          commands += filenames;
          commands += " \"" + saveFileTxt + "\"";
          system(commands.c_str());


#endif



        //char* string
        //snprintf(string, 128, "delta %+f", mainwindow->maincurve->crosshair_2.value - mainwindow->maincurve->crosshair_1.value); //to get the difference in time between the crosshairs
        //^^not super important eitherway
        //maybe override the crosshair functions to change the colors that are being used rn

        mainwindow->Escape_fun();
    }
}

void startGyroAnalysis::printTrm()
{
    int index = combobox->currentIndex();

 //   startText->setText(QString::fromStdString(std::to_string(loadTremor.TremorS[index].EMGStart)));
  //  endText->setText(QString::fromStdString(std::to_string(loadTremor.TremorS[index].EMGEnd)));

    dock->update();

    std::string pyOut(loadTremor.pyPath1);
    std::string gryopath1(loadTremor.dataFilesgyro1);
    std::string gryopath2(loadTremor.dataFilesgyro2);
    std::string gryopath3(loadTremor.dataFilesgyro3);

    int trnum = loadTremor.TremorS[index].TremorNum;
    int starto = loadTremor.TremorS[index].StartTime;
    int endo = loadTremor.TremorS[index].EndTime;

    std::string macPath = "python3 " + pyOut + " " + gryopath1 + " " + gryopath2 + " " + gryopath3 + " " + std::to_string(trnum +1) + " " + std::to_string(starto) + " " + std::to_string(endo);
    std::string winTr = "python \""+ pyOut +"\" \""+ gryopath1 +"\" \""+ gryopath2 +"\" \""+ gryopath3 +"\" \"" + std::to_string(trnum +1) +"\" \"" + std::to_string(starto) +"\" \"" + std::to_string(endo)+"\"";
    std::string winExe = " \"" + pyOut +"\" \""+ gryopath1 +"\" \""+ gryopath2 +"\" \""+ gryopath3 +"\" \"" + std::to_string(trnum +1) +"\" \"" + std::to_string(starto) +"\" \"" + std::to_string(endo)+"\"";

    printf("%s\n",macPath.c_str());
    printf("%s\n",winExe.c_str());

   //todo exe version
   char ch = pyOut.back();
    printf("%c\n",ch);

    if(ch == 'y')
    {
        printf("%s\n","pyfile works");
    }

#if defined (Q_OS_MAC)
    system(macPath.c_str());



#elif defined (Q_OS_WIN32)

    if(ch == 'y')
    {

        system(winTr.c_str());
    }
    else if(ch == 'e')
    {
        system(winExe.c_str());
    }



    STARTUPINFOA startup_info = { 0 };
    LPSTARTUPINFOA p_startup_info = &startup_info;
    PROCESS_INFORMATION proc_info = { 0 };
    LPPROCESS_INFORMATION p_proc_info = &proc_info;

    bool process_created = CreateProcess(
        NULL,
        command_line,
        NULL,
        NULL,
        FALSE,
        DETACHED_PROCESS,
        NULL,
        NULL,
        p_startup_info,
        p_proc_info
    );

    if (!process_created) { return -3; }

    DWORD process_exit;

    WaitForSingleObject(proc_info.hThread, INFINITE);

    GetExitCodeProcess(p_proc_info->hProcess, &process_exit);

    return (int)process_exit;


#endif

}
void startGyroAnalysis::save()
{
    std::ofstream myfile;
    myfile.open (loadTremor.saveFileName, std::ios::out|std::ios::trunc);
    myfile.close();

    FILE *outfile;

    // open file for writing
    outfile = fopen (loadTremor.saveFileName, "wb");

    // write struct to file
    fwrite (&loadTremor, sizeof(struct TremorData), 1, outfile);
    //  fwrite (&input2, sizeof(struct person), 1, outfile);

    if(fwrite != 0)
        printf("contents to file written successfully !\n");
    else
        printf("error writing file !\n");

    // close file
    fclose (outfile);
}
void startGyroAnalysis::backToCrosshairs()
{
    if (mainwindow->UI_import_data->loaded == true)
    {
        mainwindow->creategraphs =  new createGraphs(mainwindow);
       // ParseStructs();
        mainwindow->UI_import_data->loaded = false;
    }
    else
    //mainwindow->creategraphs =  new createGraphs(mainwindow);
    mainwindow->creategraphs->dock->show();
    combobox->clear();//different dock than this dock - should rename
    dock->hide();
}


startGyroAnalysis::~startGyroAnalysis()
{
    dock->close();
    delete dock;
}

