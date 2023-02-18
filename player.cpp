#include "player.h"
#include "world.h"

constexpr double terminal_velocity = 500;
constexpr double walk_acceleration = 480;
constexpr double jump_velocity = 150;
constexpr double gravity = 50;
constexpr double damping = 0.9;

Player::Player(const Vec<double>& position, const Vec<int>& size)
    :position{position}, size{size} {
        acceleration.y = gravity;
    }

void Player::handle_input(const SDL_Event& event) {
    if (event.type == SDL_KEYDOWN){
        SDL_Keycode key = event.key.keysym.sym;
        if (key == SDLK_RIGHT){
            acceleration.x = walk_acceleration;
        }
        else if(key == SDLK_LEFT){
            acceleration.x = -walk_acceleration;
        }
        else if(key == SDLK_SPACE){
            velocity.y = -jump_velocity;
            acceleration.y = gravity;
        }
        // else if(key == SDLK_UP){
        //     acceleration.y = -walk_acceleration;
        // }
        // else if(key == SDLK_DOWN){
        //     acceleration.y = +walk_acceleration;
        // }
        
    }
    if (event.type == SDL_KEYUP){
        SDL_Keycode key = event.key.keysym.sym;
        if (key == SDLK_RIGHT){
            acceleration.x = 0;
        }
        else if(key == SDLK_LEFT){
            acceleration.x = 0;
        }
        // else if(key == SDLK_UP){
        //     acceleration.y = 0;
        // }
        // else if(key == SDLK_DOWN){
        //     acceleration.y = 0;
        // }
    }
    
}

void Player::update(World& world, double dt){
    // make copy of physics compoents 
    Vec<double> vel = velocity;
    Vec<double> acc = acceleration;
    Vec<double> pos = position;


    // semi implicit euler
    vel += acc * dt;
    vel.x *= damping;
    // keep velocity under terminal 
    vel.x = std::clamp(vel.x, -terminal_velocity, terminal_velocity);
    vel.y = std::clamp(vel.y, -terminal_velocity, terminal_velocity);

    pos += vel * dt;

    // test x intersections first
    SDL_Rect future{static_cast<int>(pos.x),static_cast<int>(position.y), size.x, size.y};
    if (world.has_any_intersections(future)){
        // collided  => stop velocity and acceleration
        velocity.x = 0;
        acceleration.x = 0;
    }
    else {
        position.x = pos.x;
        acceleration.x = acc.x;
        velocity.x = vel.x;
    }
    // test y 
    future.x = static_cast<int>(position.x);
    future.y = static_cast<int>(pos.y);

    if (world.has_any_intersections(future)){
        velocity.y = 0;
        acceleration.y = 0;
    }
    else{
        acceleration.y = gravity;
        velocity.y = vel.y;
        position.y = pos.y;
    }

}

std::pair<SDL_Rect, Color> Player::get_sprite() const {
    int x = static_cast<int>(position.x);
    int y = static_cast<int>(position.y);
    SDL_Rect bounding_box{x,y,size.x, size.y};
    return {bounding_box, {255,0,0,255}};
}
