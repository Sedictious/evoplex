/**
*  This file is part of Evoplex.
*
*  Evoplex is a multi-agent system for networks.
*  Copyright (C) 2019 - Eleftheria Chatziargyriou
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

#ifndef GRAPHDESIGNERPAGE_H
#define GRAPHDESIGNERPAGE_H

#include <QMainWindow>

#include "maingui.h"
#include "core/mainapp.h"

class Ui_GraphDesignerPage;
class MainGUI;

namespace evoplex {

class GraphDesigner;

class GraphDesignerPage : public QWidget
{
    Q_OBJECT

public:
    explicit GraphDesignerPage(MainGUI* mainGUI);
    inline const int currentGraphId() const;

    ~GraphDesignerPage();

private:
    Ui_GraphDesignerPage * m_ui;
    MainApp* m_mainApp;
    MainGUI* m_mainGUI;
    QMainWindow* m_innerWindow;
    GraphDesigner* m_curGraphDesigner;
    int m_curGraphId;

private slots:
    void slotNewGraph();
    void slotCloseGraphDesigner();
};

inline const int GraphDesignerPage::currentGraphId()  const
{ return m_curGraphId; }

}

#endif // GRAPHDESIGNERPAGE_H
