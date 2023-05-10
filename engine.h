#pragma once

#include "world.h"
#include "camera.h"
#include "graphics.h"
#include <memory>
#include "audio.h"
#include <optional>

// Forward declare 
class Settings;
class Player;
class Engine {
public:
    Engine(const Settings& settings);
    void run();
    void load_level(const std::string& level_filename);
    void stop();
    Audio audio;
    Graphics graphics;
    Camera camera;
    std::shared_ptr<World> world;
    std::shared_ptr<Player> player;
    std::optional<std::string> next_level;
private:
    bool running{true};
    bool grid_on{false};
    void input();
    void update(double dt);
    void render();
    void setup_end_screen();
};