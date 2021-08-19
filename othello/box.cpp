#include "box.h"
#include "engine.h"
#include <QGraphicsItem>
#include <QGraphicsRectItem>
#include <QGraphicsSceneMouseEvent>

extern Board *game;
Box::Box(QGraphicsItem *parent):QGraphicsRectItem(parent)
{
    // Default settings for the board's squares
    setRect(0,0,100,100);
    brush.setStyle(Qt::SolidPattern);
    setZValue(-1);
    hasPiece = false;
    getSide = "None";
    currentPiece = new QGraphicsEllipseItem();

}

void Box::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    int row, col;
    // Computer's Move
    if(game->turn == "BLACK"){
        row = rand() % DIM;
        col = rand() % DIM;

        while (!game->canMove(row, col)){
            row = rand() % DIM;
            col = rand() % DIM;
        }

        QGraphicsEllipseItem *piece = new QGraphicsEllipseItem;
        piece->setRect(5 + row * game->shift, 55 + col * game->shift, 90, 90);
        game->tryMove(row, col, piece);

    }else{
        // Player's Move
        row = this->scenePos().x()/100;
        col = this->scenePos().y()/100;
        currentPiece->setRect(5 + this->scenePos().x(), 5 + this->scenePos().y(), 90, 90);
        game->tryMove(row, col, currentPiece);
    }
}
