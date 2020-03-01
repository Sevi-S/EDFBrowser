//
// Created by Sevi Suonenlahti on 20.12.2019.
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

    AddButtonCreate = new QPushButton(dock);
    AddButtonCreate->setGeometry(10, 100, 100, 25);
    AddButtonCreate->setText("Create");

    AddButtonCross = new QPushButton(dock);
    AddButtonCross->setGeometry(10, 150, 100, 25);
    AddButtonCross->setText("Select Area");

    AddButtonGet = new QPushButton(dock);
    AddButtonGet->setGeometry(10, 180, 100, 25);
    AddButtonGet->setText("Confirm");

    QObject::connect(AddButtonCreate,SIGNAL(clicked()),this, SLOT(testFunc()));
    QObject::connect(AddButtonCross,SIGNAL(clicked()),this, SLOT(selectArea()));
    QObject::connect(AddButtonGet,SIGNAL(clicked()),this, SLOT(getArea()));

    dock->show();
}

void createGraphs::testFunc()
{
        //make a document with timestamped info about the locations of the tremors in the data
        //then run margots code with that
        //need to do the actual python call at the end, but first make an interface to select where the tremors
        //how to access the annotations?


    std::ofstream myfile;
    myfile.open ("testDoc1.txt");
   // myfile << "this works right.gdgjhjh\n";        //need to figure out the formating and stuff for margots code
    myfile.close(); //move this more to the end probs


        //read through margots code these days and figure out the formating
    //these super ghetto methods need to be cleaned up
    //https://www.tutorialspoint.com/cplusplus/cpp_files_streams.htm - to figure out how to formating needs to be done

    //then find a way to read in

    //std::string filename = "/Users/Sevi_Pro/PycharmProjects/Python/untitled1/readFileTest.py";//find a way to run margots code on my own machine
    std::string filename = "C://Users//Margot Heijmans//windostest3//graphs.py";
    std::string command = "python ";
    command += filename;        //this right here is key+
    system(command.c_str());          //to actually call margots code

    //cant deal with inputs/ouputs from/to python BUT - > thats fine as I just need to run it and make all the graphs HOLYSHIT THIS WORKS
            //find a way to close the python shell - > all need to figure out how to make VM inside the c++ project
            //figure out how to read Txt in python, and format it in a way to run and change all the stuff that is needed for margots code

            //what needs to be on the computer so this program can be run by anyone -> all the c++ interpreters, compliere and librarys, + python stuff including librarys as well
            //then figure out all that stuff for mac and windows :X
            //transfer sample rate over aswell - just a straight input
            //use new lines and comma to sperate everything


            //manually reset the time ranges of the tremor -> also to round the timing of the tremor
}

void createGraphs::selectArea()
{
    int p,i,z=0;
    if(!mainwindow->files_open)  return;
    if(!mainwindow->signalcomps)  return;

    for(i=0; i<mainwindow->signalcomps; i++)
    {
        p = mainwindow->signalcomp[i]->filenum;
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
        int filen = mainwindow->signalcomp[i]->filenum;
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

    counter = counter +1;

    std::ofstream myfile;
    myfile.open ("testDoc1.txt", std::ios::out|std::ios::app);//std::ios::trunc at first, need to append not truncate tho
                                                        //use app instead of trunc

    myfile << "@"<< "Tremor number: " << counter<<"\n";
    myfile << "$"<<pos1 << ", " << pos2<<"\n";
    myfile << "!"<< EmgCrossLocInTimeStr << ", " << EmgCrossLocInTimeEnd <<"\n"; //put start time and end time here, remeber to uncomment the final print

    myfile.close();

    //read through margots code these days and figure out the formating
    //these super ghetto methods need to be cleaned up
    //https://www.tutorialspoint.com/cplusplus/cpp_files_streams.htm - to figure out how to formating needs to be done

    //then find a way to read in

    //std::string thisDir= qApp->applicationDirPath();

#if defined (Q_OS_MAC)
    QString thisDir = QDir::currentPath();

    std::string filename = thisDir.toUtf8().constData();
    //std::string filename = std::filesystem::current_path(); only on mac

    filename += "/graphs.py";//find a way to run margots code on my own machine

    //std::string filename = "\"C:/Users/Margot Heijmans/windostest3/graphs.py\"";

    //std::string command = "python "; this how i got it to work on margot's computer!
    //need to import boost::filesystem library to make this work on windows
    // std::string filename = "/Users/Sevi_Pro/Desktop/seviEDFbrow/edfbrowser_170_source/cmake-build-debug/graphs.py";
    std::string command = "python3 ";//use python 3 instead of python.................
    command += filename;        //this right here is key+
    system(command.c_str());


#elif defined (Q_OS_WIN32)
    QString thatDir = QDir::currentPath();

    std::string filenames = thatDir.toUtf8().constData();
    filenames += "/graphs.py";//find a way to run margots code on my own machine

    //std::string filename = "\"C:/Users/Margot Heijmans/windostest3/graphs.py\"";
    filenames = "\"" + filenames + "\"";
	printf("%s",filenames.c_str());

  std::string commands = "python ";
    commands += filenames;
    system(commands.c_str());
#endif



    //char* string
    //snprintf(string, 128, "delta %+f", mainwindow->maincurve->crosshair_2.value - mainwindow->maincurve->crosshair_1.value); //to get the difference in time between the crosshairs
    //^^not super important eitherway
    //maybe override the crosshair functions to change the colors that are being used rn

    mainwindow->Escape_fun();


}

createGraphs::~createGraphs()
{
    dock->close();
    delete dock;
}