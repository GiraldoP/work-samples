#include "box.h"
#include "engine.h"
#include <QGraphicsScene>
#include <QGraphicsTextItem>

#define DIM 8

// Sets up the initial game board
Board::Board(QWidget *parent):QGraphicsView(parent)
{
    // Draws the scene for the board
    boardScene = new QGraphicsScene;
    boardScene->setSceneRect(0,0,DIM*100,DIM*100 + 50);

    // BLACK goes first
    turn = "BLACK";

    for (int i = 0; i < DIM; i++)
    {
        for (int j = 0; j < DIM; j++)
        {
            // Creates the dark green background for the board
            class Box *box = new class Box();
            grid[i][j] = box;
            box->setPos(shift*i, shift*j + 50);
            box->brush.setColor(Qt::darkGreen);
            box->setBrush(Qt::darkGreen);
            boardScene->addItem(box);

            // Center Pieces
            if ((i >= DIM/2-1 && i <= DIM/2) && (j >= DIM/2-1 && j <= DIM/2)){
                box->currentPiece->setRect(5 + shift*i, 5 + shift*j + 50, 90, 90);
                box->hasPiece=true;

                if ((i + j)%2==0){
                    box->currentPiece->setBrush(Qt::black);
                    box->getSide = "BLACK";

                }else{
                    box->currentPiece->setBrush(Qt::white);
                    box->getSide = "WHITE";
                }

            boardScene->addItem(box->currentPiece);
            }
        }
    }

    setFixedSize(DIM*100,DIM*100 + 50);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setScene(boardScene);
    QBrush brush;
    brush.setStyle(Qt::SolidPattern);
    brush.setColor(Qt::black);
    setBackgroundBrush(brush);

    turnDisplay = new QGraphicsTextItem();
    turnDisplay->setZValue(1);
    turnDisplay->setDefaultTextColor(Qt::white);
    turnDisplay->setFont(QFont("",18));
    turnDisplay->setPlainText("CLICK ANYWHERE ON THE GRID TO START THE GAME");
    turnDisplay->setPos(width()/2-turnDisplay->boundingRect().width()/2,20);
    boardScene->addItem(turnDisplay);

}

// Function to check if a move is valid for the current player
bool Board::canMove(int r, int c)
{
    if (c < 0 || r < 0 || c >= DIM || r >= DIM || grid[r][c]->hasPiece == true)
    {
        return false;
    }
    for (int i = 0; i < 8; i++)
    {
        if (canFlip(i, r, c))
        {
            return true;
        }
    }
    return false;
}

// Makes a move at the given location
void Board::makeMove(int r, int c, QGraphicsEllipseItem *piece)
{
    // Check all 8 directions
    for (int i = 0; i < 8; i++)
    {
        // Check if it can flip the direction
        if (canFlip(i, r, c))
        {
            // Make the flip
            makeFlip(i, r, c);//, piece);
        }
    }

    // Place the piece
    if(turn == "BLACK"){
        grid[r][c]->currentPiece = piece;
        piece->setBrush(Qt::black);

    }else if(turn == "WHITE"){
        piece->setBrush(Qt::white);
    }
    grid[r][c]->getSide = turn;
    boardScene->addItem(piece);
    grid[r][c]->hasPiece = true;

    // Update the turn
    turn = (turn == "BLACK") ? "WHITE" : "BLACK";

    /* If a move exists, update turn; If board is completed, tally pieces and declare winner;
       If a move does not exist then change the turn back to previous player */
    if(existsMove()){
        if (turn == "BLACK"){
            turnDisplay->setPlainText("TURN : " + turn + " CLICK ANYWHERE ON GRID TO SEE BLACK'S MOVE");

        }else{
            turnDisplay->setPlainText("TURN : " + turn);
        }

    }else if(completeBoard()){
        tallyBoard();

    }else{
        turn = (turn =="BLACK") ? "WHITE" : "BLACK";
        turnDisplay->setPlainText(QString("NO TURNS FOR ") + ((turn == "BLACK") ? "WHITE" : "BLACK") + "! " + turn + " GOES AGAIN!");
    }

    turnDisplay->setPos(width()/2-turnDisplay->boundingRect().width()/2,20);
}

// Function to attempt a move
// The return is true if the move was successful
bool Board::tryMove(int r, int c, QGraphicsEllipseItem *piece)
{
    // Check if the current move is valid
    if (!canMove(r, c))
    {
        return false;
    }

    // Make the move
    makeMove(r, c, piece);
    return true;
}

// Function to check if a line of pieces in a given direction can be
// flipped by placing a piece at location (r, c)
bool Board::canFlip(int dir, int r, int c)
{
    // Get the direction
    int dRow = dRows[dir];
    int dCol = dCols[dir];

    // Check if there is at least one piece of the opposite color
    int curRow = dRow + r;
    int curCol = dCol + c;
    if (curRow < 0 || curRow >= DIM || curCol < 0 || curCol >= DIM ||
            grid[curRow][curCol]->getSide == turn || grid[curRow][curCol]->getSide == "None")
    {
        return false;
    }

    // Find the first value that is not the opposite color
    while (curRow >= 0 && curRow < DIM && curCol >= 0 && curCol < DIM &&
            grid[curRow][curCol]->getSide != turn && grid[curRow][curCol]->getSide != "None")
    {
        curRow += dRow;
        curCol += dCol;
    }

    // Check that the direction ends in the correct color
    return (curRow >= 0 && curRow < DIM && curCol >= 0 && curCol < DIM &&
            grid[curRow][curCol]->getSide == turn);
}

// Flips a line of pieces in a given direction originating from the cell
// (r, c)
void Board::makeFlip(int dir, int r, int c)
{
    // Get the direction
    int dRow = dRows[dir];
    int dCol = dCols[dir];

    // Check if there is at least one piece of the opposite color
    int curRow = dRow + r;
    int curCol = dCol + c;

    // Find the first value that is not the opposite color
    while (curRow >= 0 && curRow < DIM && curCol >= 0 && curCol < DIM &&
            grid[curRow][curCol]->getSide != turn && grid[curRow][curCol]->getSide != "None")
    {
        if(turn == "BLACK"){
            grid[curRow][curCol]->currentPiece->setBrush(Qt::black);
        }else if(turn == "WHITE"){
            grid[curRow][curCol]->currentPiece->setBrush(Qt::white);
        }

        grid[curRow][curCol]->getSide = turn;
        curRow += dRow;
        curCol += dCol;
    }
}

// Check if there exists a move for the current player
bool Board::existsMove()
{
    // Check every spot
    for (int i = 0; i < DIM; i++)
    {
        for (int j = 0; j < DIM; j++)
        {
            // Check if the move works
            if (canMove(i, j))
            {
                return true;
            }
        }
    }

    // No moves worked
    return false;
}

// Function that checks if there is a move
bool Board::completeBoard()
{
    // Check if current player can make a move
    if (existsMove())
    {
        return false;
    }

    // Check if the other player can make a move
    turn = (turn == "BLACK") ? "WHITE" : "BLACK";
    bool ret = !existsMove();
    turn = (turn == "BLACK") ? "WHITE" : "BLACK";
    return ret;
}

// Tallys the final board and declares winner
void Board::tallyBoard()
{
    int B = 0, W = 0;
    for (int i = 0; i < DIM; i++){
        for (int j = 0; j < DIM; j++){
            QString side = grid[i][j]->getSide;
            if (side == "BLACK"){
                B++;
            }else if(side == "WHITE"){
                W++;
            }
        }
    }

    if (B > W){
        turnDisplay->setPlainText(QString("BLACK WINS! B : ") + QString::number(B) + ", W : " + QString::number(W));
    }else if(B < W){
        turnDisplay->setPlainText(QString("WHITE WINS! B : ") + QString::number(B) + ", W : " + QString::number(W));
    }else if(B == W){
        turnDisplay->setPlainText("IT'S A TIE!");
    }
}
