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

#include "IFCTabView.hpp"

#include <QStackedWidget>
#include <QPixmap>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QStyleOption>
#include <QPainter>
#include <QResizeEvent>
#include <vector>

static const int TAB_SEPARATION = 10;

namespace openstudio {
namespace bimserver {

IFCTabView::IFCTabView(const QString & tabLabel, bool hasSubTab, QWidget * parent)
  : MainTabView(tabLabel,hasSubTab,parent)
{
  this->setObjectName("BlueGradientWidget");

  m_tabLabel = new QLabel(tabLabel,this);
  m_tabLabel->setFixedHeight(20);
  m_tabLabel->setStyleSheet("QLabel { color: white; }");
  m_tabLabel->adjustSize();
  m_tabLabel->setFixedWidth(m_tabLabel->width());
  m_tabLabel->move(7,5);

  m_mainWidget = new QWidget(this);
  m_mainWidget->setObjectName("IFCTabView");
  m_mainWidget->move(7,25);

  auto innerLayout = new QVBoxLayout();
  innerLayout->setSpacing(0);
  m_mainWidget->setLayout(innerLayout);

  m_stackedWidget = new QStackedWidget();
  m_stackedWidget->setContentsMargins(0,0,0,0);
  innerLayout->addWidget(m_stackedWidget);

  setHasSubTab(hasSubTab);

}

void IFCTabView::setHasSubTab(bool hasSubTab)
{
  m_hasSubTab = hasSubTab;

  if( hasSubTab ) {
    QString style;
    style.append("QWidget#IFCTabView { " );
    style.append("  background: #E6E6E6; " );
    //style.append("  background: #FF0000; " );
    style.append("  border-left: 1px solid black; " );
    style.append("  border-top: 1px solid black; " );
    style.append("  border-top-left-radius: 5px; " );
    style.append("}" );
    m_mainWidget->setStyleSheet(style);
    m_mainWidget->layout()->setContentsMargins(7,10,0,0);
  } else {
    QString style;
    style.append("QWidget#IFCTabView { " );
    style.append("  background: #E6E6E6; " );
    //style.append("  background: #FF0000; " );
    style.append("  border-left: 1px solid black; " );
    style.append("  border-top: 1px solid black; " );
    style.append("}" );
    m_mainWidget->setStyleSheet(style);
    m_mainWidget->layout()->setContentsMargins(0,0,0,0);
  }
}


bool IFCTabView::addSubTab(const QString & subTablabel, QWidget * widget, int id)
{
  // This method should only be called in cases where the tab will have sub tabs
  OS_ASSERT(m_hasSubTab);
  if(!m_hasSubTab) return false;

  auto button = new QPushButton(this);
  button->setText("<html><b>"+subTablabel+"</b/html>");
  button->setFixedHeight(21);
  //button->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  m_tabButtons.push_back(button);
  connect(button, &QPushButton::clicked, this, &IFCTabView::select);

  m_stackedWidget->addWidget(widget);

  m_ids.push_back(id);

  setCurrentIndex(0);
  return true;
}

void IFCTabView::select()
{
  QPushButton * button = qobject_cast<QPushButton *>(sender());

  int index = 0;

  for( auto it = m_tabButtons.begin();
       it < m_tabButtons.end();
       ++it ){
    if( *it == button ){
      break;
    } else {
      index++;
    }
  } 

  setCurrentIndex(index);
}

void IFCTabView::setCurrentIndex(int index)
{
  int xPos = m_tabLabel->width() + TAB_SEPARATION;

  for(unsigned i = 0; i < m_tabButtons.size(); i++)
  {  
    QPushButton * button = m_tabButtons[i];
    QString style;

    //style.append("QPushButton { border: none; background-color: #BBCDE3; ");
    style.append("QPushButton { border: none; background-color: #E3D1BA; ");
    style.append("              border-right: 1px solid black;");
    style.append("              border-bottom: 1px solid black;");
    style.append("              border-top: 1px solid black;");
    style.append("              border-left: 1px solid black;");
    style.append("              border-top-left-radius: 5px;");
    style.append("              border-top-right-radius: 5px;");
    style.append("              padding-left: 10px;");
    style.append("              padding-right: 10px;");
    style.append("              color: black;");
    //style.append("              color: red;");
    style.append("}");

    //button->setStyleSheet(style); 
    
    button->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    //button->adjustSize();
    
    button->move(xPos,5);

    button->stackUnder(m_mainWidget);

    //xPos += TAB_SEPARATION + button->width();
  }

  QPushButton * button = m_tabButtons[index];

  QString style;

  //style.append("QPushButton { border: none; background-color: #E6E6E6; ");
  style.append("QPushButton { border: none; background-color: #FF0000; ");
  style.append("              border-right: 1px solid black;");
  style.append("              border-bottom: none;");
  style.append("              border-top: 1px solid black;");
  style.append("              border-left: 1px solid black;");
  style.append("              border-top-left-radius: 5px;");
  style.append("              border-top-right-radius: 5px;");
  style.append("              padding-left: 10px;");
  style.append("              padding-right: 10px;");
  style.append("              color: black;");
  //style.append("              color: red;");
  style.append("}");

  //button->setStyleSheet(style); 
  button->raise();

  button->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  //button->adjustSize();

  m_stackedWidget->setCurrentIndex(index);

  emit tabSelected(m_ids[index]);
}

} // bimserver
} // openstudio
