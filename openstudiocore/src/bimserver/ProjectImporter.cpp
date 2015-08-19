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

#include <QFileDialog>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QMessageBox>
#include <QLabel>
#include <QEventLoop>
#include <QLineEdit>
#include <QPushButton>

namespace openstudio {
namespace bimserver {

  ProjectImporter::ProjectImporter(QWidget *parent) :
    OSDialog(parent) 
  {

    //m_mainTabController = std::shared_ptr<MainTabController>(new IFCTabController());
    //mainLayout->addWidget(m_mainTabController->mainContentWidget());
    m_IFCTabController = new IFCTabController(true);
    this->upperLayout()->addWidget(m_IFCTabController->mainContentWidget());

    m_bimserverConnection = nullptr;
    m_waitForOSM = new QEventLoop(this);
    m_settings = m_IFCTabController->m_settings;
  }

  ProjectImporter::~ProjectImporter() 
  {
  }

  boost::optional<model::Model> ProjectImporter::run() 
  { 
    if (m_settings->contains("addr") && m_settings->contains("port") && m_settings->contains("usrname") && m_settings->contains("psw")) {
      QString addr = m_settings->value("addr").toString();
      QString port = m_settings->value("port").toString();
      QString usrname = m_settings->value("usrname").toString();
      QString psw = m_settings->value("psw").toString();

      m_bimserverConnection = new BIMserverConnection(this, addr, port);

      connect(m_bimserverConnection, &BIMserverConnection::osmStringRetrieved, m_IFCTabController, &IFCTabController::processOSMRetrieved);
      connect(m_bimserverConnection, &BIMserverConnection::listAllProjects, m_projectsWidget, &ProjectsWidget::processProjectList);
      connect(m_bimserverConnection, &BIMserverConnection::listAllIFCRevisions, m_filesWidget, &FilesWidget::processIFCList);
      connect(m_bimserverConnection, &BIMserverConnection::operationSucceeded, this, &ProjectImporter::processSucessCases);
      connect(m_bimserverConnection, &BIMserverConnection::errorOccured, this, &ProjectImporter::processFailureCases);
      connect(m_bimserverConnection, &BIMserverConnection::bimserverError, this, &ProjectImporter::processBIMserverErrors);
      connect(this, SIGNAL(finished()), m_waitForOSM, SLOT(quit()));

      m_bimserverConnection->login(usrname, psw);

    } else {
      QMessageBox messageBox(this);
      messageBox.setText(tr("BIMserver disconnected")); 
      messageBox.setDetailedText(tr("BIMserver is not connected correctly. Please check if BIMserver is running and make sure your username and password are valid.\n"));
      messageBox.exec();
      this->show();
    }

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

    } else if (sucessCase == "checkInIFC") {
      //m_bimserverConnection->getIFCRevisionList(m_proID);

    } else if (sucessCase == "login") {
      this->show();
      m_bimserverConnection->getAllProjects();
    }
  }

  void ProjectImporter::processFailureCases(QString failureCase) 
  {
    //m_statusBar->showMessage(failureCase, 2000);
  } 

  void ProjectImporter::processBIMserverErrors() {
    this->hide();
    QMessageBox messageBox(this);
    messageBox.setText(tr("BIMserver disconnected")); 
    messageBox.setDetailedText(tr("BIMserver is not connected correctly. Please check if BIMserver is running and make sure your username and password are valid.\n"));
    messageBox.exec();
  }

} // bimserver
} // openstudio
