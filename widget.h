#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QTime>
#include <QList>
#include <QPushButton>
#include "test_scene.h"
#include "molecule.h"
#include "graph_scene.h"
#include "demon_maxvell_scene.h"

extern QList<molecule*> m_list;

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT
    QTimer g_t;
    QPushButton *cont_button;
    int count_mol;
    int T;
    bool flag;
    bool f_restart;


public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();
    void checkMod();
    void maxwellMod();
    void demon_maxwellMod();

public slots:
    void start_exp();
    void stop_exp();
    void set_graph();
    void continue_exp();
    void incMod();
    void decMod();

private:
    Ui::Widget *ui;
    test_scene *scene;
    graph_scene* scene_g;
};

#endif // WIDGET_H
