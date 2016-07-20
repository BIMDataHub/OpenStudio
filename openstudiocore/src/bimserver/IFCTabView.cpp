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

#include <vector>

#include <QLabel>
#include <QPixmap>
#include <QPainter>
#include <QBoxLayout>
#include <QPushButton>
#include <QResizeEvent>
#include <QStyleOption>
#include <QStackedWidget>

#include "../utilities/core/Assert.hpp"
#include "../shared_gui_components/OSViewSwitcher.hpp"

static const int TAB_SEPARATION = 10;

namespace openstudio {
namespace bimserver {

IFCTabView::IFCTabView(const QString & tabLabel, MainTabView::TabType tabType, QWidget * parent)
  : MainTabView(tabLabel,tabType,parent)
{
  /*
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
  */

  this->setObjectName("BlueGradientWidget");

  m_tabLabel = new QLabel(tabLabel,this);
  m_tabLabel->setFixedHeight(20);
  m_tabLabel->setStyleSheet("QLabel { color: white; }");
  m_tabLabel->adjustSize();
  m_tabLabel->setFixedWidth(m_tabLabel->width());
  m_tabLabel->move(7,5);

  auto label = new QLabel();
  label->setObjectName("H2");

  m_editView->setView(label);

  m_innerLayout = new QVBoxLayout();
  m_innerLayout->setSpacing(0);
  m_innerLayout->addWidget(m_editView);

  m_mainWidget = new QWidget(this);
  m_mainWidget->setObjectName("IFCTabView");
  m_mainWidget->move(7,25);
  m_mainWidget->setLayout(m_innerLayout);

  setTabType(tabType);
}

IFCTabView::~IFCTabView()
{
  if (m_editView) { delete m_editView; }
}

void IFCTabView::setTabType(MainTabView::TabType tabType)
{
  QString style;

  switch (tabType)
  {
  case MAIN_TAB:
    style.append("QWidget#IFCTabView { ");
    style.append("  background: #E6E6E6; ");
    style.append("  border-left: 1px solid black; ");
    style.append("  border-top: 1px solid black; ");
    style.append("}");
    m_mainWidget->layout()->setContentsMargins(0, 0, 0, 0);
    break;
  case SUB_TAB:
    style.append("QWidget#IFCTabView { ");
    style.append("  background: #E6E6E6; ");
    style.append("  border-left: 1px solid black; ");
    style.append("  border-top: 1px solid black; ");
    style.append("  border-top-left-radius: 5px; ");
    style.append("}");
    m_mainWidget->layout()->setContentsMargins(7, 10, 0, 0);
    break;
  case GRIDVIEW_SUB_TAB:
    style.append("QWidget#IFCTabView { ");
    style.append("  background: #E6E6E6; ");
    style.append("  border-left: 1px solid black; ");
    style.append("  border-top: 1px solid black; ");
    style.append("  border-top-left-radius: 5px; ");
    style.append("}");
    m_mainWidget->layout()->setContentsMargins(1, 2, 0, 0);
    break;
  default:
    OS_ASSERT(false);
  }

  m_mainWidget->setStyleSheet(style);
}

bool IFCTabView::addTabWidget(QWidget * widget)
{
  // This method should only be called in cases where the tab will not have sub tabs
  OS_ASSERT(m_tabType == MAIN_TAB);
  if (m_tabType != MAIN_TAB) return false;

  m_editView->setView(widget);

  return true;
}

bool IFCTabView::addSubTab(const QString & subTablabel, int id)
{
  // This method should only be called in cases where the tab will have sub tabs
  OS_ASSERT(m_tabType != MAIN_TAB);
  if (m_tabType == MAIN_TAB) return false;

  auto button = new QPushButton(this);
  button->setText(subTablabel);
  button->setFixedHeight(21);
  m_tabButtons.push_back(button);
  connect(button, &QPushButton::clicked, this, &IFCTabView::select);

  m_ids.push_back(id);

  setCurrentIndex(0);
  return true;
}

void IFCTabView::setSubTab(QWidget * widget)
{
  m_editView->setView(widget);
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

    style.append("QPushButton { border: none; background-color: #BBCDE3; ");
    style.append("              border-right: 1px solid black;");
    style.append("              border-bottom: 1px solid black;");
    style.append("              border-top: 1px solid black;");
    style.append("              border-left: 1px solid black;");
    style.append("              border-top-left-radius: 5px;");
    style.append("              border-top-right-radius: 5px;");
    style.append("              padding-left: 10px;");
    style.append("              padding-right: 10px;");
    style.append("              color: black;");
    style.append("}");

    button->setStyleSheet(style); 
    button->adjustSize();
    button->move(xPos,5);

    button->stackUnder(m_mainWidget);

    xPos += TAB_SEPARATION + button->width();
  }

  QPushButton * button = m_tabButtons[index];

  QString style;

  style.append("QPushButton { border: none; background-color: #E6E6E6; ");
  style.append("              border-right: 1px solid black;");
  style.append("              border-bottom: none;");
  style.append("              border-top: 1px solid black;");
  style.append("              border-left: 1px solid black;");
  style.append("              border-top-left-radius: 5px;");
  style.append("              border-top-right-radius: 5px;");
  style.append("              padding-left: 10px;");
  style.append("              padding-right: 10px;");
  style.append("              color: black;");
  style.append("}");

  button->setStyleSheet(style); 
  button->raise();

  emit tabSelected(m_ids[index]);
}

void IFCTabView::setCurrentWidget(QWidget * widget)
{
  OS_ASSERT(false);
}

void IFCTabView::paintEvent ( QPaintEvent * event )
{
  QStyleOption opt;
  opt.init(this);
  QPainter p(this);
  style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

void IFCTabView::resizeEvent( QResizeEvent * event )
{
  QSize size = event->size();  

  QSize newSize(size.width() - 7,size.height() - 25);

  m_mainWidget->resize(newSize);
}


/*
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
*/

} // bimserver
} // openstudio
