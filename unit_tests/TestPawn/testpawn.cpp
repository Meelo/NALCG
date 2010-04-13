// system includes
#include <QObject>
#include <QtTest/QtTest>

// classes to be tested
#include "../../src/logic/board.h"
#include "../../src/logic/colour.h"
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
    PieceHolder createUnitsAt03[] = {   PieceHolder(32, new Pawn(WHITE)) };
    PieceHolder createUnitsAt04[] = {   PieceHolder(40, new Pawn(WHITE)) };
    PieceHolder createUnitsAt05[] = {   PieceHolder(32, new Pawn(BLACK)) };
    PieceHolder createUnitsAt06[] = {   PieceHolder(40, new Pawn(BLACK)) };
    PieceHolder createUnitsAt07[] = {   PieceHolder(43, new Pawn(WHITE)),
                                        PieceHolder(34, new Pawn(BLACK))};
    PieceHolder createUnitsAt08[] = {   PieceHolder(43, new Pawn(WHITE)),
                                        PieceHolder(36, new Pawn(BLACK))};
    PieceHolder createUnitsAt09[] = {   PieceHolder(43, new Pawn(WHITE)),
                                        PieceHolder(34, new Pawn(WHITE))};
    PieceHolder createUnitsAt10[] = {   PieceHolder(43, new Pawn(WHITE)),
                                        PieceHolder(36, new Pawn(WHITE))};
    PieceHolder createUnitsAt11[] = {   PieceHolder(8, new Pawn(WHITE)),
                                        PieceHolder(1, new Pawn(WHITE)),
                                        PieceHolder(0, 0)};
    PieceHolder createUnitsAt12[] = {   PieceHolder(15, new Pawn(WHITE)) };

    // These are the expected valid move indices.
    std::size_t case01[] = { 32, 40 };
    std::size_t case02[] = { 36, 44 };
    std::size_t case03[] = { 40 };
    std::size_t case04[] = { };
    std::size_t case05[] = { 40 };
    std::size_t case06[] = { };
    std::size_t case07[] = { 34, 35 };
    std::size_t case08[] = { 35, 36 };
    std::size_t case09[] = { 35 };
    std::size_t case10[] = { 35 };
    std::size_t case11[] = { 0 };
    std::size_t case12[] = { 6 };

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

    QTest::newRow("starting positions, d3, edible black c4") << std::size_t(43)
        << std::vector<PieceHolder>
            (createUnitsAt07, createUnitsAt07 + sizeof(createUnitsAt07) / sizeph)
        << std::vector<std::size_t>(case07, case07 + sizeof(case07) / sizet);

    QTest::newRow("starting positions, d3, edible black e4") << std::size_t(43)
        << std::vector<PieceHolder>
            (createUnitsAt08, createUnitsAt08 + sizeof(createUnitsAt08) / sizeph)
        << std::vector<std::size_t>(case08, case08 + sizeof(case08) / sizet);

    QTest::newRow("starting positions, d3, ally at c4") << std::size_t(43)
        << std::vector<PieceHolder>
            (createUnitsAt09, createUnitsAt09 + sizeof(createUnitsAt09) / sizeph)
        << std::vector<std::size_t>(case09, case09 + sizeof(case09) / sizet);

    QTest::newRow("starting positions, d3, ally at e4") << std::size_t(43)
        << std::vector<PieceHolder>
            (createUnitsAt10, createUnitsAt10 + sizeof(createUnitsAt10) / sizeph)
        << std::vector<std::size_t>(case10, case10 + sizeof(case10) / sizet);

    QTest::newRow("pawn at a7, empty at a8, ally at b8") << std::size_t(8)
        << std::vector<PieceHolder>
            (createUnitsAt11, createUnitsAt11 + sizeof(createUnitsAt11) / sizeph)
        << std::vector<std::size_t>(case11, case11 + sizeof(case11) / sizet);

    QTest::newRow("pawn at h7, edible g6 [bug]") << std::size_t(15)
        << std::vector<PieceHolder>
            (createUnitsAt12, createUnitsAt12 + sizeof(createUnitsAt12) / sizeph)
        << std::vector<std::size_t>(case12, case12 + sizeof(case12) / sizet);
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
        if (ph.piece) QVERIFY(squares.at(location).hasPiece());
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

void TestPawn::blackValidMoves_data()
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
    PieceHolder createUnitsAt02[] = {   PieceHolder(18, new Pawn(BLACK)) };
    PieceHolder createUnitsAt03[] = {   PieceHolder(44, new Pawn(BLACK)) };
    PieceHolder createUnitsAt04[] = {   PieceHolder(27, new Pawn(BLACK)),
                                        PieceHolder(35, new Pawn(BLACK))};
    PieceHolder createUnitsAt05[] = {   PieceHolder(27, new Pawn(BLACK)),
                                        PieceHolder(35, new Pawn(WHITE))};
    PieceHolder createUnitsAt06[] = {   PieceHolder(36, new Pawn(BLACK)),
                                        PieceHolder(43, new Pawn(WHITE)),
                                        PieceHolder(45, new Pawn(WHITE))};

    // These are the expected valid move indices.
    std::size_t case01[] = { 21, 29 };
    std::size_t case02[] = { 26 };
    std::size_t case03[] = { 51, 53 };
    std::size_t case04[] = { };
    std::size_t case05[] = { };
    std::size_t case06[] = { 43, 44, 45 };

    QTest::newRow("starting positions, f7") << std::size_t(13)
        << std::vector<PieceHolder>
            (createUnitsAt01, createUnitsAt01 + sizeof(createUnitsAt01) / sizeph)
        << std::vector<std::size_t>(case01, case01 + sizeof(case01) / sizet);

    QTest::newRow("starting positions, c6") << std::size_t(18)
        << std::vector<PieceHolder>
            (createUnitsAt02, createUnitsAt02 + sizeof(createUnitsAt02) / sizeph)
        << std::vector<std::size_t>(case02, case02 + sizeof(case02) / sizet);

    QTest::newRow("pawn e3, blocked by e2, edible at d2,f2") << std::size_t(44)
        << std::vector<PieceHolder>
            (createUnitsAt03, createUnitsAt03 + sizeof(createUnitsAt03) / sizeph)
        << std::vector<std::size_t>(case03, case03 + sizeof(case03) / sizet);

    QTest::newRow("pawn d5, blocked by black d4") << std::size_t(27)
        << std::vector<PieceHolder>
            (createUnitsAt04, createUnitsAt04 + sizeof(createUnitsAt04) / sizeph)
        << std::vector<std::size_t>(case04, case04 + sizeof(case04) / sizet);

    QTest::newRow("pawn d5, blocked by white d4") << std::size_t(27)
        << std::vector<PieceHolder>
            (createUnitsAt05, createUnitsAt05 + sizeof(createUnitsAt05) / sizeph)
        << std::vector<std::size_t>(case05, case05 + sizeof(case05) / sizet);

    QTest::newRow("pawn e4, edible d3 and f3, empty e3") << std::size_t(36)
        << std::vector<PieceHolder>
            (createUnitsAt06, createUnitsAt06 + sizeof(createUnitsAt06) / sizeph)
        << std::vector<std::size_t>(case06, case06 + sizeof(case06) / sizet);

}

void TestPawn::blackValidMoves()
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
