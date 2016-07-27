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
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QListWidget>
#include <QPushButton>
#include <QFileDialog>
#include <QMessageBox>
#include <QFont>
#include <QMap>

#include "../shared_gui_components/Buttons.hpp"

namespace openstudio {
namespace bimserver {

  FilesWidget::FilesWidget(QWidget * parent)
    : QWidget(parent)
  {
    // MainLayout
    auto fileLayout = new QVBoxLayout;
    auto btmFileLayout = new QHBoxLayout;
    setLayout(fileLayout);

    QFont f( "Arial", 15);

    m_ifcList = new QListWidget(this);
    m_ifcMap = new QMap<int, QString>;

    m_newButton = new QPushButton(tr("Upload File"), this);
    connect(m_newButton, SIGNAL(clicked()),this, SLOT(newButton_clicked()));
    m_okButton = new QPushButton(tr("Import"), this);
    connect(m_okButton, SIGNAL(clicked()), this, SLOT(okButton_clicked()));

    m_ifcList->setFont(f);
    m_newButton->setFont(f);
    m_okButton->setFont(f);
    m_okButton->setEnabled(false);
    m_newButton->setEnabled(false);

    connect(m_ifcList, SIGNAL(itemDoubleClicked(QListWidgetItem *)), this, SLOT(Doubleclicked(QListWidgetItem *)));
    connect(m_ifcList, SIGNAL(itemClicked(QListWidgetItem *)), this, SLOT(Singleclicked(QListWidgetItem *)));

    fileLayout->addWidget(m_ifcList,1);
    btmFileLayout->addSpacing(10);
    btmFileLayout->addWidget(m_newButton,1,Qt::AlignLeft);
    btmFileLayout->addSpacing(100);
    btmFileLayout->addWidget(m_okButton,1,Qt::AlignRight);
    btmFileLayout->addSpacing(10);
    fileLayout->addLayout(btmFileLayout,1);
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
      messageBox.setText(tr("Illegal IFC File Input"));
      messageBox.exec();
    }
  }

  void FilesWidget::okButton_clicked()
  {
    if (m_ifcList->currentItem()) {
      int m_ifcVer = m_ifcList->currentItem()->text().section(":", 1, 1).toInt();
      QString m_ifcID = m_ifcMap->value(m_ifcVer);
      emit updated(m_ifcID);
    } else {
      QMessageBox messageBox(this);
      messageBox.setText(tr("Select IFC file first!"));
      messageBox.exec();
    }
  }

  void FilesWidget::processIFCList(QStringList iList)
  {
    int Version = 1;
    m_ifcList->clear();
    m_ifcMap->clear();
    foreach(QString itm, iList) {
      m_ifcMap->insert(Version, itm.section(":", 0, 0));
      m_ifcList->addItem("IFC Version:"+ QString::number(Version));
      Version ++;
    }
    m_newButton->setEnabled(true);
    m_okButton->setEnabled(true);
  }

  void FilesWidget::DoubleClicked(QListWidgetItem * listItem)
  {
    int m_ifcVer = listItem->text().section(":", 1, 1).toInt();
    QString m_ifcID = m_ifcMap->value(m_ifcVer);
    emit updated(m_ifcID); 
  }

  void FilesWidget::SingleClicked(QListWidgetItem * listItem)
  {
    //m_okButton->setEnabled(true);
  }

  void FilesWidget::clearList()
  {
    m_ifcList->clear();
  }

} // bimserver
} // openstudio
