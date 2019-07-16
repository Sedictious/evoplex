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

#include <QDebug>
#include <QComboBox> 
#include <QSpinBox>
#include <QStringList>
#include <QMessageBox>
#include <QTableWidget>

#include "core/include/attrsgenerator.h"
#include "core/include/enum.h"
#include "core/plugin.h"
#include "core/graphplugin.h"

#include "attributes.h"
#include "graphdesigner.h"
#include "graphwidget.h"
#include "attrsgendlg.h"
#include "titlebar.h"
#include "linebutton.h"
#include "ui_graphdesigner.h"

namespace evoplex {

    GraphDesigner::GraphDesigner(MainApp* mainApp, QWidget *parent)
        : QWidget(parent),
        m_mainApp(mainApp),
        m_ui(new Ui_GraphDesigner),
        m_bAdd(new QtMaterialIconButton(QIcon(":/icons/material/add_white_24"), this)),
        m_bRemove(new QtMaterialIconButton(QIcon(":/icons/material/delete_white_24"), this))
    {
        setWindowTitle("Graph Designer");
        setObjectName("GraphDesigner");
        m_ui->setupUi(this);

        m_bRemove->setColor(Qt::white);
        m_bRemove->setIconSize(QSize(24, 24));
        m_ui->gdWidget->layout()->addWidget(m_bRemove);
        m_ui->gdWidget->layout()->setAlignment(m_bRemove, Qt::AlignRight);
        connect(m_bRemove, SIGNAL(pressed()), parent, SLOT(slotCloseGraphDesigner()));

        m_bAdd->setColor(Qt::white);
        m_bAdd->setIconSize(QSize(24, 24));
        m_ui->gdWidget->layout()->addWidget(m_bAdd);
        connect(m_bAdd, SIGNAL(pressed()), SLOT(slotAddGraphDesigner()));

        int m_graphDesignerId = 0;

        auto newTreeItem = [this](const QString& title, bool expand) {
            auto t = new QTreeWidgetItem(m_ui->treeWidget);
            t->setText(0, title);
            t->setToolTip(0, title);
            t->setExpanded(expand);
            return t;
        };

        // - the tree widget: attributes
        m_treeItemAttrs = newTreeItem("Attributes", true);

        // number of node attributes
        QSpinBox* node_attr_num = new QSpinBox(m_ui->treeWidget);
        node_attr_num->setRange(0, 1000000);
        addTreeWidgetItem(m_treeItemAttrs, "number of node attributes", node_attr_num, 1);
        m_nodeAttrTable = new QTableWidget(0, 2);

        // table holding the node attributes
        addTreeWidgetItem(m_treeItemAttrs, "node attriutes", m_nodeAttrTable, 2);
        m_nodeAttrTable->setHorizontalHeaderItem(0, new QTableWidgetItem("Attribute"));
        m_nodeAttrTable->setHorizontalHeaderItem(1, new QTableWidgetItem("Range"));
        m_nodeAttrTable->verticalHeader()->setVisible(false);
        m_nodeAttrTable->horizontalHeader()->setStretchLastSection(true);
        connect(node_attr_num, SIGNAL(valueChanged(int)), SLOT(slotNodeTableUpdate(int)));

        //number of edge attributes
        QSpinBox* edge_attr_num = new QSpinBox(m_ui->treeWidget);
        edge_attr_num->setRange(0, 1000000);
        addTreeWidgetItem(m_treeItemAttrs, "number of edge attributes", edge_attr_num, 3);

        // table holding the edge attributes
        m_edgeAttrTable = new QTableWidget(0, 2);
        addTreeWidgetItem(m_treeItemAttrs, "edge attributes", m_edgeAttrTable, 4);
        m_edgeAttrTable->setHorizontalHeaderItem(0, new QTableWidgetItem("Attribute"));
        m_edgeAttrTable->setHorizontalHeaderItem(1, new QTableWidgetItem("Range"));
        m_edgeAttrTable->verticalHeader()->setVisible(false);
        m_edgeAttrTable->horizontalHeader()->setStretchLastSection(true);
        connect(edge_attr_num, SIGNAL(valueChanged(int)), SLOT(slotEdgeTableUpdate(int)));

        // setup the tree widget: graph
        m_treeItemGraphs = newTreeItem("Graph", true);
        // -- nodes generator
        LineButton* nodesCmd = new LineButton(this, LineButton::None);
        connect(nodesCmd->button(), SIGNAL(pressed()), SLOT(slotNodesWidget()));
        addGeneralAttr(m_treeItemGraphs, GENERAL_ATTR_NODES, nodesCmd);
        // --  graphs available
        QComboBox* cb = new QComboBox(m_ui->treeWidget);
        cb->insertItem(0, "--");
        connect(cb, SIGNAL(currentIndexChanged(int)), SLOT(slotGraphSelected(int)));
        addGeneralAttr(m_treeItemGraphs, GENERAL_ATTR_GRAPHID, cb);

        // --  graph type
        cb = new QComboBox(m_ui->treeWidget);
        cb->insertItem(0, _enumToString<GraphType>(GraphType::Undirected),
            static_cast<int>(GraphType::Undirected));
        cb->insertItem(1, _enumToString<GraphType>(GraphType::Directed),
            static_cast<int>(GraphType::Directed));
        m_graphTypeIdx = m_treeItemGraphs->childCount();
        addGeneralAttr(m_treeItemGraphs, GENERAL_ATTR_GRAPHTYPE, cb);

        // -- edges generator
        LineButton* edgesCmd = new LineButton(this, LineButton::None);
        connect(edgesCmd->button(), SIGNAL(pressed()), SLOT(slotEdgesWidget()));
        m_edgesAttrsIdx = m_treeItemGraphs->childCount();
        addGeneralAttr(m_treeItemGraphs, GENERAL_ATTR_EDGEATTRS, edgesCmd);
        m_treeItemGraphs->child(m_edgesAttrsIdx)->setHidden(true);

        for (const Plugin* p : m_mainApp->plugins()) { slotPluginAdded(p); }
        connect(m_mainApp, SIGNAL(pluginAdded(const Plugin*)),
            SLOT(slotPluginAdded(const Plugin*)));
        connect(m_mainApp, SIGNAL(pluginRemoved(PluginKey, PluginType)),
            SLOT(slotPluginRemoved(PluginKey, PluginType)));
    }

    GraphDesigner::~GraphDesigner()
    {
        delete m_ui;
        delete m_nodeAttrTable;
        delete m_edgeAttrTable;
    }

    AttrWidget* GraphDesigner::addGeneralAttr(QTreeWidgetItem* itemRoot,
        const QString& attrName, QWidget* customWidget)
    {
        AttrWidget* widget = new AttrWidget(
            m_mainApp->generalAttrsScope().value(attrName), this, customWidget);
        m_attrWidgets.insert(attrName, widget);

        QTreeWidgetItem* item = new QTreeWidgetItem(itemRoot);
        item->setText(0, attrName);
        item->setToolTip(0, attrName);
        m_ui->treeWidget->setItemWidget(item, 1, widget);

        return widget;
    }

    void GraphDesigner::addTreeWidgetItem(QTreeWidgetItem* itemRoot,
        const QString& attrName, QWidget* customWidget, int i)
    {
        QTreeWidgetItem* item = new QTreeWidgetItem(itemRoot);
        item->setText(0, attrName);
        item->setToolTip(0, attrName);
        m_ui->treeWidget->setItemWidget(item, 1, customWidget);
    }

    void GraphDesigner::slotGraphSelected(int cbIdx)
    {
        auto cb = qobject_cast<QComboBox*>(m_attrWidgets.value(GENERAL_ATTR_GRAPHID)->widget());
        m_selectedGraphKey = cb->itemData(cbIdx).value<PluginKey>();
        pluginSelected(m_treeItemGraphs, m_selectedGraphKey);

        const GraphPlugin* graph = m_mainApp->graph(m_selectedGraphKey);

        if (graph) {
            m_treeItemGraphs->child(m_graphTypeIdx)->setHidden(
                graph->validGraphTypes().size() < 2);
            m_treeItemGraphs->child(m_edgesAttrsIdx)->setHidden(
                !graph->supportsEdgeAttrsGen());
        } else {
            m_treeItemGraphs->child(m_graphTypeIdx)->setHidden(true);
            m_treeItemGraphs->child(m_edgesAttrsIdx)->setHidden(true);
        }
    }

    void GraphDesigner::addPluginAttrs(QTreeWidgetItem* tree, const Plugin* plugin)
    {
        if (plugin->pluginAttrsNames().size() <= 0) {
            return; // nothing to add
        }

        QTreeWidgetItemIterator it(m_ui->treeWidget);
        while (*it) {
            if ((*it)->parent() == tree &&
                (*it)->data(0, Qt::UserRole).value<PluginKey>() == plugin->key()) {
                return; // plugins already exists
            }
            ++it;
        }

        const QString key_ = Plugin::keyStr(plugin->key()) + "_";
        for (const QString& attrName : plugin->pluginAttrsNames()) {
            QTreeWidgetItem* item = new QTreeWidgetItem(tree);
            item->setText(0, attrName);
            item->setToolTip(0, attrName);
            item->setData(0, Qt::UserRole, QVariant::fromValue(plugin->key()));
            item->setHidden(true);
            auto attrWidget = new AttrWidget(plugin->pluginAttrRange(attrName), this);
            m_ui->treeWidget->setItemWidget(item, 1, attrWidget);
            // add the uid as prefix to avoid clashes.
            m_attrWidgets.insert(key_ + attrWidget->attrName(), attrWidget);
        }
    }

    void GraphDesigner::pluginSelected(QTreeWidgetItem* itemRoot, const PluginKey& key)
    {
        for (int i = 0; i < itemRoot->childCount(); ++i) {
            QTreeWidgetItem* row = itemRoot->child(i);
            PluginKey _key = row->data(0, Qt::UserRole).value<PluginKey>();
            bool hide = !_key.first.isEmpty() && _key != key;
            row->setHidden(hide);
            m_ui->treeWidget->itemWidget(row, 1)->setDisabled(hide);
        }
    }

    void GraphDesigner::slotPluginAdded(const Plugin* plugin)
    {
        int numVersions = 0;

        QComboBox* cb;
        if (plugin->type() == PluginType::Graph) {
            addPluginAttrs(m_treeItemGraphs, plugin);


            cb = qobject_cast<QComboBox*>(m_attrWidgets.value(GENERAL_ATTR_GRAPHID)->widget());
            numVersions = m_mainApp->graphs().values(plugin->id()).size();
        }
        else if (plugin->type() == PluginType::Model) {
            //ignore model plugins
            return;
        }
        else {
            qFatal("invalid plugin type!");
        }

        // let's append the version number only when there's
        // more than one version available
        QString label;
        if (numVersions == 1) {
            label = plugin->id();
        }
        else if (numVersions == 2) {
            // this is the first time we have 2 versions of the same plugin
            // so, we must fix the label of the existing plugin as well
            int existingIdx = cb->findText(plugin->id());
            PluginKey existing = cb->itemData(existingIdx).value<PluginKey>();
            Q_ASSERT(existingIdx >= 0 && existing.first == plugin->id());
            cb->setItemText(existingIdx, Plugin::keyStr(existing));
            label = Plugin::keyStr(plugin->key());
        }
        else if (numVersions > 2) {
            label = Plugin::keyStr(plugin->key());
        }
        else {
            qFatal("the plugin was not loaded correctly! It should never happen!");
        }

        cb->blockSignals(true);
        cb->insertItem(cb->count(), label, QVariant::fromValue(plugin->key()));
        cb->model()->sort(0);
        cb->setCurrentText("--"); // moves to --
        cb->blockSignals(false);
    }

    void GraphDesigner::slotPluginRemoved(PluginKey key, PluginType type)
    {
        QTreeWidgetItem* tree;
        QComboBox* cb;

        tree = m_treeItemGraphs;
        cb = qobject_cast<QComboBox*>(m_attrWidgets.value(GENERAL_ATTR_GRAPHID)->widget());


        cb->blockSignals(true);
        cb->removeItem(cb->findData(QVariant::fromValue(key)));
        cb->blockSignals(false);

        // remove all fields which belog to this plugin
        const QString keyStr_ = Plugin::keyStr(key) + "_";
        auto it = m_attrWidgets.begin();
        while (it != m_attrWidgets.end()) {
            if (it.key().startsWith(keyStr_)) {
                it = m_attrWidgets.erase(it);
            }
            else {
                ++it;
            }
        }
        for (int i = 0; i < tree->childCount(); ++i) {
            if (tree->child(i)->data(0, Qt::UserRole).value<PluginKey>() == key) {
                delete tree->takeChild(i);
                --i;
            }
        }
    }

    void GraphDesigner::slotAddGraphDesigner()
    {
        AttributesScope attrsScope;
        QString error;
        parseAttributes(error);

        // auto col0 = AttributeRange::parse(0, "test0", "int[0,1000]");
        //attrsScope.insert(col0->attrName(), col0);
        GraphInputsPtr inpts = readInputs(m_graphDesignerId, error);
        if (!error.isEmpty()) {
            qDebug() << error;
            return;
        }
        AbstractGraph* a_g = dynamic_cast<AbstractGraph*>(inpts->graphPlugin()->create());
        GraphWidget* graph = new GraphWidget(GraphWidget::Mode::Graph, a_g, m_nodeAttrs, this);
        graph->setVisible(true);

    }

    void GraphDesigner::parseAttributes(QString& error)
    {
        QString _name;
        QString _attrRange;

        for (int i = 0; i < m_nodeAttrTable->rowCount(); ++i) {
            _name = m_nodeAttrTable->item(i, 0)->text();
            _attrRange = m_nodeAttrTable->item(i, 1)->text();
            auto attrs = AttributeRange::parse(i, _name, _attrRange);
            if (!attrs.get()->isValid()) {
                error = "Unable to parse node attributes";
                return;
            }
            m_nodeAttrs.insert(attrs->attrName(), attrs);
        }

        for (int i = 0; i < m_edgeAttrTable->rowCount(); ++i) {
            _name = m_edgeAttrTable->item(i, 0)->text();
            _attrRange = m_edgeAttrTable->item(i, 1)->text();
            auto attrs = AttributeRange::parse(i, _name, _attrRange);
            if (!attrs.get()->isValid()) {
                error = "Unable to parse edge attributes";
                return;
            }
            m_edgeAttrs.insert(attrs->attrName(), attrs);
        }
    }

    void GraphDesigner::slotEdgesWidget()
    {
        QString error;
        parseAttributes(error);

        if (!error.isEmpty()) {
            QMessageBox::warning(this, "Attributes Generator",
                error);
            return;
        }

        if (m_selectedGraphKey == PluginKey()) {
            QMessageBox::warning(this, "Graph",
                "Please, select a valid 'graphId' first.");
            return;
        }

        if (m_edgeAttrs.empty()) {
            QMessageBox::information(this, "Attributes Generator",
                "You haven't provided any valid edge attributes.");
            return;
        }

        const QString& cmd = m_attrWidgets.value(GENERAL_ATTR_EDGEATTRS)->value().toQString();
        AttrsGenDlg* adlg = new AttrsGenDlg(this, AttrsGenDlg::Mode::Edges,
            m_edgeAttrs, cmd);

        if (adlg->exec() == QDialog::Accepted) {
            m_attrWidgets.value(GENERAL_ATTR_EDGEATTRS)->setValue(adlg->readCommand());
        }
        adlg->deleteLater();
    }

    void GraphDesigner::slotNodesWidget()
    {
        QString error;
        parseAttributes(error);
        if (!error.isEmpty()) {
            QMessageBox::warning(this, "Attributes Generator",
                error);
            return;
        }

        const QString& cmd = m_attrWidgets.value(GENERAL_ATTR_NODES)->value().toQString();

        AttrsGenDlg* adlg = new AttrsGenDlg(this, AttrsGenDlg::Mode::Nodes,
            m_nodeAttrs, cmd);

        if (adlg->exec() == QDialog::Accepted) {
            m_attrWidgets.value(GENERAL_ATTR_NODES)->setValue(adlg->readCommand());
        }
        adlg->deleteLater();
    }

    std::unique_ptr<GraphInputs> GraphDesigner::readInputs(const int graphId, QString& error) const
    {
        if (m_selectedGraphKey == PluginKey()) {
            error = "Please, select a valid 'graphId'.";
            return nullptr;
        }

        QStringList header;
        QStringList values;

        header << GENERAL_ATTR_EXPID;

        values << QString::number(graphId);

        const QString graphKey_ = Plugin::keyStr(m_selectedGraphKey) + "_";

        for (auto it = m_attrWidgets.cbegin(); it != m_attrWidgets.cend(); ++it) {
            if (!it.value()->isEnabled()) {
                continue;
            }

            // added later
            if (it.key() == GENERAL_ATTR_GRAPHID ||
                it.key() == GENERAL_ATTR_MODELID) {
                continue;
            }

            // if it's a plugin field, we need to remove the
            // version number from the key
            if (it.key().startsWith(graphKey_)) {
                header << m_selectedGraphKey.first + "_" + it.value()->attrName();
            }
            else {
                header << it.value()->attrName();
            }

            values << it.value()->value().toQString('g', 8);
        }

        header << GENERAL_ATTR_GRAPHID << GENERAL_ATTR_GRAPHVS;
        values << m_selectedGraphKey.first << QString::number(m_selectedGraphKey.second);
        qDebug() << header << values;

        QString errorMsg;
        qDebug() << header << values;

        auto inputs = GraphInputs::parse(m_mainApp, header, values, errorMsg);
        if (!inputs || !errorMsg.isEmpty()) {
            error += "Unable to create the graph.\nError: \"" + errorMsg + "\"";
            qDebug() << error;
            return nullptr; // this GUI should never create experiments with errors
        }
        return inputs;
    }

    void GraphDesigner::slotNodeTableUpdate(int val)
    {
        m_nodeAttrTable->setRowCount(val);
    }

    void GraphDesigner::slotEdgeTableUpdate(int val)
    {
        m_edgeAttrTable->setRowCount(val);
    }

} // evoplex
