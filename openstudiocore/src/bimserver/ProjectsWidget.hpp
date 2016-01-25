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

#include <QDialog>
#include <QListWidget>
#include <QStatusBar>
#include <QSettings>
#include <QEventLoop>
#include <QCloseEvent>
#include <QKeyEvent>

namespace openstudio {
namespace bimserver {

class ProjectsWidget : public QWidget
{
    Q_OBJECT
    public:
  
    //ProjectsWidget(const model::Model & model, QWidget * parent = nullptr);
    ProjectsWidget(QWidget * parent = nullptr);
    ~ProjectsWidget() {}

    public slots:

    void clearList();
    void processProjectList(QStringList PList);

    signals:

    void newproject(QString new_proString);
    void rmvproject(QString new_proString);
    void updated(QString new_proID);

    private:
    
    //model::Model m_model;

    QPushButton *newButton;
    QPushButton *rmvButton;
    QPushButton *selectButton;

    QListWidget *m_proList;
    
    private slots:

    void newButton_clicked();
    void rmvButton_clicked();
    void selectButton_clicked();

  };

} // bimserver
} // openstudio

#endif 
//BIMSERVER_PROJECTSWIDGET_HPP
