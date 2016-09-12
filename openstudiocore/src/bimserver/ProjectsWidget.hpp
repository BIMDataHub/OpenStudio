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

#ifndef BIMSERVER_PROJECTSWIDGET_HPP
#define BIMSERVER_PROJECTSWIDGET_HPP

#include "../model/Model.hpp"
#include "../shared_gui_components/OSDialog.hpp"
#include "../openstudio_lib/MainTabView.hpp"
#include "../openstudio_lib/MainTabController.hpp"

#include <QMap>
#include <QDialog>
#include <QLabel>
#include <QKeyEvent>
#include <QSettings>
#include <QStatusBar>
#include <QEventLoop>
#include <QCloseEvent>
#include <QListWidget>

namespace openstudio {
namespace bimserver {

class ProjectsWidget : public QWidget
{
    Q_OBJECT
    public:
  
    ProjectsWidget(QWidget * parent = nullptr);
    ~ProjectsWidget() {}
    bool nameConflict(QString name);

    public slots:

    void clearList();
    void processProjectList(QStringList PList);

    signals:

    void nextTab(int index);
    void updated(QString new_proID);
    void newproject(QString new_proString);
    void rmvproject(QString new_proString);
    //void rnmproject(QString new_proString);

    private:

    QMap<QString, QString> *m_proMap;
    QListWidget *m_proList;
    QPushButton *newButton;
    QPushButton *rmvButton;
    QPushButton *nextButton;
    //QPushButton *renameButton;
    
    private slots:

    void newButton_clicked();
    void rmvButton_clicked();
    void nextButton_clicked();
    //void renameButton_clicked();
    void DoubleClicked(QListWidgetItem *);
    void SingleClicked(QListWidgetItem *);

  };

} // bimserver
} // openstudio

#endif 
//BIMSERVER_PROJECTSWIDGET_HPP
