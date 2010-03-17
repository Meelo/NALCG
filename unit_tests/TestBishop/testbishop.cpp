// system includes
#include <QObject>
#include <QtTest/QtTest>

// classes to be tested
#include "../../src/logic/board.h"
#include "../../src/logic/chessboard.h"
#include "../../src/logic/square.h"
#include "../../src/logic/chesspieces-meta.h"

class TestBishop : public QObject
{
    Q_OBJECT

private slots:
    // Test case declarations
    void whiteValidMoves_data();
    void whiteValidMoves();
    void blackValidMoves_data();
    void blackValidMoves();

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
void TestBishop::whiteValidMoves_data()
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

    // These are the expected valid move indices.
    std::size_t case01[] = { };

    QTest::newRow("starting positions, a2") << std::size_t(58) 
        << std::vector<PieceHolder>
            (createUnitsAt01, createUnitsAt01 + sizeof(createUnitsAt01) / sizeph)
        << std::vector<std::size_t>(case01, case01 + sizeof(case01) / sizet);
    
}

void TestBishop::whiteValidMoves()
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
    
    std::vector<std::size_t> bishopMoves = board->getValidMoves(unit_location);

    QCOMPARE(bishopMoves.size(), valid_moves.size());

    for (std::size_t i = 0; i < valid_moves.size(); ++i)
    {
        bool found = false;
        for (std::size_t j = 0; j < bishopMoves.size(); ++j)
        {
            if (valid_moves[i] == bishopMoves[j])
            {
                found = true;
                break;
            }
        }
        QVERIFY(found);
    }

    delete board;
}

void TestBishop::blackValidMoves_data()
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
    PieceHolder createUnitsAt02[] = {   PieceHolder(46, new Bishop(Piece::WHITE)),
                                        PieceHolder(37, new Bishop(Piece::WHITE))};
    PieceHolder createUnitsAt03[] = {   PieceHolder(46, new Bishop(Piece::WHITE)),
                                        PieceHolder(39, new Bishop(Piece::WHITE)),
                                        PieceHolder(28, new Bishop(Piece::WHITE))};

    // These are the expected valid move indices.
    std::size_t case01[] = { };
    std::size_t case02[] = { 39 };
    std::size_t case03[] = { 37 };

    QTest::newRow("starting positions, a2") << std::size_t(2) 
        << std::vector<PieceHolder>
            (createUnitsAt01, createUnitsAt01 + sizeof(createUnitsAt01) / sizeph)
        << std::vector<std::size_t>(case01, case01 + sizeof(case01) / sizet);
     
    QTest::newRow("bishop at g3, empty h4") << std::size_t(46) 
        << std::vector<PieceHolder>
            (createUnitsAt02, createUnitsAt02 + sizeof(createUnitsAt02) / sizeph)
        << std::vector<std::size_t>(case02, case02 + sizeof(case02) / sizet);   
    
    QTest::newRow("bishop at g3, empty f4") << std::size_t(46) 
        << std::vector<PieceHolder>
            (createUnitsAt03, createUnitsAt03 + sizeof(createUnitsAt03) / sizeph)
        << std::vector<std::size_t>(case03, case03 + sizeof(case03) / sizet);   

}

void TestBishop::blackValidMoves()
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
        if (ph.piece) QVERIFY(squares.at(location).hasPiece());
    }

    Board* board = new ChessBoard(squares);
    
    std::vector<std::size_t> bishopMoves = board->getValidMoves(unit_location);

    QCOMPARE(bishopMoves.size(), valid_moves.size());

    for (std::size_t i = 0; i < valid_moves.size(); ++i)
    {
        bool found = false;
        for (std::size_t j = 0; j < bishopMoves.size(); ++j)
        {
            if (valid_moves[i] == bishopMoves[j])
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

QTEST_MAIN(TestBishop)
#include "testbishop.moc"
