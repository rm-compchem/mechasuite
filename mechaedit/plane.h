#ifndef PLANE_H
#define PLANE_H


class Plane
{
public:
    Plane();

    bool selected = false;
    int indices[4] = {0, 0, 0, 0};

    float color[4] = {0.0, 0.5, 1.0, 0.7};
};

#endif // PLANE_H
