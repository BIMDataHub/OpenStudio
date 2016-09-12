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

namespace openstudio {
namespace bimserver{

  IFCTabController::IFCTabController(bool isIP)
    : MainTabController(new IFCTabView()),
    m_settingsWidget(new SettingsWidget()),
    m_projectsWidget(new ProjectsWidget()),
    m_filesWidget(new FilesWidget())
  {
    /*
    m_settingsWidget = new SettingsWidget();
    addQObject(m_settingsWidget);
    this->mainContentWidget()->addSubTab("Settings", SETTINGS);
    this->mainContentWidget()->selectSubTabByIndex(SETTINGS);
    this->mainContentWidget()->setSubTab(m_settingsWidget);

    m_projectsWidget = new ProjectsWidget();
    addQObject(m_projectsWidget);
    this->mainContentWidget()->addSubTab("Projects", PROJECTS);
    this->mainContentWidget()->selectSubTabByIndex(PROJECTS);
    this->mainContentWidget()->setSubTab(m_projectsWidget);

    m_filesWidget = new FilesWidget();
    addQObject(m_filesWidget);
    this->mainContentWidget()->addSubTab("Files", FILES);
    this->mainContentWidget()->selectSubTabByIndex(FILES);
    this->mainContentWidget()->setSubTab(m_filesWidget);

    m_importWidget = new ImportWidget();
    addQObject(m_importWidget);
    this->mainContentWidget()->addSubTab("Import", m_importWidget, IMPORT); 
    */

    mainContentWidget()->addSubTab("Settings", SETTINGS);
    mainContentWidget()->addSubTab("Projects", PROJECTS);
    mainContentWidget()->addSubTab("Files", FILES);
    setSubTab(0);

    connect(this->mainContentWidget(), &MainTabView::tabSelected, this, &IFCTabController::setSubTab);

  }

  IFCTabController::~IFCTabController()
  {
    disconnect(this->mainContentWidget(), &MainTabView::tabSelected, this, &IFCTabController::setSubTab);
  }

  void IFCTabController::setSubTab(int index)
  {
    if (m_currentIndex == index) {
      return;
    }
    else {
      m_currentIndex = index;
    }

  if (qobject_cast<SettingsWidget *>(m_currentView)) {
    //disconnect(this, &IFCTabController::toggleUnitsClicked, qobject_cast<BuildingInspectorView *>(m_currentView), &BuildingInspectorView::toggleUnitsClicked);
    //disconnect(qobject_cast<BuildingInspectorView *>(m_currentView), &BuildingInspectorView::dropZoneItemClicked, this, &IFCTabController::dropZoneItemClicked);
  }
  else if (qobject_cast<ProjectsWidget *>(m_currentView)) {
    //disconnect(this, &IFCTabController::toggleUnitsClicked, qobject_cast<FacilityStoriesGridView *>(m_currentView), &FacilityStoriesGridView::toggleUnitsClicked);
    //disconnect(qobject_cast<FacilityStoriesGridView *>(m_currentView), &FacilityStoriesGridView::dropZoneItemSelected, this, &IFCTabController::dropZoneItemSelected);
  }
  else if (qobject_cast<FilesWidget *>(m_currentView)) {
    //disconnect(this, &IFCTabController::toggleUnitsClicked, qobject_cast<FacilityShadingGridView *>(m_currentView), &FacilityShadingGridView::toggleUnitsClicked);
    //disconnect(qobject_cast<FacilityShadingGridView *>(m_currentView), &FacilityShadingGridView::dropZoneItemSelected, this, &IFCTabController::dropZoneItemSelected);
  }
  else if (m_currentView) {
    // Oops! Should never get here
    OS_ASSERT(false);
  }

  switch (index){
  case 0:
  {
    //m_settingsWidget = new SettingsWidget();
    //addQObject(m_settingsWidget);
    //connect(this, &FacilityTabController::toggleUnitsClicked, buildingInspectorView, &BuildingInspectorView::toggleUnitsClicked);
    //connect(buildingInspectorView, &BuildingInspectorView::dropZoneItemClicked, this, &FacilityTabController::dropZoneItemClicked);
    this->mainContentWidget()->setSubTab(m_settingsWidget);
    m_currentView = m_settingsWidget;
    break;
  }
  case 1:
  {
    //m_projectsWidget = new ProjectsWidget();
    //addQObject(m_projectsWidget);
    //connect(this, &FacilityTabController::toggleUnitsClicked, facilityStoriesGridView, &FacilityStoriesGridView::toggleUnitsClicked);
    //connect(facilityStoriesGridView, &FacilityStoriesGridView::dropZoneItemSelected, this, &FacilityTabController::dropZoneItemSelected);
    this->mainContentWidget()->setSubTab(m_projectsWidget);
    m_currentView = m_projectsWidget;
    break;
  }
  case 2:
  {
    //m_filesWidget = new FilesWidget();
    //addQObject(m_filesWidget);
    //connect(this, &FacilityTabController::toggleUnitsClicked, facilityShadingGridView, &FacilityShadingGridView::toggleUnitsClicked);
    //connect(facilityShadingGridView, &FacilityShadingGridView::dropZoneItemSelected, this, &FacilityTabController::dropZoneItemSelected);
    this->mainContentWidget()->setSubTab(m_filesWidget);
    m_currentView = m_filesWidget;
    break;
  }
  default:
    OS_ASSERT(false);
    break;
  }
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
