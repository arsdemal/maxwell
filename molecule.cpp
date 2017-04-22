#include "molecule.h"
#include "test_scene.h"
#include "widget.h"

QList <molecule*> matrix[STEP_MATRIX][STEP_MATRIX];
double molecule::maxSpeed;

double molecule::getMaxSpeed()
{
    return maxSpeed;
}

molecule::molecule(double molx, double moly, double molsx, double molsy, double mrad, QList <molecule*> (*m)[STEP_MATRIX][STEP_MATRIX])
{
    m_x = molx;
    m_y = moly;
    m_radius = mrad;
    m_speed_x = molsx;
    m_speed_y = molsy;
    matrix = m;
    setPos(m_x,m_y);
    lp_x = (STEP_MATRIX*(m_x - MIN_X)/(MAX_X - MIN_X));
    lp_y = (STEP_MATRIX*(m_y - MIN_Y)/(MAX_Y - MIN_Y));
}

molecule::~molecule()
{

}

QRectF molecule::boundingRect() const
{
    return QRectF( -m_radius-1, -m_radius-1,
                   2*m_radius+2, 2*m_radius+2);
}

void molecule::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QBrush brush;
    brush.setStyle(Qt::SolidPattern);

    double speed = sqrt(pow(m_speed_x,2)+pow(m_speed_y,2));

    if (speed>maxSpeed/2) {
        if (speed<maxSpeed) {
            brush.setColor(QColor(255,0,255*2*(maxSpeed-speed)/maxSpeed,255));
        } else {
            brush.setColor(QColor(255,0,0,255));
        }
    } else {
        brush.setColor(QColor(255*2*speed/maxSpeed,0,255,255));
    }

    painter->setBrush(brush);
    painter->drawEllipse( -m_radius, -m_radius,
                          2*m_radius, 2*m_radius);
}

inline double my_abs(double a)
{
    return (a > 0) ? a : -a;
}

inline double mul_pair(QPair<double,double> a, QPair<double,double> b)
{
    return a.first*b.first + a.second*b.second;
}

inline QPair<double,double> mul_pair(double a, QPair<double,double> b)
{
    QPair<double,double> c;
    c.first = b.first*a;
    c.second = b.second*a;
    return c;
}

inline QPair<double,double> div_pair(QPair<double,double> a, double b)
{
    a.first = a.first/b;
    a.second = a.second/b;
    return a;
}

inline double mod_pair(QPair<double,double> a)
{
    return sqrt(pow(a.first,2) + pow(a.second,2) );
}

inline bool molecule::my_collision(molecule *b){

    double x1 = m_x, y1 = m_y;
    double x2 = b->m_x, y2 = b->m_y;
    double r = m_radius;

    if(sqrt(pow(x1 - x2,2) + pow(y1 - y2,2)) < 2*r)
    {
        return 1;
    }

    return 0;
}

void molecule::mac(molecule *b)
{
    QPair<double,double> u1, u2;
    QPair<double,double> dir, dir2;

    u1.first = m_speed_x;
    u1.second = m_speed_y;
    u2.first = b->m_speed_x;
    u2.second = b->m_speed_y;
    dir.first = m_x - b->m_x;
    dir.second = m_y - b->m_y;

    double AB = mod_pair(dir);
    double abs_u = mod_pair(u1);
    double cos_a = ( abs_u != 0) ? mul_pair(dir,u1)/(AB*abs_u) : 0;
    double D1;
    double BC;

    D1 = pow(AB,2)*(pow(cos_a,2) - 1) + 4*pow(m_radius,2);

    BC = cos_a*AB+sqrt(D1);
    double a = (abs_u != 0) ? BC/abs_u : 0;

    m_x -= m_speed_x*a;
    m_y -= m_speed_y*a;

    dir.first = m_x - b->m_x;
    dir.second = m_y - b->m_y;

    if( dir.first == 0)
    {
        dir2.first = 1;
        dir2.second = 0;
    }
    else
        if( dir.second == 0)
        {
            dir2.second = 1;
            dir2.first = 0;
        }
    else
        {
            dir2.second = 1;
            dir2.first = -dir.second/dir.first;
        }
    QPair<double,double> u1_par, u2_par;
    QPair<double,double> u1_per, u2_per;

    u1_par = mul_pair(mul_pair(u1,dir)/pow(mod_pair(dir),2),dir);
    u2_par = mul_pair(mul_pair(u2,dir)/pow(mod_pair(dir),2),dir);

    u1_per = mul_pair(mul_pair(u1,dir2)/pow(mod_pair(dir2),2),dir2);
    u2_per = mul_pair(mul_pair(u2,dir2)/pow(mod_pair(dir2),2),dir2);

    m_speed_x = u2_par.first + u1_per.first;
    m_speed_y = u2_par.second + u1_per.second;
    b->m_speed_x = u1_par.first + u2_per.first;
    b->m_speed_y = u1_par.second + u2_per.second;

    m_x += (1 - a)*m_speed_x;
    m_y += (1 - a)*m_speed_y;
}

void molecule::setMaxSpeed(double speed)
{
    maxSpeed = speed;
}

void molecule::advance(int phase)
{
    if(phase == 0) {
        return;
    }

    int lp_xb = lp_x;
    int lp_yb = lp_y;

    m_x += m_speed_x;
    m_y += m_speed_y;

    lp_x = (STEP_MATRIX*((m_x - MIN_X)/(MAX_X - MIN_X)));
    lp_y = (STEP_MATRIX*((m_y - MIN_Y)/(MAX_Y - MIN_Y)));
    if(lp_x < 0)
        lp_x = 0;
    if(lp_x > STEP_MATRIX-1 )
        lp_x = STEP_MATRIX - 1;
    if(lp_y < 0)
        lp_y = 0;
    if(lp_y > STEP_MATRIX-1 )
        lp_y = STEP_MATRIX - 1;

    if(m_x + m_radius > MAX_X || m_x - m_radius < MIN_X)
    {
        if(m_x + m_radius > MAX_X)
            m_x = 2*MAX_X - (m_speed_x + m_x + m_radius) - m_radius;

        if(m_x - m_radius < MIN_X)
            m_x = 2*MIN_X - (m_x - m_radius +m_speed_x) + m_radius;

        m_speed_x = -m_speed_x;
        setPos(m_x,m_y);
    }

    if(m_y + m_radius > MAX_Y || m_y - m_radius < MIN_Y)
    {
        if(m_y + m_radius > MAX_Y)
            m_y = 2*MAX_Y - (m_speed_y + m_y + m_radius) - m_radius;

        if(m_y - m_radius < MIN_Y)
            m_y = 2*MIN_Y - (m_y - m_radius +m_speed_y) + m_radius;

        m_speed_y = -m_speed_y;
    }

    int list_count = 0;

    if(lp_x > 0 && lp_x < (STEP_MATRIX - 1) && lp_y > 0 && lp_y < (STEP_MATRIX - 1))
    {
        for( int i = lp_x - 1; i <= lp_x + 1; i++ )
            for( int j = lp_y - 1; j <= lp_y + 1; j++)
            {
                list_count = (*matrix)[i][j].count();
                for( int q = 0; q < list_count; q++)
                {
                    bool f;
                    molecule* g = (*matrix)[i][j].at(q);
                    if(g != this && my_collision(g)) // && inf_col.indexOf(g) == -1)
                    {
                        mac((*matrix)[i][j].at(q));
                    }
                }
            }
    }
    else
        if((lp_x > 0 && (lp_x < (STEP_MATRIX - 1)) && lp_y == 0) ||
                (lp_x > 0 && (lp_x < (STEP_MATRIX - 1)) && lp_y == (STEP_MATRIX - 1)) || \
                (lp_y > 0 && lp_y < (STEP_MATRIX - 1) && lp_x == (STEP_MATRIX - 1)) || \
                (lp_y > 0 && lp_y < (STEP_MATRIX - 1) && lp_x == 0))
        {
            int a,b,c,d;
            if(lp_y == 0)
            {
                a = lp_x - 1;
                b = lp_x + 1;
                c = lp_y;
                d = lp_y + 1;
            }
            if( lp_y == STEP_MATRIX - 1)
            {
                a = lp_x - 1;
                b = lp_x + 1;
                c = lp_y - 1;
                d = lp_y;
            }
            if(lp_x == 0)
            {
                c = lp_y - 1;
                d = lp_y + 1;
                a = lp_x;
                b = lp_x + 1;
            }
            if( lp_x == STEP_MATRIX - 1)
            {
                c = lp_y - 1;
                d = lp_y + 1;
                a = lp_x - 1;
                b = lp_x;
            }
            for( int i = a; i <= b; i++ )
                for( int j = c; j <= d; j++)
                {
                    list_count = (*matrix)[i][j].count();
                    for( int q = 0; q < list_count; q++)
                    {
                        molecule* g = (*matrix)[i][j].at(q);
                        if(g != this && my_collision(g)) // && inf_col.indexOf(g) == -1)
                        {
                            mac((*matrix)[i][j].at(q));
                        }
                    }
                }
        }
        else
        {
            int a,b,c,d;
            if(lp_y == 0 && lp_x == 0)
            {
                a = 0;
                b = 1;
                c = 0;
                d = 1;
            }
            if( lp_y == STEP_MATRIX - 1 && lp_x == 0)
            {
                a = 0;
                b = 1;
                c = STEP_MATRIX - 2;
                d = STEP_MATRIX - 1;
            }
            if(lp_y == STEP_MATRIX - 1 && lp_x == STEP_MATRIX - 1)
            {
                a = STEP_MATRIX - 2;
                b = STEP_MATRIX - 1;
                c = STEP_MATRIX - 2;
                d = STEP_MATRIX - 1;
            }
            if( lp_x == STEP_MATRIX - 1 && lp_y == 0)
            {
                a = STEP_MATRIX - 2;
                b = STEP_MATRIX - 1;
                c = 0;
                d = 1;
            }
            for( int i = a; i <= b; i++ )
                for( int j = c; j <= d; j++)
                {
                    list_count = (*matrix)[i][j].count();
                    for( int q = 0; q < list_count; q++)
                    {
                        molecule* g = (*matrix)[i][j].at(q);
                        if(g != this && my_collision(g)) // && inf_col.indexOf(g) == -1)
                        {
                            mac((*matrix)[i][j].at(q));
                        }
                    }
                }
        }

    if( lp_xb != lp_x || lp_yb != lp_y)
    {
        (*matrix)[lp_x][lp_y].append(this);
        (*matrix)[lp_xb][lp_yb].removeAt((*matrix)[lp_xb][lp_yb].indexOf(this));
    }

    setPos(m_x,m_y);
}
