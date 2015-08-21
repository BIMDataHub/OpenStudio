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
#include <QMessageBox>

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

    m_newButton = new QPushButton(tr("New IFC File"), this);
    connect(m_newButton, SIGNAL(clicked()),this, SLOT(newButton_clicked()));
    m_okButton = new QPushButton(tr("Okay"), this);
    connect(m_okButton, SIGNAL(clicked()), this, SLOT(okButton_clicked()));

    fileLayout->addWidget(m_ifcList,0,0,1,2);
    fileLayout->addWidget(m_newButton,1,0,1,1); 
    fileLayout->addWidget(m_okButton,1,1,1,1); 
  }

  void FilesWidget::newButton_clicked() 
  {
    QString new_ifcString = QFileDialog::getOpenFileName(this,
    tr("Open IFC File"), ".",
    tr("IFC files (*.ifc)"));

    if (!new_ifcString.isEmpty()) {
      emit newfile(new_ifcString);
    } else {
      QMessageBox messageBox(this);
      messageBox.setText(tr("New IFC File Failed")); 
      messageBox.exec();
    }
  }

  void FilesWidget::okButton_clicked() 
  {
    if (m_ifcList->currentItem()) {
      QString m_ifcID = m_ifcList->currentItem()->text().section(":", 0, 0);
      emit updated(m_ifcID);
    } else {
      QMessageBox messageBox(this);
      messageBox.setText(tr("Select IFC File First")); 
      messageBox.exec();
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
