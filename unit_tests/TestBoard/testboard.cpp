// system includes
#include <QObject>
#include <QtTest/QtTest>
#include <iostream>
#include <vector>

// classes to be tested
#include "../../src/logic/board.h"
#include "../../src/logic/square.h"

class TestBoard : public QObject
{
    Q_OBJECT

private slots:
    // Test case declarations
    void boardSize_data();
    void boardSize();
    void createBoard_data();
    void createBoard();
    void checkPieces_data();
    void checkPieces();
    void isInitialized();
};

// Test case implementations

void TestBoard::boardSize_data()
{
    QTest::addColumn<std::size_t>("width");
    QTest::addColumn<std::size_t>("height");
    QTest::addColumn<std::size_t>("total_size");

    QTest::newRow("board dimensions") 
        << std::size_t(8) << std::size_t(8) << std::size_t(64);
}

void TestBoard::boardSize()
{
    QFETCH(std::size_t, width);
    QFETCH(std::size_t, height);
    QFETCH(std::size_t, total_size);

    Board board;

    QCOMPARE(board.getWidth(), width);
    QCOMPARE(board.getHeight(), height);
    QCOMPARE(board.getTotalSize(), total_size);
}

void TestBoard::createBoard_data()
{
    QTest::addColumn<std::size_t>("expected");

    QTest::newRow("8x8 board") << std::size_t(64);
}

void TestBoard::createBoard()
{
    QFETCH(std::size_t, expected);

    std::vector<Square> squares = Board::createBoard();
    QCOMPARE(squares.size(), expected);
}

void TestBoard::checkPieces_data()
{
    QTest::addColumn<std::size_t>("expected");

    QTest::newRow("8x8 board") << std::size_t(64);
}

void TestBoard::checkPieces()
{
    QFETCH(std::size_t, expected);

    std::vector<Square> squares = Board::createBoard();
    QCOMPARE(squares.size(), expected);
}

void TestBoard::isInitialized()
{
    Board board;
}

// End of Tests

QTEST_MAIN(TestBoard)
#include "testboard.moc"