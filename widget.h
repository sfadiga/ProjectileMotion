#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QGraphicsScene>
#include <QTimer>
#include <QTime>
#include <QSet>

#include "entity.h"

static const int MS_UPDATE = 16;

namespace Ui {
class Widget;
}

//!
//! \brief The Widget class - this is the main window class, it also contains the controls to the scene.
//!
class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = nullptr);
    ~Widget();

public slots:
    void update();

private slots:
    void on_pushButtonLauch_clicked();

private:
    Ui::Widget *ui;
    QGraphicsScene *scene_;
    QTimer updateTimer_;
    QTime clock_;

    Entity* tank_;
    QSet<Entity*> projectiles_;

};

#endif // WIDGET_H
