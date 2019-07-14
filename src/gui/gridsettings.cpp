/**
 *  This file is part of Evoplex.
 *
 *  Evoplex is a multi-agent system for networks.
 *  Copyright (C) 2018 - Marcos Cardinot <marcos@cardinot.net>
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "gridsettings.h"
#include "ui_gridsettings.h"

namespace evoplex {

GridSettings::GridSettings(ColorMapMgr* cMgr, ExperimentPtr exp, GridView* parent)
    : QDialog(parent, MainGUI::kDefaultDlgFlags),
      m_ui(new Ui_GridSettings),
      m_cMgr(cMgr),
      m_exp(exp)
{
    m_ui->setupUi(this);

    connect(m_exp.get(), SIGNAL(restarted()), SLOT(init()));
    connect(m_ui->nodesColor, SIGNAL(cmapUpdated(ColorMap*)),
            parent, SLOT(setNodeCMap(ColorMap*)));

    connect(m_ui->bOk, SIGNAL(pressed()), SLOT(close()));
    connect(m_ui->bRestore, SIGNAL(pressed()), SLOT(restoreSettings()));
    connect(m_ui->bSaveAsDefault, SIGNAL(pressed()), SLOT(saveAsDefault()));

    init();
}

GridSettings::~GridSettings()
{
    delete m_ui;
}

void GridSettings::init()
{
    Q_ASSERT_X(m_exp->modelPlugin(), "GridSettings",
               "tried to init the graph settings for a null model!");

    QSettings userPrefs;
    auto cmap = m_cMgr->defaultCMapKey();
    CMapKey n(userPrefs.value("graphSettings/nodeCMap", cmap.first).toString(),
              userPrefs.value("graphSettings/nodeCMapSize", cmap.second).toInt());
    m_ui->nodesColor->init(m_cMgr, n, m_exp->modelPlugin()->nodeAttrsScope());
}

void GridSettings::restoreSettings()
{
    auto cmap = m_cMgr->defaultCMapKey();
    QSettings userPrefs;
    userPrefs.setValue("graphSettings/nodeCMap", cmap.first);
    userPrefs.setValue("graphSettings/nodeCMapSize", cmap.second);
    init();
}

void GridSettings::saveAsDefault()
{
    QSettings userPrefs;
    userPrefs.setValue("graphSettings/nodeCMap", nodeColorSelector()->cmapName());
    userPrefs.setValue("graphSettings/nodeCMapSize", nodeColorSelector()->cmapSize());
}

AttrColorSelector* GridSettings::nodeColorSelector() const
{
    return m_ui->nodesColor;
}

} // evoplex
