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

#include "IFCTabController.hpp"

#include "IFCTabView.hpp"
#include "SettingsWidget.hpp"
#include "ProjectsWidget.hpp"
#include "FilesWidget.hpp"
#include "ImportWidget.hpp"

#include <QApplication>
#include <QDateTime>
#include <QDialog>
#include <QFileDialog>
#include <QString>

namespace openstudio {
namespace bimserver{

  IFCTabController::IFCTabController(bool isIP)
    : MainTabController(new IFCTabView("IFC Importer", true)),
      m_settingsWidget(nullptr),
      m_projectsWidget(nullptr),
      m_filesWidget(nullptr),
      m_importWidget(nullptr),
      m_isIP(isIP)
  {
    m_settingsWidget = new SettingsWidget();
    addQObject(m_settingsWidget);
    this->mainContentWidget()->addSubTab("Settings", m_settingsWidget, SETTINGS);

    m_projectsWidget = new ProjectsWidget();
    addQObject(m_projectsWidget);
    this->mainContentWidget()->addSubTab("Projects", m_projectsWidget, PROJECTS);

    m_filesWidget = new FilesWidget();
    addQObject(m_filesWidget);
    this->mainContentWidget()->addSubTab("Files", m_filesWidget, FILES);

    //m_importWidget = new ImportWidget();
    //addQObject(m_importWidget);
    //this->mainContentWidget()->addSubTab("Import", m_importWidget, IMPORT); 
  }

/*
  void IFCTabController::processSettings(QSettings *m_settings)
  {
    QString addr, port, usrname, psw;

    if (m_settings->contains("addr")) {
      addr = m_settings->value("addr").toString();
      //set_baddEdit->setText(addr);
    }

    if (m_settings->contains("port")) {
      port = m_settings->value("port").toString();
      //set_bportEdit->setText(port);
    }
  
    if (m_settings->contains("usrname")) {
      usrname = m_settings->value("usrname").toString();
      //set_unameEdit->setText(usrname);
    }

    if (m_settings->contains("psw")) {
      psw = m_settings->value("psw").toString();
      //set_upassEdit->setText(psw);
    }

    m_bimserverConnection = new BIMserverConnection(this, addr, port);
    m_bimserverConnection->login(usrname, psw);
  }

  void IFCTabController::processOSMRetrieved(QString osmString) 
  {
    m_OSM = osmString;
    emit finished();
  }

  void IFCTabController::resetProID(QString newID)
  {
    m_proID = newID;
    m_bimserverConnection->getIFCRevisionList(m_proID);
  }

  void IFCTabController::resetIFCID(QString newID)
  {
    m_ifcID = newID;
    m_bimserverConnection->download(m_ifcID);
  }

  void IFCTabController::newProject(QString newID)
  {
    m_bimserverConnection->createProject(newID);
    // TODO-refresh
  }

  void IFCTabController::newFile(QString newID)
  {
    m_bimserverConnection->checkInIFCFile(m_proID, newID);
    // TODO-refresh  
  }
*/
  
}
}
