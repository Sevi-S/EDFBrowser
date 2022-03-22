
#include <stdlib.h>
#include <stdio.h>

#include <QApplication>
#include <QObject>
#include <QPixmap>
#include <QSplashScreen>
#include <QTimer>
#include <QStyle>
#include <QStyleFactory>
#include <QPen>
#include <QColor>
#include <QFont>
#include <QEventLoop>


#include "mainwindow.h"


int main(int argc, char *argv[])
{
#if defined(_MSC_VER) || defined(_MSC_FULL_VER) || defined(_MSC_BUILD)
#error "Wrong compiler or platform!"
#endif

  /* avoid surprises! */
  if((sizeof(char)      != 1) ||
     (sizeof(short)     != 2) ||
     (sizeof(int)       != 4) ||
     (sizeof(long long) != 8) ||
     (sizeof(float)     != 4) ||
     (sizeof(double)    != 8))
  {
    fprintf(stderr, "Wrong compiler or platform!\n");
    return EXIT_FAILURE;
  }

#if defined(__LP64__)
  if(sizeof(long) != 8)
  {
    fprintf(stderr, "Wrong compiler or platform!\n");
    return EXIT_FAILURE;
  }
#else
  if(sizeof(long) != 4)
  {
    fprintf(stderr, "Wrong compiler or platform!\n");
    return EXIT_FAILURE;
  }
#endif

#if defined(__MINGW64__)
  if(sizeof(long) != 4)
  {
    fprintf(stderr, "Wrong compiler or platform!\n");
    return EXIT_FAILURE;
  }
#endif

  QApplication app(argc, argv);

//   app.setAttribute(Qt::AA_DontUseNativeMenuBar);

  QPixmap pixmap(":/images/splash.png");

  QPainter p(&pixmap);
  QFont sansFont("Noto Sans", 10);
  p.setFont(sansFont);
  p.setPen(Qt::black);
  if(!strcmp(PROGRAM_BETA_SUFFIX, ""))
  {
    p.drawText(250, 260, 300, 30, Qt::AlignLeft | Qt::TextSingleLine, "version " PROGRAM_VERSION "    " THIS_APP_BITS_W);
  }
  else
  {
    p.drawText(150, 260, 300, 30, Qt::AlignLeft | Qt::TextSingleLine, "version " PROGRAM_VERSION " " PROGRAM_BETA_SUFFIX "    " THIS_APP_BITS_W);
  }

  QSplashScreen splash(pixmap, Qt::WindowStaysOnTopHint);

  QTimer t1;
  t1.setSingleShot(true);
#if QT_VERSION >= 0x050000
  t1.setTimerType(Qt::PreciseTimer);
#endif
  QObject::connect(&t1, SIGNAL(timeout()), &splash, SLOT(close()));

  if(QCoreApplication::arguments().size()<2)
  {
    splash.show();

    t1.start(3000);

    QEventLoop evlp;
    QTimer::singleShot(100, &evlp, SLOT(quit()));
    evlp.exec();
  }

#if QT_VERSION >= 0x050000
#ifdef Q_OS_LINUX
  qApp->setStyle(QStyleFactory::create("Fusion"));
#endif
#endif
  qApp->setStyleSheet("QMessageBox { messagebox-text-interaction-flags: 5; }");

  class UI_Mainwindow MainWindow;

  return app.exec();
}





