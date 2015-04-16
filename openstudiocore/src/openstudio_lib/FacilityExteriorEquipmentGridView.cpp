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

#include "FacilityExteriorEquipmentGridView.hpp"

#include "ModelObjectInspectorView.hpp"
#include "ModelObjectItem.hpp"
#include "ModelSubTabView.hpp"
#include "OSAppBase.hpp"
#include "OSDocument.hpp"
#include "OSDropZone.hpp"

#include "../shared_gui_components/OSGridView.hpp"

#include "../model/ExteriorLights.hpp"
#include "../model/ExteriorLights_Impl.hpp"
#include "../model/Model.hpp"
#include "../model/Model_Impl.hpp"
#include "../model/ModelObject.hpp"
#include "../model/ModelObject_Impl.hpp"
#include "../model/Schedule.hpp"
#include "../model/Schedule_Impl.hpp"

#include "../utilities/idd/IddEnums.hxx"
#include "../utilities/idd/OS_Exterior_Lights_FieldEnums.hxx"

#include <QBoxLayout>
#include <QCheckBox>
#include <QComboBox>
#include <QLabel>

// These defines provide a common area for field display names
// used on column headers, and other grid widgets

#define NAME "Exterior Lights Name"
#define SELECTED "All"

// EXTERIOR LIGHTS
#define EXTERIORLIGHTSDEFINITION "Exterior Lights Definition"
#define SCHEDULE "Schedule"
#define CONTROLOPTION "Control Option"
#define MULTIPLIER "Multiplier"
#define ENDUSESUBCATEGORY "End Use Subcategory"

namespace openstudio {

  struct ModelObjectNameSorter{
    // sort by name
    bool operator()(const model::ModelObject & lhs, const model::ModelObject & rhs){
      return (lhs.name() < rhs.name());
    }
  };

  FacilityExteriorEquipmentGridView::FacilityExteriorEquipmentGridView(bool isIP, const model::Model & model, QWidget * parent)
    : QWidget(parent),
    m_isIP(isIP)
  {
    auto mainLayout = new QVBoxLayout();
    mainLayout->setSpacing(0);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    setLayout(mainLayout);

    auto exteriorLights = model.getModelObjects<model::ExteriorLights>();
    auto exteriorLightsModelObjects = subsetCastVector<model::ModelObject>(exteriorLights);

    m_gridController = new FacilityExteriorEquipmentGridController(m_isIP, "Exterior Lights", IddObjectType::OS_Exterior_Lights, model, exteriorLightsModelObjects);
    auto gridView = new OSGridView(m_gridController, "Exterior Lights", "Drop\nZone", false, parent);

    // Load Filter

    QLabel * label = nullptr;

    QVBoxLayout * layout = nullptr;

    bool isConnected = false;

    mainLayout->addWidget(gridView, 0, Qt::AlignTop);

    mainLayout->addStretch(1);

    // GridController

    OS_ASSERT(m_gridController);

    isConnected = connect(gridView, SIGNAL(dropZoneItemClicked(OSItem*)), this, SIGNAL(dropZoneItemClicked(OSItem*)));
    OS_ASSERT(isConnected);

    isConnected = connect(this, SIGNAL(itemSelected(OSItem *)), gridView, SIGNAL(itemSelected(OSItem *)));
    OS_ASSERT(isConnected);

    isConnected = connect(this, SIGNAL(selectionCleared()), gridView, SLOT(onSelectionCleared()));
    OS_ASSERT(isConnected);

    isConnected = connect(gridView, SIGNAL(gridRowSelected(OSItem*)), this, SIGNAL(gridRowSelected(OSItem*)));
    OS_ASSERT(isConnected);

    isConnected = connect(this, SIGNAL(toggleUnitsClicked(bool)), m_gridController, SIGNAL(toggleUnitsClicked(bool)));
    OS_ASSERT(isConnected);

    isConnected = connect(this, SIGNAL(toggleUnitsClicked(bool)), m_gridController, SLOT(toggleUnits(bool)));
    OS_ASSERT(isConnected);

  }

  std::vector<model::ModelObject> FacilityExteriorEquipmentGridView::selectedObjects() const
  {
    return m_gridController->selectedObjects();
  }

  void FacilityExteriorEquipmentGridView::onDropZoneItemClicked(OSItem* item)
  {
  }

  FacilityExteriorEquipmentGridController::FacilityExteriorEquipmentGridController(bool isIP,
    const QString & headerText,
    IddObjectType iddObjectType,
    model::Model model,
    std::vector<model::ModelObject> modelObjects) :
    OSGridController(isIP, headerText, iddObjectType, model, modelObjects)
  {
    setCategoriesAndFields();
  }

  void FacilityExteriorEquipmentGridController::setCategoriesAndFields()
  {

    {
      std::vector<QString> fields;
      //fields.push_back(EXTERIORLIGHTSDEFINITION);
      //fields.push_back(SCHEDULE);
      fields.push_back(CONTROLOPTION);
      fields.push_back(MULTIPLIER);
      //fields.push_back(ENDUSESUBCATEGORY);
      std::pair<QString, std::vector<QString> > categoryAndFields = std::make_pair(QString("Exterior Lights"), fields);
      m_categoriesAndFields.push_back(categoryAndFields);
    }

    OSGridController::setCategoriesAndFields();

  }

  FacilityExteriorEquipmentGridView * FacilityExteriorEquipmentGridController::gridView()
  {
    OS_ASSERT(qobject_cast<OSGridView *>(this->parent()));

    return qobject_cast<FacilityExteriorEquipmentGridView *>(this->parent()->parent());
  }

  void FacilityExteriorEquipmentGridController::categorySelected(int index)
  {
    OSGridController::categorySelected(index);
  }

  void FacilityExteriorEquipmentGridController::addColumns(const QString &category, std::vector<QString> & fields)
  {
    // always show name and selected columns
    fields.insert(fields.begin(), { NAME, SELECTED });

    m_baseConcepts.clear();

    for (const auto &field : fields) {

      if (field == NAME) {
        addNameLineEditColumn(Heading(QString(NAME), false, false),
          false,
          false,
          CastNullAdapter<model::ExteriorLights>(&model::ExteriorLights::name),
          CastNullAdapter<model::ExteriorLights>(&model::ExteriorLights::setName)
          );
      }
      else if (field == SELECTED) {
        auto checkbox = QSharedPointer<QCheckBox>(new QCheckBox());
        checkbox->setToolTip("Check to select all rows");
        connect(checkbox.data(), &QCheckBox::stateChanged, this, &FacilityExteriorEquipmentGridController::selectAllStateChanged);

        addSelectColumn(Heading(QString(SELECTED), false, false, checkbox), "Check to select this row");
      }
      else if (field == EXTERIORLIGHTSDEFINITION) {
      //EXTERIORLIGHTSDEFINITION
      //ExteriorLightsDefinition exteriorLightsDefinition() const;
      //bool setExteriorLightsDefinition(const ExteriorLightsDefinition& exteriorLightsDefinition);
        addNameLineEditColumn(Heading(QString(NAME), false, false),
          false,
          false,
          CastNullAdapter<model::ExteriorLights>(&model::ExteriorLights::name),
          CastNullAdapter<model::ExteriorLights>(&model::ExteriorLights::setName)
          );
      }
      else if (field == SCHEDULE) {
      //SCHEDULE
      //boost::optional<Schedule> schedule() const;
      //bool setSchedule(Schedule& schedule);
      //void resetSchedule();
        //addDropZoneColumn(Heading(QString(SCHEDULE)),
        //  CastNullAdapter<model::ExteriorLights>(&model::ExteriorLights::schedule),
        //  CastNullAdapter<model::ExteriorLights>(&model::ExteriorLights::setSchedule)//,
        //  //boost::optional<std::function<void(model::ExteriorLights*)>>(CastNullAdapter<model::ExteriorLights>(&model::ExteriorLights::resetSchedule))
        //);
      }
      else if (field == CONTROLOPTION){
      //CONTROLOPTION
      //static std::vector<std::string> controlOptionValues();
      //std::string controlOption() const;
      //bool setControlOption(std::string controlOption);
      //void resetControlOption();
      //bool isControlOptionDefaulted() const;
        addComboBoxColumn<std::string, model::ExteriorLights>(
          Heading(QString(CONTROLOPTION)),
          static_cast<std::string(*)(const std::string&)>(&openstudio::toString),
          std::function<std::vector<std::string>()>(&model::ExteriorLights::controlOptionValues),
          CastNullAdapter<model::ExteriorLights>(&model::ExteriorLights::controlOption),
          CastNullAdapter<model::ExteriorLights>(&model::ExteriorLights::setControlOption),
          boost::optional<std::function<void(model::ExteriorLights *)> >(),
          boost::optional<DataSource>()
          );
      }
      else if (field == MULTIPLIER){
      //MULTIPLIER
      //double multiplier() const;
      //bool setMultiplier(double multiplier);
      //void resetMultiplier();
      //bool isMultiplierDefaulted() const;   
        addValueEditColumn(Heading(QString(MULTIPLIER)),
          NullAdapter(&model::ExteriorLights::multiplier),
          NullAdapter(&model::ExteriorLights::setMultiplier)
          );
      }
      else if (field == ENDUSESUBCATEGORY){
        //ENDUSESUBCATEGORY
        //std::string endUseSubcategory() const;
        //void setEndUseSubcategory(std::string endUseSubcategory);
        //void resetEndUseSubcategory();
        //bool isEndUseSubcategoryDefaulted() const;

        //addNameLineEditColumn(Heading(QString(ENDUSESUBCATEGORY), false, false),
        //  false,
        //  false,
        //  CastNullAdapter<model::ExteriorLights>(&model::ExteriorLights::endUseSubcategory),
        //  CastNullAdapter<model::ExteriorLights>(&model::ExteriorLights::setEndUseSubcategory)
        //  );
      }
      else {
        // unhandled
        OS_ASSERT(false);
      }
    }
  }

  QString FacilityExteriorEquipmentGridController::getColor(const model::ModelObject & modelObject)
  {
    QColor defaultColor(Qt::lightGray);
    return defaultColor.name();
  }

  void FacilityExteriorEquipmentGridController::checkSelectedFields()
  {
    if (!this->m_hasHorizontalHeader) return;

    OSGridController::checkSelectedFields();
  }

  void FacilityExteriorEquipmentGridController::onItemDropped(const OSItemId& itemId)
  {
  }

  void FacilityExteriorEquipmentGridController::refreshModelObjects()
  {
    auto exteriorLights = m_model.getModelObjects<model::ExteriorLights>();
    m_modelObjects = subsetCastVector<model::ModelObject>(exteriorLights);
    std::sort(m_modelObjects.begin(), m_modelObjects.end(), ModelObjectNameSorter());
  }

  void FacilityExteriorEquipmentGridController::onComboBoxIndexChanged(int index)
  {
  }

} // openstudio