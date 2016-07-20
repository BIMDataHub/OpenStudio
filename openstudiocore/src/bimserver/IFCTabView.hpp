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

#ifndef BIMSERVER_IFCTABVIEW_HPP
#define BIMSERVER_IFCTABVIEW_HPP

#include "../openstudio_lib/MainTabView.hpp"
#include <QWidget>
#include <vector>

class QLabel;
class QPushButton;
class QStackedWidget;
class QVBoxLayout;

namespace openstudio {

class OSViewSwitcher;

namespace bimserver {	

class IFCTabView : public MainTabView
{
  Q_OBJECT

public:
  IFCTabView(const QString & tabLabel, MainTabView::TabType tabType, QWidget * parent = nullptr);
  //bool addSubTab(const QString & subTabLabel, QWidget * widget, int id);
  //void setHasSubTab(bool hasSubTab);
  virtual ~IFCTabView();

  void setTabType(TabType tabTyp);
  bool addTabWidget(QWidget * widget);
  bool addSubTab(const QString & subTabLabel, int id);
  void setSubTab(QWidget * widget);
  bool selectSubTabByIndex(int index);
  QPointer<OSViewSwitcher> m_editView;

private slots:
  void select();

signals:
  //void tabSelected(int id);
  void tabSelected(int id);
  void toggleUnitsClicked(bool displayIP);

protected:
  //void setCurrentIndex(int index);
  void setCurrentIndex(int index);
  void setCurrentWidget(QWidget * widget);
  void paintEvent( QPaintEvent * event ) override;
  void resizeEvent( QResizeEvent * event ) override;

private:
  /*
  QLabel * m_tabLabel;
  QStackedWidget * m_stackedWidget;
  QWidget * m_mainWidget;

  std::vector<QString> m_selectedPixmaps;
  std::vector<QString> m_neighborSelectedPixmaps;
  std::vector<QString> m_unSelectedPixmaps;
  std::vector<QPushButton *> m_tabButtons; 
  std::vector<int> m_ids; 

  bool m_hasSubTab;
  */
  QLabel * m_tabLabel = nullptr;
  QWidget * m_mainWidget = nullptr;
  QWidget * m_currentInnerWidget = nullptr;
  QVBoxLayout * m_innerLayout = nullptr;

  std::vector<QString> m_selectedPixmaps;
  std::vector<QString> m_neighborSelectedPixmaps;
  std::vector<QString> m_unSelectedPixmaps;
  std::vector<QPushButton *> m_tabButtons;
  std::vector<int> m_ids;

  TabType m_tabType;
};

} // bimserver
} // openstudio

#endif 
// BIMSERVER_IFCTABVIEW_HPP
