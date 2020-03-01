//
// Created by Sevi Suonenlahti on 27.11.2019.
//

#include "hilbertDock.h"

//there might be no point in making hilbert transfrom "scrollable"

UI_HilbertDockWindow::UI_HilbertDockWindow(QWidget *w_parent)
{
    //printf("%s \n", "hilbert transofrm should veryyyyyy happen soon");
    //create model of the window first

    //next import library that can preform the hilbert transform
    mainwindow = (UI_Mainwindow *)w_parent;

    dashboard = 0;

    vlayout1 = new QHBoxLayout;//this make the box for the list of runs atm- not needed later on

    signalcomp = NULL;

    // crashes the program -> i think it has to do with the signalcomp
    // its because these files get made/called before they are used -> and only made visible when needed


    HilbertDialog = new QDialog;

    HilbertDialog->setMinimumSize(800, 200);
    HilbertDialog->setMaximumSize(12000, 350);
    HilbertDialog->setWindowTitle("Signals");

    dock = new QDockWidget("Hilbert Transform", w_parent);

    dock->setFeatures(QDockWidget::DockWidgetClosable | QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetFloatable);
    dock->setAllowedAreas(Qt::TopDockWidgetArea | Qt::BottomDockWidgetArea);
    dock->setMinimumHeight(300);



    //more widgets

 //auto amplitudeLabel = new QLabel; //does not do anything atm for somereason
   // amplitudeLabel->setText("Amplitude");
   // amplitudeLabel->setMinimumSize(100, 15);
   // amplitudeLabel->setAlignment(Qt::AlignHCenter);

    //dock->setWidget(amplitudeLabel);

    /*
    auto vlayout2 = new QVBoxLayout;
    vlayout2->setSpacing(5);
    vlayout2->addStretch(100);
    vlayout2->addWidget(amplitudeLabel, 0, Qt::AlignHCenter);*/


}

void UI_HilbertDockWindow::init(int signal_num)
{
    printf("%s","dock has been initialized");

    setDashboard();

    int chan_num = signal_num;
    //chan_num =+ 1;
     std::string str = std::to_string(chan_num);

   // printf("%s", "    ");
    printf("%s",  str.c_str());
    printf("%s", "    is the channel number");
       // dock->show();//this right here is the key

        //need to pass chan_num to use it in the hilbert calls
        //call other functions in this function (kind of a local main)
    GetInfo();


    dock->show();
}
void UI_HilbertDockWindow::GetInfo()
{
     //from spec_dock


    printf("%s", "finding input signal");
      //get info from the input signal

    int i;

    QListWidgetItem *item;

    auto list = new QListWidget(HilbertDialog);

    for(i=0; i<mainwindow->signalcomps; i++)
    {
        item = new QListWidgetItem;
        item->setText(mainwindow->signalcomp[i]->signallabel);//this is how u get the file data (filename atm)
        item->setData(Qt::UserRole, QVariant(i));//^^just get whole list of imported files for now
        list->addItem(item);
    }
    dock->setWidget(list);


}
void UI_HilbertDockWindow::GetTimes()
{

    //get times of events based on annotations - how to read annotations
    //use channel numebrs and timescale
    //lowkey maybe still import margot's code to do both hilbert and ffts

    //big external windows with all the graphs of each moment

}


void UI_HilbertDockWindow::setDashboard()
{

    auto curve1 = new SignalCurve;//background graph "paper" itself


    if(dashboard)
    {
        dashboard = 0;
        vlayout1->removeWidget(curve1);
        dock->setWidget(curve1);
        dock->setMinimumHeight(300);
    }
    else
    {


        dashboard = 1;
        vlayout1->addWidget(curve1, 100);
        dock->setWidget(HilbertDialog);
    }

    HilbertDialog->setLayout(vlayout1);
}

void UI_HilbertDockWindow::DoTransform()
{
    //import the stream of data points


    //take fft of data --- via KISS FFT


    //separate the positive and negative --- idk how yet

    //multiply with -i and +i respectively --- should be pretty straight forward

    //join them again ----shoooould hopefully..? be straightforward

    //do inverse fft ---via KISS FFT



    //call generate graphs
}

void UI_HilbertDockWindow::GenerateGraphs()
{
    //get raw data that has been hilbert transformed, graph it nicely
    // have to figure out graphing protocol in the
}


UI_HilbertDockWindow::~UI_HilbertDockWindow()//destructor, clears memory once calls are done - this is very KEY function
{

    delete HilbertDialog;
    //need to lookinto if need more memory cleaning stuffs


}