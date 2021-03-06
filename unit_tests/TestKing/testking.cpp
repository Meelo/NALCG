// system includes
#include <QObject>
#include <QtTest/QtTest>

// classes to be tested
#include "../../src/logic/board.h"
#include "../../src/logic/colour.h"
#include "../../src/logic/chessboard.h"
#include "../../src/logic/square.h"
#include "../../src/logic/chesspieces-meta.h"

class TestKing : public QObject
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
void TestKing::whiteValidMoves_data()
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
    PieceHolder createUnitsAt02[] = {   PieceHolder(61, 0) };
    PieceHolder createUnitsAt03[] = {   PieceHolder(59, 0) };
    PieceHolder createUnitsAt04[] = {   PieceHolder(52, 0) };
    PieceHolder createUnitsAt05[] = {   PieceHolder(51, 0) };
    PieceHolder createUnitsAt06[] = {   PieceHolder(53, 0) };
    PieceHolder createUnitsAt07[] = {   PieceHolder(9,  new King(WHITE)),
                                        PieceHolder(1,  new Bishop(BLACK)),
                                        PieceHolder(2,  new Rook(BLACK)),
                                        PieceHolder(8,  new Bishop(BLACK)),
                                        PieceHolder(10, new Bishop(BLACK)),
                                        PieceHolder(16, new Pawn(BLACK)),
                                        PieceHolder(17, new Pawn(BLACK)),
                                        PieceHolder(18, new Pawn(BLACK)),
                                        PieceHolder(3, 0),
                                        PieceHolder(11, 0) };
    PieceHolder createUnitsAt08[] = {   PieceHolder(34, new King(WHITE)) };
    PieceHolder createUnitsAt09[] = {   PieceHolder(57, 0),
                                        PieceHolder(58, 0),
                                        PieceHolder(59, 0) };
    PieceHolder createUnitsAt10[] = {   PieceHolder(61, 0),
                                        PieceHolder(62, 0) };
    // These are the expected valid move indices.
    std::size_t case01[] = { };
    std::size_t case02[] = { 61 };
    std::size_t case03[] = { 59 };
    std::size_t case04[] = { 52 };
    std::size_t case05[] = { 51 };
    std::size_t case06[] = { 53 };
    std::size_t case07[] = { 0, 2, 16, 18 };
    std::size_t case08[] = { 25, 26, 27, 33, 35, 41, 42, 43 };
    std::size_t case09[] = { 58, 59 };
    std::size_t case10[] = { 61, 62 };

    QTest::newRow("starting positions, e1") << std::size_t(60)
        << std::vector<PieceHolder>
            (createUnitsAt01, createUnitsAt01 + sizeof(createUnitsAt01) / sizeph)
        << std::vector<std::size_t>(case01, case01 + sizeof(case01) / sizet);

    QTest::newRow("starting positions, e1, empty f1") << std::size_t(60)
        << std::vector<PieceHolder>
            (createUnitsAt02, createUnitsAt02 + sizeof(createUnitsAt02) / sizeph)
        << std::vector<std::size_t>(case02, case02 + sizeof(case02) / sizet);

    QTest::newRow("starting positions, e1, empty d1") << std::size_t(60)
        << std::vector<PieceHolder>
            (createUnitsAt03, createUnitsAt03 + sizeof(createUnitsAt03) / sizeph)
        << std::vector<std::size_t>(case03, case03 + sizeof(case03) / sizet);

    QTest::newRow("starting positions, e1, empty e2") << std::size_t(60)
        << std::vector<PieceHolder>
            (createUnitsAt04, createUnitsAt04 + sizeof(createUnitsAt04) / sizeph)
        << std::vector<std::size_t>(case04, case04 + sizeof(case04) / sizet);

    QTest::newRow("starting positions, e1, empty d2") << std::size_t(60)
        << std::vector<PieceHolder>
            (createUnitsAt05, createUnitsAt05 + sizeof(createUnitsAt05) / sizeph)
        << std::vector<std::size_t>(case05, case05 + sizeof(case05) / sizet);

    QTest::newRow("starting positions, e1, empty f2") << std::size_t(60)
        << std::vector<PieceHolder>
            (createUnitsAt06, createUnitsAt06 + sizeof(createUnitsAt06) / sizeph)
        << std::vector<std::size_t>(case06, case06 + sizeof(case06) / sizet);

    QTest::newRow("surrounded by food") << std::size_t(9)
        << std::vector<PieceHolder>
            (createUnitsAt07, createUnitsAt07 + sizeof(createUnitsAt07) / sizeph)
        << std::vector<std::size_t>(case07, case07 + sizeof(case07) / sizet);

    QTest::newRow("all movements possible") << std::size_t(34)
        << std::vector<PieceHolder>
            (createUnitsAt08, createUnitsAt08 + sizeof(createUnitsAt08) / sizeph)
        << std::vector<std::size_t>(case08, case08 + sizeof(case08) / sizet);

    QTest::newRow("Leftside castling possible") << std::size_t(60)
        << std::vector<PieceHolder>
            (createUnitsAt09, createUnitsAt09 + sizeof(createUnitsAt09) / sizeph)
        << std::vector<std::size_t>(case09, case09 + sizeof(case09) / sizet);

    QTest::newRow("Rightside castling possible") << std::size_t(60)
        << std::vector<PieceHolder>
            (createUnitsAt10, createUnitsAt10 + sizeof(createUnitsAt10) / sizeph)
        << std::vector<std::size_t>(case10, case10 + sizeof(case10) / sizet);

}

void TestKing::whiteValidMoves()
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

    std::vector<std::size_t> kingMoves = board->getValidMoves(unit_location);

    QCOMPARE(kingMoves.size(), valid_moves.size());

    for (std::size_t i = 0; i < valid_moves.size(); ++i)
    {
        bool found = false;
        for (std::size_t j = 0; j < kingMoves.size(); ++j)
        {
            if (valid_moves[i] == kingMoves[j])
            {
                found = true;
                break;
            }
        }
        QVERIFY(found);
    }

    delete board;
}

void TestKing::blackValidMoves_data()
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
    PieceHolder createUnitsAt02[] = {   PieceHolder(5, 0) };
    PieceHolder createUnitsAt03[] = {   PieceHolder(3, 0) };
    PieceHolder createUnitsAt04[] = {   PieceHolder(12, 0) };
    PieceHolder createUnitsAt05[] = {   PieceHolder(11, 0) };
    PieceHolder createUnitsAt06[] = {   PieceHolder(13, 0) };
    PieceHolder createUnitsAt07[] = {   PieceHolder(51, new King(BLACK)),
                                        PieceHolder(42, new Pawn(WHITE)),
                                        PieceHolder(43, new Pawn(WHITE)),
                                        PieceHolder(44, new Pawn(WHITE)) };
    PieceHolder createUnitsAt08[] = {   PieceHolder(26, new King(BLACK)) };

    // These are the expected valid move indices.
    std::size_t case01[] = { };
    std::size_t case02[] = { 5 };
    std::size_t case03[] = { 3 };
    std::size_t case04[] = { 12 };
    std::size_t case05[] = { 11 };
    std::size_t case06[] = { 13 };
    std::size_t case07[] = { 42, 43, 44, 50, 52, 58, 59, 60 };
    std::size_t case08[] = { 17, 18, 19, 25, 27, 33, 34, 35 };

    QTest::newRow("starting positions, e8") << std::size_t(4)
        << std::vector<PieceHolder>
            (createUnitsAt01, createUnitsAt01 + sizeof(createUnitsAt01) / sizeph)
        << std::vector<std::size_t>(case01, case01 + sizeof(case01) / sizet);

    QTest::newRow("starting positions, e8, empty f8") << std::size_t(4)
        << std::vector<PieceHolder>
            (createUnitsAt02, createUnitsAt02 + sizeof(createUnitsAt02) / sizeph)
        << std::vector<std::size_t>(case02, case02 + sizeof(case02) / sizet);

    QTest::newRow("starting positions, e8, empty d8") << std::size_t(4)
        << std::vector<PieceHolder>
            (createUnitsAt03, createUnitsAt03 + sizeof(createUnitsAt03) / sizeph)
        << std::vector<std::size_t>(case03, case03 + sizeof(case03) / sizet);

    QTest::newRow("starting positions, e8, empty e7") << std::size_t(4)
        << std::vector<PieceHolder>
            (createUnitsAt04, createUnitsAt04 + sizeof(createUnitsAt04) / sizeph)
        << std::vector<std::size_t>(case04, case04 + sizeof(case04) / sizet);

    QTest::newRow("starting positions, e8, empty d7") << std::size_t(4)
        << std::vector<PieceHolder>
            (createUnitsAt05, createUnitsAt05 + sizeof(createUnitsAt05) / sizeph)
        << std::vector<std::size_t>(case05, case05 + sizeof(case05) / sizet);

    QTest::newRow("starting positions, e8, empty f7") << std::size_t(4)
        << std::vector<PieceHolder>
            (createUnitsAt06, createUnitsAt06 + sizeof(createUnitsAt06) / sizeph)
        << std::vector<std::size_t>(case06, case06 + sizeof(case06) / sizet);

    QTest::newRow("surrounded by food") << std::size_t(51)
        << std::vector<PieceHolder>
            (createUnitsAt07, createUnitsAt07 + sizeof(createUnitsAt07) / sizeph)
        << std::vector<std::size_t>(case07, case07 + sizeof(case07) / sizet);

    QTest::newRow("all movements possible") << std::size_t(26)
        << std::vector<PieceHolder>
            (createUnitsAt08, createUnitsAt08 + sizeof(createUnitsAt08) / sizeph)
        << std::vector<std::size_t>(case08, case08 + sizeof(case08) / sizet);
}

void TestKing::blackValidMoves()
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

    std::vector<std::size_t> kingMoves = board->getValidMoves(unit_location);


    QCOMPARE(kingMoves.size(), valid_moves.size());

    for (std::size_t i = 0; i < valid_moves.size(); ++i)
    {
        bool found = false;
        for (std::size_t j = 0; j < kingMoves.size(); ++j)
        {
            if (valid_moves[i] == kingMoves[j])
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

QTEST_MAIN(TestKing)
#include "testking.moc"
