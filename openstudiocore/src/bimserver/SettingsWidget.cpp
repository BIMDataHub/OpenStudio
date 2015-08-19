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

#include "SettingsWidget.hpp"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QLabel>
#include <QFrame>
#include <QRadioButton>
#include <QButtonGroup>
#include <QPushButton>
#include <QLineEdit>

namespace openstudio {
namespace bimserver{

  //SettingsWidget::SettingsWidget(const model::Model & model, QWidget * parent)
  SettingsWidget::SettingsWidget(QWidget * parent)
    : QWidget(parent)
  {
  // Main Layout

    auto m_setLayout = new QGridLayout;
    //setDialog.setWindowTitle(tr("BIMserver Settings"));
    setLayout(m_setLayout);

    QLabel *set_introLabel = new QLabel(tr("Please enter the BIMserver information: "), this);
    QLabel *set_baddLabel = new QLabel(tr("BIMserver Address: http://"), this);
    QLineEdit *set_baddEdit = new QLineEdit(this);
    set_baddEdit->setPlaceholderText("eg: 127.0.0.1");
    QLabel *set_bportLabel = new QLabel(tr("BIMserver Port:"), this);
    QLineEdit *set_bportEdit = new QLineEdit(this);
    set_bportEdit->setPlaceholderText("eg: 8082");
    QLabel *set_unameLabel = new QLabel(tr("User Name:"), this);
    QLineEdit *set_unameEdit = new QLineEdit(this);
    set_unameEdit->setPlaceholderText("eg: admin@bimserver.org");
    QLabel *set_upassLabel = new QLabel(tr("Password: "), this);
    QLineEdit *set_upassEdit = new QLineEdit(this);
    set_upassEdit->setPlaceholderText("eg: admin");
    QPushButton *set_okButton = new QPushButton(tr("Connect"), this);
    QPushButton *set_cancelButton = new QPushButton(tr("Cancel"), this);  

    m_settings = new QSettings("OpenStudio", "BIMserverConnection");

    if (m_settings->contains("addr")) {
      QString addr = m_settings->value("addr").toString();
      set_baddEdit->setText(addr);
    }

    if (m_settings->contains("port")) {
      QString port = m_settings->value("port").toString();
      set_bportEdit->setText(port);
    }

    if (m_settings->contains("usrname")) {
      QString usrname = m_settings->value("usrname").toString();
      set_unameEdit->setText(usrname);
    }

    if (m_settings->contains("psw")) {
      QString psw = m_settings->value("psw").toString();
      set_upassEdit->setText(psw);
    }

    m_setLayout->addWidget(set_introLabel,0,0,1,2);
    m_setLayout->addWidget(set_baddLabel,1,0,1,1);
    m_setLayout->addWidget(set_baddEdit,1,1,1,1);
    m_setLayout->addWidget(set_bportLabel,2,0,1,1);
    m_setLayout->addWidget(set_bportEdit,2,1,1,1);
    m_setLayout->addWidget(set_unameLabel,3,0,1,1);
    m_setLayout->addWidget(set_unameEdit,3,1,1,1);
    m_setLayout->addWidget(set_upassLabel,4,0,1,1);
    m_setLayout->addWidget(set_upassEdit,4,1,1,1);
    m_setLayout->addWidget(set_okButton,5,0,1,1);
    m_setLayout->addWidget(set_cancelButton,5,1,1,1); 

    connect(set_okButton, SIGNAL(clicked()), this, SLOT(accepted()));
    connect(set_cancelButton, SIGNAL(clicked()), this, SLOT(rejected()));
  }

  void SettingsWidget::accepted()
  {
    QString address = set_baddEdit->text();
    QString port = set_bportEdit->text();
    QString usrname = set_unameEdit->text();
    QString psw = set_upassEdit->text();

    if (!address.isEmpty() && !port.isEmpty() && !usrname.isEmpty() && !psw.isEmpty())
    { 
      m_settings->setValue("addr", address);
      m_settings->setValue("port", port);
      m_settings->setValue("usrname",usrname);
      m_settings->setValue("psw", psw);
      emit updated(m_settings);
    }
    else {
      // clear both m_proList and m_ifcList
      emit reset();
    }
  } 

  void SettingsWidget::rejected()
  {
    //if (this->isHidden()) {
    emit finished();
    //} 
  }    

} // bimserver
} // openstudio
