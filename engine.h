#pragma once

#include "world.h"
#include "camera.h"
#include "graphics.h"
#include <memory>

// Forward declare 
class Player;
class Settings;

class Engine {
public:
    Engine(const Settings& settings);
    void run();
    void load_level();
    Graphics graphics;
    Camera camera;
    World world;

    std::shared_ptr<Player> player;

private:
    bool running{true};
    bool grid_on{false};
    void input();
    void update(double dt);
    void render();
};