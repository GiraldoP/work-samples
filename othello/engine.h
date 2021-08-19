#ifndef ENGINE_H
#define ENGINE_H

#include "box.h"
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsTextItem>
#include <QLabel>

#define DIM 8

class Board:public QGraphicsView
{
    class Box *grid[DIM][DIM];

    public:
    QGraphicsScene *boardScene;
    QGraphicsTextItem *turnDisplay;
    QString turn;
    int shift = 100;
    int dRows[8] = {-1,-1,-1,0,0,1,1,1};
    int dCols[8] = {-1,0,1,-1,1,-1,0,1};

    // Constructor
    Board(QWidget *parent=0);
    // End of Constructor

    // Functions

    // Function to check if a move is valid for the current player
    bool canMove(int r, int c);

    // Makes a move at the given location
    void makeMove(int r, int c, QGraphicsEllipseItem *piece);

    // The return is true if the move was successful
    bool tryMove(int r, int c, QGraphicsEllipseItem *piece);

    // Function to check if a line of pieces in a given direction can be
    // flipped by placing a piece at location (r, c)
    bool canFlip(int dir, int r, int c);

    // Flips a line of pieces in a given direction originating from the cell
    // (r, c)
    void makeFlip(int dir, int r, int c);//, QGraphicsEllipseItem *piece);

    // Check if there exists a move for the current player
    bool existsMove();

    // Function that checks if there is a move
    bool completeBoard();

    // Tallys the final board and declares winner
    void tallyBoard();
    /* End of Functions */

};
#endif // ENGINE_H
