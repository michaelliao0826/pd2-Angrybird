#include "bluebird.h"

Bluebird::Bluebird(float x, float y, float radius, QTimer *timer, QPixmap pixmap, b2World *world, QGraphicsScene *scene):Bird(x, y, radius, timer, pixmap, world, scene)
{
    t = timer;
    image = pixmap;
    w = world;
    s = scene;
}

Bluebird::~Bluebird()
{
    if(useTimes == 0)
    {
        for(int i = 0 ; i < 2 ; i++)
        {
            delete blue[i];
        }
    }
}

void Bluebird::skill()
{
    b2Vec2 speed = g_body->GetLinearVelocity();
    float currentX = g_body->GetPosition().x;
    float currentY = g_body->GetPosition().y;

    g_body->SetLinearVelocity(b2Vec2(speed.x*1.2,speed.y*1.2));

    blue[0] = new Bluebird(currentX, currentY+0.5, 0.5, t, QPixmap(":/blue.png").scaled(30*1,30*1),w,s);
    blue[0]->setLinearVelocity(b2Vec2(speed.x*1.2,speed.y*2.0));

    blue[1] = new Bluebird(currentX, currentY-0.5, 0.5, t, QPixmap(":/blue.png").scaled(30*1,30*1), w, s);
    blue[1]->setLinearVelocity(b2Vec2(speed.x*1.2,speed.y*0.25));
}
