#include "greenbird.h"

Greenbird::Greenbird(float x, float y, float radius, QTimer *timer, QPixmap pixmap, b2World *world, QGraphicsScene *scene):Bird(x, y, radius, timer, pixmap, world, scene)
{

}

void Greenbird::skill()
{
    b2Vec2 speed = g_body->GetLinearVelocity();
    g_body->SetLinearVelocity(b2Vec2(speed.x*-1,speed.y));
}
