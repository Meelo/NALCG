// system includes
#include <QObject>
#include <QtTest/QtTest>
#include <exception>

// classes to be tested
#include "../../src/board.h"

class TestBoard : public QObject
{
    Q_OBJECT

private slots:
    // Test case declarations
    void instanceCreation();
};

// Test case implementations

void TestBoard::instanceCreation()
{
    Board *board = 0;
    try
    {
//        board = new Board();
    }
    catch (std::exception& ex)
    {
       QVERIFY(true);
       return;
    }
    
    QVERIFY(false);
    delete board;
}

// End of Tests

QTEST_MAIN(TestBoard)
#include "testboard.moc"
