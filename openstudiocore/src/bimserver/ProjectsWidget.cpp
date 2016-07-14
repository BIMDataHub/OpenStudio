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

#include <QMap>
#include <QFont>
#include <QLabel>
#include <QDialog>
#include <QLineEdit>
#include <QMessageBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QPushButton>
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

    m_proList = new QListWidget(this);
    m_proMap = new QMap<QString, QString>;

    m_newButton = new QPushButton(tr("Create Project"), this);    
    connect(m_newButton, SIGNAL(clicked()),this, SLOT(newButton_clicked()));
    m_rmvButton = new QPushButton(tr("Remove"), this); 
    connect(m_rmvButton, SIGNAL(clicked()),this, SLOT(rmvButton_clicked()));
    m_nextButton = new QPushButton(tr("Next Tab"), this); 
    connect(m_nextButton, SIGNAL(clicked()), this, SLOT(nextButton_clicked()));

    QFont f( "Arial", 15);
    m_proList->setFont(f);
    m_newButton->setFont(f);
    m_rmvButton->setFont(f);
    m_nextButton->setFont(f);
    m_newButton->setEnabled(false);
    m_rmvButton->setEnabled(false);
    m_nextButton->setEnabled(false);

    connect(m_proList, SIGNAL(itemDoubleClicked(QListWidgetItem *)), this, SLOT(DoubleClicked(QListWidgetItem *)));
    connect(m_proList, SIGNAL(itemClicked(QListWidgetItem *)), this, SLOT(SingleClicked(QListWidgetItem *)));

    projectLayout->addWidget(m_proList,1);
    btmProjLayout->addSpacing(10);
    btmProjLayout->addWidget(m_newButton,1,Qt::AlignLeft);
    btmProjLayout->addSpacing(10);
    btmProjLayout->addWidget(m_rmvButton,1,Qt::AlignCenter);
    btmProjLayout->addSpacing(300);
    btmProjLayout->addWidget(m_nextButton,1,Qt::AlignRight);
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
      if(nameConflict(new_proString)){
        QMessageBox confBox(this);
        confBox.setText(tr("Name Conflict, New Project Not Created")); 
        confBox.exec();
      }
      else{
        emit newproject(new_proString);
      }
    } else {
      //QMessageBox errBox(this);
      //errBox.setText(tr("Illegal Input, New Project Not Created")); 
      //errBox.exec();
    }
  }

  bool ProjectsWidget::nameConflict(QString name){
    foreach (QString key, m_proMap->keys()) {
        if (!QString::compare(key, name, Qt::CaseInsensitive))
          return true;
      }
    return false;
  }

  void ProjectsWidget::rmvButton_clicked() 
  {
    if (m_proList->currentItem()) {
        QString m_proName = m_proList->currentItem()->text();
        QString m_proID = m_proMap->value(m_proName);
        emit rmvproject(m_proID);
        QMessageBox messageBox(this);
        messageBox.setText("Project " + m_proName + " Removed"); 
        messageBox.exec();
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
    m_rmvButton->setEnabled(true);
    m_nextButton->setEnabled(true);
  }
  
  void ProjectsWidget::clearList()
  {
    m_proList->clear();
  }

  void ProjectsWidget::processProjectList(QStringList pList) 
  {
    m_proList->clear();
    m_proMap->clear();
    foreach(QString itm, pList) {
      m_proMap->insert(itm.section(":", 1, 1), itm.section(":", 0, 0));
      m_proList->addItem(itm.section(":", 1, 1));
    }
    m_newButton->setEnabled(true);
    m_rmvButton->setEnabled(false);
    m_nextButton->setEnabled(false);
  }

} // bimserver
} // openstudio
