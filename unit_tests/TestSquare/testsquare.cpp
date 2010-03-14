// system includes
#include <QObject>
#include <QtTest/QtTest>

// classes to be tested
#include "../../src/logic/square.h"
#include "../../src/logic/piece.h"
#include "../../src/logic/pawn.h"

class TestSquare : public QObject
{
    Q_OBJECT

private slots:
    // Test case declarations
    void constructing_data();
    void constructing();
    void addingPiecesToSquare_data();
    void addingPiecesToSquare();
};
// Use Q_DECLARE_METATYPE in order to use 'custom' types in _data() functions.
Q_DECLARE_METATYPE(std::string)
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
    QTest::addColumn<Piece*>("piece");
    QTest::addColumn<std::string>("expectedName");
    QTest::addColumn<Piece::Colour>("expectedColour");

    Piece* piece = 0;
    QTest::newRow("adding a white pawn") 
        << (piece = new Pawn(white)) << pawn << white;
    QTest::newRow("adding a black pawn") 
        << (piece = new Pawn(black)) << pawn << black;
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

// End of Tests

QTEST_MAIN(TestSquare)
#include "testsquare.moc"
