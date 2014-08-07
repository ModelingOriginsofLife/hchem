#ifndef ZONE_CPP
#define ZONE_CPP

#include <vector>

#include "../dynamics/particle.cpp"
#include "../dynamics/bond.cpp"
#include "../geometry/segment.cpp"

struct zone
{
    vector <particle *> particles;
    vector <segment *> segments;
};

#endif