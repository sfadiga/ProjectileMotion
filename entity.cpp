#include "entity.h"

#include <QPainter>
#include <QtMath>
#include <QDebug>

Entity::Entity(QGraphicsItem *parent) : QGraphicsObject (parent), angle_(0.0)
{}

void Entity::update(int dt)
{
    pos().setX(pos().x() + (vel_ * dt));
    pos().setY(pos().y() + (vel_ * dt));
}

void Entity::setPosX(int x)
{
    setX(x);
}

void Entity::setAngle(int x)
{
    angle_ = x;
    QGraphicsObject::update(boundingRect());
}

void Entity::setVel(const qreal &v)
{
    vel_ = v;
}

void Entity::setGravity(const qreal &g)
{
    gravity_ = g;
}

void Entity::setTimescale(const qreal &timescale)
{
    timescale_ = timescale;
}

Tank::Tank(QGraphicsItem *parent) : Entity(parent)
{
    // define the triangle
    QPoint p1(0, 0);
    QPoint p2(40, 0);
    QPoint p3(20, -40);
    points_.append(p1);
    points_.append(p2);
    points_.append(p3);
}

QRectF Tank::boundingRect() const
{
    return QRectF(0, -40, 40, 40);
}

void Tank::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    //draw the tank (a triangle)
    painter->setBrush(Qt::green);
    painter->drawConvexPolygon(points_);

    //draw the cannon (a ball that moves along an arc defined by angle_)
    painter->setBrush(Qt::darkGreen);
    QPointF c = boundingRect().center();
    qreal x = qFastCos(qDegreesToRadians(-angle_)) * 30 + c.x();
    qreal y = qFastSin(qDegreesToRadians(-angle_)) * 30 + c.y();
    cannonPos_.setX(x);
    cannonPos_.setY(y);
    painter->drawEllipse(cannonPos_, 6, 6);
}

QPointF Tank::cannonPos()
{
    return mapToScene(cannonPos_.x(), cannonPos_.y()+6); // return the cannon coordinates in scene
}

void Tank::init()
{
}

Projectile::Projectile(QGraphicsItem *parent) : Entity(parent), time_(0.0)
{
    // define the projectile, represented by a triangle
    QPoint p1(0, 0);
    QPoint p2(10, 0);
    QPoint p3(5, -15);
    points_.append(p1);
    points_.append(p3);
    points_.append(p2);

}

void Projectile::init()
{
    // calc initial velocities in x and y
    vx_ = vel_ * qFastCos(qDegreesToRadians(angle_));
    vy_ = vel_ * qFastSin(qDegreesToRadians(angle_));
    // set initial x, y for calc
    x_ = pos().x();
    y_ = pos().y();
    //rotate relative to the center of the sprite (see QGraphicsView conventions)
    setTransformOriginPoint(boundingRect().center());
    setRotation(angle_); //rotations in qt are clockwise
}

QRectF Projectile::boundingRect() const
{
    return QRectF(0, -15, 10, 10);
}

void Projectile::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    //draw a projectile as a tiny triangle
    painter->setBrush(Qt::red);
    painter->drawConvexPolygon(points_);
}


void Projectile::update(int dt)
{
    // apply the motion equation to the projectile
    time_ += (dt/1000.0) * timescale_;
    qreal x = x_ + (vx_ * time_);
    qreal y = y_ - (vy_ * time_ + ((gravity_/2.0) * time_ * time_));
    setPos(x, y);
    // calculate the projectile angle
    qreal vy = vy_ + gravity_ * time_;
    qreal a = qRadiansToDegrees(qAtan(vy/vx_));

    // 90/270 degree is necessary to keep the projectile angle oriented to the correct position
    if(angle_ > 90)
      setRotation(270-a);
    else
      setRotation(90-a);

}

