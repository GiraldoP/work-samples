#ifndef BOX_H
#define BOX_H

#include <QGraphicsRectItem>
#include <QBrush>
#include "engine.h"

class Box:public QGraphicsRectItem
{
public:

    Box(QGraphicsItem *parent=0);

    QBrush brush;
    Box *currentBox;
    QGraphicsEllipseItem *currentPiece;
    bool hasPiece = false;
    QString getSide;

    void mousePressEvent(QGraphicsSceneMouseEvent *event);
};

#endif // BOX_H
