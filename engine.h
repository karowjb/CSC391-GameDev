#pragma once

#include "world.h"
#include "camera.h"
#include "graphics.h"
#include <memory>
#include "audio.h"

// Forward declare 
class Settings;
class Player;
class Engine {
public:
    Engine(const Settings& settings);
    void run();
    void load_level(const std::string& level_filename);
    Audio audio;
    Graphics graphics;
    Camera camera;
    std::shared_ptr<World> world;
    std::shared_ptr<Player> player;
private:
    bool running{true};
    bool grid_on{false};
    void input();
    void update(double dt);
    void render();
};