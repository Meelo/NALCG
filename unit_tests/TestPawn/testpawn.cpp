// system includes
#include <QObject>
#include <QtTest/QtTest>

// classes to be tested
#include "../../src/logic/board.h"
#include "../../src/logic/square.h"
#include "../../src/logic/piece.h"
#include "../../src/logic/pawn.h"

class TestPawn : public QObject
{
    Q_OBJECT

private slots:
    // Test case declarations
    void whiteValidMoves_data();
    void whiteValidMoves();
   
};
Q_DECLARE_METATYPE(Board)

// Test case implementations
void TestPawn::whiteValidMoves_data()
{
    QTest::addColumn<Board>("scenario");
}

void TestPawn::whiteValidMoves()
{

}

// End of Tests

QTEST_MAIN(TestPawn)
#include "testpawn.moc"
