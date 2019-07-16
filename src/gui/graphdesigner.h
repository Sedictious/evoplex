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

#ifndef GRAPH_DESIGNER_H
#define GRAPH_DESIGNER_H

#include <QDockWidget>
#include <QWidget>
#include <QTreeWidgetItem>
#include <QHash>
#include <QTableWidget>

#include "core/graphinputs.h"
#include "core/project.h"

#include "external/qt-material-widgets/qtmaterialiconbutton.h"

#include "attrwidget.h"
#include "graphdesignerpage.h"

class Ui_GraphDesigner;

namespace evoplex {

class GraphDesigner : public QWidget
{

    Q_OBJECT

public:
    explicit GraphDesigner(MainApp* mainApp, QWidget* parent);
    ~GraphDesigner();


private slots:
    void slotPluginAdded(const Plugin* plugin);
    void slotPluginRemoved(PluginKey key, PluginType type);
    void slotGraphSelected(int cbIdx);
    void slotNodeTableUpdate(int val);
    void slotEdgeTableUpdate(int val);
    void slotAddGraphDesigner();
    void slotEdgesWidget();
    void slotNodesWidget();

private:
    MainApp * m_mainApp;
    ProjectPtr m_project;
    PluginKey m_selectedGraphKey;

    Ui_GraphDesigner* m_ui;
    QTreeWidgetItem* m_treeItemGraphs;
    QTreeWidgetItem* m_treeItemAttrs;
    QHash<QString, AttrWidget*> m_attrWidgets;
    AttrWidget* m_enableOutputs;
    int m_graphTypeIdx;  // position of the graphtype combobox in the graph tree
    int m_edgesAttrsIdx; // position of the edgesAttrs field in the graph tree
    int m_graphDesignerId;

    QtMaterialIconButton* m_bAdd;
    QtMaterialIconButton* m_bRemove;
    
    AttrWidget* addGeneralAttr(QTreeWidgetItem* itemRoot,
        const QString& attrName, QWidget* customWidget = nullptr);
    void addTreeWidgetItem(QTreeWidgetItem* itemRoot, 
        const QString& attrName,
QWidget* customWidget, int i);

    QTableWidget* m_nodeAttrTable;
    QTableWidget* m_edgeAttrTable;

    AttributesScope m_nodeAttrs;
    AttributesScope m_edgeAttrs;

    std::unique_ptr<GraphInputs> readInputs(const int graphId, QString& error) const;

    //QList<AttributesScope> 
    void parseAttributes(QString& error);

    void pluginSelected(QTreeWidgetItem* itemRoot, const PluginKey& key);
    void addPluginAttrs(QTreeWidgetItem* tree, const Plugin* plugin);
};
}

#endif // GRAPH_DESIGNER_H