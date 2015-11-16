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

#include "../openstudio_lib/SubTabView.hpp"
#include <QWidget>
#include <vector>

class QStackedWidget;
class QPushButton;
class QLabel;

namespace openstudio {
namespace bimserver {	

class IFCTabView : public MainTabView
{
  Q_OBJECT

public:
  IFCTabView(const QString & tabLabel, bool hasSubTabs, QWidget * parent = nullptr);
  bool addSubTab(const QString & subTabLabel, QWidget * widget, int id);
  void setHasSubTab(bool hasSubTab);
  virtual ~IFCTabView() {}

private slots:
  void select() ;

signals:
  void tabSelected(int id);

protected:
  void setCurrentIndex(int index);

private:
  QLabel * m_tabLabel;
  QStackedWidget * m_stackedWidget;
  QWidget * m_mainWidget;

  std::vector<QString> m_selectedPixmaps;
  std::vector<QString> m_neighborSelectedPixmaps;
  std::vector<QString> m_unSelectedPixmaps;
  std::vector<QPushButton *> m_tabButtons; 
  std::vector<int> m_ids; 

  bool m_hasSubTab;
};

} // bimserver
} // openstudio

#endif 
// BIMSERVER_IFCTABVIEW_HPP
