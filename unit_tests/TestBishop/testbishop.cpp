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
    PieceHolder createUnitsAt02[] = {   PieceHolder(46, new Bishop(Piece::WHITE)),
                                        PieceHolder(37, new Bishop(Piece::WHITE))};
    PieceHolder createUnitsAt03[] = {   PieceHolder(46, new Bishop(Piece::WHITE)),
                                        PieceHolder(39, new Bishop(Piece::WHITE)),
                                        PieceHolder(28, new Bishop(Piece::WHITE))};
    PieceHolder createUnitsAt04[] = {   PieceHolder(39, new Bishop(Piece::WHITE)),
                                        PieceHolder(30, new Bishop(Piece::WHITE))};
    PieceHolder createUnitsAt05[] = {   PieceHolder(53, new Bishop(Piece::WHITE)),
                                        PieceHolder(44, new Bishop(Piece::WHITE))};
    PieceHolder createUnitsAt06[] = {   PieceHolder(53, new Bishop(Piece::WHITE)),
                                        PieceHolder(44, new Bishop(Piece::WHITE)),
                                        PieceHolder(39, new Bishop(Piece::BLACK))};
    PieceHolder createUnitsAt07[] = {   PieceHolder(53, new Bishop(Piece::WHITE)),
                                        PieceHolder(44, new Bishop(Piece::WHITE)),
                                        PieceHolder(46, new Bishop(Piece::BLACK)),
                                        PieceHolder(39, new Bishop(Piece::BLACK))};
    PieceHolder createUnitsAt08[] = {   PieceHolder(35, new Bishop(Piece::WHITE))};
    PieceHolder createUnitsAt09[] = {   PieceHolder(35, new Bishop(Piece::WHITE)),
                                        PieceHolder(53, new Bishop(Piece::BLACK))};

    // These are the expected valid move indices.
    std::size_t case01[] = { };
    std::size_t case02[] = { 39 };
    std::size_t case03[] = { 37 };
    std::size_t case04[] = { 46 };
    std::size_t case05[] = { 39, 46 };
    std::size_t case06[] = { 39, 46 };
    std::size_t case07[] = { 46 };
    std::size_t case08[] = { 8, 14, 17, 21, 26, 28, 42, 44 };
    std::size_t case09[] = { 8, 14, 17, 21, 26, 28, 42, 44, 53 };

    QTest::newRow("starting positions, c1") << std::size_t(58) 
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
    
    QTest::newRow("bishop at h4, empty g3") << std::size_t(39) 
        << std::vector<PieceHolder>
            (createUnitsAt04, createUnitsAt04 + sizeof(createUnitsAt04) / sizeph)
        << std::vector<std::size_t>(case04, case04 + sizeof(case04) / sizet);      
    QTest::newRow("bishop at f2, empty g3 and h4") << std::size_t(53) 
        << std::vector<PieceHolder>
            (createUnitsAt05, createUnitsAt05 + sizeof(createUnitsAt05) / sizeph)
        << std::vector<std::size_t>(case05, case05 + sizeof(case05) / sizet);
    
    QTest::newRow("bishop at f2, empty g3 and edible h4") << std::size_t(53)
        << std::vector<PieceHolder>
            (createUnitsAt06, createUnitsAt06 + sizeof(createUnitsAt06) / sizeph)
        << std::vector<std::size_t>(case06, case06 + sizeof(case06) / sizet);
    
    QTest::newRow("bishop at f2, edible g3 and edible h4") << std::size_t(53)
        << std::vector<PieceHolder>
            (createUnitsAt07, createUnitsAt07 + sizeof(createUnitsAt07) / sizeph)
        << std::vector<std::size_t>(case07, case07 + sizeof(case07) / sizet);

    QTest::newRow("bishop at d4, can move into many places") << std::size_t(35)
        << std::vector<PieceHolder>
            (createUnitsAt08, createUnitsAt08 + sizeof(createUnitsAt08) / sizeph)
        << std::vector<std::size_t>(case08, case08 + sizeof(case08) / sizet);

    QTest::newRow("bishop at d4, can move into more places") << std::size_t(35)
        << std::vector<PieceHolder>
            (createUnitsAt09, createUnitsAt09 + sizeof(createUnitsAt09) / sizeph)
        << std::vector<std::size_t>(case09, case09 + sizeof(case09) / sizet);

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
    PieceHolder createUnitsAt02[] = { PieceHolder(9, 0) };
    PieceHolder createUnitsAt03[] = { PieceHolder(11, 0) };
    PieceHolder createUnitsAt04[] = { PieceHolder(11, new Bishop(Piece::WHITE)) };
    PieceHolder createUnitsAt05[] = { PieceHolder(58, new Bishop(Piece::BLACK)) };
    PieceHolder createUnitsAt06[] = {   PieceHolder(44, new Bishop(Piece::BLACK)),
                                        PieceHolder(35, new Bishop(Piece::WHITE)),
                                        PieceHolder(36, new Bishop(Piece::WHITE)),
                                        PieceHolder(37, new Bishop(Piece::WHITE)),
                                        PieceHolder(43, new Bishop(Piece::WHITE)),
                                        PieceHolder(45, new Bishop(Piece::WHITE))};
    PieceHolder createUnitsAt07[] = {   PieceHolder(20, new Bishop(Piece::BLACK)),
                                        PieceHolder(19, new Bishop(Piece::BLACK)),
                                        PieceHolder(21, new Bishop(Piece::BLACK)),
                                        PieceHolder(27, new Bishop(Piece::BLACK)),
                                        PieceHolder(28, new Bishop(Piece::BLACK)),
                                        PieceHolder(29, new Bishop(Piece::BLACK))};
    PieceHolder createUnitsAt08[] = { PieceHolder(23, new Bishop(Piece::BLACK)) };
    PieceHolder createUnitsAt09[] = { PieceHolder(32, new Bishop(Piece::BLACK)) };


    // These are the expected valid move indices.
    std::size_t case01[] = { };
    std::size_t case02[] = { 9, 16 };
    std::size_t case03[] = { 11, 20, 29, 38, 47 };
    std::size_t case04[] = { 11 };
    std::size_t case05[] = { 49, 51 };
    std::size_t case06[] = { 35, 37, 51, 53 };
    std::size_t case07[] = { };
    std::size_t case08[] = { 30, 37, 44, 51 };
    std::size_t case09[] = { 18, 25, 41, 50 };

    QTest::newRow("starting positions, c8") << std::size_t(2) 
        << std::vector<PieceHolder>
            (createUnitsAt01, createUnitsAt01 + sizeof(createUnitsAt01) / sizeph)
        << std::vector<std::size_t>(case01, case01 + sizeof(case01) / sizet);
    
    QTest::newRow("starting positions, opening at b7") << std::size_t(2) 
        << std::vector<PieceHolder>
            (createUnitsAt02, createUnitsAt02 + sizeof(createUnitsAt02) / sizeph)
        << std::vector<std::size_t>(case02, case02 + sizeof(case02) / sizet);

    QTest::newRow("starting positions, opening at d7") << std::size_t(2) 
        << std::vector<PieceHolder>
            (createUnitsAt03, createUnitsAt03 + sizeof(createUnitsAt03) / sizeph)
        << std::vector<std::size_t>(case03, case03 + sizeof(case03) / sizet);

    QTest::newRow("starting positions, edible at d7") << std::size_t(2) 
        << std::vector<PieceHolder>
            (createUnitsAt04, createUnitsAt04 + sizeof(createUnitsAt04) / sizeph)
        << std::vector<std::size_t>(case04, case04 + sizeof(case04) / sizet);

    QTest::newRow("black bishop in white bishop's position") << std::size_t(58) 
        << std::vector<PieceHolder>
            (createUnitsAt05, createUnitsAt05 + sizeof(createUnitsAt05) / sizeph)
        << std::vector<std::size_t>(case05, case05 + sizeof(case05) / sizet);

    QTest::newRow("black bishop at e3, surrounded by white") << std::size_t(44) 
        << std::vector<PieceHolder>
            (createUnitsAt06, createUnitsAt06 + sizeof(createUnitsAt06) / sizeph)
        << std::vector<std::size_t>(case06, case06 + sizeof(case06) / sizet);

    QTest::newRow("black bishop at e6, surrounded by black") << std::size_t(20) 
        << std::vector<PieceHolder>
            (createUnitsAt07, createUnitsAt07 + sizeof(createUnitsAt07) / sizeph)
        << std::vector<std::size_t>(case07, case07 + sizeof(case07) / sizet);

    QTest::newRow("right side, edge") << std::size_t(23) 
        << std::vector<PieceHolder>
            (createUnitsAt08, createUnitsAt08 + sizeof(createUnitsAt08) / sizeph)
        << std::vector<std::size_t>(case08, case08 + sizeof(case08) / sizet);

    QTest::newRow("left side, edge") << std::size_t(32) 
        << std::vector<PieceHolder>
            (createUnitsAt09, createUnitsAt09 + sizeof(createUnitsAt09) / sizeph)
        << std::vector<std::size_t>(case09, case09 + sizeof(case09) / sizet);
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
