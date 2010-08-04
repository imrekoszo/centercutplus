#include <stdint.h>
#include <cstdlib>
#include <fstream>
#include <sstream>

#include <QtCore/QString>
#include <QtTest/QtTest>

#include <dsp_centercutplus/centercutengine.h>
#include <dsp_centercutplus/ffengine.h>

class CentercutplustestsTest : public QObject
{
    Q_OBJECT

public:
    CentercutplustestsTest();

private Q_SLOTS:
    void CenterCutProcessSamplesTest();
    void ConvertSamplesTest();
    void ConvertSamplesTest_data();
    void CreatePostWindowTest();
    void CreatePostWindowTest_data();
    void VDComputeFHTTest();
    void VDComputeFHTTest_data();
    void VDCreateBitRevTableTest();
    void VDCreateBitRevTableTest_data();
    void VDCreateHalfSineTableTest();
    void VDCreateHalfSineTableTest_data();
    void VDCreateRaisedCosineWindowTest();
    void VDCreateRaisedCosineWindowTest_data();

private:
    std::string _logDir;
};

namespace
{
    typedef QVector<double> doubleVector_type;
    typedef QVector<unsigned> uintVector_type;
    typedef QVector<uint8_t> byteVector_type;

    bool doubleEquals(double a, double b)
    {
        static const double kAlmostZero = 0.0000000001;
        return qAbs(a - b) < std::max(qAbs(a) * kAlmostZero, kAlmostZero);
    }
    const std::string dataFileExt = ".log";
    const std::string testCasePostfix = " Testcase %0";
}

Q_DECLARE_METATYPE(doubleVector_type)
Q_DECLARE_METATYPE(uintVector_type)
Q_DECLARE_METATYPE(byteVector_type)


CentercutplustestsTest::CentercutplustestsTest()
{
    // INIT TESTDATA DIR
    const char* cclogdir = std::getenv("CCLOGDIR");
    _logDir = cclogdir;
    if(_logDir.length() > 0)
    {
        _logDir += "\\";
    }
    // /INIT TESTDATA DIR
}

void CentercutplustestsTest::CenterCutProcessSamplesTest()
{
    QVERIFY2(false, "Test case incomplete");
}

void CentercutplustestsTest::ConvertSamplesTest()
{
    QFETCH(int, type);
    QFETCH(byteVector_type, sampB);
    QFETCH(doubleVector_type, sampD);
    QFETCH(int, sampleCount);
    QFETCH(int, bitsPerSample);
    QFETCH(int, chanCount);
    QFETCH(byteVector_type, expectedSampB);
    QFETCH(doubleVector_type, expectedSampD);

    static const int BYTES_TO_DOUBLE = 0;
    CenterCutEngine sut;
    sut.ConvertSamples(type, sampB.begin(), sampD.begin(), sampleCount,
                       bitsPerSample, chanCount);

    if(type == BYTES_TO_DOUBLE)
    {
        QVERIFY(std::equal(sampD.begin(), sampD.end(), expectedSampD.begin(),
                           doubleEquals));
    }
    else
    {
        QCOMPARE(sampB, expectedSampB);
    }
}

void CentercutplustestsTest::ConvertSamplesTest_data()
{
    static const int BYTES_TO_DOUBLE = 0;

    const std::string testName = "ConvertSamples";
    std::ifstream ifile((_logDir + testName + dataFileExt).c_str());
    const QString testCaseName((testName + testCasePostfix).c_str());
    int testCaseIndex = 0;

    QTest::addColumn<int>("type");
    QTest::addColumn<byteVector_type>("sampB");
    QTest::addColumn<doubleVector_type>("sampD");
    QTest::addColumn<int>("sampleCount");
    QTest::addColumn<int>("bitsPerSample");
    QTest::addColumn<int>("chanCount");
    QTest::addColumn<byteVector_type>("expectedSampB");
    QTest::addColumn<doubleVector_type>("expectedSampD");

    std::string header;
    while(ifile >> header && header == testName)
    {
        int type, sampleCount, bitsPerSample, chanCount;

        if(!(ifile >> type >> sampleCount >> bitsPerSample >> chanCount))
        {
            continue;
        }

        const int bytesPerSample = (bitsPerSample + 7) / 8;
        const int bCount = sampleCount * bytesPerSample * chanCount;
        const int dCount = sampleCount * chanCount;
        byteVector_type sampB(bCount), expectedSampB(bCount);
        doubleVector_type sampD(dCount), expectedSampD(dCount);

        if(type == BYTES_TO_DOUBLE)
        {
            uint32_t bBuffer;
            for(int i = 0; i < bCount && ifile >> bBuffer; ++i)
            {
                sampB[i] = static_cast<uint8_t>(bBuffer);
            }

            double expectedBuffer;
            for(int i = 0; i < dCount && ifile >> expectedBuffer; ++i)
            {
                expectedSampD[i] = expectedBuffer;
            }

            if(sampB.count() != bCount || expectedSampD.count() != dCount)
            {
                continue;
            }
        }
        else
        {
            double dBuffer;
            for(int i = 0; i < dCount && ifile >> dBuffer; ++i)
            {
                sampD[i] = dBuffer;
            }

            uint32_t expectedBuffer;
            for(int i = 0; i < bCount && ifile >> expectedBuffer; ++i)
            {
                expectedSampB[i] = static_cast<uint8_t>(expectedBuffer);
            }

            if(sampD.count() != dCount || expectedSampB.count() != bCount)
            {
                continue;
            }
        }

        QTest::newRow(testCaseName.arg(testCaseIndex++).toLocal8Bit().data())
                << type << sampB << sampD << sampleCount << bitsPerSample
                << chanCount << expectedSampB << expectedSampD;
    }
}

void CentercutplustestsTest::CreatePostWindowTest()
{
    QFETCH(int, windowSize);
    QFETCH(int, power);
    QFETCH(doubleVector_type, expected);
    doubleVector_type dst(windowSize);

    FFEngine::CreatePostWindow(dst.begin(), windowSize, power);

    QVERIFY(std::equal(dst.begin(), dst.end(), expected.begin(),
                       doubleEquals));
}

void CentercutplustestsTest::CreatePostWindowTest_data()
{
    const std::string testName = "CreatePostWindow";
    std::ifstream ifile((_logDir + testName + dataFileExt).c_str());
    const QString testCaseName((testName + testCasePostfix).c_str());
    int testCaseIndex = 0;

    QTest::addColumn<int>("windowSize");
    QTest::addColumn<int>("power");
    QTest::addColumn<doubleVector_type>("expected");

    std::string header;
    while(ifile >> header && header == testName)
    {
        int windowSize;
        int power;

        if(!(ifile >> windowSize >> power))
        {
            continue;
        }

        double buffer;
        doubleVector_type expected(windowSize);
        for(int i = 0; i < windowSize && ifile >> buffer; ++i)
        {
            expected[i] = buffer;
        }

        if(expected.count() != windowSize)
        {
            continue;
        }

        QTest::newRow(testCaseName.arg(testCaseIndex++).toLocal8Bit().data())
                << windowSize << power << expected;
    }
}

void CentercutplustestsTest::VDComputeFHTTest()
{
    QFETCH(doubleVector_type, A);
    QFETCH(int, nPoints);
    QFETCH(doubleVector_type, sinTab);
    QFETCH(doubleVector_type, expected);

    FFEngine::VDComputeFHT(A.begin(), nPoints, sinTab.constBegin());

    QVERIFY(std::equal(A.begin(), A.end(), expected.begin(),
                       doubleEquals));
}

void CentercutplustestsTest::VDComputeFHTTest_data()
{
    const std::string testName = "VDComputeFHT";
    std::ifstream ifile((_logDir + testName + dataFileExt).c_str());
    const QString testCaseName((testName + testCasePostfix).c_str());
    int testCaseIndex = 0;

    QTest::addColumn<doubleVector_type>("A");
    QTest::addColumn<int>("nPoints");
    QTest::addColumn<doubleVector_type>("sinTab");
    QTest::addColumn<doubleVector_type>("expected");

    std::string header;
    while(ifile >> header && header == testName)
    {
        int nPoints;

        if(!(ifile >> nPoints))
        {
            continue;
        }

        double aBuffer, sinTabBuffer;
        doubleVector_type a(nPoints), sinTab(nPoints);
        for(    int i = 0;
                i < nPoints && ifile >> aBuffer >> sinTabBuffer;
                ++i)
        {
            a[i] = aBuffer;
            sinTab[i] = sinTabBuffer;
        }

        double expectedBuffer;
        doubleVector_type expected(nPoints);
        for(    int i = 0;
                i < nPoints && ifile >> expectedBuffer;
                ++i)
        {
            expected[i] = expectedBuffer;
        }

        if( expected.count() != nPoints
                || a.count() != nPoints
                || sinTab.count() != nPoints)
        {
            continue;
        }

        QTest::newRow(testCaseName.arg(testCaseIndex++).toLocal8Bit().data())
                << a << nPoints << sinTab << expected;
    }
}

void CentercutplustestsTest::VDCreateBitRevTableTest()
{
    QFETCH(int, n);
    QFETCH(uintVector_type, expected);
    uintVector_type dst(n);

    FFEngine::VDCreateBitRevTable(dst.begin(), n);

    QVERIFY(expected == dst);
}

void CentercutplustestsTest::VDCreateBitRevTableTest_data()
{
    const std::string testName = "VDCreateBitRevTable";
    std::ifstream ifile((_logDir + testName + dataFileExt).c_str());
    const QString testCaseName((testName + testCasePostfix).c_str());
    int testCaseIndex = 0;

    QTest::addColumn<int>("n");
    QTest::addColumn<uintVector_type>("expected");

    std::string header;
    while(ifile >> header && header == testName)
    {
        int n;

        if(!(ifile >> n))
        {
            continue;
        }

        unsigned buffer;
        uintVector_type expected(n);
        for(int i = 0; i < n && ifile >> buffer; ++i)
        {
            expected[i] = buffer;
        }

        if(expected.count() != n)
        {
            continue;
        }

        QTest::newRow(testCaseName.arg(testCaseIndex++).toLocal8Bit().data())
                << n << expected;
    }
}

void CentercutplustestsTest::VDCreateHalfSineTableTest()
{
    QFETCH(int, n);
    QFETCH(doubleVector_type, expected);
    doubleVector_type dst(n);

    FFEngine::VDCreateHalfSineTable(dst.begin(), n);

    QVERIFY(std::equal(dst.begin(), dst.end(), expected.begin(),
                       doubleEquals));
}

void CentercutplustestsTest::VDCreateHalfSineTableTest_data()
{
    const std::string testName = "VDCreateHalfSineTable";
    std::ifstream ifile((_logDir + testName + dataFileExt).c_str());
    const QString testCaseName((testName + testCasePostfix).c_str());
    int testCaseIndex = 0;

    QTest::addColumn<int>("n");
    QTest::addColumn<doubleVector_type>("expected");

    std::string header;
    while(ifile >> header && header == testName)
    {
        int n;

        if(!(ifile >> n))
        {
            continue;
        }

        double buffer;
        doubleVector_type expected(n);
        for(int i = 0; i < n && ifile >> buffer; ++i)
        {
            expected[i] = buffer;
        }

        if(expected.count() != n)
        {
            continue;
        }

        QTest::newRow(testCaseName.arg(testCaseIndex++).toLocal8Bit().data())
                << n << expected;
    }
}

void CentercutplustestsTest::VDCreateRaisedCosineWindowTest()
{
    QFETCH(int, n);
    QFETCH(double, power);
    QFETCH(doubleVector_type, expected);
    doubleVector_type dst(n);

    FFEngine::VDCreateRaisedCosineWindow(dst.begin(), n, power);

    QVERIFY(std::equal(dst.begin(), dst.end(), expected.begin(),
                       doubleEquals));
}

void CentercutplustestsTest::VDCreateRaisedCosineWindowTest_data()
{
    const std::string testName = "VDCreateRaisedCosineWindow";
    std::ifstream ifile((_logDir + testName + dataFileExt).c_str());
    const QString testCaseName((testName + testCasePostfix).c_str());
    int testCaseIndex = 0;

    QTest::addColumn<int>("n");
    QTest::addColumn<double>("power");
    QTest::addColumn<doubleVector_type>("expected");

    std::string header;
    while(ifile >> header && header == testName)
    {
        int n;
        double power;

        if(!(ifile >> n >> power))
        {
            continue;
        }

        double buffer;
        doubleVector_type expected(n);
        for(int i = 0; i < n && ifile >> buffer; ++i)
        {
            expected[i] = buffer;
        }

        if(expected.count() != n)
        {
            continue;
        }

        QTest::newRow(testCaseName.arg(testCaseIndex++).toLocal8Bit().data())
                << n << power << expected;
    }
}


QTEST_APPLESS_MAIN(CentercutplustestsTest);

#include "tst_centercutplusteststest.moc"
