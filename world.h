#pragma once
#include "tilemap.h"

class World {
public:
    World(int width, int height);
    void add_platform(int x, int y, int width, int height);
    Tilemap tilemap;
};