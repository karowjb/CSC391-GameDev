#pragma once

#include "graphics.h"
#include <SDL2/SDL.h>
#include "physics.h"
#include <vec.h>
#include <memory>
#include "fsm.h"

// forward declaration
class World;

class Player {
public:
    Player(const Vec<double>& position, const Vec<int>& size);
    void handle_input(const SDL_Event& event);
    void update(World& world, double dt);
    std::pair<Vec<double>, Color> get_sprite() const;
    Physics physics;
    Vec<int> size;
    Color color{255,0,0,255};
    const double walk_acceleration = 5.0;
    const double jump_velocity = 10.0;
    std::unique_ptr<State>state;

};
