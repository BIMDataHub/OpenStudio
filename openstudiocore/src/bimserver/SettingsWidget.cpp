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

#include <QUrl>
#include <QIcon>
#include <QLabel>
#include <QFrame>
#include <QLabel>
#include <QPixmap>
#include <QLineEdit>
#include <QPushButton>
#include <QMessageBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QRadioButton>
#include <QButtonGroup>
#include <QDesktopServices>

namespace openstudio {
namespace bimserver{

  SettingsWidget::SettingsWidget(QWidget * parent)
    : QWidget(parent)
  {

    auto m_setLayout = new QHBoxLayout;
    auto m_setLeftLayout = new QVBoxLayout;
    auto m_setRightLayout = new QVBoxLayout;

    QFont f( "Arial", 15);

    set_logoLabel = new QLabel(this);
    set_baddLabel = new QLabel(tr("<html><b>BIMserver Address</b</html>"), this);
    this->set_baddEdit = new QLineEdit(this);
    set_baddEdit->setText("127.0.0.1");
    set_bportLabel = new QLabel(tr("<html><b>BIMserver Portnum</b</html>"), this);
    this->set_bportEdit = new QLineEdit(this);
    set_bportEdit->setText("8082");
    set_unameLabel = new QLabel(tr("<html><b>User Name</b/html>"), this);
    this->set_unameEdit = new QLineEdit(this);
    set_unameEdit->setText("admin@bimserver.org");
    set_upassLabel = new QLabel(tr("<html><b>Password</b/html>"), this);
    this->set_upassEdit = new QLineEdit(this);
    set_upassEdit->setText("admin");
    set_upassEdit->setEchoMode(QLineEdit::Password);
    set_sevStatus = new QLabel(tr("<html><b>Sever Status:</b/html>"), this);
    //set_sevStatus->setWordWrap(true);

    set_baddLabel->setFont(f);
    set_baddEdit->setFont(f);
    set_bportLabel->setFont(f);
    set_bportEdit->setFont(f);
    set_unameLabel->setFont(f);
    set_unameEdit->setFont(f);
    set_upassLabel->setFont(f);
    set_upassEdit->setFont(f);
    set_sevStatus->setFont(f);  
    
    QPixmap pixmap("/Users/yufu/Documents/OpenStudio-1/openstudiocore/src/bimserver/images/cbeilogo.png");
    QIcon ButtonIcon(pixmap);
    QPushButton *set_logoButton = new QPushButton;
    set_logoButton->setIcon(ButtonIcon);
    set_logoButton->setIconSize(pixmap.rect().size());
    set_okButton = new QPushButton(tr("Connect"), this);
    set_nextButton = new QPushButton(tr("Next Tab"), this); 
    connect(set_okButton, SIGNAL(clicked()), this, SLOT(okButton_clicked()));
    connect(set_nextButton, SIGNAL(clicked()), this, SLOT(nextButton_clicked()));
    connect(set_logoButton, SIGNAL(clicked()), this, SLOT(logoButton_clicked()));
    
    set_okButton->setFont(f);
    set_nextButton->setFont(f);
    set_nextButton->setEnabled(false);
    set_logoButton->setFlat(true);

    m_setLeftLayout->addSpacing(80);
    m_setLeftLayout->addWidget(set_baddLabel,2);
    m_setLeftLayout->addWidget(set_baddEdit,2);
    m_setLeftLayout->addSpacing(10);
    m_setLeftLayout->addWidget(set_bportLabel,2);
    m_setLeftLayout->addWidget(set_bportEdit,2);
    m_setLeftLayout->addSpacing(10);
    m_setLeftLayout->addWidget(set_unameLabel,2);
    m_setLeftLayout->addWidget(set_unameEdit,2);
    m_setLeftLayout->addSpacing(10);
    m_setLeftLayout->addWidget(set_upassLabel,2);
    m_setLeftLayout->addWidget(set_upassEdit,2);
    m_setLeftLayout->addSpacing(80);

    m_setRightLayout->addWidget(set_logoButton,1,Qt::AlignRight);
    m_setRightLayout->addSpacing(200);
    m_setRightLayout->addWidget(set_okButton,1,Qt::AlignCenter);
    m_setRightLayout->addWidget(set_sevStatus,4,Qt::AlignCenter);
    m_setRightLayout->addSpacing(200);
    m_setRightLayout->addWidget(set_nextButton,1,Qt::AlignRight);
    m_setRightLayout->addSpacing(10);

    m_setLayout->addLayout(m_setLeftLayout,1);
    m_setLayout->addLayout(m_setRightLayout,1);
    setLayout(m_setLayout);
  }

  void SettingsWidget::okButton_clicked()
  {
    QString address = set_baddEdit->text();
    QString port = set_bportEdit->text();
    QString usrname = set_unameEdit->text();
    QString psw = set_upassEdit->text();

    if (!address.isEmpty() && !port.isEmpty()
      && !usrname.isEmpty() && !psw.isEmpty())
    {
      m_settings = new QSettings("OpenStudio", "BIMserverConnection");
      m_settings->setValue("addr", address);
      m_settings->setValue("port", port);
      m_settings->setValue("usrname", usrname);
      m_settings->setValue("psw", psw);

      emit updated(m_settings);
    }

    else {
      QMessageBox messageBox(this);
      messageBox.setTextFormat(Qt::RichText);
      messageBox.setText(tr("User Credentials Illegal, Learn more <a href='http://nrel.github.io/OpenStudio-user-documentation/tutorials/tutorial_ifcimport/>here</a>"));
      messageBox.exec();
      emit reset();
    }
  }

  void SettingsWidget::nextButton_clicked()
  {
    emit nextTab(1);
  } 

  void SettingsWidget::logoButton_clicked()
  {
    QDesktopServices::openUrl(QUrl("http://cbei.psu.edu"));
  } 
  
  void SettingsWidget::successStatus()
  {
    set_sevStatus->setText(tr("<b>Server Status:</b> BIMServer Connected"));
    set_nextButton->setEnabled(true);
  }

  void SettingsWidget::failedStatus()
  {
    set_sevStatus->setText(tr("<b>Server Status:</b> Fail to Connect"));
    set_nextButton->setEnabled(false);
  }

} // bimserver
} // openstudio
