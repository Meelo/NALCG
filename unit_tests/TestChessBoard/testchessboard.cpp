// system includes
#include <QObject>
#include <QtTest/QtTest>
#include <iostream>

// classes to be tested
#include "../../src/board.h"
#include "../../src/chessboard.h"

class TestChessBoard : public QObject
{
    Q_OBJECT

private slots:
    // Test case declarations
    void boardSize_data();
    void boardSize();
    void isNotInitialized();
    void isInitialized();
};

// Test case implementations

void TestChessBoard::boardSize_data()
{
    QTest::addColumn<std::size_t>("width");
    QTest::addColumn<std::size_t>("height");
    QTest::addColumn<std::size_t>("total_size");

    QTest::newRow("board dimensions") 
        << std::size_t(8) << std::size_t(8) << std::size_t(64);
}

void TestChessBoard::boardSize()
{
    QFETCH(std::size_t, width);
    QFETCH(std::size_t, height);
    QFETCH(std::size_t, total_size);

    Board *board = new ChessBoard();

    QCOMPARE(board->getWidth(), width);
    QCOMPARE(board->getHeight(), height);
    QCOMPARE(board->getTotalSize(), total_size);

    delete board;
}

void TestChessBoard::isNotInitialized()
{
    Board *board = new ChessBoard();
    QVERIFY(!board->isInitialized());

    delete board;
}

void TestChessBoard::isInitialized()
{
    Board *board = new ChessBoard();
    
    delete board;
}

// End of Tests

QTEST_MAIN(TestChessBoard)
#include "testchessboard.moc"
