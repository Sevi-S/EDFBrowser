//
// Created by Sevi Suonenlahti on 3.9.2020.
//

#include "createGraphs.h"

createGraphs::createGraphs(QWidget *w_parent) {
    QPalette palette;

    counter = 0;
    mainwindow = (UI_Mainwindow *) w_parent;

    //temp stuff for now
    dock = new QDockWidget("generate the graphs for your tremors here", w_parent);
    dock->setAllowedAreas( Qt::RightDockWidgetArea);
    dock->setFeatures(QDockWidget::DockWidgetClosable | QDockWidget::DockWidgetMovable| QDockWidget::DockWidgetFloatable);
    //Qt::LeftDockWidgetArea |
    //   dock->setAttribute(Qt::WA_TranslucentBackground);
    //  dock->setWindowFlags(Qt::FramelessWindowHint);
    dock->setMinimumHeight(250);
    dock->setMaximumHeight(300);
    dock->setMinimumWidth(500);


    auto color = mainwindow->maincurve->backgroundcolor;
    // color.setAlpha(10);
    palette.setColor(QPalette::Text, mainwindow->maincurve->text_color);
    palette.setColor(QPalette::Base, color);// try to make the window transparent
    dock->setAutoFillBackground(true);//this is the only thing that makes the background appeat at all for somereason
    dock->setPalette(palette);

    // AddButtonCreate = new QPushButton(dock);
    // AddButtonCreate->setGeometry(10, 100, 100, 25);
    // AddButtonCreate->setText("Create");

    if (mainwindow->UI_import_data->loaded == true)
    {
        counter = mainwindow->startgyroanalysis->loadTremor.tremorNum;
    }
   // counter = mainwindow->startgyroanalysis->loadTremor.tremorNum;

    tremNumba = new QLabel(dock);
    tremNumba->setGeometry(10, 30, 350, 25);
    tremNumba->setText(QString::fromStdString("Number of Tremors selected = " + std::to_string(counter)));


    AddButtonCross = new QPushButton(dock);
    AddButtonCross->setGeometry(10, 150, 100, 25);
    AddButtonCross->setText("Select Area");

    AddButtonGet = new QPushButton(dock);
    AddButtonGet->setGeometry(10, 180, 100, 25);
    AddButtonGet->setText("Confirm");

    AddButtonCreate = new QPushButton(dock);
    AddButtonCreate->setGeometry(100, 180, 100, 25);
    AddButtonCreate->setText("Next");

    //  QObject::connect(AddButtonCreate,SIGNAL(clicked()),this, SLOT(testFunc()));
    QObject::connect(AddButtonCross,SIGNAL(clicked()),this, SLOT(selectArea()));
    QObject::connect(AddButtonGet,SIGNAL(clicked()),this, SLOT(getArea()));
    QObject::connect(AddButtonCreate,SIGNAL(clicked()),this, SLOT(gotToAnalysis()));
    dock->show();


    mainwindow->startgyroanalysis = new startGyroAnalysis(mainwindow);
    mainwindow->startgyroanalysis->dock->hide();
 //   if(mainwindow->UI_import_data->loaded == true)
 //   {

  //  }
  //  else
    //    {
      //      mainwindow->startgyroanalysis->convertStruct();
      //  }
    mainwindow->startgyroanalysis->convertStruct();
   // if(mainwindow->UI_import_data->saveTremor.tremorNum)


    for(int i=0; i<counter; i++)
    {
        Tremor[i].TremorNum = mainwindow->UI_import_data->saveTremor.TremorS[i].TremorNum;
        Tremor[i].StartTime = mainwindow->UI_import_data->saveTremor.TremorS[i].StartTime;
        Tremor[i].EndTime = mainwindow->UI_import_data->saveTremor.TremorS[i].EndTime;
        Tremor[i].EMGStart = mainwindow->UI_import_data->saveTremor.TremorS[i].EMGStart;
        Tremor[i].EMGEnd =  mainwindow->UI_import_data->saveTremor.TremorS[i].EMGEnd;
    }
}

void createGraphs::selectArea()
{
    int p,i,z=0;
    if(!mainwindow->files_open)  return;
    if(!mainwindow->signalcomps)  return;

    for(i=0; i<mainwindow->signalcomps; i++)
    {
        p = mainwindow->get_filenum(mainwindow->signalcomp[i]->edfhdr);
        int sf = mainwindow->signalcomp[i]->edfhdr->edfparam[mainwindow->signalcomp[i]->edfsignal[0]].sf_int;
        if (sf == 200)
        {

            mainwindow->maincurve->exec_sidemenu(z); // find  away to not be which main curve but which file->>
                mainwindow->maincurve->sidemenuButton2->click();

            mainwindow->maincurve->exec_sidemenu(z+1); //(changed from z-1 to p, so that the crosshairs will now be in the same file -> can get begingin and end data for the tremor)
                 mainwindow->maincurve->sidemenuButton2->click();
            break;
        }
        else z++;
    }
}

void createGraphs::getArea()
{
    long double pos1 = (mainwindow->maincurve->crosshair_1.time_relative)* 0.0000001;
    long double pos2 = (mainwindow->maincurve->crosshair_2.time_relative)* 0.0000001;    //THIS IS THE MAIN CROSSHAIR
    long double EmgCrossLocInTimeStr = 0.1;
    long double EmgCrossLocInTimeEnd = 0.1;
    //holy shit - we need the crosshair values for both crosshairs for both files...
    //convert emg time to gyrotime.. and get the values
    //send over pathinfo and names etc.. from import dat file

    int sf = 0;
    for(int i=0; i<mainwindow->signalcomps; i++)
    {
        sf = mainwindow->signalcomp[i]->edfhdr->edfparam[mainwindow->signalcomp[i]->edfsignal[0]].sf_int;
        int filen = mainwindow->get_filenum(mainwindow->signalcomp[i]->edfhdr);
        long double strT = 0.1;
        long double gryoViTime = 0.1;
        if (sf ==256)//hard coded 256 for EMG sample rate, will change to !200 if the sample rate gets changed
        {
            strT = (mainwindow->edfheaderlist[filen]->viewtime) * 0.0000001;//convert the view time into sample number, then add the number of samples between the original targets->profit

            if(filen == 0)
            {
                gryoViTime = (mainwindow->edfheaderlist[filen+1]->viewtime)* 0.0000001;
            }
            else if(filen <= 1)
            {
                gryoViTime = (mainwindow->edfheaderlist[filen-1]->viewtime)* 0.0000001;
            }

            long double crossHLocInTimeOnScreen = pos1 - gryoViTime;
            long long winSize = pos2 - pos1;// put in directly

            EmgCrossLocInTimeStr = strT + crossHLocInTimeOnScreen;
            EmgCrossLocInTimeEnd = EmgCrossLocInTimeStr + winSize;

            break;
        }

    }


 //   std::ofstream myfile;
 //   myfile.open (mainwindow->UI_import_data->saveFile, std::ios::out|std::ios::app);//std::ios::trunc at first, need to append not truncate tho
    //use app instead of trunc

    printf("%i\n",Tremor[counter].TremorNum);
    printf("%i\n",counter);

    counter = counter +1;

    Tremor[counter-1].TremorNum = counter-1;
    Tremor[counter-1].StartTime = pos1;
    Tremor[counter-1].EndTime = pos2;
    Tremor[counter-1].EMGStart = EmgCrossLocInTimeStr;
    Tremor[counter-1].EMGEnd = EmgCrossLocInTimeEnd;

    //mainwindow->UI_import_data->saveTremor.TremorS[counter].TremorNum = counter;//to the new struct, ignore using this hacky fucking text file

    mainwindow->Escape_fun();

  //  mainwindow->startgyroanalysis = new startGyroAnalysis(mainwindow);
   // gotToAnalysis();
   // printf("%s",mainwindow->UI_import_data->saveFile);

    //mainwindow->startgyroanalysis->convertStruct();//todo - move to own func
    //mainwindow->startgyroanalysis->ParseStructs(mainwindow->UI_import_data->saveTremor);


    tremNumba->setText(QString::fromStdString("Number of Tremors selected = " + std::to_string(counter)));
    mainwindow->UI_import_data->saveTremor.tremorNum = counter;

}
void createGraphs::gotToAnalysis()
{
    mainwindow->UI_import_data->saveTremor.tremorNum = counter;
    for(int i=0; i<counter; i++)
    {
         /*       mainwindow->startgyroanalysis->loadTremor.tremorNum = counter;
                mainwindow->startgyroanalysis->loadTremor.TremorS[i].TremorNum = Tremor[i].TremorNum;
                mainwindow->startgyroanalysis->loadTremor.TremorS[i].StartTime = Tremor[i].StartTime;
                mainwindow->startgyroanalysis->loadTremor.TremorS[i].EndTime = Tremor[i].EndTime;
                mainwindow->startgyroanalysis->loadTremor.TremorS[i].EMGStart = Tremor[i].EMGStart;
                mainwindow->startgyroanalysis->loadTremor.TremorS[i].EMGEnd = Tremor[i].EMGEnd; */

        mainwindow->UI_import_data->saveTremor.TremorS[i].TremorNum = Tremor[i].TremorNum;
        mainwindow->UI_import_data->saveTremor.TremorS[i].StartTime = Tremor[i].StartTime;
        mainwindow->UI_import_data->saveTremor.TremorS[i].EndTime = Tremor[i].EndTime;
        mainwindow->UI_import_data->saveTremor.TremorS[i].EMGStart = Tremor[i].EMGStart;
        mainwindow->UI_import_data->saveTremor.TremorS[i].EMGEnd = Tremor[i].EMGEnd;

        //todo convert to maindwindow trem?

      //  mainwindow->UI_import_data->saveTremor
    }


    mainwindow->startgyroanalysis->dock->show();
    mainwindow->startgyroanalysis->convertStruct();



    mainwindow->startgyroanalysis->ParseStructs();


    dock->close();
}


createGraphs::~createGraphs()
{
    dock->close();
    delete dock;
}