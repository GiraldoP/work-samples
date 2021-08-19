#include <QApplication>
#include <QGraphicsSceneMouseEvent>
//#include "game.h"
#include "engine.h"

Board *game;
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    game = new Board();
    game->show();
    return a.exec();
}
