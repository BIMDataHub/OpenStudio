/**********************************************************************
*  Copyright (c) 2008-2015, Alliance for Sustainable Energy.
*  All rights reserved.
*
*  This library is free software; you can redistribute it and/or
*  modify it under the terms of the GNU Lesser General Public
*  License as published by the Free Software Foundation; either
*  version 2.1 of the License, or (at your option) any later version.
*
*  This library is distributed in the hope that it will be useful,
*  but WITHOUT ANY WARRANTY; without even the implied warranty of
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
*  Lesser General Public License for more details.
*
*  You should have received a copy of the GNU Lesser General Public
*  License along with this library; if not, write to the Free Software
*  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
**********************************************************************/

#ifndef BIMSERVER_FILESWIDGET_HPP
#define BIMSERVER_FILESWIDGET_HPP

#include "../model/Model.hpp"
#include "../shared_gui_components/OSDialog.hpp"
#include "../openstudio_lib/MainTabView.hpp"
#include "../openstudio_lib/MainTabController.hpp"

#include <QDialog>
#include <QLabel>
#include <QListWidget>
#include <QStatusBar>
#include <QSettings>
#include <QEventLoop>
#include <QCloseEvent>
#include <QKeyEvent>
#include <QMap>

namespace openstudio {
namespace bimserver {
  
  class FilesWidget : public QWidget
  {
    Q_OBJECT
    public:
  
    FilesWidget(QWidget * parent = nullptr);
    ~FilesWidget() {}
    //bool nameConflict(QString name);

    public slots:

    void clearList();
    void processIFCList(QStringList IFCList);

    signals:
    
    void newfile(QString new_ifcString);
    void updated(QString new_ifcID);

    private:

    QPushButton *m_newButton;
    QPushButton *m_okButton;
    QListWidget *m_ifcList;
    QMap<int, QString> *m_ifcMap;

    private slots:
    
    void newButton_clicked();
    void okButton_clicked();
    void DoubleClicked(QListWidgetItem *);
    void SingleClicked(QListWidgetItem *);
    
  };

  } // bimserver
} // openstudio

#endif 
//BIMSERVER_FILESWIDGET_HPP 
