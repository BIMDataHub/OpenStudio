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

#ifndef BIMSERVER_PROJECTIMPORTER_HPP
#define BIMSERVER_PROJECTIMPORTER_HPP

#include "BIMserverAPI.hpp"
#include "BIMserverConnection.hpp"
#include "IFCTabController.hpp"

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

class QMargins;

namespace openstudio {
namespace bimserver {

  class BIMSERVER_API ProjectImporter: public OSDialog
  {
    Q_OBJECT

    public:

    /// Default constructor
    ProjectImporter(QWidget *parent);
    /// Virtual destructor
    ~ProjectImporter();
    /// Start importing IFC workflow
    boost::optional<model::Model> run();
    /// Reimplemented the close event and guide it to the run() function
    void closeEvent(QCloseEvent *event) override;
    /// Reimplemented the key press event of ESC and guide it to the run() function
    void keyPressEvent(QKeyEvent *event) override;

    public slots:

    /// process success cases for createProject, checkInIFC, and login
    void processSucessCases(QString sucessCase);
    /// process all failure cases if BIMserver outputs an exception. Print it 
    void processFailureCases(QString failureCase);
    /// process if BIMserver is not connected.
    void processBIMserverErrors();
    void processOSMRetrieved(QString osmString);
    void processSettings(QSettings *);
    void resetProID(QString newID);
    void resetIFCID(QString newID);
    void newProject(QString newID);
    void rmvProject(QString newID);
    void newFile(QString newID);

    signals:
    /// OSM String is retrieved.
    void finished();
    /// Successful/Failed login at BIMServer
    void loginStatus(QString);

    private:

    QString     m_OSM;
    QString     m_proID;
    QString     m_ifcID;
    QSettings   *m_settings;
    QEventLoop  *m_waitForOSM;  

    IFCTabController *m_IFCTabController;  
    BIMserverConnection *m_bimserverConnection;
    
    SettingsWidget * m_settingsWidget;
    ProjectsWidget * m_projectsWidget;
    FilesWidget * m_filesWidget;
    ImportWidget * m_importWidget;
  };

} // bimserver
} // openstudio

#endif 
// BIMSERVER_PROJECTIMPORTER_HPP 
