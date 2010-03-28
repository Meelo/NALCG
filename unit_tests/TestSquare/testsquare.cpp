// system includes
#include <QObject>
#include <QtTest/QtTest>
#include <vector>
#include <string>

// classes to be tested
#include "../../src/logic/square.h"
#include "../../src/logic/chessboard.h"
#include "../../src/logic/chesspieces-meta.h"

class TestSquare : public QObject
{
    Q_OBJECT

private slots:
    // Test case declarations
    void constructing_data();
    void constructing();
    void addingPiecesToSquare_data();
    void addingPiecesToSquare();
    void removingPiecesFromSquare_data();
    void removingPiecesFromSquare();
    void getPiece_data();
    void getPiece();
};

// Use Q_DECLARE_METATYPE in order to use 'custom' types in _data() functions.
Q_DECLARE_METATYPE(std::string)
Q_DECLARE_METATYPE(Square)
Q_DECLARE_METATYPE(Piece::Colour)
Q_DECLARE_METATYPE(Piece*)

// Test case implementations
void TestSquare::constructing_data()
{
    QTest::addColumn<Piece*>("piece");
    QTest::addColumn<bool>("sendParams");
    QTest::addColumn<bool>("isNull");

    Piece* piece = 0;
    QTest::newRow("empty construction") << piece << false << true;
    QTest::newRow("construct with null") << piece << true << true;
    QTest::newRow("construct with white pawn") 
        << (piece = new Pawn(Piece::WHITE)) << true << false;
    QTest::newRow("construct with black pawn") 
        << (piece = new Pawn(Piece::BLACK)) << true << false;
}

void TestSquare::constructing()
{
    QFETCH(Piece*, piece);
    QFETCH(bool, sendParams);
    QFETCH(bool, isNull);
    
    if (!sendParams)
    {
        Square square;
        QVERIFY(!square.hasPiece());
    }
    else
    {
        Square square(piece);
        QCOMPARE(square.hasPiece(), !isNull);
    }
}

void TestSquare::addingPiecesToSquare_data()
{
    Piece::Colour white = Piece::WHITE;
    Piece::Colour black = Piece::BLACK;
    std::string pawn("Pawn");
    std::string rook("Rook");
    std::string knight("Knight");
    std::string bishop("Bishop");
    std::string queen("Queen");
    std::string king("King");
    QTest::addColumn<Piece*>("piece");
    QTest::addColumn<std::string>("expectedName");
    QTest::addColumn<Piece::Colour>("expectedColour");

    Piece* piece = 0;
    QTest::newRow("adding a white pawn") 
        << (piece = new Pawn(white)) << pawn << white;
    QTest::newRow("adding a black pawn") 
        << (piece = new Pawn(black)) << pawn << black;
    QTest::newRow("adding a white rook") 
        << (piece = new Rook(white)) << rook << white;
    QTest::newRow("adding a black rook") 
        << (piece = new Rook(black)) << rook << black;
    QTest::newRow("adding a white knight") 
        << (piece = new Knight(white)) << knight << white;
    QTest::newRow("adding a black knight") 
        << (piece = new Knight(black)) << knight << black;
    QTest::newRow("adding a white bishop") 
        << (piece = new Bishop(white)) << bishop << white;
    QTest::newRow("adding a black bishop") 
        << (piece = new Bishop(black)) << bishop << black;
    QTest::newRow("adding a white queen") 
        << (piece = new Queen(white)) << queen << white;
    QTest::newRow("adding a black queen") 
        << (piece = new Queen(black)) << queen << black;
    QTest::newRow("adding a white king") 
        << (piece = new King(white)) << king << white;
    QTest::newRow("adding a black king") 
        << (piece = new King(black)) << king << black;

}

void TestSquare::addingPiecesToSquare()
{
    QFETCH(Piece*, piece);
    QFETCH(std::string, expectedName);
    QFETCH(Piece::Colour, expectedColour);
    
    Square square;
    QVERIFY(!square.hasPiece());
    
    square.addPiece(piece);
    QVERIFY(square.hasPiece());

    QCOMPARE(square.getNameOfPiece(), expectedName);
    QCOMPARE(square.getColourOfPiece(), expectedColour);
}

void TestSquare::removingPiecesFromSquare_data()
{
    std::string empty = "";
    std::string pawn = "Pawn";
    std::string king = "King";
    QTest::addColumn<std::size_t>("index");
    QTest::addColumn<std::string>("before");
    QTest::addColumn<std::string>("after");

    QTest::newRow("removing pawn from a2") << std::size_t(48) << pawn << empty;
    QTest::newRow("trying to remove from empty slot (a3)") 
        << std::size_t(40) << empty << empty;
    QTest::newRow("removing king from e8") << std::size_t(4) << king << empty;
}

void TestSquare::removingPiecesFromSquare()
{
    QFETCH(std::size_t, index);
    QFETCH(std::string, before);
    QFETCH(std::string, after);

    std::vector<Square> squares = ChessBoard::createBoard();
    QVERIFY(squares.size() > index);
    
    QCOMPARE(squares.at(index).getNameOfPiece(), before);

    Piece* piece = squares.at(index).removePiece();
    if (piece) 
    {
        QCOMPARE(piece->getName(), before);
        delete piece;
    }
    QCOMPARE(squares.at(index).getNameOfPiece(), after);
}

void TestSquare::getPiece_data()
{
    std::string empty = "";
    std::string pawn = "Pawn";
    std::string king = "King";
    QTest::addColumn<Square>("square");
    QTest::addColumn<std::string>("expected");

    QTest::newRow("get, expect pawn") << Square(new Pawn(Piece::WHITE)) << pawn;
    QTest::newRow("get, expect king") << Square(new King(Piece::BLACK)) << king;
    QTest::newRow("get, expect nothing") << Square() << empty;
}

void TestSquare::getPiece()
{
    QFETCH(Square, square);
    QFETCH(std::string, expected);
    if (expected == "")
    {
        QVERIFY(square.getPiece() == 0);
    }
    else
    {
        QCOMPARE(square.getPiece()->getName(), expected);
    }
}

// End of Tests

QTEST_MAIN(TestSquare)
#include "testsquare.moc"
