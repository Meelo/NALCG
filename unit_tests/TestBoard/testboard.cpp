// system includes
#include <QObject>
#include <QtTest/QtTest>
#include <iostream>
#include <vector>

// classes to be tested
#include "../../src/logic/board.h"
#include "../../src/logic/chessboard.h"
#include "../../src/logic/square.h"

class TestBoard : public QObject
{
    Q_OBJECT

private slots:
    // Test case declarations
    void boardSize_data();
    void boardSize();
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

    Board* board = new ChessBoard(ChessBoard::createBoard());

    QCOMPARE(board->getWidth(), width);
    QCOMPARE(board->getHeight(), height);
    QCOMPARE(board->getTotalSize(), total_size);

    delete board;
}

// End of Tests

QTEST_MAIN(TestBoard)
#include "testboard.moc"
