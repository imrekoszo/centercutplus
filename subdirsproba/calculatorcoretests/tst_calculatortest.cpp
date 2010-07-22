#include <algorithm>
#include <fstream>
#include <sstream>

#include <QtCore/QString>
#include <QtTest/QtTest>

#include "calculatorcore/calculator.h"

namespace
{
    typedef QVector<double> doubleVector_type;
    bool doubleEquals(double a, double b)
    {
        return qAbs(a - b) < 0.0000001;
    }
}

Q_DECLARE_METATYPE(doubleVector_type)

class CalculatorTest : public QObject
{
    Q_OBJECT

public:
    CalculatorTest();

private Q_SLOTS:
    void AddTest_Ok();
    void AddTest_Ok_data();
    void MultiplyTest_Ok();
    void MultiplyTest_Ok_data();
    void ManipulateTest_Ok();
    void ManipulateTest_Ok_data();
};

CalculatorTest::CalculatorTest()
{
}

void CalculatorTest::AddTest_Ok()
{
    QFETCH(int, a);
    QFETCH(int, b);
    QFETCH(int, expected);

    Calculator calculator;

    QCOMPARE(calculator.Add(a, b), expected);
}

void CalculatorTest::AddTest_Ok_data()
{
    QTest::addColumn<int>("a");
    QTest::addColumn<int>("b");
    QTest::addColumn<int>("expected");

    std::ifstream ifile("add0.log");
    std::string header;
    int a, b, expected;
    QString testCaseName("Add Testcase %0");
    int testCaseIndex = 0;

    while(ifile >> header >> a >> b >> expected && header == "Add")
    {
        QTest::newRow(testCaseName.arg(testCaseIndex++).toLocal8Bit().data())
                << a << b << expected;
    }
}

void CalculatorTest::MultiplyTest_Ok()
{
    QFETCH(double, a);
    QFETCH(double, b);
    QFETCH(double, expected);

    Calculator calculator;

    QCOMPARE(calculator.Multiply(a, b), expected);
}

void CalculatorTest::MultiplyTest_Ok_data()
{
    QTest::addColumn<double>("a");
    QTest::addColumn<double>("b");
    QTest::addColumn<double>("expected");

    std::ifstream ifile("multiply0.log");
    std::string header;
    double a, b, expected;
    QString testCaseName("Multiply Testcase %0");
    int testCaseIndex = 0;

    while(ifile >> header >> a >> b >> expected && header == "Multiply")
    {
        QTest::newRow(testCaseName.arg(testCaseIndex++).toLocal8Bit().data())
                << a << b << expected;
    }
}

void CalculatorTest::ManipulateTest_Ok()
{
    QFETCH(doubleVector_type, data);
    QFETCH(unsigned, length);
    QFETCH(double, multiplier);
    QFETCH(doubleVector_type, expected);

    Calculator calculator;
    calculator.Manipulate(data.begin(), length, multiplier);

    QVERIFY(std::equal(data.begin(), data.end(), expected.begin(),
                       doubleEquals));
}

void CalculatorTest::ManipulateTest_Ok_data()
{
    QTest::addColumn<doubleVector_type>("data");
    QTest::addColumn<unsigned>("length");
    QTest::addColumn<double>("multiplier");
    QTest::addColumn<doubleVector_type>("expected");

    std::ifstream ifile("manipulate0.log");
    std::string header;
    QString testCaseName("Manipulate Testcase %0");
    int testCaseIndex = 0;

    while(ifile >> header)
    {
        if(header != "Manipulate")
        {
            continue;
        }

        unsigned length;
        double multiplier;

        if(!(ifile >> length) || !(ifile >> multiplier))
        {
            continue;
        }

        doubleVector_type data(static_cast<int>(length));
        doubleVector_type expected(static_cast<int>(length));
        double buffer;

        for(unsigned i = 0; i < length && ifile >> buffer; ++i)
        {
            data[i] = buffer;
        }

        for(unsigned i = 0; i < length && ifile >> buffer; ++i)
        {
            expected[i] = buffer;
        }

        if(data.count() != expected.count() ||
                static_cast<unsigned>(data.count()) != length)
        {
            continue;
        }

        QTest::newRow(testCaseName.arg(testCaseIndex++).toLocal8Bit().data())
                << data << length << multiplier << expected;
    }
}

QTEST_APPLESS_MAIN(CalculatorTest);

#include "tst_calculatortest.moc"
