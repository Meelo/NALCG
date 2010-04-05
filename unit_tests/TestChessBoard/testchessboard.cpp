// system includes
#include <QObject>
#include <QtTest/QtTest>
#include <iostream>
#include <string>
#include <vector>

// classes to be tested
#include "../../src/logic/board.h"
#include "../../src/logic/colour.h"
#include "../../src/logic/chessboard.h"
#include "../../src/logic/square.h"
#include "../../src/logic/chesspieces-meta.h"

class TestChessBoard : public QObject
{
    Q_OBJECT

private slots:
    // Test case declarations
    void createBoard_data();
    void createBoard();
    void checkUnits_data();
    void checkUnits();
    void getPosition_data();
    void getPosition();
    void getCoordinates_data();
    void getCoordinates();
};

// Use Q_DECLARE_METATYPE in order to use 'custom' types in _data() functions.
Q_DECLARE_METATYPE(std::string)
Q_DECLARE_METATYPE(Colour)

// Test case implementations
void TestChessBoard::createBoard_data()
{
    QTest::addColumn<std::size_t>("expected");
    QTest::addColumn<std::size_t>("black_start");
    QTest::addColumn<std::size_t>("black_end");
    QTest::addColumn<std::size_t>("white_start");
    QTest::addColumn<std::size_t>("white_end");

    QTest::newRow("8x8 board") << std::size_t(64) << std::size_t(0) 
        << std::size_t(16) << std::size_t(48) << std::size_t(64);
}

void TestChessBoard::createBoard()
{
    QFETCH(std::size_t, expected);
    QFETCH(std::size_t, black_start);
    QFETCH(std::size_t, black_end);
    QFETCH(std::size_t, white_start);
    QFETCH(std::size_t, white_end);

    std::vector<Square> squares = ChessBoard::createBoard();
    QCOMPARE(squares.size(), expected);

    // black units
    for (std::size_t i = black_start; i < black_end; ++i)
    {
        QVERIFY(squares.at(i).hasPiece());
    }

    // empty squares
    for (std::size_t i = black_end; i < white_start; ++i)
    {
        QVERIFY(!squares.at(i).hasPiece());
    }

    // white units
    for (std::size_t i = white_start; i < white_end; ++i)
    {
        QVERIFY(squares.at(i).hasPiece());
    }
}

void TestChessBoard::checkUnits_data()
{
    Colour white = WHITE;
    Colour black = BLACK;
    Colour undefined = UNDEFINED;
    std::string empty("");
    std::string pawn("Pawn");
    std::string rook("Rook");
    std::string knight("Knight");
    std::string bishop("Bishop");
    std::string queen("Queen");
    std::string king("King");

    QTest::addColumn<std::size_t>("position");
    QTest::addColumn<Colour>("colour");
    QTest::addColumn<std::string>("unit");

    QTest::newRow("black rook at a8")   << std::size_t(0)  << black << rook;
    QTest::newRow("black knight at b8") << std::size_t(1)  << black << knight;
    QTest::newRow("black bishop at c8") << std::size_t(2)  << black << bishop;
    QTest::newRow("black queen at d1")  << std::size_t(3)  << black << queen;
    QTest::newRow("black king at e1")   << std::size_t(4)  << black << king;
    QTest::newRow("black bishop at f8") << std::size_t(5)  << black << bishop;
    QTest::newRow("black knight at g8") << std::size_t(6)  << black << knight;
    QTest::newRow("black rook at h8")   << std::size_t(7)  << black << rook;
    
    QTest::newRow("black pawn at a7")   << std::size_t(8)  << black << pawn;
    QTest::newRow("black pawn at b7")   << std::size_t(9)  << black << pawn;
    QTest::newRow("black pawn at c7")   << std::size_t(10) << black << pawn;
    QTest::newRow("black pawn at d7")   << std::size_t(11) << black << pawn;
    QTest::newRow("black pawn at e7")   << std::size_t(12) << black << pawn;
    QTest::newRow("black pawn at f7")   << std::size_t(13) << black << pawn;
    QTest::newRow("black pawn at g7")   << std::size_t(14) << black << pawn;
    QTest::newRow("black pawn at h7")   << std::size_t(15) << black << pawn;

    // All the empty squares needs to be tested as well
    for (std::size_t i = 16; i < 48; ++i)
    {
        QTest::newRow("empty slots") << i << undefined << empty;
    }
    
    QTest::newRow("white pawn at a2")   << std::size_t(48) << white << pawn;
    QTest::newRow("white pawn at b2")   << std::size_t(49) << white << pawn;
    QTest::newRow("white pawn at c2")   << std::size_t(50) << white << pawn;
    QTest::newRow("white pawn at d2")   << std::size_t(51) << white << pawn;
    QTest::newRow("white pawn at e2")   << std::size_t(52) << white << pawn;
    QTest::newRow("white pawn at f2")   << std::size_t(53) << white << pawn;
    QTest::newRow("white pawn at g2")   << std::size_t(54) << white << pawn;
    QTest::newRow("white pawn at h2")   << std::size_t(55) << white << pawn;

    QTest::newRow("white rook at a1")   << std::size_t(56) << white << rook;
    QTest::newRow("white knight at b1") << std::size_t(57) << white << knight;
    QTest::newRow("white bishop at c1") << std::size_t(58) << white << bishop;
    QTest::newRow("white queen at d1")  << std::size_t(59) << white << queen;
    QTest::newRow("white king at e1")   << std::size_t(60) << white << king;
    QTest::newRow("white bishop at f1") << std::size_t(61) << white << bishop;
    QTest::newRow("white knight at g1") << std::size_t(62) << white << knight;
    QTest::newRow("white rook at h1")   << std::size_t(63) << white << rook;
}

void TestChessBoard::checkUnits()
{
    QFETCH(std::size_t, position);
    QFETCH(Colour, colour);
    QFETCH(std::string, unit);

    std::vector<Square> squares = ChessBoard::createBoard();
    QCOMPARE(squares.size(), std::size_t(64));

    QVERIFY( position < std::size_t(64) );
    Square& shouldHaveUnit = squares.at(position);

    QCOMPARE(shouldHaveUnit.getNameOfPiece(), unit);
    QCOMPARE(shouldHaveUnit.getColourOfPiece(), colour);
}

void TestChessBoard::getCoordinates_data()
{
    QTest::addColumn<std::size_t>("position");
    QTest::addColumn<std::size_t>("column");
    QTest::addColumn<std::size_t>("row");
    QTest::addColumn<bool>("valid");

    QTest::newRow("with position index 0, coordinates should be (0, 0)")
        << std::size_t(0) << std::size_t(0) << std::size_t(0) << true;

    QTest::newRow("with position index 7, coordinates should be (7, 0)")
        << std::size_t(7) << std::size_t(7) << std::size_t(0) << true;

    QTest::newRow("with position index 63, coordinates should be (7, 7)")
        << std::size_t(63) << std::size_t(7) << std::size_t(7) << true;

    QTest::newRow("with position index 56, coordinates should be (0, 7)")
        << std::size_t(56) << std::size_t(0) << std::size_t(7) << true;

    QTest::newRow("with position index 49, coordinates should be (1, 6)")
        << std::size_t(49) << std::size_t(1) << std::size_t(6) << true;

    QTest::newRow("with position index 29, coordinates should be (5, 3)")
        << std::size_t(29) << std::size_t(5) << std::size_t(3) << true;

    QTest::newRow("with position index 18, coordinates should be (2, 2)")
        << std::size_t(18) << std::size_t(2) << std::size_t(2) << true;

    QTest::newRow("with position index 52, coordinates should be (4, 6)")
        << std::size_t(52) << std::size_t(4) << std::size_t(6) << true;

    QTest::newRow("with position index 47, coordinates should be (7, 5)")
        << std::size_t(47) << std::size_t(7) << std::size_t(5) << true;

    QTest::newRow("with position index 12, coordinates should be (4, 1)")
        << std::size_t(12) << std::size_t(4) << std::size_t(1) << true;

    QTest::newRow("with position index 57, coordinates should be (1, 7)")
        << std::size_t(57) << std::size_t(1) << std::size_t(7) << true;

    QTest::newRow("with position index 64, coordinates should be (0, 0)")
        << std::size_t(64) << std::size_t(-1) << std::size_t(-1) << false;
}

void TestChessBoard::getCoordinates()
{
    QFETCH(std::size_t, position);
    QFETCH(std::size_t, column);
    QFETCH(std::size_t, row);
    QFETCH(bool, valid);

    std::size_t x = 0, y = 0;
    bool ret = ChessBoard::getCoordinates(position, x, y);

    QCOMPARE(ret, valid);
    QCOMPARE(x, column);
    QCOMPARE(y, row);
}

void TestChessBoard::getPosition_data()
{
    QTest::addColumn<std::size_t>("column");
    QTest::addColumn<std::size_t>("row");
    QTest::addColumn<std::size_t>("expected");

    QTest::newRow("coordinates (0, 0) should be same as 0 index")
        << std::size_t(0) << std::size_t(0) << std::size_t(0);

    QTest::newRow("coordinates (0, 7) should be same as 56 index")
        << std::size_t(0) << std::size_t(7) << std::size_t(56);

    QTest::newRow("coordinates (7, 0) should be same as 7 index")
        << std::size_t(7) << std::size_t(0) << std::size_t(7);

    QTest::newRow("coordinates (7, 7) should be same as 63 index")
        << std::size_t(7) << std::size_t(7) << std::size_t(63);

    QTest::newRow("coordinates (4, 5) should be same as 44 index")
        << std::size_t(4) << std::size_t(5) << std::size_t(44);

    QTest::newRow("coordinates (1, 4) should be same as 33 index")
        << std::size_t(1) << std::size_t(4) << std::size_t(33);

    QTest::newRow("coordinates (2, 6) should be same as 50 index")
        << std::size_t(2) << std::size_t(6) << std::size_t(50);

    QTest::newRow("coordinates (6, 3) should be same as 30 index")
        << std::size_t(6) << std::size_t(3) << std::size_t(30);

    QTest::newRow("coordinates (8, 8) should be same as -1 index")
        << std::size_t(8) << std::size_t(8) << std::size_t(-1);
}

void TestChessBoard::getPosition()
{
    QFETCH(std::size_t, column);
    QFETCH(std::size_t, row);
    QFETCH(std::size_t, expected);

    std::size_t position = ChessBoard::getPosition(column, row);

    QCOMPARE(position, expected);
}

// End of Tests

QTEST_MAIN(TestChessBoard)
#include "testchessboard.moc"
