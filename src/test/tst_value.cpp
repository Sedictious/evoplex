/**
 *  This file is part of Evoplex.
 *
 *  Evoplex is a multi-agent system for networks.
 *  Copyright (C) 2018 - Marcos Cardinot <marcos@cardinot.net>
 *  Copyright (C) 2018 - Ethan Padden <e.padden1@nuigalway.ie>
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

#include <stdexcept>
#include <QtTest>
#include <value.h>

using namespace evoplex;

class TestValue: public QObject
{
    Q_OBJECT

private slots:
    void initTestCase() {}
    void cleanupTestCase() {}
    void tst_valueInvalid();
    void tst_valueBool();
    void tst_valueDouble();
    void tst_valueInt();
    void tst_valueChar();
    void tst_valueString();
};

void TestValue::tst_valueInvalid()
{
    Value v;

    // VALUE TESTS
    // Testing validity
    QCOMPARE(v.isValid(), false);

    // Testing type
    QVERIFY(v.type() == Value::INVALID);
    QVERIFY(!v.isBool());
    QVERIFY(!v.isChar());
    QVERIFY(!v.isDouble());
    QVERIFY(!v.isInt());
    QVERIFY(!v.isString());

    // Testing value
    QCOMPARE(v.toQString(), QString());
    QVERIFY_EXCEPTION_THROWN(v.toBool(), std::logic_error);
    QVERIFY_EXCEPTION_THROWN(v.toChar(), std::logic_error);
    QVERIFY_EXCEPTION_THROWN(v.toDouble(), std::logic_error);
    QVERIFY_EXCEPTION_THROWN(v.toInt(), std::logic_error);
    QVERIFY_EXCEPTION_THROWN(v.toString(), std::logic_error);

    // COMPARISONS
    // same invalid type
    Value i1, i2;
    QCOMPARE(i1 == i2, true);
    QCOMPARE(i1 != i2, false);
    QVERIFY_EXCEPTION_THROWN(i1 >= i2, std::invalid_argument);
    QVERIFY_EXCEPTION_THROWN(i1 <= i2, std::invalid_argument);
    QVERIFY_EXCEPTION_THROWN(i1 >  i2, std::invalid_argument);
    QVERIFY_EXCEPTION_THROWN(i1 <  i2, std::invalid_argument);

    // different valid types
    const Value v1(1);
    const Value v2(1.0);
    QCOMPARE(v1 == v2, false);
    QCOMPARE(v1 != v2, true);
    QVERIFY_EXCEPTION_THROWN(v1 >= v2, std::invalid_argument);
    QVERIFY_EXCEPTION_THROWN(v1 <= v2, std::invalid_argument);
    QVERIFY_EXCEPTION_THROWN(v1 >  v2, std::invalid_argument);
    QVERIFY_EXCEPTION_THROWN(v1 <  v2, std::invalid_argument);

    // creating a Value from another Value
    Value vInvalid;
    QCOMPARE(Value(vInvalid) == vInvalid, true);
    QCOMPARE(Value(vInvalid).isValid(), false);
}

void TestValue::tst_valueBool()
{
    auto tst = [](Value v, bool b) {
        // Testing type
        QCOMPARE(v.isValid(), true);
        QVERIFY(v.type() == Value::BOOL);
        QVERIFY(v.isBool());
        QVERIFY(!v.isChar());
        QVERIFY(!v.isDouble());
        QVERIFY(!v.isInt());
        QVERIFY(!v.isString());
        // Testing value
        QCOMPARE(v.toBool(), b);
        QCOMPARE(v.toQString(), b ? QString("1") : QString("0"));
        QVERIFY_EXCEPTION_THROWN(v.toChar(), std::logic_error);
        QVERIFY_EXCEPTION_THROWN(v.toDouble(), std::logic_error);
        QVERIFY_EXCEPTION_THROWN(v.toInt(), std::logic_error);
        QVERIFY_EXCEPTION_THROWN(v.toString(), std::logic_error);
    };

    auto tstBool = [tst](bool b){
        Value v1(b); // constructor
        tst(v1, b);
        Value v2 = b; // constructor
        tst(v2, b);
        Value v3 = Value(b); // operator=(Value)
        tst(v3, b);
        std::vector<bool> bs = {b};
        Value v4 = bs.at(0); // specialized vector<bool>
        tst(v3, b);
    };

    tstBool(true);
    tstBool(false);

    // COMPARISON TESTS
    const Value v1(true);
    const Value v2(false);
    QCOMPARE(v1 == v2, false);
    QCOMPARE(v1 != v2, true);
    QCOMPARE(v1 >= v2, true);
    QCOMPARE(v1 <= v2, false);
    QCOMPARE(v1 >  v2, true);
    QCOMPARE(v1 <  v2, false);

    // creating a Value from another Value
    Value vBool(false);
    QCOMPARE(Value(vBool), vBool);
    QCOMPARE(Value(vBool).isBool(), true);
    QCOMPARE(Value(vBool).toBool(), false);
}

void TestValue::tst_valueDouble()
{
    Value v;

    // VALUE TESTS
    v = Value(4.5123788);

    // Testing validity
    QCOMPARE(v.isValid(), true);

    // Testing type
    QVERIFY(v.type() == Value::DOUBLE);
    QVERIFY(v.isDouble());
    QVERIFY(!v.isBool());
    QVERIFY(!v.isChar());
    QVERIFY(!v.isInt());
    QVERIFY(!v.isString());

    // Testing value
    QCOMPARE(v.toDouble(), 4.5123788);
    QCOMPARE(v.toQString(), QString("4.5123788"));
    QVERIFY_EXCEPTION_THROWN(v.toBool(), std::logic_error);
    QVERIFY_EXCEPTION_THROWN(v.toChar(), std::logic_error);
    QVERIFY_EXCEPTION_THROWN(v.toInt(), std::logic_error);
    QVERIFY_EXCEPTION_THROWN(v.toString(), std::logic_error);

    // COMPARISON TESTS
    const Value v1(103.200000008);
    const Value v2(103.200000007);
    QCOMPARE(v1 == v2, false);
    QCOMPARE(v1 != v2, true);
    QCOMPARE(v1 >= v2, true);
    QCOMPARE(v1 <= v2, false);
    QCOMPARE(v1 >  v2, true);
    QCOMPARE(v1 <  v2, false);

    const Value v3(103.2000000079);
    QCOMPARE(v1 == v3, true);
    QCOMPARE(v1 != v3, false);
    QCOMPARE(v1 >= v3, true);
    QCOMPARE(v1 <= v3, true);
    QCOMPARE(v1 >  v3, false);
    QCOMPARE(v1 <  v3, false);

    // creating a Value from another Value
    Value vDouble(9.31231);
    QCOMPARE(Value(vDouble), vDouble);
    QCOMPARE(Value(vDouble).isDouble(), true);
    QCOMPARE(Value(vDouble).toDouble(), 9.31231);
}

void TestValue::tst_valueInt()
{
    Value v;

    // VALUE TESTS
    v = Value(-10);

    // Testing validity
    QCOMPARE(v.isValid(), true);

    // Testing type
    QVERIFY(v.type() == Value::INT);
    QVERIFY(v.isInt());
    QVERIFY(!v.isBool());
    QVERIFY(!v.isChar());
    QVERIFY(!v.isDouble());
    QVERIFY(!v.isString());

    // Testing value
    QCOMPARE(v.toInt(), -10);
    QCOMPARE(v.toQString(), QString("-10"));
    QVERIFY_EXCEPTION_THROWN(v.toBool(), std::logic_error);
    QVERIFY_EXCEPTION_THROWN(v.toChar(), std::logic_error);
    QVERIFY_EXCEPTION_THROWN(v.toDouble(), std::logic_error);
    QVERIFY_EXCEPTION_THROWN(v.toString(), std::logic_error);

    // COMPARISON TESTS
    const Value v1(100);
    const Value v2 = Value(50);
    QCOMPARE(v1 == v2, false);
    QCOMPARE(v1 != v2, true);
    QCOMPARE(v1 >= v2, true);
    QCOMPARE(v1 <= v2, false);
    QCOMPARE(v1 >  v2, true);
    QCOMPARE(v1 <  v2, false);

    // creating a Value from another Value
    Value vInt(9765112);
    QCOMPARE(Value(vInt), vInt);
    QCOMPARE(Value(vInt).isInt(), true);
    QCOMPARE(Value(vInt).toInt(), 9765112);
}

void TestValue::tst_valueChar()
{
    Value v;

    // VALUE TESTS
    char chr = 'a';
    v = Value(chr);

    // Testing validity
    QCOMPARE(v.isValid(), true);

    // Testing type
    // Verify that it is a string, as chr is a character pointer.
    QVERIFY(v.type() == Value::CHAR);
    QVERIFY(v.isChar());
    QVERIFY(!v.isString());
    QVERIFY(!v.isBool());
    QVERIFY(!v.isDouble());
    QVERIFY(!v.isInt());

    // Testing value
    QCOMPARE(v.toChar(), chr);
    QCOMPARE(v.toQString(), QString("a"));
    QVERIFY_EXCEPTION_THROWN(v.toBool(), std::logic_error);
    QVERIFY_EXCEPTION_THROWN(v.toDouble(), std::logic_error);
    QVERIFY_EXCEPTION_THROWN(v.toInt(), std::logic_error);
    QVERIFY_EXCEPTION_THROWN(v.toString(), std::logic_error);

    // COMPARISON TESTS
    const Value v1('d');
    const Value v2('c');
    QCOMPARE(v1 == v2, false);
    QCOMPARE(v1 != v2, true);
    QCOMPARE(v1 >= v2, true);
    QCOMPARE(v1 <= v2, false);
    QCOMPARE(v1 >  v2, true);
    QCOMPARE(v1 <  v2, false);

    // creating a Value from another Value
    const Value vChar('a');
    QCOMPARE(Value(vChar), vChar);
    QCOMPARE(Value(vChar).isChar(), true);
    QCOMPARE(Value(vChar).toChar(), 'a');
}

void TestValue::tst_valueString()
{
    Value v;

    // VALUE TESTS
    QString str("abc£ãã&");
    const char* chr = "abc£ãã&";
    v = Value(str);

    // Testing validity
    QCOMPARE(v.isValid(), true);

    // Testing type
    QVERIFY(v.type() == Value::STRING);
    QVERIFY(v.isString());
    QVERIFY(!v.isBool());
    QVERIFY(!v.isChar());
    QVERIFY(!v.isDouble());
    QVERIFY(!v.isInt());

    // Testing value
    QCOMPARE(v.toString(), chr);
    QCOMPARE(v.toQString(), str);
    QVERIFY_EXCEPTION_THROWN(v.toBool(), std::logic_error);
    QVERIFY_EXCEPTION_THROWN(v.toChar(), std::logic_error);
    QVERIFY_EXCEPTION_THROWN(v.toDouble(), std::logic_error);
    QVERIFY_EXCEPTION_THROWN(v.toInt(), std::logic_error);

    // COMPARISON TESTS
    const Value v1("aaaabc$ iua^sa");
    const Value v2("aaaabc$ iua^sd");
    QCOMPARE(v1 == v2, false);
    QCOMPARE(v1 != v2, true);
    QCOMPARE(v1 >= v2, false);
    QCOMPARE(v1 <= v2, true);
    QCOMPARE(v1 > v2, false);
    QCOMPARE(v1 < v2, true);
    const Value v3("abcd efg");
    const Value v4("abcd efg");
    QCOMPARE(v3 == v4, true);
    QCOMPARE(v3 != v4, false);
    QCOMPARE(v3 >= v4, true);
    QCOMPARE(v3 <= v4, true);
    QCOMPARE(v3 > v4, false);
    QCOMPARE(v3 < v4, false);

    // creating a Value from another Value
    Value vString("evoplex");
    QCOMPARE(Value(vString), vString);
    QCOMPARE(Value(vString).isString(), true);
    QCOMPARE(Value(vString).toString(), "evoplex");
    Value vCopy("aaaa");
    vCopy = Value("");
    QCOMPARE(vCopy, Value(""));
    Value vCopy2 = Value("");
    QCOMPARE(vCopy2, Value(""));
}

QTEST_MAIN(TestValue)
#include "tst_value.moc"
