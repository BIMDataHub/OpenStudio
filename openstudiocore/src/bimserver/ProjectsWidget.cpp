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

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QDialog>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>
#include <QFont>
#include <QMap>

#include "../shared_gui_components/Buttons.hpp"

namespace openstudio {
namespace bimserver {

  ProjectsWidget::ProjectsWidget(QWidget * parent)
    : QWidget(parent)
  {
    // MainLayout
    auto projectLayout = new QVBoxLayout;
    auto btmProjLayout = new QHBoxLayout;
    setLayout(projectLayout);

    QFont f( "Arial", 15);

    m_proList = new QListWidget(this);
    m_proMap = new QMap<QString, QString>;

    newButton = new QPushButton(tr("Create Project"), this);    
    connect(newButton, SIGNAL(clicked()),this, SLOT(newButton_clicked()));
    rmvButton = new QPushButton(tr("Remove Project"), this); 
    connect(rmvButton, SIGNAL(clicked()),this, SLOT(rmvButton_clicked()));
    nextButton = new QPushButton(tr("Next Tab"), this); 
    connect(nextButton, SIGNAL(clicked()), this, SLOT(nextButton_clicked()));

    m_proList->setFont(f);
    newButton->setFont(f);
    rmvButton->setFont(f);
    nextButton->setFont(f);

    newButton->setEnabled(false);
    rmvButton->setEnabled(false);
    nextButton->setEnabled(false);

    connect(m_proList, SIGNAL(itemDoubleClicked(QListWidgetItem *)), this, SLOT(DoubleClicked(QListWidgetItem *)));
    connect(m_proList, SIGNAL(itemClicked(QListWidgetItem *)), this, SLOT(SingleClicked(QListWidgetItem *)));

    projectLayout->addWidget(m_proList,1);
    btmProjLayout->addSpacing(10);
    btmProjLayout->addWidget(newButton,1,Qt::AlignLeft);
    btmProjLayout->addSpacing(10);
    btmProjLayout->addWidget(rmvButton,1,Qt::AlignCenter);
    btmProjLayout->addSpacing(300);
    btmProjLayout->addWidget(nextButton,1,Qt::AlignRight);
    btmProjLayout->addSpacing(10);  
    projectLayout->addLayout(btmProjLayout,1);
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
      messageBox.setText(tr("Illegal Input, New Project Not Created")); 
      messageBox.exec();
    }
  }

  void ProjectsWidget::rmvButton_clicked() 
  {
    if (m_proList->currentItem()) {
        QString m_proName = m_proList->currentItem()->text();
        QString m_proID = m_proMap->value(m_proName);
        emit rmvproject(m_proID);

      } else {
        QMessageBox messageBox(this);
        messageBox.setText(tr("Please select project first!")); 
        messageBox.exec();
      }
  }

  void ProjectsWidget::nextButton_clicked()
  {
    if (m_proList->currentItem()) {
      QString m_proName = m_proList->currentItem()->text();
      QString m_proID = m_proMap->value(m_proName);
      emit updated(m_proID);
      } else {
      QMessageBox messageBox(this);
      messageBox.setText(tr("Please select project first!")); 
      messageBox.exec();
    }
    emit nextTab(2);
  } 

  void ProjectsWidget::DoubleClicked(QListWidgetItem * listItem)
  {
    QString m_proName = m_proList->currentItem()->text();
    QString m_proID = m_proMap->value(m_proName);
    emit updated(m_proID); 
    emit nextTab(2);
  }

  void ProjectsWidget::SingleClicked(QListWidgetItem * listItem)
  {
    newButton->setEnabled(true);
    rmvButton->setEnabled(true);
    nextButton->setEnabled(true);
  }
  
  void ProjectsWidget::clearList()
  {
    m_proList->clear();
    newButton->setEnabled(true);
    rmvButton->setEnabled(false);
    nextButton->setEnabled(false);
  }

  void ProjectsWidget::processProjectList(QStringList pList) 
  {
    m_proList->clear();
    m_proMap->clear();
    foreach(QString itm, pList) {
      m_proMap->insert(itm.section(":", 1, 1), itm.section(":", 0, 0));
      m_proList->addItem(itm.section(":", 1, 1));
    }
    newButton->setEnabled(true);
    rmvButton->setEnabled(false);
    nextButton->setEnabled(false);
  }

} // bimserver
} // openstudio
