// system includes
#include <QObject>
#include <QtTest/QtTest>

// classes to be tested
#include "../../src/logic/board.h"
#include "../../src/logic/colour.h"
#include "../../src/logic/chessboard.h"
#include "../../src/logic/square.h"
#include "../../src/logic/chesspieces-meta.h"

class TestQueen : public QObject
{
    Q_OBJECT

private slots:
    // Test case declarations
    void whiteValidMoves_data();
    void whiteValidMoves();
    void blackValidMoves_data();
    void blackValidMoves();
};

// Test case implementations

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
void TestQueen::whiteValidMoves_data()
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
    PieceHolder createUnitsAt02[] = {   PieceHolder(50, 0) };
    PieceHolder createUnitsAt03[] = {   PieceHolder(52, 0) };
    PieceHolder createUnitsAt04[] = {   PieceHolder(51, new Queen(WHITE)),
                                        PieceHolder(33, new Pawn(WHITE)),
                                        PieceHolder(37, new Pawn(WHITE)),
                                        PieceHolder(27, new Pawn(WHITE)) };
    PieceHolder createUnitsAt05[] = {   PieceHolder(50, new Pawn(BLACK)) };
    PieceHolder createUnitsAt06[] = {   PieceHolder(51, new Pawn(BLACK)) };
    PieceHolder createUnitsAt07[] = {   PieceHolder(52, new Pawn(BLACK)) };
    PieceHolder createUnitsAt08[] = {   PieceHolder(10, new Queen(WHITE)),
                                        PieceHolder(18, new Pawn(BLACK)) };
    PieceHolder createUnitsAt09[] = {   PieceHolder(10, new Queen(WHITE)) };
    PieceHolder createUnitsAt10[] = {   PieceHolder(36, new Queen(WHITE)) };

    // These are the expected valid move indices.
    std::size_t case01[] = { };
    std::size_t case02[] = { 50, 41, 32 };
    std::size_t case03[] = { 52, 45, 38, 31 };
    std::size_t case04[] = { 35, 42, 43, 44 };
    std::size_t case05[] = { 50 };
    std::size_t case06[] = { 51 };
    std::size_t case07[] = { 52 };
    std::size_t case08[] = { 1, 2, 3, 9, 11, 17, 18, 19, 24, 28, 37, 46 };
    std::size_t case09[] = {    1, 2, 3, 9, 11, 17, 18, 19,
                                24, 26, 28, 34, 37, 42, 46 };
    std::size_t case10[] = {     9, 12, 15, 18, 20, 22, 27, 28, 39,
                                32, 33, 34, 35, 37, 38, 39, 43, 44, 45 };

    QTest::newRow("starting positions, d1") << std::size_t(59)
        << std::vector<PieceHolder>
            (createUnitsAt01, createUnitsAt01 + sizeof(createUnitsAt01) / sizeph)
        << std::vector<std::size_t>(case01, case01 + sizeof(case01) / sizet);

    QTest::newRow("starting positions, empty c2") << std::size_t(59)
        << std::vector<PieceHolder>
            (createUnitsAt02, createUnitsAt02 + sizeof(createUnitsAt02) / sizeph)
        << std::vector<std::size_t>(case02, case02 + sizeof(case02) / sizet);

    QTest::newRow("starting positions, empty e2") << std::size_t(59)
        << std::vector<PieceHolder>
            (createUnitsAt03, createUnitsAt03 + sizeof(createUnitsAt03) / sizeph)
        << std::vector<std::size_t>(case03, case03 + sizeof(case03) / sizet);

    QTest::newRow("queen moved to d2") << std::size_t(51)
        << std::vector<PieceHolder>
            (createUnitsAt04, createUnitsAt04 + sizeof(createUnitsAt04) / sizeph)
        << std::vector<std::size_t>(case04, case04 + sizeof(case04) / sizet);

    QTest::newRow("starting positions, edible c2") << std::size_t(59)
        << std::vector<PieceHolder>
            (createUnitsAt05, createUnitsAt05 + sizeof(createUnitsAt05) / sizeph)
        << std::vector<std::size_t>(case05, case05 + sizeof(case05) / sizet);

    QTest::newRow("starting positions, edible d2") << std::size_t(59)
        << std::vector<PieceHolder>
            (createUnitsAt06, createUnitsAt06 + sizeof(createUnitsAt06) / sizeph)
        << std::vector<std::size_t>(case06, case06 + sizeof(case06) / sizet);

    QTest::newRow("starting positions, edible e2") << std::size_t(59)
        << std::vector<PieceHolder>
            (createUnitsAt07, createUnitsAt07 + sizeof(createUnitsAt07) / sizeph)
        << std::vector<std::size_t>(case07, case07 + sizeof(case07) / sizet);

    QTest::newRow("surrounded by baddies") << std::size_t(10)
        << std::vector<PieceHolder>
            (createUnitsAt08, createUnitsAt08 + sizeof(createUnitsAt08) / sizeph)
        << std::vector<std::size_t>(case08, case08 + sizeof(case08) / sizet);

    QTest::newRow("at enemy base (c7)") << std::size_t(10)
        << std::vector<PieceHolder>
            (createUnitsAt09, createUnitsAt09 + sizeof(createUnitsAt09) / sizeph)
        << std::vector<std::size_t>(case09, case09 + sizeof(case09) / sizet);

    QTest::newRow("in the middle of board") << std::size_t(36)
        << std::vector<PieceHolder>
            (createUnitsAt10, createUnitsAt10 + sizeof(createUnitsAt10) / sizeph)
        << std::vector<std::size_t>(case10, case10 + sizeof(case10) / sizet);
}

void TestQueen::whiteValidMoves()
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

    std::vector<std::size_t> queenMoves = board->getValidMoves(unit_location);

    QCOMPARE(queenMoves.size(), valid_moves.size());

    for (std::size_t i = 0; i < valid_moves.size(); ++i)
    {
        bool found = false;
        for (std::size_t j = 0; j < queenMoves.size(); ++j)
        {
            if (valid_moves[i] == queenMoves[j])
            {
                found = true;
                break;
            }
        }
        QVERIFY(found);
    }

    delete board;
}

void TestQueen::blackValidMoves_data()
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
    PieceHolder createUnitsAt02[] = {   PieceHolder(2, 0) };
    PieceHolder createUnitsAt03[] = {   PieceHolder(4, 0),
                                        PieceHolder(5, new King(BLACK)) };
    PieceHolder createUnitsAt04[] = {   PieceHolder(10, 0) };
    PieceHolder createUnitsAt05[] = {   PieceHolder(12, 0) };
    PieceHolder createUnitsAt06[] = {   PieceHolder(11, new Queen(BLACK)),
                                        PieceHolder(25, new Pawn(BLACK)),
                                        PieceHolder(35, new Pawn(BLACK)),
                                        PieceHolder(29, new Pawn(BLACK)) };
    PieceHolder createUnitsAt07[] = {   PieceHolder(10, new Pawn(WHITE)) };
    PieceHolder createUnitsAt08[] = {   PieceHolder(11, new Pawn(WHITE)) };
    PieceHolder createUnitsAt09[] = {   PieceHolder(12, new Pawn(WHITE)) };
    PieceHolder createUnitsAt10[] = {   PieceHolder(52, new Queen(BLACK)),
                                        PieceHolder(44, new Pawn(WHITE)),
                                        PieceHolder(34, new Pawn(WHITE)),
                                        PieceHolder(38, new Pawn(WHITE)) };
    PieceHolder createUnitsAt11[] = {   PieceHolder(53, new Queen(BLACK)) };
    PieceHolder createUnitsAt12[] = {   PieceHolder(38, new Queen(BLACK)) };
    PieceHolder createUnitsAt13[] = {   PieceHolder(29, new Queen(BLACK)),
                                        PieceHolder(18, new Bishop(WHITE)),
                                        PieceHolder(11, 0) };
    PieceHolder createUnitsAt14[] = {   PieceHolder(42, new Queen(BLACK)),
                                        PieceHolder(18, new Bishop(WHITE)),
                                        PieceHolder(11, 0) };

    // These are the expected valid move indices.
    std::size_t case01[] = { };
    std::size_t case02[] = { 2 };
    std::size_t case03[] = { 4 };
    std::size_t case04[] = { 10, 17, 24 };
    std::size_t case05[] = { 12, 21, 30, 39 };
    std::size_t case06[] = { 18, 19, 20, 27 };
    std::size_t case07[] = { 10 };
    std::size_t case08[] = { 11 };
    std::size_t case09[] = { 12 };
    std::size_t case10[] = { 34, 38, 43, 44, 45, 51, 53, 59, 60, 61 };
    std::size_t case11[] = {    17, 21, 26, 29, 35, 37, 39,
                                44, 45, 46, 52, 54, 60, 61, 62 };
    std::size_t case12[] = {    20, 22, 29, 30, 31, 32, 33, 34, 35,
                                36, 37, 39, 45, 46, 47, 52, 54 };
    std::size_t case13[] = { 11 };
    std::size_t case14[] = { 18 };

    QTest::newRow("starting positions, d8") << std::size_t(3)
        << std::vector<PieceHolder>
            (createUnitsAt01, createUnitsAt01 + sizeof(createUnitsAt01) / sizeph)
        << std::vector<std::size_t>(case01, case01 + sizeof(case01) / sizet);

    QTest::newRow("starting positions, empty c8") << std::size_t(3)
        << std::vector<PieceHolder>
            (createUnitsAt02, createUnitsAt02 + sizeof(createUnitsAt02) / sizeph)
        << std::vector<std::size_t>(case02, case02 + sizeof(case02) / sizet);

    QTest::newRow("starting positions, empty e8") << std::size_t(3)
        << std::vector<PieceHolder>
            (createUnitsAt03, createUnitsAt03 + sizeof(createUnitsAt03) / sizeph)
        << std::vector<std::size_t>(case03, case03 + sizeof(case03) / sizet);

    QTest::newRow("starting positions, c7") << std::size_t(3)
        << std::vector<PieceHolder>
            (createUnitsAt04, createUnitsAt04 + sizeof(createUnitsAt04) / sizeph)
        << std::vector<std::size_t>(case04, case04 + sizeof(case04) / sizet);

    QTest::newRow("starting positions, e7") << std::size_t(3)
        << std::vector<PieceHolder>
            (createUnitsAt05, createUnitsAt05 + sizeof(createUnitsAt05) / sizeph)
        << std::vector<std::size_t>(case05, case05 + sizeof(case05) / sizet);

    QTest::newRow("queen moved to d7, almost blocked") << std::size_t(11)
        << std::vector<PieceHolder>
            (createUnitsAt06, createUnitsAt06 + sizeof(createUnitsAt06) / sizeph)
        << std::vector<std::size_t>(case06, case06 + sizeof(case06) / sizet);

    QTest::newRow("starting positions, edible c7") << std::size_t(3)
        << std::vector<PieceHolder>
            (createUnitsAt07, createUnitsAt07 + sizeof(createUnitsAt07) / sizeph)
        << std::vector<std::size_t>(case07, case07 + sizeof(case07) / sizet);

    QTest::newRow("starting positions, edible d7") << std::size_t(3)
        << std::vector<PieceHolder>
            (createUnitsAt08, createUnitsAt08 + sizeof(createUnitsAt08) / sizeph)
        << std::vector<std::size_t>(case08, case08 + sizeof(case08) / sizet);

    QTest::newRow("starting positions, edible e7") << std::size_t(3)
        << std::vector<PieceHolder>
            (createUnitsAt09, createUnitsAt09 + sizeof(createUnitsAt09) / sizeph)
        << std::vector<std::size_t>(case09, case09 + sizeof(case09) / sizet);

    QTest::newRow("surrounded by food at enemy base") << std::size_t(52)
        << std::vector<PieceHolder>
            (createUnitsAt10, createUnitsAt10 + sizeof(createUnitsAt10) / sizeph)
        << std::vector<std::size_t>(case10, case10 + sizeof(case10) / sizet);

    QTest::newRow("in enemy base, backwards movement") << std::size_t(53)
        << std::vector<PieceHolder>
            (createUnitsAt11, createUnitsAt11 + sizeof(createUnitsAt11) / sizeph)
        << std::vector<std::size_t>(case11, case11 + sizeof(case11) / sizet);

    QTest::newRow("in the middle of map") << std::size_t(38)
        << std::vector<PieceHolder>
            (createUnitsAt12, createUnitsAt12 + sizeof(createUnitsAt12) / sizeph)
        << std::vector<std::size_t>(case12, case12 + sizeof(case12) / sizet);

    QTest::newRow("king is under attack by bishop at c6, queen at f5 [bug]")
        << std::size_t(29) << std::vector<PieceHolder>
        (createUnitsAt13, createUnitsAt13 + sizeof(createUnitsAt13) / sizeph)
        << std::vector<std::size_t>(case13, case13 + sizeof(case13) / sizet);

    QTest::newRow("king is under attack by bishop at c6, queen at c3 [bug]")
        << std::size_t(42) << std::vector<PieceHolder>
        (createUnitsAt14, createUnitsAt14 + sizeof(createUnitsAt14) / sizeph)
        << std::vector<std::size_t>(case14, case14 + sizeof(case14) / sizet);
}

void TestQueen::blackValidMoves()
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

    std::vector<std::size_t> queenMoves = board->getValidMoves(unit_location);


    QCOMPARE(queenMoves.size(), valid_moves.size());

    for (std::size_t i = 0; i < valid_moves.size(); ++i)
    {
        bool found = false;
        for (std::size_t j = 0; j < queenMoves.size(); ++j)
        {
            if (valid_moves[i] == queenMoves[j])
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

QTEST_MAIN(TestQueen)
#include "testqueen.moc"
