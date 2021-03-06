#include "widget.h"
#include "ui_widget.h"
#include "molecule.h"
#include "post.h"
#include <QString>

#define N_P 50 // количество столбиков в графике
#define RAD 4 // радиус шарика
#define K 10
#define MAXWELL 0
#define DEMON_MAXWELL 1
#define COUNT_MOD 2

QList<molecule*> m_list;
QList<post*> p_list;
post *my_post = new post(20,50,0,0);
double u0 = 0;
QList <molecule*> m[STEP_MATRIX][STEP_MATRIX];

int p_height[N_P] = {};
int p_height_avg[N_P] = {};
int count_exp = 1;
int counterMod = 0;

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    ui->graphicsView->setRenderHint(QPainter::Antialiasing);

    connect(ui->startButton,SIGNAL(clicked()),this,SLOT(start_exp()));
    connect(ui->stopButton,SIGNAL(clicked()),this,SLOT(stop_exp()));
    connect(ui->exitButton, SIGNAL(clicked()), this, SLOT(close()));
    connect(ui->decModButton,SIGNAL(clicked()),this,SLOT(decMod()));
    connect(ui->incModButton,SIGNAL(clicked()),this,SLOT(incMod()));


    flag = 0;
    f_restart = 0;
}

Widget::~Widget()
{
    delete ui;
}

double my_rand(double a, double b)
{
    return double(qrand())/RAND_MAX*(b - a) + a;
}

void Widget::start_exp()
{
    if( !f_restart){

        scene = new test_scene;
        ui->graphicsView->fitInView(scene->sceneRect());
        ui->graphicsView->setScene(scene);
        ui->graphicsView->setHorizontalScrollBarPolicy ( Qt::ScrollBarAlwaysOff );
        ui->graphicsView->setVerticalScrollBarPolicy ( Qt::ScrollBarAlwaysOff );

        bool ok;
        count_mol = ui->lineEdit->text().toInt(&ok, 10);
        u0 = ui->lineEdit_2->text().toInt(&ok, 10);

        molecule::setMaxSpeed(u0);

        QTime midnight(0,0,0);
        qsrand(midnight.secsTo(QTime::currentTime()));

        switch (counterMod) {
        case 0:
            maxwellMod();
            break;
        case 1:
            demon_maxwellMod();
            break;
        }

        ui->startButton->setText("Restart");
        f_restart = 1;
    }
    else
    {
        scene->~test_scene();
        scene_g->~graph_scene();
        m_list.clear();
        for(int i = 0; i < STEP_MATRIX; i++)
            for( int j = 0; j < STEP_MATRIX; j++)
                m[i][j].clear();
        p_list.clear();
        g_t.stop();
        flag = 0;
        f_restart = 0;
        this->start_exp();
    }
}

void Widget::maxwellMod() {
    connect( &g_t, SIGNAL(timeout()),this, SLOT(set_graph()));
    g_t.setInterval(20);

    scene_g = new graph_scene;
    ui->graphicsView_2->setScene(scene_g);
    ui->graphicsView_2->setRenderHint(QPainter::Antialiasing);
    ui->graphicsView_2->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);



    int n = count_mol;
    int j = -8;
    while( n > 0)
    {

        for( int i = -5; i < 5; i++)
        {
            double psi = my_rand(0,360)/360*2*M_PI;
            molecule *b = new molecule(i*(RAD*2 + 6), j*(RAD*2 + 6) ,cos(psi)*u0,sin(psi)*u0,RAD, &m);
            m_list.append(b);
            m[b->lp_x][b->lp_y].append(b);
            scene->addItem(b);
            n--;
            if( n < 1) {
                //b->setPen;
                break;
            }
        }
        j++;
    }
    T = 12*u0*u0;
    double a = 0.6;
    double b = 1;
    QPen pen(Qt::red);
    double du = u0/10;
    for( double u1 = 0; u1 < 3*u0; u1 += du )
    {
        double n1 = count_mol*K*(exp(-12*u1*u1/T)-exp(-12*(u1+du)*(u1+du)/T));
        double n2 = count_mol*K*(exp(-12*(u1+du)*(u1+du)/T)-exp(-12*(u1+2*du)*(u1+2*du)/T));
        scene_g->addLine(u1/u0*10*K*b,-n1*a,(u1+du)/u0*10*K*b,-n2*a,pen);
    }
    scene_g->addLine(0,0,0,-200);
    scene_g->addLine(0,-200,5,-190);
    scene_g->addLine(0,-200,-5,-190);
    scene_g->addLine(0,0,300,0);
    scene_g->addLine(290,5,300,0);
    scene_g->addLine(290,-5,300,0);
    QGraphicsTextItem *text1 = scene_g->addText(QString("V, speed"));
    QGraphicsTextItem *text2 = scene_g->addText(QString("N, count"));
    text1->setPos(260,0);
    text2->setPos(-70,-210);
    g_t.start();
}

void Widget::demon_maxwellMod() {
    molecule* b = new molecule(0,0,0,0,RAD,&m);
    m_list.append(b);
    m[b->lp_x][b->lp_y].append(b);
    scene->addItem(b);
}

void Widget::checkMod() {
    switch (counterMod) {
    case 0:
        ui->textMode->setText("Maxwell");
        break;
    case 1:
        ui->textMode->setText("Demon of Maxwell");
        break;
    default:
        break;
    }
}

void Widget::stop_exp()
{
    scene->m_t.stop();
    g_t.stop();

    cont_button = new QPushButton(this);
    cont_button->setText("Continue");
    cont_button->setGeometry(530, 60, 101, 41);
    cont_button->show();
    connect(cont_button,SIGNAL(clicked()),this,SLOT(continue_exp()));
}

void Widget::set_graph()
{
    QList<molecule*> m_g = m_list;
    int n = m_g.count();
    double* m_speed = new double[n];

    memset(p_height,0,sizeof(p_height));

    if(!flag)
    {
        memset(p_height_avg,0,sizeof(p_height_avg));
        count_exp = 1;
    }

    for(int i = 0; i < n; i++)
    {
        m_speed[i] = sqrt(pow(m_g[i]->m_speed_x,2) + pow(m_g[i]->m_speed_y,2));

        int a = floor(m_speed[i]/(3*u0)*N_P);
        p_height[a]++;
    }

    if(!flag)
    {
        for( int i = 0; i < N_P; i++)
        {
            p_height_avg[i] += p_height[i];
            post* p = new post(3*K*10/N_P,K*p_height_avg[i]/count_exp,i*3*K*10/N_P,-K*p_height_avg[i]/count_exp);
            p_list.append(p);
            scene_g->addItem(p);
        }
        flag = 1;
        count_exp++;
    }
    else
    {
        for( int i = 0; i < N_P; i++)
        {
            p_height_avg[i] += p_height[i];
            p_list[i]->set_size(K*p_height_avg[i]/count_exp);
            p_list[i]->setY(-K*p_height_avg[i]/count_exp);
        }
        count_exp++;
    }
}

void Widget::continue_exp()
{
    scene->m_t.start();
    g_t.start();
    delete cont_button;
}

void Widget::incMod()
{
    counterMod = (counterMod + 1)%COUNT_MOD;
    checkMod();
}

void Widget::decMod()
{
    counterMod = (counterMod - 1)%COUNT_MOD;
    checkMod();
}
