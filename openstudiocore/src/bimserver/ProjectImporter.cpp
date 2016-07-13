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

#include "ProjectImporter.hpp"

#include "IFCTabController.hpp"
#include "BIMserverConnection.hpp"

#include "../osversion/VersionTranslator.hpp"

#include <QLabel>
#include <QLineEdit>
#include <QEventLoop>
#include <QFileDialog>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QMessageBox>
#include <QPushButton>

namespace openstudio {
namespace bimserver {

  ProjectImporter::ProjectImporter(QWidget *parent) :
    OSDialog(parent)
  {

    #ifdef Q_OS_MAC
      setWindowFlags(Qt::WindowCloseButtonHint);
    #else
      setWindowFlags(Qt::WindowCloseButtonHint);
    #endif

    this->okButton()->hide();
    this->cancelButton()->hide();
    this->setLayoutContentsMargins(QMargins(0,50,0,0));

    m_IFCTabController = new IFCTabController(true);
    this->upperLayout()->addWidget(m_IFCTabController->mainContentWidget());

    m_bimserverConnection = nullptr;
    m_waitForOSM = new QEventLoop(this);

    m_settingsWidget = m_IFCTabController->m_settingsWidget;
    m_projectsWidget = m_IFCTabController->m_projectsWidget;
    m_filesWidget = m_IFCTabController->m_filesWidget;
    
    connect(m_settingsWidget, &SettingsWidget::reset,       m_projectsWidget, &ProjectsWidget::clearList);
    connect(m_settingsWidget, &SettingsWidget::reset,       m_filesWidget, &FilesWidget::clearList);
    connect(m_settingsWidget, &SettingsWidget::updated,     this, &ProjectImporter::processSettings);
    connect(m_settingsWidget, &SettingsWidget::nextTab,     this, &ProjectImporter::nextTo);
    
    connect(m_projectsWidget, &ProjectsWidget::newproject,  this, &ProjectImporter::newProject);
    connect(m_projectsWidget, &ProjectsWidget::rmvproject,  this, &ProjectImporter::rmvProject);
    connect(m_projectsWidget, &ProjectsWidget::updated,     this, &ProjectImporter::resetProID); 
    connect(m_projectsWidget, &ProjectsWidget::nextTab,     this, &ProjectImporter::nextTo);

    connect(m_filesWidget,    &FilesWidget::newfile,        this, &ProjectImporter::newFile);
    connect(m_filesWidget,    &FilesWidget::updated,        this, &ProjectImporter::resetIFCID);
    connect(m_filesWidget,    &FilesWidget::nextTab,        this, &ProjectImporter::nextTo);

    connect(this, &ProjectImporter::loginFailure,           m_settingsWidget, &SettingsWidget::failedStatus);
    connect(this, &ProjectImporter::loginSuccess,           m_settingsWidget, &SettingsWidget::successStatus);
    
    this->show();
  }

  ProjectImporter::~ProjectImporter()
  {
  }

  boost::optional<model::Model> ProjectImporter::run()
  {

    //execute event loop
    m_waitForOSM->exec();

    //Reverse Translate from osmString.
    if (!m_OSM.isEmpty()) {
      std::stringstream stringStream(m_OSM.toStdString());
      openstudio::osversion::VersionTranslator vt;
      return vt.loadModel(stringStream);
    } else {
      return boost::none;
    }
  }

  void ProjectImporter::closeEvent(QCloseEvent *event) {
    emit finished();
    event->accept();
  }

  void ProjectImporter::keyPressEvent(QKeyEvent *event) {
    if (event->key() == Qt::Key_Escape) {
      emit finished();
      event->accept();
    } else {
      event->ignore();
    }
  }

  void ProjectImporter::processSucessCases(QString sucessCase)
  {
    if (sucessCase == "createProject") {
      m_bimserverConnection->getAllProjects();
    } else if (sucessCase == "deleteProject") {
      m_bimserverConnection->getAllProjects();
      m_bimserverConnection->getIFCRevisionList(m_proID);
    } else if (sucessCase == "checkInIFC") {
      sleep(10);
      m_bimserverConnection->getIFCRevisionList(m_proID);
      QMessageBox checkBox(this);
      checkBox.setText(tr("IFC File Uploaded"));
      checkBox.exec();
      //this->show();
    } else if (sucessCase == "login") {
      m_bimserverConnection->getAllProjects();
      emit loginSuccess();
    }
  }

  void ProjectImporter::processFailureCases(QString failureCase)
  {
    QMessageBox failBox(this);
    failBox.setText(tr("BIMserver Failed"));
    failBox.setDetailedText(failureCase);
    failBox.exec();
    //this->show();
  }

  void ProjectImporter::processBIMserverErrors() {
    emit loginFailure();
    QMessageBox loginBox(this);
    loginBox.setText(tr("Login Failed"));
    loginBox.setDetailedText(tr("Please double check your credentials"));
    loginBox.exec();
    //this->show();
  }

  void ProjectImporter::processSettings(QSettings *m_settings)
  {
    QString addr, port, usrname, psw;

    if (m_settings->contains("addr")) {
      addr = m_settings->value("addr").toString();
    }

    if (m_settings->contains("port")) {
      port = m_settings->value("port").toString();
    }

    if (m_settings->contains("usrname")) {
      usrname = m_settings->value("usrname").toString();
    }

    if (m_settings->contains("psw")) {
      psw = m_settings->value("psw").toString();
    }

    m_bimserverConnection = new BIMserverConnection(this, addr, port);
    connect(m_bimserverConnection, &BIMserverConnection::osmStringRetrieved, this, &ProjectImporter::processOSMRetrieved);
    connect(m_bimserverConnection, &BIMserverConnection::listAllProjects, m_projectsWidget, &ProjectsWidget::processProjectList);
    connect(m_bimserverConnection, &BIMserverConnection::listAllIFCRevisions, m_filesWidget, &FilesWidget::processIFCList);
    connect(m_bimserverConnection, &BIMserverConnection::operationSucceeded, this, &ProjectImporter::processSucessCases);
    connect(m_bimserverConnection, &BIMserverConnection::errorOccured, this, &ProjectImporter::processFailureCases);
    connect(m_bimserverConnection, &BIMserverConnection::bimserverError, this, &ProjectImporter::processBIMserverErrors);
    connect(this, SIGNAL(finished()), m_waitForOSM, SLOT(quit()));
    m_bimserverConnection->login(usrname, psw);
  }

  void ProjectImporter::processOSMRetrieved(QString osmString)
  {
    m_OSM = osmString;
    emit finished();
  }

  void ProjectImporter::justQuit()
  {
    emit finished();
  }

  void ProjectImporter::newProject(QString newID)
  {
    m_bimserverConnection->createProject(newID);
  }

  void ProjectImporter::rmvProject(QString newID)
  {
    m_bimserverConnection->deleteProject(newID);
  }

  void ProjectImporter::newFile(QString newID)
  {
    m_bimserverConnection->checkInIFCFile(m_proID, newID);
  }

  void ProjectImporter::resetProID(QString newID)
  {
    m_proID = newID;
    m_bimserverConnection->getIFCRevisionList(m_proID);
  }

  void ProjectImporter::resetIFCID(QString newID)
  {
    m_ifcID = newID;
    m_bimserverConnection->download(m_ifcID);
  }

  void ProjectImporter::nextTo(int index)
  {
    m_IFCTabController->mainContentWidget()->selectSubTabByIndex(index);
  }

} // bimserver
} // openstudio
