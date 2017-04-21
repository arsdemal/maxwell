#ifndef GRAPH_SCENE_H
#define GRAPH_SCENE_H

#include <QGraphicsScene>
#include <QTimer>

#define MAXG_X 200
#define MAXG_Y 150
#define MING_X -200
#define MING_Y -150

class graph_scene: public QGraphicsScene
{
    QTimer qraph_t;
public:
    graph_scene();
    ~graph_scene();
};

#endif // GRAPH_SCENE_H
