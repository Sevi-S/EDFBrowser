//
// Created by Sevi Suonenlahti on 3.9.2020.
//

#include "sync_manager.h"

sync_manager::sync_manager(QWidget *w_parent) {
    QPalette palette;

    mainwindow = (UI_Mainwindow *) w_parent;
    dock = new QDockWidget("Sync the runs", w_parent);
    dock->setAllowedAreas( Qt::RightDockWidgetArea);
    dock->setFeatures(QDockWidget::DockWidgetClosable | QDockWidget::DockWidgetMovable| QDockWidget::DockWidgetFloatable);
    //Qt::LeftDockWidgetArea |
    //   dock->setAttribute(Qt::WA_TranslucentBackground);
    //  dock->setWindowFlags(Qt::FramelessWindowHint);
    dock->setMinimumHeight(250);
    dock->setMaximumHeight(250);
    dock->setMinimumWidth(400);

    auto color = mainwindow->maincurve->backgroundcolor;
    // color.setAlpha(10);
    palette.setColor(QPalette::Text, mainwindow->maincurve->text_color);
    palette.setColor(QPalette::Base, color);// try to make the window transparent
    dock->setAutoFillBackground(true);//this is the only thing that makes the background appeat at all for somereason
    dock->setPalette(palette);

    SyncDialog = new QDialog(w_parent);


    label2 = new QLabel(SyncDialog);
    label2->setGeometry(10, 10, 300, 25);
    label2->setText("Synchronize the EMG data with the Gyroscopes");

    AddButtonBack = new QPushButton(SyncDialog);
    AddButtonBack->setGeometry(10, 150, 100, 25);
    AddButtonBack->setText("Back");

    AddButtonCross = new QPushButton(SyncDialog);
    AddButtonCross->setGeometry(10, 200, 150, 25);
    AddButtonCross->setText("Synchronize");

    totOffSet1 = new QLabel(SyncDialog);
    totOffSet1->setGeometry(110, 75, 250, 25);

    label3 = new QLabel(SyncDialog);
    label3->setGeometry(10, 100, 250, 25);

    AddbuttonGyr1 = new QPushButton(SyncDialog);
    AddbuttonGyr1->setGeometry(10,40,100,25);
    AddbuttonGyr1->setText("Wearable 1");

    AddbuttonGyr2 = new QPushButton(SyncDialog);
    AddbuttonGyr2->setGeometry(10,75,100,25);
    AddbuttonGyr2->setText("Wearable 2");

    AddbuttonGyr3 = new QPushButton(SyncDialog);
    AddbuttonGyr3->setGeometry(10,110,100,25);
    AddbuttonGyr3->setText("Wearable 3");

    label4 = new QLabel(SyncDialog);
    label4->setGeometry(10, 130, 300, 25);
    //  label4->setText("Note - The blue line will be moved to the red line");
//    label4->setText("To synchronize right click the curve or label on the left side to enable crosshairs");

    QObject::connect(AddButtonBack, SIGNAL(clicked()),this, SLOT(openImporter()));
       QObject::connect(AddButtonCross, SIGNAL(clicked()), this, SLOT(doSyncStuff()));
    connect(mainwindow, SIGNAL(syncByCrosshairs()), this, SLOT(itsHappening()));//to find out every time the sync by crosshairs has been called
    //use similar method to emit signal from here to simulate click in mainwindow

    //todo make the dock not disapear

    QObject::connect(AddbuttonGyr1, SIGNAL(clicked()),this, SLOT(file1()));
    QObject::connect(AddbuttonGyr2, SIGNAL(clicked()),this, SLOT(file2()));
    QObject::connect(AddbuttonGyr3, SIGNAL(clicked()),this, SLOT(file3()));

    dock ->setWidget(SyncDialog);
    dock->show();


    clickedfile =0;
    crossHairs();

}

void sync_manager::file1()
{
    if(!mainwindow->files_open)  return;
    if(!mainwindow->signalcomps)  return;

    clickedfile = 1;
    crossHairs();
}

void sync_manager::file2()
{
    if(!mainwindow->files_open)  return;
    if(!mainwindow->signalcomps)  return;

    if(mainwindow->files_open < 3)
    {
    return;
    }
    else
        {
            clickedfile = 2;
            crossHairs();
        }

   // clickedfile = 0;
}

void sync_manager::file3()
{
    if(!mainwindow->files_open)  return;
    if(!mainwindow->signalcomps)  return;

    if(mainwindow->files_open < 4)
    {
        return;
    }
    else
    {
        clickedfile = 3;
        crossHairs();
    }
}

void sync_manager::openImporter()
{
    SyncDialog->close();
    dock->hide();
    mainwindow->UI_import_data->ImportDialog->show();
//    mainwindow->import_gyrodata->ImportDialog->show();
    //   ImportDialog->show();
    mainwindow->Escape_fun();
}

void sync_manager::itsHappening()
{
    if (mainwindow->maincurve->crosshair_1.active &&mainwindow->maincurve->crosshair_2.active)
    {

        crosshairOffset = mainwindow->maincurve->crosshair_1.edf_hdr->viewtime -
                          mainwindow->maincurve->crosshair_2.edf_hdr->viewtime;
        totalOffset = crosshairOffset - (initialOffset / 0.0000001);
        totalOffset = totalOffset * 0.0000001;

        QString printOffset2 = QString("The total offset is: %1 seconds").arg(totalOffset);
        totOffSet1->setText(printOffset2);
      //  printf("%s\n","clickedfile");
      //  printf("%f\n",totalOffset);

            if (clickedfile == 1)
            {
                printf("%i\n",clickedfile);
                mainwindow->UI_import_data->saveTremor.syncOffset1 = totalOffset;
                printf("%f\n",mainwindow->UI_import_data->saveTremor.syncOffset1);
            }
            if (clickedfile == 2)
            {
                mainwindow->UI_import_data->saveTremor.syncOffset2 = totalOffset;
                printf("%f\n",mainwindow->UI_import_data->saveTremor.syncOffset2);
            }
            if (clickedfile == 3)
            {
                mainwindow->UI_import_data->saveTremor.syncOffset3 = totalOffset;//todo
                printf("%d\n",mainwindow->UI_import_data->saveTremor.syncOffset3);
            }

       // clickedfile = 0;//just to make sure
    }
}
void sync_manager::doSyncStuff()
{
    mainwindow->sync_by_crosshairs();



    itsHappening();


    mainwindow->Escape_fun();
    clickedfile = 0;
  //  crossHairs();
}


void sync_manager::crossHairs()
{
    int x,q,emgnum;
    if(!mainwindow->files_open)  return;
    if(!mainwindow->signalcomps)  return;
    if(clickedfile==0) return;


        for (i = 0; i < mainwindow->signalcomps; i++)//get which file the EMG is
        {
           int sf = mainwindow->signalcomp[i]->edfhdr->edfparam[mainwindow->signalcomp[i]->edfsignal[0]].sf_int;
           if (sf == 256)
           {

                   emgnum = mainwindow->get_filenum(mainwindow->signalcomp[i]->edfhdr);
                   x = i;
           //        printf("%i\n",x);
                   break;
           }
        }

    int files = mainwindow->files_open;

    for (i = 0; i < mainwindow->signalcomps; i++)
    {
        if(emgnum == 0)
        {
            if (clickedfile == mainwindow->get_filenum(mainwindow->signalcomp[i]->edfhdr))
            {
                q = i;
                break;
                //break;
            }
        }
        else
            {
                if (clickedfile - 1 == mainwindow->get_filenum(mainwindow->signalcomp[i]->edfhdr))
                {
                    q = i;
                  //  q = i;
                    break;
                    //break;
                }//wearable 2, is still selecting the first
            }
    }

        mainwindow->maincurve->exec_sidemenu(x); // find  away to not be which main curve but which file->>
        mainwindow->maincurve->sidemenuButton2->click();

        mainwindow->maincurve->exec_sidemenu(q); // find  away to not be which main curve but which file->>
        mainwindow->maincurve->sidemenuButton2->click();


    if (mainwindow->maincurve->crosshair_1.active &&mainwindow->maincurve->crosshair_2.active)
    {    //important check or it will crash constantly
       // int filen = mainwindow->get_filenum(mainwindow->signalcomp[i]->edfhdr);
        if (mainwindow->viewtime_sync != VIEWTIME_USER_DEF_SYNCED) {
             // initialOffset = mainwindow->edfheaderlist[filen]->viewtime -
            //                  mainwindow->edfheaderlist[mainwindow->maincurve->crosshair_2.file_num]->viewtime;

            initialOffset = mainwindow->maincurve->crosshair_1.edf_hdr->viewtime - mainwindow->maincurve->crosshair_2.edf_hdr->viewtime;
            initialOffset = initialOffset * 0.0000001; //this is causing the crash
        }
    }
      itsHappening();
 //   mainwindow->Escape_fun();
  //  clickedfile = 0;
}

//enable the crosshairs immidiatly, then use them to find the timings aswell,
//use it to move the entire file instead of just a channel

//make a function that will press the sync button, use that to constantly track the timings of the files

sync_manager::~sync_manager()
{
    SyncDialog->close();
    mainwindow->Escape_fun();
    delete SyncDialog;
}