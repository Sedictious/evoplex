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

#include <QtTest>
#include <edge.h>
#include <node.h>

namespace evoplex {
class TestEdge: public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void cleanupTestCase() {}

private:
    Node::constructor_key key;
    NodePtr m_nodeA;
    NodePtr m_nodeB;
};

void TestEdge::initTestCase()
{
    m_nodeA = std::make_shared<UNode>(key, 0, Attributes());
    m_nodeB = std::make_shared<UNode>(key, 1, Attributes());
}

} // evoplex
QTEST_MAIN(evoplex::TestEdge)
#include "tst_edge.moc"
