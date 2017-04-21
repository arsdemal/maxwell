#include "maxvell_scene.h"

maxwell_scene::maxwell_scene() : QGraphicsScene()
{
    setSceneRect(MIN_X,MIN_Y,MAX_X-MIN_X,MAX_Y-MIN_Y);
    m_t.setInterval(20);
    QObject::connect( &m_t, SIGNAL(timeout()), this, SLOT(advance()));
    m_t.start();
}

maxwell_scene::~maxwell_scene()
{

}
