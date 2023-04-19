#pragma once
#include "tilemap.h"
#include "vec.h"
#include "command.h"
#include "enemy.h"
class Player;
class Level;
class Sprite;

class World {
public:
    World(const Level& level);
    // void add_platform(int x, int y, int width, int height);
    void move_to(Vec<double>& position, const Vec<int>& size, Vec<double>& velocity);
    bool collides(const Vec<double>& position) const;
    std::shared_ptr<Command> touch_tiles(const Player& player);
    Tilemap tilemap;
    std::vector<std::pair<Sprite, int>> backgrounds;
    std::vector<std::shared_ptr<Enemy>> enemies;
};
