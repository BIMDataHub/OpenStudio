#ifndef BIMSERVER_IFCTABCONTROLLER_HPP
#define BIMSERVER_IFCTABCONTROLLER_HPP

#include "IFCTabView.hpp"
#include "SettingsWidget.hpp"
#include "ProjectsWidget.hpp"
#include "FilesWidget.hpp"
#include "ImportWidget.hpp"
#include "BIMserverConnection.hpp"

#include "../model/Model.hpp"
#include "../shared_gui_components/OSDialog.hpp"
#include "../openstudio_lib/MainTabView.hpp"
#include "../openstudio_lib/MainTabController.hpp"

#include <QDialog>
#include <QListWidget>
#include <QStatusBar>
#include <QSettings>
#include <QEventLoop>
#include <QCloseEvent>
#include <QKeyEvent>

#include "../openstudio_lib/MainTabController.hpp"
#include "../model/Model.hpp"
#include "../utilities/core/UUID.hpp"
#include <boost/smart_ptr.hpp>
#include <QObject>

namespace openstudio {
namespace bimserver {

//class OSItemId;

//class MainTabView;

class SettingsWidget;

class ProjectsWidegt;

class FilesWidegt;

class ImportWidget;

class IFCTabController : public MainTabController
{
  Q_OBJECT

  public:

  IFCTabController(bool isIP);

  virtual ~IFCTabController();

  enum TabID
  {
    SETTINGS,
    PROJECTS,
    FILES,
    IMPORT
  };

  SettingsWidget * m_settingsWidget;
  ProjectsWidget * m_projectsWidget;
  FilesWidget * m_filesWidget;
  //ImportWidget * m_importWidget;

  public slots:

  virtual void setSubTab(int index) override;

  private:

  QWidget * m_currentView = nullptr;

  int m_currentIndex = -1;

};

} // bimserver
} // openstudio

#endif // BIMSERVER_IFCTABCONTROLLER_HPP 
