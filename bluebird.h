#ifndef BLUEBIRD_H
#define BLUEBIRD_H

#include <bird.h>
#include <QPixmap>
#include <QGraphicsScene>
#include <QTimer>

#define BIRD_DENSITY 10.0f
#define BIRD_FRICTION 0.2f
#define BIRD_RESTITUTION 0.5f

class Bluebird : public Bird
{
public:
    Bluebird(float x, float y, float radius, QTimer *timer, QPixmap pixmap, b2World *world, QGraphicsScene *scene);
    ~Bluebird();
    void skill();

private:
    QTimer *t;
    QPixmap image;
    b2World *w;
    QGraphicsScene *s;
    Bird *blue[2];

};


#endif // BLUEBIRD_H
