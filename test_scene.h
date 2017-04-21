#ifndef EXP_SCENE_H
#define EXP_SCENE_H

#include <QGraphicsScene>
#include <QTimer>
#include <QList>
#include "molecule.h"

#define MIN_X -200
#define MIN_Y -200
#define MAX_X 200
#define MAX_Y 200

class test_scene: public QGraphicsScene
{

public:
    test_scene();
    ~test_scene();
    QTimer m_t;
};

#endif // EXP_SCENE_H
