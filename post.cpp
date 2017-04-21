#include "post.h"

post::post(int w, int h, int p_x, int p_y)
{
    p_w = w;
    p_h = h;
    setPos(p_x,p_y);
}

post::~post()
{

}

QRectF post::boundingRect() const
{
    return QRectF(0,0,p_w,p_h);
}

void post::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->drawRect(0,0,p_w,p_h);
}

void post::set_size(int h)
{
    if (p_h != h) {
           prepareGeometryChange();
           p_h = h;
       }
}
