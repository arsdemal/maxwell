#ifndef MOL_H
#define MOL_H

#include <QGraphicsItem>
#include <QPainter>
#include <qmath.h>
#include <QPair>
#include <QList>

#define STEP_MATRIX 100
#define STEP 10

class molecule: public QGraphicsItem
{
    QList <molecule*> (*matrix)[STEP_MATRIX][STEP_MATRIX];
public:
    double m_speed_x;
    double m_speed_y;
    int lp_x;
    int lp_y;
    //QList <mol*> inf_col; // информация о столкновениях с другими молекулами
private:
    double m_x;
    double m_y;
    double m_radius;


public:
    molecule(double molx, double moly, double molsx, double molsy, double mrad, QList<molecule *> (*m)[STEP_MATRIX][STEP_MATRIX]);
    ~molecule();
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    void advance(int phase);
    bool my_collision(molecule *b);
    void mac(molecule *b);
};

#endif // MOL_H
