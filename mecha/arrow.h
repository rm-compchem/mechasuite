#ifndef ARROW_H
#define ARROW_H


class Arrow
{
public:
    Arrow();

    //variables
//    bool selected = false;
    unsigned int indices[3] = {0, 0, 0};
//    float color[4] = {0.0, 0.5, 1.0, 0.7};
    float radiusBaseIncrement = 0.0;
};

#endif // ARROW_H
