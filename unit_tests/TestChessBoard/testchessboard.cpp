// system includes
#include <QObject>
#include <QtTest/QtTest>
#include <iostream>
#include <string>
#include <vector>

// classes to be tested
#include "../../src/logic/board.h"
#include "../../src/logic/chessboard.h"
#include "../../src/logic/square.h"
#include "../../src/logic/piece.h"
#include "../../src/logic/pawn.h"

class TestChessBoard : public QObject
{
    Q_OBJECT

private slots:
    // Test case declarations
    void createBoard_data();
    void createBoard();
    void checkUnits_data();
    void checkUnits();
    void isInitialized();
};

// Use Q_DECLARE_METATYPE in order to use 'custom' types in _data() functions.
Q_DECLARE_METATYPE(std::string)

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
        QVERIFY(squares.at(i).hasUnit());
    }

    // empty squares
    for (std::size_t i = black_end; i < white_start; ++i)
    {
        QVERIFY(!squares.at(i).hasUnit());
    }

    // white units
    for (std::size_t i = white_start; i < white_end; ++i)
    {
        QVERIFY(squares.at(i).hasUnit());
    }
}

void TestChessBoard::checkUnits_data()
{
    std::string white("white");
    std::string black("black");
    std::string empty("empty");
    std::string pawn("Pawn");
    std::string rook("Rook");
    std::string knight("Knight");
    std::string bishop("Bishop");
    std::string queen("Queen");
    std::string king("King");

    QTest::addColumn<std::size_t>("position");
    QTest::addColumn<std::string>("colour");
    QTest::addColumn<std::string>("unit");

    QTest::newRow("black rook at a8") << std::size_t(0)  << black << rook;
    QTest::newRow("black rook at h8") << std::size_t(7)  << black << rook;
    QTest::newRow("black pawn at a7") << std::size_t(8)  << black << pawn;
    QTest::newRow("black pawn at b7") << std::size_t(9)  << black << pawn;
    QTest::newRow("black pawn at c7") << std::size_t(10) << black << pawn;
    QTest::newRow("black pawn at d7") << std::size_t(11) << black << pawn;
    QTest::newRow("black pawn at e7") << std::size_t(12) << black << pawn;
    QTest::newRow("black pawn at f7") << std::size_t(13) << black << pawn;
    QTest::newRow("black pawn at g7") << std::size_t(14) << black << pawn;
    QTest::newRow("black pawn at h7") << std::size_t(15) << black << pawn;

    // All the empty squares needs to be tested as well
    for (std::size_t i = 16; i < 48; ++i)
    {
        QTest::newRow("empty slots") << i << empty << empty;
    }
    
    QTest::newRow("white pawn at a2") << std::size_t(48) << white << pawn;
    QTest::newRow("white pawn at b2") << std::size_t(49) << white << pawn;
    QTest::newRow("white pawn at c2") << std::size_t(50) << white << pawn;
    QTest::newRow("white pawn at d2") << std::size_t(51) << white << pawn;
    QTest::newRow("white pawn at e2") << std::size_t(52) << white << pawn;
    QTest::newRow("white pawn at f2") << std::size_t(53) << white << pawn;
    QTest::newRow("white pawn at g2") << std::size_t(54) << white << pawn;
    QTest::newRow("white pawn at h2") << std::size_t(55) << white << pawn;
    QTest::newRow("white rook at a1") << std::size_t(56) << white << rook;
    QTest::newRow("white rook at h1") << std::size_t(63) << white << rook;
}

void TestChessBoard::checkUnits()
{
    QFETCH(std::size_t, position);
    QFETCH(std::string, colour);
    QFETCH(std::string, unit);

    std::vector<Square> squares = ChessBoard::createBoard();
    QCOMPARE(squares.size(), std::size_t(64));

    Square shouldHaveUnit = squares.at(position);

    QCOMPARE(shouldHaveUnit.getName(), unit);
    QCOMPARE(shouldHaveUnit.getColour(), colour);
}

void TestChessBoard::isInitialized()
{
    Board* board = new ChessBoard();

    delete board;
}
// End of Tests

QTEST_MAIN(TestChessBoard)
#include "testchessboard.moc"
