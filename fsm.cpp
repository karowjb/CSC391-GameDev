#include "fsm.h"
#include "player.h"
#include "world.h"
#include <iostream>

////////////
//State
///////////

std::unique_ptr<State> State::update(Player& player, World& world, double dt){
    Physics old = player.physics;
    player.physics.update(dt);
  
    // attempt to move x first then y
    Vec<double> future{player.physics.position.x, old.position.y};
    Vec<double> vx{player.physics.velocity.x,0};
    world.move_to(future, player.size, vx);

    Vec<double> vy{0,player.physics.velocity.y};
    future.y = player.physics.position.y;
    world.move_to(future, player.size, vy);

    //update position and velocity
    player.physics.position = future;
    player.physics.velocity = {vx.x, vy.y};

    return nullptr;
}


////////////
// Standing
////////////

bool on_platform(const Player& player, const World& world) {
    constexpr double epsilon = 1e-2;
    Vec<double> left_foot{player.physics.position.x, player.physics.position.y - epsilon};
    Vec<double> right_foot{player.physics.position.x+player.size.x, player.physics.position.y - epsilon};
    return world.collides(left_foot) || world.collides(right_foot);
}

std::unique_ptr<State> Standing::handle_input(Player& player, const SDL_Event& event) {
    if (event.type == SDL_KEYDOWN){
        SDL_Keycode key = event.key.keysym.sym;
        if (key == SDLK_SPACE || key == SDLK_UP){
            return std::make_unique<Jumping>(player.jump_velocity);
        }
        if (key == SDLK_RIGHT || key == SDLK_d){
            return std::make_unique<Running>(player.walk_acceleration);
        }
        if (key == SDLK_LEFT || key == SDLK_a){
            return std::make_unique<Running>(-player.walk_acceleration);
        }
        if (key == SDLK_x){
            return std::make_unique<Shooting>();
        }
    }
    return nullptr;
}
std::unique_ptr<State> Standing::update(Player& player, World& world, double dt){
    State::update(player,world,dt);
    player.physics.velocity.x *= damping; // Physics::damp()
    if (player.physics.velocity.y < 0){
        player.physics.acceleration.x = 0;
        return std::make_unique<Falling>(gravity);

    }
    // slide off a platform 
    //return a new state?
    return nullptr;
}

void Standing::enter(Player& player){
    player.next_command = std::make_unique<Stop>();
}


///////////
// Jumping
///////////

Jumping::Jumping(double velocity)
    :velocity{velocity}{}

std::unique_ptr<State> Jumping::handle_input(Player& player, const SDL_Event& event) {
      if (event.type == SDL_KEYDOWN){
        SDL_Keycode key = event.key.keysym.sym;
        if (key == SDLK_DOWN || key == SDLK_s){
            // player.walk_acceleration = -player.walk_acceleration;
            player.physics.velocity.y += (gravity*2);
        }
        else if (key == SDLK_LEFT || key == SDLK_a){
            player.physics.velocity.x = -2.5;
        }
         else if (key == SDLK_RIGHT || key == SDLK_d){
            player.physics.velocity.x = 2.5;
        }
    }
    // reduced movement in left / right directions
    // Boosted jump by holding spacebar space -> add a small vy
    // down -> drop faster
    return nullptr;
}

std::unique_ptr<State> Jumping::update(Player& player, World& world, double dt){
    State::update(player,world,dt);
    if (player.physics.velocity.y == 0){
        return std::make_unique<Standing>();
    }
    if (player.physics.velocity.y < 0){
        return std::make_unique<Falling>(gravity);

    }
    return nullptr;
}

void Jumping::enter(Player& player){
    player.color = {0,0,255,255};
    player.next_command = std::make_unique<Jump>(velocity);
}

void Jumping::exit(Player& player){
    player.physics.acceleration.x = 0;
}

////////////
// Running
////////////

Running::Running(double acceleration)
    :acceleration{acceleration}{}

std::unique_ptr<State> Running::handle_input(Player& player, const SDL_Event& event) {
    if (event.type == SDL_KEYDOWN){
        SDL_Keycode key = event.key.keysym.sym;
        if (key == SDLK_LEFT || key == SDLK_a){
            player.physics.velocity.x += -(player.walk_acceleration);
        }
        else if(key == SDLK_RIGHT || key == SDLK_d){
            player.physics.velocity.x += player.walk_acceleration;
        }
        else if (key == SDLK_SPACE || key == SDLK_UP){
            return std::make_unique<Jumping>(player.jump_velocity);
        }
        else if (key == SDLK_x){
            return std::make_unique<Shooting>();
        }
        else if (key == SDLK_c){
            return std::make_unique<Sliding>(player.slide_velocity);
        }
    }
    else if (event.type == SDL_KEYUP){
        player.physics.acceleration.x = 0;
    }
    return nullptr;
}

std::unique_ptr<State> Running::update(Player& player, World& world, double dt){
    State::update(player,world,dt);

    player.physics.velocity.x *= damping; // Physics::damp()

    if (player.physics.acceleration.x == 0){
        return std::make_unique<Standing>();
    }    
    return nullptr;
}

void Running::enter(Player& player){
    player.color = {255,0,255,255};
    player.next_command = std::make_unique<Accelerate>(acceleration);
}

void Running::exit(Player& player){
    player.physics.acceleration.x = 0;
}

// ////////////
// // Shooting
// ////////////

std::unique_ptr<State> Shooting::handle_input(Player& player, const SDL_Event& event) {
    if (event.type == SDL_KEYDOWN){
        SDL_Keycode key = event.key.keysym.sym;
        if (key == SDLK_SPACE || key == SDLK_UP){
            return std::make_unique<Jumping>(player.jump_velocity);
        }
        if (key == SDLK_RIGHT || key == SDLK_d){
            return std::make_unique<Running>(player.walk_acceleration);
        }
        if (key == SDLK_LEFT || key == SDLK_a){
            return std::make_unique<Running>(-player.walk_acceleration);
        }
        if (key == SDLK_x){
            return std::make_unique<Shooting>();
        }
    }
    else if (event.type == SDL_KEYUP){
        return std::make_unique<Standing>();
    }
    return nullptr;
}

std::unique_ptr<State> Shooting::update(Player& player, World& world, double dt){
    State::update(player,world,dt);
    return nullptr;
}
void Shooting::enter(Player& player){
    player.next_command = std::make_unique<Shoot>(1.0);
}
//////////
// Sliding 
//////////

Sliding::Sliding(double speed)
    :speed{speed}{}

std::unique_ptr<State> Sliding::handle_input(Player& player, const SDL_Event& event) {
    if (event.type == SDL_KEYDOWN){
        SDL_Keycode key = event.key.keysym.sym;
        if (key == SDLK_SPACE || key == SDLK_UP){
            return std::make_unique<Jumping>(player.jump_velocity);
        }
        if (key == SDLK_x){
            return std::make_unique<Shooting>();
        }
    }
    else if (event.type == SDL_KEYUP){
        player.physics.acceleration.x = 0;
    }

    return nullptr;
}
void Sliding::enter(Player& player){
    if (player.physics.velocity.x < 0){
        player.next_command = std::make_unique<Slide>(-(speed*4));    
    }
    else if (player.physics.velocity.x > 0){
        player.next_command = std::make_unique<Slide>(speed * 4);    
    }
}

void Sliding::exit(Player& player){
    player.physics.acceleration.x = 0;
}
std::unique_ptr<State> Sliding::update(Player& player, World& world, double dt){
    State::update(player,world,dt);
    player.physics.velocity.x *= damping;
    if (player.physics.acceleration.x == 0){
        return std::make_unique<Standing>();
    }
    return nullptr;
}


/////////////
// Falling 
////////////

Falling::Falling(double speed)
    :speed{speed}{}

std::unique_ptr<State> Falling::handle_input(Player& player, const SDL_Event& event){
    if (event.type == SDL_KEYDOWN){
        SDL_Keycode key = event.key.keysym.sym;
        if (key == SDLK_DOWN || key == SDLK_s){
            player.physics.velocity.y += (gravity*2);
        }
        else if (key == SDLK_LEFT || key == SDLK_a){
            player.physics.velocity.x = -2.5;
        }
         else if (key == SDLK_RIGHT || key == SDLK_d){
            player.physics.velocity.x = 2.5;
        }
    }
    return nullptr;
}
std::unique_ptr<State> Falling::update(Player& player, World& world, double dt) {
    State::update(player,world,dt);
    if (player.physics.velocity.y == 0){
        return std::make_unique<Standing>();
    }
    return nullptr;
}
void Falling::enter(Player& player) {
    player.next_command = std::make_unique<Fall>(speed);
}