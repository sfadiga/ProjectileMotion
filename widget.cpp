#include "widget.h"
#include "ui_widget.h"
#include <QSlider>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    scene_ = new QGraphicsScene(0, 0, 760, 540);
    ui->graphicsView->setScene(scene_);

    tank_ = new Tank();
    tank_->setPosX(ui->horizontalSliderPosition->value());
    tank_->setY(540);
    tank_->setAngle(ui->horizontalSliderAngle->value());
    connect(ui->horizontalSliderPosition, SIGNAL(valueChanged(int)), tank_, SLOT(setPosX(int)));
    connect(ui->horizontalSliderAngle, SIGNAL(valueChanged(int)), tank_, SLOT(setAngle(int)));
    scene_->addItem(tank_);

    connect(&updateTimer_, &QTimer::timeout, this, &Widget::update);
    clock_.start();
    updateTimer_.start(MS_UPDATE);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::update()
{
    int elapsed = clock_.restart();

    // update all entities states
    foreach(Entity* e, projectiles_)
    {
        // check if the entity is visible on the scene
        if(e->pos().x() < (-50) || // adds margin area
                e->pos().x() > 760 ||
                e->pos().y() < (-50) || // adds margin area
                e->pos().y() > 540) // simple and fast to check if the entity is out of the scene
        {
            // resets internal entity value, also "return" an entity to the its pool
            projectiles_.remove(e);
            scene_->removeItem(e);
            delete e;
        }
        else
        {
            e->update(elapsed);
        }

    }
}

void Widget::on_pushButtonLauch_clicked()
{
    Entity* proj = new Projectile();
    scene_->addItem(proj);
    proj->setAngle(ui->horizontalSliderAngle->value());
    proj->setGravity(ui->doubleSpinBoxGravity->value());
    proj->setVel(ui->spinBoxVelocity->value());
    proj->setTimescale(ui->doubleSpinBoxTimescale->value());
    QPointF c = static_cast<Tank*>(tank_)->cannonPos();
    proj->setPos(c);
    proj->init();
    projectiles_.insert(proj);
}
