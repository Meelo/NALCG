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

struct PieceHolder
{
    std::size_t location;
    Piece* piece;
    PieceHolder(std::size_t location, Piece* piece) : location(location),
        piece(piece) { };
};

// Use Q_DECLARE_METATYPE in order to use 'custom' types in _data() functions.
Q_DECLARE_METATYPE(std::string)
Q_DECLARE_METATYPE(std::vector<PieceHolder>)
Q_DECLARE_METATYPE(std::vector<std::size_t>)

// Test case implementations
void TestPawn::whiteValidMoves_data()
{
    QTest::addColumn<std::size_t>("unit_location");
    QTest::addColumn<std::vector<PieceHolder> >("new_units");
    QTest::addColumn<std::vector<std::size_t> >("valid_moves");

    std::size_t sizeph = sizeof(PieceHolder);
    std::size_t sizet = sizeof(std::size_t);
    
    // All values are counted as indices of one dimensional array.
    // Further proof can be found from chessboard.cpp in src/logic/

    // These will be handled as instructions for creating new units
    // before running the actual tests.
    PieceHolder createUnitsAt01[] = { };
    PieceHolder createUnitsAt02[] = { };
    PieceHolder createUnitsAt03[] = { PieceHolder(32, new Pawn(Piece::WHITE)) };
    PieceHolder createUnitsAt04[] = { PieceHolder(40, new Pawn(Piece::WHITE)) };
    PieceHolder createUnitsAt05[] = { PieceHolder(32, new Pawn(Piece::BLACK)) };
    PieceHolder createUnitsAt06[] = { PieceHolder(40, new Pawn(Piece::BLACK)) };

    // These are the expected valid move indices.
    std::size_t case01[] = { 32, 40 };
    std::size_t case02[] = { 36, 44 };
    std::size_t case03[] = { 40 };
    std::size_t case04[] = { };
    std::size_t case05[] = { 40 };
    std::size_t case06[] = { };

    QTest::newRow("starting positions, a2") << std::size_t(48) 
        << std::vector<PieceHolder>
            (createUnitsAt01, createUnitsAt01 + sizeof(createUnitsAt01) / sizeph)
        << std::vector<std::size_t>(case01, case01 + sizeof(case01) / sizet);
    
    QTest::newRow("starting positions, e2") << std::size_t(52)
        << std::vector<PieceHolder>
            (createUnitsAt02, createUnitsAt02 + sizeof(createUnitsAt02) / sizeph)
        << std::vector<std::size_t>(case02, case02 + sizeof(case02) / sizet);
    
    QTest::newRow("starting positions, a2 blocked by a4") << std::size_t(48)
        << std::vector<PieceHolder>
            (createUnitsAt03, createUnitsAt03 + sizeof(createUnitsAt03) / sizeph)
        << std::vector<std::size_t>(case03, case03 + sizeof(case03) / sizet);

    QTest::newRow("starting positions, a2 blocked by a3") << std::size_t(48)
        << std::vector<PieceHolder>
            (createUnitsAt04, createUnitsAt04 + sizeof(createUnitsAt04) / sizeph)
        << std::vector<std::size_t>(case04, case04 + sizeof(case04) / sizet);
    
    QTest::newRow("starting positions, a2 blocked by black a4") << std::size_t(48)
        << std::vector<PieceHolder>
            (createUnitsAt05, createUnitsAt05 + sizeof(createUnitsAt05) / sizeph)
        << std::vector<std::size_t>(case05, case05 + sizeof(case05) / sizet);

    QTest::newRow("starting positions, a2 blocked by black a3") << std::size_t(48)
        << std::vector<PieceHolder>
            (createUnitsAt06, createUnitsAt06 + sizeof(createUnitsAt06) / sizeph)
        << std::vector<std::size_t>(case06, case06 + sizeof(case06) / sizet);
}

void TestPawn::whiteValidMoves()
{
    QFETCH(std::size_t, unit_location);
    QFETCH(std::vector<PieceHolder>, new_units);
    QFETCH(std::vector<std::size_t>, valid_moves);
    
    std::vector<Square> squares = ChessBoard::createBoard();
    
    for (std::size_t i = 0; i < new_units.size(); ++i)
    {
        PieceHolder ph = new_units.at(i);
        std::size_t location = ph.location;
        squares.at(location).addPiece(ph.piece);
        QVERIFY(squares.at(location).hasPiece());
    }

    Board* board = new ChessBoard(squares);
    
    std::vector<std::size_t> pawnMoves = board->getValidMoves(unit_location);

    QCOMPARE(pawnMoves.size(), valid_moves.size());

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
