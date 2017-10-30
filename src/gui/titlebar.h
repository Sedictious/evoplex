/**
 * Copyright (C) 2017 - Marcos Cardinot
 * @author Marcos Cardinot <mcardinot@gmail.com>
 */

#ifndef TITLEBAR_H
#define TITLEBAR_H

#include <QDockWidget>
#include <QWidget>

#include "core/experiment.h"

class Ui_TitleBar;

namespace evoplex {

class TitleBar : public QWidget
{
    Q_OBJECT

public:
    explicit TitleBar(Experiment* exp, QDockWidget* parent);
    ~TitleBar();

signals:
    void trialSelected(int);

protected:
    virtual void paintEvent(QPaintEvent* pe);

private:
    Ui_TitleBar* m_ui;
};
}

#endif // TITLEBAR_H
