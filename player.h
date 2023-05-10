#pragma once

#include "graphics.h"
#include <SDL2/SDL.h>
#include "physics.h"
#include <vec.h>
#include <memory>
#include "fsm.h"
#include "command.h"
#include "engine.h"
#include "sprite.h"
#include "object.h"
#include "projectile.h"
// forward declaration
class World;
class Player : public Object {
public:
    Player(Engine& engine, const Vec<double>& position, const Vec<int>& size);
    std::unique_ptr<Command>  handle_input(Engine& engine, const SDL_Event& event);
    void update(Engine& engine, double dt);
    std::pair<Vec<double>, Color> get_sprite() const;
    Vec<int> size;
    
    Color color{255,0,0,255};
    AnimatedSprite standing, running, jumping, falling, sliding;
    const double slide_velocity = 5.0;
    const double walk_acceleration = 3.0;
    const double jump_velocity = 10.0;
    std::unique_ptr<State>state;
    std::unique_ptr<Command> next_command;
    Sprite bow;
    Sword sword;
    Projectile arrow;
};
