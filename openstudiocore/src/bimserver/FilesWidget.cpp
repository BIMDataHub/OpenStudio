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
#include "FilesWidget.hpp"

#include <QLineEdit>
#include <QGridLayout>
#include <QListWidget>
#include <QPushButton>
#include <QFileDialog>

namespace openstudio {
namespace bimserver {

//FilesWidget::FilesWidget(const model::Model & model, QWidget * parent)
  FilesWidget::FilesWidget(QWidget * parent)  
    : QWidget(parent)
    //m_model(model)
  {
  // MainLayout
    auto fileLayout = new QGridLayout;
    setLayout(fileLayout);

    m_ifcList = new QListWidget(this);

    QPushButton *newButton = new QPushButton(tr("New IFC File"), this);
    connect(newButton, SIGNAL(clicked()),this, SLOT(newButton_clicked()));
    QPushButton *okButton = new QPushButton(tr("Okay"), this);
    connect(okButton, SIGNAL(clicked()), this, SLOT(okButton_clicked()));

    fileLayout->addWidget(m_ifcList,0,0,1,2);
    fileLayout->addWidget(newButton,1,0,1,1); 
    fileLayout->addWidget(okButton,1,1,1,1); 
  }

  void FilesWidget::newButton_clicked() 
  {
    //if (m_proList->currentItem()) {
      //QString m_proID = m_proList->currentItem()->text().section(":", 0, 0);
      QString new_ifcString = QFileDialog::getOpenFileName(this,
      tr("Open IFC File"), ".",
      tr("IFC files (*.ifc)"));

      if (!new_ifcString.isEmpty()) {
        //m_statusBar->showMessage("IFC File " + new_ifcString + " loaded.", 2000);
        emit newfile(new_ifcString);
        //m_bimserverConnection->checkInIFCFile(m_proID, new_ifcString);
      }
    //}  else {
      //m_statusBar->showMessage(tr("Please select a project to check in a new IFC version."), 2000);
    //}
  }

  void FilesWidget::okButton_clicked() 
  {
    if (m_ifcList->currentItem()) {
      QString m_ifcID = m_ifcList->currentItem()->text().section(":", 0, 0);

      emit updated(m_ifcID);
      //m_bimserverConnection->download(m_ifcID);

    } else {
      //m_statusBar->showMessage(tr("Please select a IFC version before proceeding."), 2000);
    }
  }

  void FilesWidget::processIFCList(QStringList iList) 
  {
    m_ifcList->clear();

    foreach(QString itm, iList) {
      m_ifcList->addItem(itm);
    }

  }

  void FilesWidget::clearList()
  {
    m_ifcList->clear();
  }

} // bimserver
} // openstudio
