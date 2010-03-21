// system includes
#include <QObject>
#include <QtTest/QtTest>

// classes to be tested
#include "../../src/logic/board.h"
#include "../../src/logic/chessboard.h"
#include "../../src/logic/square.h"
#include "../../src/logic/chesspieces-meta.h"

class TestKnight : public QObject
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
void TestKnight::whiteValidMoves_data()
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
    PieceHolder createUnitsAt03[] = { PieceHolder(51, 0) };
    PieceHolder createUnitsAt04[] = { PieceHolder(52, 0) };
    PieceHolder createUnitsAt05[] = { PieceHolder(40, new Pawn(Piece::BLACK)) };
    PieceHolder createUnitsAt06[] = { PieceHolder(51, new Pawn(Piece::BLACK)) };
    PieceHolder createUnitsAt07[] = { PieceHolder(52, new Pawn(Piece::BLACK)) };
    PieceHolder createUnitsAt08[] = { PieceHolder(47, new Pawn(Piece::BLACK)) };
    PieceHolder createUnitsAt09[] = {   PieceHolder(34, new Knight(Piece::WHITE)),
                                        PieceHolder(49, 0), 
                                        PieceHolder(51, 0) };
    PieceHolder createUnitsAt10[] = {   PieceHolder(34, new Knight(Piece::WHITE)),
                                        PieceHolder(17, new Pawn(Piece::WHITE)),
                                        PieceHolder(19, new Pawn(Piece::WHITE)),
                                        PieceHolder(24, new Pawn(Piece::WHITE)),
                                        PieceHolder(28, new Pawn(Piece::WHITE)),
                                        PieceHolder(40, new Pawn(Piece::WHITE)),
                                        PieceHolder(44, new Pawn(Piece::WHITE)) };
    
    // These are the expected valid move indices.
    std::size_t case01[] = { 40, 42 };
    std::size_t case02[] = { 45, 47 };
    std::size_t case03[] = { 40, 42, 51 };
    std::size_t case04[] = { 45, 47, 52 };
    std::size_t case05[] = { 40, 42 };
    std::size_t case06[] = { 40, 42, 51 };
    std::size_t case07[] = { 45, 47, 52 };
    std::size_t case08[] = { 45, 47 };
    std::size_t case09[] = { 17, 19, 24, 28, 40, 44, 49, 51 };
    std::size_t case10[] = { };


    QTest::newRow("starting positions, b1") << std::size_t(57)
        << std::vector<PieceHolder>
            (createUnitsAt01, createUnitsAt01 + sizeof(createUnitsAt01) / sizeph)
        << std::vector<std::size_t>(case01, case01 + sizeof(case01) / sizet);

    QTest::newRow("starting positions, g1") << std::size_t(62)
        << std::vector<PieceHolder>
            (createUnitsAt02, createUnitsAt02 + sizeof(createUnitsAt02) / sizeph)
        << std::vector<std::size_t>(case02, case02 + sizeof(case02) / sizet);
   
    QTest::newRow("starting positions, b1, empty at d2") << std::size_t(57)
        << std::vector<PieceHolder>
            (createUnitsAt03, createUnitsAt03 + sizeof(createUnitsAt03) / sizeph)
        << std::vector<std::size_t>(case03, case03 + sizeof(case03) / sizet);
   
    QTest::newRow("starting positions, g1, empty at e2") << std::size_t(62)
        << std::vector<PieceHolder>
            (createUnitsAt04, createUnitsAt04 + sizeof(createUnitsAt04) / sizeph)
        << std::vector<std::size_t>(case04, case04 + sizeof(case04) / sizet);

    QTest::newRow("starting positions, b1, edible at a3") << std::size_t(57)
        << std::vector<PieceHolder>
            (createUnitsAt05, createUnitsAt05 + sizeof(createUnitsAt05) / sizeph)
        << std::vector<std::size_t>(case05, case05 + sizeof(case05) / sizet);
    
    QTest::newRow("starting positions, b1, edible at d2") << std::size_t(57)
        << std::vector<PieceHolder>
            (createUnitsAt06, createUnitsAt06 + sizeof(createUnitsAt06) / sizeph)
        << std::vector<std::size_t>(case06, case06 + sizeof(case06) / sizet);
    
    QTest::newRow("starting positions, g1, edible at h3") << std::size_t(62)
        << std::vector<PieceHolder>
            (createUnitsAt07, createUnitsAt07 + sizeof(createUnitsAt07) / sizeph)
        << std::vector<std::size_t>(case07, case07 + sizeof(case07) / sizet);
    
    QTest::newRow("starting positions, g1, edible at e2") << std::size_t(62)
        << std::vector<PieceHolder>
            (createUnitsAt08, createUnitsAt08 + sizeof(createUnitsAt08) / sizeph)
        << std::vector<std::size_t>(case08, case08 + sizeof(case08) / sizet);
    
    QTest::newRow("middle of the map, all movements possible" ) << std::size_t(34)
        << std::vector<PieceHolder>
            (createUnitsAt09, createUnitsAt09 + sizeof(createUnitsAt09) / sizeph)
        << std::vector<std::size_t>(case09, case09 + sizeof(case09) / sizet);
    
    QTest::newRow("middle of the map, all movements blocked" ) << std::size_t(34)
        << std::vector<PieceHolder>
            (createUnitsAt10, createUnitsAt10 + sizeof(createUnitsAt10) / sizeph)
        << std::vector<std::size_t>(case10, case10 + sizeof(case10) / sizet);

}

void TestKnight::whiteValidMoves()
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
    
    std::vector<std::size_t> knightMoves = board->getValidMoves(unit_location);
    static int counter = 0;
    ++counter;
    std::cout << "--- " << counter << " ---" << std::endl;
    for (std::size_t i = 0; i < knightMoves.size(); ++i)
    {
        std::cout << knightMoves.at(i) << " ";
    }
    std::cout << std::endl << std::endl;
    QCOMPARE(knightMoves.size(), valid_moves.size());

    for (std::size_t i = 0; i < valid_moves.size(); ++i)
    {
        bool found = false;
        for (std::size_t j = 0; j < knightMoves.size(); ++j)
        {
            if (valid_moves[i] == knightMoves[j])
            {
                found = true;
                break;
            }
        }
        QVERIFY(found);
    }

    delete board;
}

void TestKnight::blackValidMoves_data()
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
    PieceHolder createUnitsAt03[] = { PieceHolder(11, 0) };
    PieceHolder createUnitsAt04[] = { PieceHolder(12, 0) };

    // These are the expected valid move indices.
    std::size_t case01[] = { 16, 18};
    std::size_t case02[] = { 21, 23 };
    std::size_t case03[] = { 11, 16, 18 };
    std::size_t case04[] = { 12, 21, 23 };

    QTest::newRow("starting positions, b8") << std::size_t(1) 
        << std::vector<PieceHolder>
            (createUnitsAt01, createUnitsAt01 + sizeof(createUnitsAt01) / sizeph)
        << std::vector<std::size_t>(case01, case01 + sizeof(case01) / sizet);

    QTest::newRow("starting positions, g8") << std::size_t(6) 
        << std::vector<PieceHolder>
            (createUnitsAt02, createUnitsAt02 + sizeof(createUnitsAt02) / sizeph)
        << std::vector<std::size_t>(case02, case02 + sizeof(case02) / sizet);
    
    QTest::newRow("starting positions, b8, empty d7") << std::size_t(1) 
        << std::vector<PieceHolder>
            (createUnitsAt03, createUnitsAt03 + sizeof(createUnitsAt03) / sizeph)
        << std::vector<std::size_t>(case03, case03 + sizeof(case03) / sizet);

    QTest::newRow("starting positions, g8, empty e7") << std::size_t(6) 
        << std::vector<PieceHolder>
            (createUnitsAt04, createUnitsAt04 + sizeof(createUnitsAt04) / sizeph)
        << std::vector<std::size_t>(case04, case04 + sizeof(case04) / sizet);

}

void TestKnight::blackValidMoves()
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
    
    std::vector<std::size_t> knightMoves = board->getValidMoves(unit_location);
    static int counterB = 0;
    ++counterB;
    std::cout << "--- " << counterB << " ---" << std::endl;
    for (std::size_t i = 0; i < knightMoves.size(); ++i)
    {
        std::cout << knightMoves.at(i) << " ";
    }
    std::cout << std::endl << std::endl;
    QCOMPARE(knightMoves.size(), valid_moves.size());

    for (std::size_t i = 0; i < valid_moves.size(); ++i)
    {
        bool found = false;
        for (std::size_t j = 0; j < knightMoves.size(); ++j)
        {
            if (valid_moves[i] == knightMoves[j])
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

QTEST_MAIN(TestKnight)
#include "testknight.moc"
