#ifndef ANIMATION_H
#define ANIMATION_H

#include <QObject>
#include "globals.h"

class Animation
{
public:
    Animation();
    array<float, 3> com;  //center of mass
    array<array<float,3>,3> cell;
    float energy;
    int step;
    float time;
};

#endif // ANIMATION_H
