#pragma once
#include <vector>
#include "sprite.h"
#include <memory>
#include "command.h"

class Tile {
    public:
        Sprite sprite;
        bool blocking{false};
        std::shared_ptr<Command> command{nullptr};
};

class Tilemap {
public:
    Tilemap(int width, int height);
    const Tile& operator()(int x, int y) const;
    Tile& operator()(int x, int y);
    const int width;
    const int height;
private:
    std::vector<Tile> tiles;
    void check_bounds(int x, int y) const;
};