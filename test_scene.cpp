#include "test_scene.h"

test_scene::test_scene() : QGraphicsScene()
{
    setSceneRect(MIN_X,MIN_Y,MAX_X-MIN_X,MAX_Y-MIN_Y);
    m_t.setInterval(20);
    QObject::connect( &m_t, SIGNAL(timeout()), this, SLOT(advance()));
    m_t.start();
}

test_scene::~test_scene()
{

}
