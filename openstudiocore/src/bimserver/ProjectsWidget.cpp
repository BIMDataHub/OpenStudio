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
#include "ProjectsWidget.hpp"

#include <QGridLayout>
#include <QDialog>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>

namespace openstudio {
namespace bimserver {

  //ProjectsWidget::ProjectsWidget(const model::Model & model, QWidget * parent)
  ProjectsWidget::ProjectsWidget(QWidget * parent)
    : QWidget(parent)
      //m_model(model)
  {
    // MainLayout
    auto projectLayout = new QGridLayout;
    setLayout(projectLayout);

    m_proList = new QListWidget(this);

    QPushButton *newButton = new QPushButton(tr("New Project"), this);
    connect(newButton, SIGNAL(clicked()),this, SLOT(newButton_clicked()));
    QPushButton *selectButton = new QPushButton(tr("Select"), this);
    connect(selectButton, SIGNAL(clicked()), this, SLOT(selectButton_clicked()));

    projectLayout->addWidget(m_proList,0,0,1,2);
    projectLayout->addWidget(newButton,1,0,1,1); 
    projectLayout->addWidget(selectButton,1,1,1,1);  
  }

  void ProjectsWidget::newButton_clicked() 
  {
    QDialog newDialog(this);  
    QGridLayout newLayout;
    newDialog.setWindowTitle(tr("New Project"));
    newDialog.setLayout(&newLayout);

    QLabel new_introLabel(tr("Please enter the project name: "), this);
    QLabel new_nameLabel(tr("Project Name:"), this);
    QLineEdit new_nameEdit;
    QPushButton new_okButton(tr("Create Project"), this);
    QPushButton new_cancelButton(tr("Cancel"), this);

    newLayout.addWidget(&new_introLabel,0,0,1,2);
    newLayout.addWidget(&new_nameLabel,1,0,1,1);
    newLayout.addWidget(&new_nameEdit,1,1,1,1);
    newLayout.addWidget(&new_okButton,2,0,1,1);
    newLayout.addWidget(&new_cancelButton,2,1,1,1);

    connect(&new_okButton, SIGNAL(clicked()), &newDialog, SLOT(accept()));
    connect(&new_cancelButton, SIGNAL(clicked()), &newDialog, SLOT(reject()));

    if (newDialog.exec()==QDialog::Accepted) {
      QString new_proString = new_nameEdit.text();
      emit newproject(new_proString);
    } else {
      QMessageBox messageBox(this);
      messageBox.setText(tr("New Project Not Created")); 
      messageBox.exec();
    }
  }

  void ProjectsWidget::selectButton_clicked() 
  {
    if (m_proList->currentItem()) {
      QString m_proID = m_proList->currentItem()->text().section(":", 0, 0);
      emit updated(m_proID);
      } else {
      QMessageBox messageBox(this);
      messageBox.setText(tr("Select Project First")); 
      messageBox.exec();
    }
  }

  void ProjectsWidget::clearList()
  {
    m_proList->clear();
  }

  void ProjectsWidget::processProjectList(QStringList pList) 
  {
    m_proList->clear();
    foreach(QString itm, pList) {
      m_proList->addItem(itm);
    }
  }

} // bimserver
} // openstudio
