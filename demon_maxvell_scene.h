#ifndef TRAP_OF_MAXVELL_SCENE_H
#define TRAP_OF_MAXVELL_SCENE_H

#include <QGraphicsScene>
#include <QTimer>

class demon_maxvell_scene: public QGraphicsScene
{
public:
    demon_maxvell_scene();
    ~demon_maxvell_scene();
    QTimer clk;
};

#endif // TRAP_OF_MAXVELL_SCENE_H
