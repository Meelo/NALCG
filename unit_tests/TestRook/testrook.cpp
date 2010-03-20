// system includes
#include <QObject>
#include <QtTest/QtTest>

// classes to be tested
#include "../../src/logic/board.h"
#include "../../src/logic/chessboard.h"
#include "../../src/logic/square.h"
#include "../../src/logic/chesspieces-meta.h"

class TestRook : public QObject
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
void TestRook::whiteValidMoves_data()
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
    PieceHolder createUnitsAt02[] = {   PieceHolder(48, 0),
                                        PieceHolder(40, new Pawn(Piece::WHITE)) };
    PieceHolder createUnitsAt03[] = {   PieceHolder(55, 0),
                                        PieceHolder(39, new Pawn(Piece::WHITE)) };
    PieceHolder createUnitsAt04[] = {   PieceHolder(57, 0) };
    PieceHolder createUnitsAt05[] = {   PieceHolder(61, 0), PieceHolder(62, 0) };
    PieceHolder createUnitsAt06[] = {   PieceHolder(35, new Rook(Piece::WHITE)),
                                        PieceHolder(19, new Pawn(Piece::BLACK)) };
    PieceHolder createUnitsAt07[] = {   PieceHolder(5, new Rook(Piece::WHITE)) };
    PieceHolder createUnitsAt08[] = {   PieceHolder(37, new Rook(Piece::WHITE)),
                                        PieceHolder(36, new Pawn(Piece::WHITE)),
                                        PieceHolder(38, new Pawn(Piece::WHITE))};


    // These are the expected valid move indices.
    std::size_t case01[] = { };
    std::size_t case02[] = { 48 };
    std::size_t case03[] = { 55, 47 };
    std::size_t case04[] = { 57 };
    std::size_t case05[] = { 61, 62 };
    std::size_t case06[] = { 19, 27, 32, 33, 34, 36, 37, 38, 39, 43 };
    std::size_t case07[] = { 4, 6, 13 };
    std::size_t case08[] = { 13, 21, 29, 45 };

    QTest::newRow("starting positions, a1") << std::size_t(56)
        << std::vector<PieceHolder>
            (createUnitsAt01, createUnitsAt01 + sizeof(createUnitsAt01) / sizeph)
        << std::vector<std::size_t>(case01, case01 + sizeof(case01) / sizet);

    QTest::newRow("pawn moved from a2 to a3") << std::size_t(56)
        << std::vector<PieceHolder>
            (createUnitsAt02, createUnitsAt02 + sizeof(createUnitsAt02) / sizeph)
        << std::vector<std::size_t>(case02, case02 + sizeof(case02) / sizet);

    QTest::newRow("pawn moved from h2 to h4") << std::size_t(63)
        << std::vector<PieceHolder>
            (createUnitsAt03, createUnitsAt03 + sizeof(createUnitsAt03) / sizeph)
        << std::vector<std::size_t>(case03, case03 + sizeof(case03) / sizet);
    
    QTest::newRow("starting point, empty b1") << std::size_t(56)
        << std::vector<PieceHolder>
            (createUnitsAt04, createUnitsAt04 + sizeof(createUnitsAt04) / sizeph)
        << std::vector<std::size_t>(case04, case04 + sizeof(case04) / sizet);

    QTest::newRow("starting point, empty f1 and g1") << std::size_t(63)
        << std::vector<PieceHolder>
            (createUnitsAt05, createUnitsAt05 + sizeof(createUnitsAt05) / sizeph)
        << std::vector<std::size_t>(case05, case05 + sizeof(case05) / sizet);
    
    QTest::newRow("movement available all around rook") << std::size_t(35)
        << std::vector<PieceHolder>
            (createUnitsAt06, createUnitsAt06 + sizeof(createUnitsAt06) / sizeph)
        << std::vector<std::size_t>(case06, case06 + sizeof(case06) / sizet);

    QTest::newRow("white rook in black base") << std::size_t(5)
        << std::vector<PieceHolder>
            (createUnitsAt07, createUnitsAt07 + sizeof(createUnitsAt07) / sizeph)
        << std::vector<std::size_t>(case07, case07 + sizeof(case07) / sizet);
    
    QTest::newRow("sideways movement blocked") << std::size_t(37)
        << std::vector<PieceHolder>
            (createUnitsAt08, createUnitsAt08 + sizeof(createUnitsAt08) / sizeph)
        << std::vector<std::size_t>(case08, case08 + sizeof(case08) / sizet);
}

void TestRook::whiteValidMoves()
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
    
    std::vector<std::size_t> rookMoves = board->getValidMoves(unit_location);
    QCOMPARE(rookMoves.size(), valid_moves.size());

    for (std::size_t i = 0; i < valid_moves.size(); ++i)
    {
        bool found = false;
        for (std::size_t j = 0; j < rookMoves.size(); ++j)
        {
            if (valid_moves[i] == rookMoves[j])
            {
                found = true;
                break;
            }
        }
        QVERIFY(found);
    }

    delete board;
}

void TestRook::blackValidMoves_data()
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
    PieceHolder createUnitsAt02[] = { PieceHolder(15, 0) };
    PieceHolder createUnitsAt03[] = { PieceHolder(6, 0) };
    PieceHolder createUnitsAt04[] = {   PieceHolder(8, 0),
                                        PieceHolder(24, new Pawn(Piece::BLACK)) };
    PieceHolder createUnitsAt05[] = {   PieceHolder(51, new Rook(Piece::BLACK)),
                                        PieceHolder(43, new Pawn(Piece::WHITE)) };
    PieceHolder createUnitsAt06[] = {   PieceHolder(33, new Rook(Piece::BLACK)),
                                        PieceHolder(9,  new Pawn(Piece::WHITE)) };
    PieceHolder createUnitsAt07[] = {   PieceHolder(34, new Rook(Piece::BLACK)),
                                        PieceHolder(42, new Pawn(Piece::BLACK)),
                                        PieceHolder(26, new Pawn(Piece::BLACK)),
                                        PieceHolder(38, new Pawn(Piece::BLACK)) };
    PieceHolder createUnitsAt08[] = {   PieceHolder(29, new Rook(Piece::BLACK)),
                                        PieceHolder(21, new Pawn(Piece::BLACK)),
                                        PieceHolder(28, new Pawn(Piece::BLACK)),
                                        PieceHolder(30, new Pawn(Piece::BLACK)),
                                        PieceHolder(37, new Pawn(Piece::BLACK)) };
    PieceHolder createUnitsAt09[] = {   PieceHolder(29, new Rook(Piece::BLACK)),
                                        PieceHolder(21, new Pawn(Piece::WHITE)),
                                        PieceHolder(28, new Pawn(Piece::WHITE)),
                                        PieceHolder(30, new Pawn(Piece::WHITE)),
                                        PieceHolder(37, new Pawn(Piece::WHITE)) };

    // These are the expected valid move indices.
    std::size_t case01[] = { };
    std::size_t case02[] = { 15, 23, 31, 39, 47, 55 };
    std::size_t case03[] = { 6 };
    std::size_t case04[] = { 8, 16 };
    std::size_t case05[] = { 43, 50, 52, 59 };
    std::size_t case06[] = { 9, 17, 25, 32, 34, 35, 36, 37, 38, 39, 41, 49 };
    std::size_t case07[] = { 32, 33, 35, 36, 37 };
    std::size_t case08[] = { };
    std::size_t case09[] = { 21, 28, 30, 37 };

    QTest::newRow("starting positions, a8") << std::size_t(0) 
        << std::vector<PieceHolder>
            (createUnitsAt01, createUnitsAt01 + sizeof(createUnitsAt01) / sizeph)
        << std::vector<std::size_t>(case01, case01 + sizeof(case01) / sizet);

    QTest::newRow("starting positiions, no pawn at h7") << std::size_t(7) 
        << std::vector<PieceHolder>
            (createUnitsAt02, createUnitsAt02 + sizeof(createUnitsAt02) / sizeph)
        << std::vector<std::size_t>(case02, case02 + sizeof(case02) / sizet);

    QTest::newRow("starting positions, no  knight at g8") << std::size_t(7) 
        << std::vector<PieceHolder>
            (createUnitsAt03, createUnitsAt03 + sizeof(createUnitsAt03) / sizeph)
        << std::vector<std::size_t>(case03, case03 + sizeof(case03) / sizet);

    QTest::newRow("starting positions, pawn moved to a5") << std::size_t(0) 
        << std::vector<PieceHolder>
            (createUnitsAt04, createUnitsAt04 + sizeof(createUnitsAt04) / sizeph)
        << std::vector<std::size_t>(case04, case04 + sizeof(case04) / sizet);

    QTest::newRow("rook ate d2") << std::size_t(51) 
        << std::vector<PieceHolder>
            (createUnitsAt05, createUnitsAt05 + sizeof(createUnitsAt05) / sizeph)
        << std::vector<std::size_t>(case05, case05 + sizeof(case05) / sizet);

    QTest::newRow("movement all around") << std::size_t(33) 
        << std::vector<PieceHolder>
            (createUnitsAt06, createUnitsAt06 + sizeof(createUnitsAt06) / sizeph)
        << std::vector<std::size_t>(case06, case06 + sizeof(case06) / sizet);

    QTest::newRow("only sideways movement allowed") << std::size_t(34) 
        << std::vector<PieceHolder>
            (createUnitsAt07, createUnitsAt07 + sizeof(createUnitsAt07) / sizeph)
        << std::vector<std::size_t>(case07, case07 + sizeof(case07) / sizet);

    QTest::newRow("blocked by allies, middle of board") << std::size_t(29) 
        << std::vector<PieceHolder>
            (createUnitsAt08, createUnitsAt08 + sizeof(createUnitsAt08) / sizeph)
        << std::vector<std::size_t>(case08, case08 + sizeof(case08) / sizet);

    QTest::newRow("surrounded by baddies, middle of board") << std::size_t(29) 
        << std::vector<PieceHolder>
            (createUnitsAt09, createUnitsAt09 + sizeof(createUnitsAt09) / sizeph)
        << std::vector<std::size_t>(case09, case09 + sizeof(case09) / sizet);
}

void TestRook::blackValidMoves()
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
    
    std::vector<std::size_t> rookMoves = board->getValidMoves(unit_location);
    QCOMPARE(rookMoves.size(), valid_moves.size());

    for (std::size_t i = 0; i < valid_moves.size(); ++i)
    {
        bool found = false;
        for (std::size_t j = 0; j < rookMoves.size(); ++j)
        {
            if (valid_moves[i] == rookMoves[j])
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

QTEST_MAIN(TestRook)
#include "testrook.moc"
