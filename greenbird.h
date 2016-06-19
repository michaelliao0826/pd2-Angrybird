#ifndef GREENBIRD_H
#define GREENBIRD_H

#include <bird.h>
#include <QPixmap>
#include <QGraphicsScene>
#include <QTimer>

#define BIRD_DENSITY 10.0f
#define BIRD_FRICTION 0.2f
#define BIRD_RESTITUTION 0.5f

class Greenbird : public Bird
{
public:
    Greenbird(float x, float y, float radius, QTimer *timer, QPixmap pixmap, b2World *world, QGraphicsScene *scene);
    void skill();
private:

};

#endif // GREENBIRD_H
