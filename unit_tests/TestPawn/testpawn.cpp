// system includes
#include <QObject>
#include <QtTest/QtTest>

// classes to be tested
#include "../../src/logic/board.h"
#include "../../src/logic/chessboard.h"
#include "../../src/logic/square.h"
#include "../../src/logic/chesspieces-meta.h"

class TestPawn : public QObject
{
    Q_OBJECT

private slots:
    // Test case declarations
    void whiteValidMoves_data();
    void whiteValidMoves();
   
};

// Use Q_DECLARE_METATYPE in order to use 'custom' types in _data() functions.
Q_DECLARE_METATYPE(std::string)
Q_DECLARE_METATYPE(std::vector<std::size_t>)

// Test case implementations
void TestPawn::whiteValidMoves_data()
{
    QTest::addColumn<std::size_t>("unit_location");
    QTest::addColumn<std::vector<std::size_t> >("valid_moves");

    // All values are counted as indices of one dimensional array.
    // Further proof can be found from chessboard.cpp in src/logic/
    std::size_t case01[] = { 32, 40 };
    std::size_t case02[] = { 36, 44 };

    QTest::newRow("starting positions, a2") << std::size_t(48) 
        << std::vector<std::size_t>(case01, 
            case01 + sizeof(case01) / sizeof(case01[0]) );
    QTest::newRow("starting positions, e2") << std::size_t(52)
        << std::vector<std::size_t>(case02, 
            case02 + sizeof(case02) / sizeof(case02[0]) );
}

void TestPawn::whiteValidMoves()
{
    QFETCH(std::size_t, unit_location);
    QFETCH(std::vector<std::size_t>, valid_moves);

    Board* board = new ChessBoard(ChessBoard::createBoard());
    
    std::vector<std::size_t> pawnMoves = board->getValidMoves(unit_location);

    for (std::size_t i = 0; i < valid_moves.size(); ++i)
    {
        bool found = false;
        for (std::size_t j = 0; j < pawnMoves.size(); ++j)
        {
            if (valid_moves[i] == pawnMoves[j])
            {
                found = true;
                break;
            }
        }
        QVERIFY(found);
    }

    delete board;
}

// End of Tests

QTEST_MAIN(TestPawn)
#include "testpawn.moc"
