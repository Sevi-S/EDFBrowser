/*
***************************************************************************
*
* Author: Teunis van Beelen
*
* Copyright (C) 2010 - 2020 Teunis van Beelen
*
* Email: teuniz@protonmail.com
*
***************************************************************************
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, version 3 of the License.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
*
***************************************************************************
*/


#ifndef UI_NEXFIN2EDFFORM_H
#define UI_NEXFIN2EDFFORM_H


#include <QtGlobal>
#include <QApplication>
#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QDateTimeEdit>
#include <QPushButton>
#include <QObject>
#include <QFileDialog>
#include <QRadioButton>
#include <QComboBox>
#include <QCursor>
#include <QMessageBox>
#include <QString>
#include <QPixmap>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "global.h"
#include "utils.h"


#define MAX_SIGNALS 32



class UI_NEXFIN2EDFwindow : public QObject
{
  Q_OBJECT

public:
  UI_NEXFIN2EDFwindow(char *recent_dir=NULL, char *save_dir=NULL);

private:

QDialog       *myobjectDialog;

QPushButton   *pushButton1,
              *pushButton2;

QLabel        *PatientnameLabel,
              *RecordingLabel,
              *DatetimeLabel;

QLineEdit     *PatientnameLineEdit,
              *RecordingLineEdit;

QDateTimeEdit *StartDatetimeedit;

QRadioButton  *radio100button,
              *radio103button,
              *radio032button;

QComboBox     *analoginputscombobox;

int file_type;

char *recent_opendir,
     *recent_savedir;

void enable_widgets(bool);


private slots:

void SelectFileButton();
void radiobuttonchecked(bool);

};




#endif


