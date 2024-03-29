/*
***************************************************************************
*
* Author: Teunis van Beelen
*
* Copyright (C) 2020 Teunis van Beelen
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



#include "hrv_dock.h"


UI_hrv_dock::UI_hrv_dock(QWidget *w_parent, struct hrv_dock_param_struct par)
{
  QLabel *ruler_label;

  QFrame *frame;

  QGridLayout *grid_layout;

  mainwindow = (UI_Mainwindow *)w_parent;

  param = par;

  is_deleted = 0;

  frame = new QFrame;
  frame->setFrameStyle(QFrame::NoFrame);
  frame->setLineWidth(0);
  frame->setMidLineWidth(0);
  frame->setContentsMargins(0, 0, 0, 0);

  hrv_curve = new hrv_curve_widget;
  hrv_curve->setMinimumHeight(100);
  hrv_curve->setMinimumWidth(100);
  hrv_curve->setContentsMargins(0, 0, 0, 0);
  hrv_curve->set_params(&param);
  hrv_curve->set_range(40, 180);

  trck_indic = new simple_tracking_indicator3;
  trck_indic->set_maximum(param.edfhdr->recording_len_sec);
  trck_indic->setContentsMargins(0, 0, 0, 0);

  srl_indic = new simple_ruler_indicator3;
  srl_indic->set_params(&param);
  srl_indic->setContentsMargins(0, 0, 0, 0);
  srl_indic->setMinimumWidth(70);
  srl_indic->set_range(40, 180);

  ruler_label = new QLabel;
  ruler_label->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
  ruler_label->setText("bpm / Time");
  ruler_label->setContentsMargins(0, 0, 0, 0);

  grid_layout = new QGridLayout(frame);
  grid_layout->addWidget(srl_indic,  0, 0);
  grid_layout->addWidget(hrv_curve, 0, 1);
  grid_layout->addWidget(ruler_label, 1, 0);
  grid_layout->addWidget(trck_indic, 1, 1);
  grid_layout->setColumnStretch(1, 100);

  hrv_dock = new QToolBar("Heart Rate", mainwindow);
  hrv_dock->setOrientation(Qt::Horizontal);
  hrv_dock->setAllowedAreas(Qt::TopToolBarArea | Qt::BottomToolBarArea);
  hrv_dock->addWidget(frame);
  hrv_dock->setAttribute(Qt::WA_DeleteOnClose, true);

  QObject::connect(mainwindow, SIGNAL(file_position_changed(long long)), this, SLOT(file_pos_changed(long long)));
  QObject::connect(mainwindow, SIGNAL(file_position_changed(long long)), this, SLOT(file_pos_changed(long long)));
  QObject::connect(hrv_dock,   SIGNAL(destroyed(QObject *)),             this, SLOT(hrv_dock_destroyed(QObject *)));

  file_pos_changed(0);
}


UI_hrv_dock::~UI_hrv_dock()
{
  if(!is_deleted)
  {
    is_deleted = 1;

    mainwindow->removeToolBar(hrv_dock);

    param.edfhdr->hrv_dock[param.instance_num] = 0;

    mainwindow->hrv_dock[param.instance_num] = NULL;
  }
}


void UI_hrv_dock::update_curve(void)
{
  hrv_curve->update();
}


void UI_hrv_dock::hrv_dock_destroyed(QObject *)
{
  if(!is_deleted)
  {
    is_deleted = 1;

    param.edfhdr->hrv_dock[param.instance_num] = 0;

    mainwindow->hrv_dock[param.instance_num] = NULL;
  }

  delete this;
}


void UI_hrv_dock::file_pos_changed(long long)
{
  trck_indic->set_position((int)((param.edfhdr->viewtime + (mainwindow->pagetime / 2)) / 10000000LL));
}


simple_tracking_indicator3::simple_tracking_indicator3(QWidget *w_parent) : QWidget(w_parent)
{
  setAttribute(Qt::WA_OpaquePaintEvent);

  setFixedHeight(16);

  pos = 0;
  max = 100;
}


void simple_tracking_indicator3::set_position(long long pos_)
{
  pos = pos_;

  update();
}


void simple_tracking_indicator3::set_maximum(long long max_)
{
  max = max_;
}


void simple_tracking_indicator3::paintEvent(QPaintEvent *)
{
  int i, w, h, pos_x, step=0;

  char str[128]={""};

  w = width();
  h = height();

  QPainter painter(this);

  painter.fillRect(0, 0, w, h, Qt::lightGray);

  painter.setPen(Qt::black);

  if(((double)w / (double)max) > 0.02)
  {
    step = 1;
  }
  else if(((double)w / (double)max) > 0.01)
    {
      step = 2;
    }
    else if(((double)w / (double)max) > 0.005)
      {
        step = 4;
      }

  if(step)
  {
    for(i=0; i<200; i+=step)
    {
      pos_x = (((double)(i * 3600) / (double)max) * (double)w) + 0.5;

      if(pos_x > w)
      {
        break;
      }

      painter.drawLine(pos_x, 0, pos_x, h);

      snprintf(str, 128, "%ih", i);

      painter.drawText(pos_x + 4, h - 2, str);
    }
  }

  draw_small_arrow(&painter, (int)((((double)pos / (double)max) * w) + 0.5), 0, 0, Qt::black);
}


void simple_tracking_indicator3::draw_small_arrow(QPainter *painter, int xpos, int ypos, int rot, QColor color)
{
  QPainterPath path;

  if(rot == 0)
  {
    path.moveTo(xpos,      ypos);
    path.lineTo(xpos - 8, ypos + 15);
    path.lineTo(xpos + 8, ypos + 15);
    path.lineTo(xpos,      ypos);

    painter->fillPath(path, color);
  }
}


simple_ruler_indicator3::simple_ruler_indicator3(QWidget *w_parent) : QWidget(w_parent)
{
  setAttribute(Qt::WA_OpaquePaintEvent);

  setFixedWidth(60);

  max_val = 180;

  min_val = 40;
}


void simple_ruler_indicator3::set_params(struct hrv_dock_param_struct *parms)
{
  param = *parms;
}


void simple_ruler_indicator3::paintEvent(QPaintEvent *)
{
  int i, w, h;

  char str[32]={""};

  double pixel_per_unit,
         offset;

  w = width();
  h = height();

  QPainter painter(this);

  painter.fillRect(0, 0, w, h, Qt::lightGray);

  painter.setPen(Qt::black);

  pixel_per_unit = (double)h / 6.0;

  offset = (double)h / 12.0;

  painter.drawLine(w - 4, offset, w - 4, h - offset);

  for(i=0; i<6; i++)
  {
    painter.drawLine(w - 4, (int)((pixel_per_unit * i) + 0.5 + offset), w - 13, (int)((pixel_per_unit * i) + 0.5 + offset));

    snprintf(str, 32, "%i", (int)(min_val + ((5 - i) * ((max_val - min_val) / 5.0)) ));

    painter.drawText(QRectF(2, (int)((pixel_per_unit * i) + 0.5 + offset) - 9, 40, 25), Qt::AlignRight | Qt::AlignHCenter, str);
  }
}


void simple_ruler_indicator3::set_range(int min, int max)
{
  max_val = max;

  min_val = min;
}


hrv_curve_widget::hrv_curve_widget(QWidget *w_parent) : QWidget(w_parent)
{
  mainwindow = NULL;

  setAttribute(Qt::WA_OpaquePaintEvent);

  bpm_min = 50;

  bpm_max = 180;

  bpm_range = bpm_max - bpm_min;
}


void hrv_curve_widget::set_params(struct hrv_dock_param_struct *parms)
{
  param = *parms;

  mainwindow = param.mainwindow;
}


void hrv_curve_widget::set_range(int min, int max)
{
  bpm_max = max;

  bpm_min = min;

  bpm_range = bpm_max - bpm_min;
}


void hrv_curve_widget::paintEvent(QPaintEvent *)
{
  int w, h, i, n,
      pos_x1=0,
      pos_x2=0,
      skip_first=1;

  double pixel_per_unit,
         pixel_per_sec,
         offset,
         bpm=0,
         old_bpm=0;

  long long hr_ival=0,
            former_onset=-1;

  char str[64];

  struct annotation_list *annot_list;

  struct annotationblock *annot;

  w = width();
  h = height();

  QPainter painter(this);
#if QT_VERSION >= 0x050000
  painter.setRenderHint(QPainter::Qt4CompatiblePainting, true);
#endif
  painter.fillRect(0, 0, w, h, Qt::lightGray);

  offset = ((double)h / 12.0);

  pixel_per_unit = ((double)(h - (2 * offset)) / (double)bpm_range);

  offset = h - offset;

  offset += bpm_min * pixel_per_unit;

  painter.setPen(Qt::red);
//  painter.setPen(QPen(QBrush(Qt::red, Qt::SolidPattern), 2, Qt::SolidLine, Qt::SquareCap, Qt::BevelJoin));

  if(mainwindow == NULL)  return;

  pixel_per_sec = (double)w / (double)(param.edfhdr->recording_len_sec);

  annot_list = &param.edfhdr->annot_list;

  n = edfplus_annotation_size(annot_list);

  for(i=0; i<n; i++)
  {
    annot = edfplus_annotation_get_item(annot_list, i);
    if(annot == NULL)
    {
      break;
    }

    strlcpy(str, annot->description, 48);
    remove_trailing_spaces(str);
    remove_leading_spaces(str);

    if(!strcmp(str, param.annot_name))
    {
      hr_ival = annot->onset - former_onset;

      bpm = (60.0 * TIME_DIMENSION) / (double)hr_ival;

      pos_x2 = (((double)(annot->onset) * pixel_per_sec) / ((double)TIME_DIMENSION)) + 0.5;

      if(skip_first || annot->hided_in_list)
      {
        skip_first = 0;
      }
      else
      {
        painter.drawLine(pos_x1, (offset - (old_bpm * pixel_per_unit)) + 0.5,
                         pos_x2, (offset - (bpm * pixel_per_unit)) + 0.5);
      }

      pos_x1 = pos_x2;

      former_onset = annot->onset;

      old_bpm = bpm;
    }
  }
}





























