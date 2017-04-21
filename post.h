#ifndef POST_H
#define POST_H

#include <QGraphicsItem>
#include <QPainter>

class post: public QGraphicsItem
{
public:
    int p_w;
    int p_h;
public:
    post(int w, int h, int p_x, int p_y);
    ~post();
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    void set_size(int h);
};

#endif // POST_H
