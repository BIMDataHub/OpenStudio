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

#include "ImportWidget.hpp"

#include <QGridLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QButtonGroup>
#include <QPushButton>

namespace openstudio {
namespace bimserver {

//ImportWidget::ImportWidget(const model::Model & model, QWidget * parent)
  ImportWidget::ImportWidget(QWidget * parent)
    : QWidget(parent)
    //m_model(model)
  {
    //auto importLayout = new QGridLayout;
    auto m_impLayout = new QHBoxLayout;
    auto m_impLeftLayout = new QVBoxLayout;
    auto m_impMiddleLayout = new QVBoxLayout;
    auto m_impRightLayout = new QVBoxLayout;

    imp_impLabel = new QLabel(tr("<html><b>Importing file ...</b</html>"), this);
    imp_busyWig = new BusyWidget(this);
    cancelButton = new QPushButton(tr("Cancel Import"), this);
    connect(cancelButton, SIGNAL(clicked()), this, SLOT(app_ended()));

    m_impLeftLayout->addWidget(imp_impLabel,1,Qt::AlignLeft);
    m_impLeftLayout->addSpacing(180);
    m_impMiddleLayout->addSpacing(60);
    m_impMiddleLayout->addWidget(imp_busyWig,1,Qt::AlignCenter);
    m_impMiddleLayout->addSpacing(60);
    m_impRightLayout->addSpacing(180);
    m_impRightLayout->addWidget(cancelButton,1,Qt::AlignRight);

    m_impLayout->addLayout(m_impLeftLayout,1);
    m_impLayout->addLayout(m_impMiddleLayout,1);
    m_impLayout->addLayout(m_impRightLayout,1);
    setLayout(m_impLayout);
  }

  void ImportWidget::app_ended() 
  {
    emit finished();
  }

} // bimserver
} // openstudio
