// system includes
#include <QObject>
#include <QtTest/QtTest>

// classes to be tested
#include "../../src/board.h"

class TestBoard : public QObject
{
    Q_OBJECT

private slots:
    // Test case declarations

};

// Test case implementations

// End of Tests

QTEST_MAIN(TestBoard)
#include "testboard.moc"
