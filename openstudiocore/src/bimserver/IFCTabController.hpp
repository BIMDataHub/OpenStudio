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

class OSItemId;

class MainTabView;

class SettingsWidget;

class ProjectsWidegt;

class FilesWidegt;

class ImportWidget;

class IFCTabController : public MainTabController
{
  Q_OBJECT

  public:

  IFCTabController(bool isIP);

  virtual ~IFCTabController() {}

  enum TabID
  {
    SETTINGS,
    PROJECTS,
    FILES,
    IMPORT
  };

  QSettings *m_settings;
  
  signals:
  /// OSM String is retrieved.
  void finished();

  public slots:
  void processOSMRetrieved(QString osmString);
  void processSettings(QSettings *);
  void resetProID(QString newID);
  void resetIFCID(QString newID);
  void newProject(QString newID);
  void newFile(QString newID);

private:
  SettingsWidget * m_settingsWidget;
  ProjectsWidget * m_projectsWidget;
  FilesWidget * m_filesWidget;
  ImportWidget * m_importWidget;

  QString     m_OSM;
  QString     m_proID;
  QString     m_ifcID;

  bool m_isIP;
  BIMserverConnection *m_bimserverConnection;
};

} // bimserver
} // openstudio

#endif // BIMSERVER_IFCTABCONTROLLER_HPP 
