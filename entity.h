#ifndef ENTITY_H
#define ENTITY_H

#include <QGraphicsObject>

//!
//! \brief The Entity class - abstract class with some standard methods defined to be used in the exe loop
//!
class Entity : public QGraphicsObject
{
    Q_OBJECT
public:
    Entity(QGraphicsItem *parent = nullptr);
    virtual ~Entity() = default;
    virtual void init() = 0;
    virtual void update(int dt);

    void setTimescale(const qreal &timescale);

public slots:
    void setPosX(int x);
    void setAngle(int x);
    void setVel(const qreal &v);
    void setGravity(const qreal &g);
protected:
    qreal vel_;
    qreal angle_;
    qreal gravity_;
    qreal timescale_;
};

//!
//! \brief The Tank class - the shooting entity, it is possible to move it horizontally and change the launch angle
//!
class Tank : public Entity
{
    Q_OBJECT
public:
    Tank(QGraphicsItem *parent = nullptr);
    ~Tank() override = default;
    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;

    QPointF cannonPos();

    // Entity interface
public:
    void init() override;

private:
    QVector<QPoint> points_;
    QPointF cannonPos_;
};

//!
//! \brief The Projectile class - a small triangle tha is projected from the tank, it is the entity that implements
//! the projectile equations
//! most of the equations came from here: https://en.wikipedia.org/wiki/Projectile_motion
//!
class Projectile : public Entity
{
    Q_OBJECT
public:
    Projectile(QGraphicsItem *parent = nullptr);
    ~Projectile() override = default;
    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;

    // Entity interface
public:
    void update(int dt) override;
    void init() override;

private:
    QVector<QPoint> points_;
    qreal time_; // advances animation timing
    qreal vx_; // starting velocity in x, calculated
    qreal vy_; // starting velocity in y, calculated
    qreal x_; // current position in x, calculated
    qreal y_; // current position in y, calculated

};

#endif // ENTITY_H
