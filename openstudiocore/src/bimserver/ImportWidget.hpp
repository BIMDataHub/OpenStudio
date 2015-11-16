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

#ifndef BIMSERVER_IMPORTWIDGET_HPP
#define BIMSERVER_IMPORTWIDGET_HPP

#include "../model/Model.hpp"
#include "../shared_gui_components/OSDialog.hpp"
#include "../shared_gui_components/BusyWidget.hpp"
#include "../openstudio_lib/MainTabView.hpp"
#include "../openstudio_lib/MainTabController.hpp"

#include <QLabel>
#include <QDialog>
#include <QListWidget>
#include <QStatusBar>
#include <QSettings>
#include <QEventLoop>
#include <QCloseEvent>
#include <QKeyEvent>

namespace openstudio {
namespace bimserver {  
  class ImportWidget: public QWidget
  {
    Q_OBJECT
    public:
    
    //ImportWidget(const model::Model & model, QWidget * parent = nullptr);
    ImportWidget(QWidget * parent = nullptr);
    ~ImportWidget() {}
    
    signals:

    void finished();

    private:
    QPushButton *cancelButton; 
    QLabel *imp_impLabel; 
    BusyWidget *imp_busyWig;

    private slots:
    void app_ended(); 
  };

} // bimserver
} // openstudio

#endif 
//BIMSERVER_IMPORTWIDGET_HPP  

  
