#include "animation.h"

Animation::Animation()
{
    time = 0.0;
    energy = 0.0;
    step = 0;
    //initialize to zeo the COM
    for(unsigned int i=0;i<3;i++){
        com[i] = 0.0;
    }
}
