#include "fsm.h"
#include "player.h"
#include "world.h"
#include <iostream>
#include "command.h"
#include "randomness.h"

////////////
//State
///////////

std::unique_ptr<State> State::update(Player& player, Engine& engine, double dt){
    Physics old = player.physics;
    player.physics.update(dt);
  
    // attempt to move x first then y
    Vec<double> future{player.physics.position.x, old.position.y};
    Vec<double> vx{player.physics.velocity.x,0};
    engine.world->move_to(future, player.size, vx);

    Vec<double> vy{0,player.physics.velocity.y};
    future.y = player.physics.position.y;
    engine.world->move_to(future, player.size, vy);

    //update position and velocity
    player.physics.position = future;
    player.physics.velocity = {vx.x, vy.y};

    // see if we touch a tile that has a command
    auto command = engine.world->touch_tiles(player);
    if (command){
        command->execute(player, engine);
    }

    return nullptr;
}


////////////
// Standing
////////////

bool on_platform(const Player& player, const Engine& engine) {
    constexpr double epsilon = 1e-2;
    Vec<double> left_foot{player.physics.position.x, player.physics.position.y - epsilon};
    Vec<double> right_foot{player.physics.position.x+player.size.x, player.physics.position.y - epsilon};
    return engine.world->collides(left_foot) || engine.world->collides(right_foot);
}

std::unique_ptr<State> Standing::handle_input(Player& player, Engine& , const SDL_Event& event) {
    if (event.type == SDL_KEYDOWN){
        SDL_Keycode key = event.key.keysym.sym;
        if (key == SDLK_SPACE || key == SDLK_UP){
            return std::make_unique<Jumping>(player.jump_velocity);
        }
        else if (key == SDLK_RIGHT || key == SDLK_d){
            return std::make_unique<Running>(player.walk_acceleration);
        }
        else if (key == SDLK_LEFT || key == SDLK_a){
            return std::make_unique<Running>(-player.walk_acceleration);
        }
        else if (key == SDLK_x){
            return std::make_unique<Shooting>();
        }
        else if (key == SDLK_f){
            return std::make_unique<Swinging>();
        }
        else if (key == SDLK_q){
            return std::make_unique<AttackAll>();
        }
    }
    return nullptr;
}
std::unique_ptr<State> Standing::update(Player& player, Engine& engine, double dt){
    State::update(player,engine,dt);
    player.physics.velocity.x *= damping; // Physics::damp()
    player.standing.update(dt);
    player.sprite = player.standing.get_sprite();
    if (player.physics.velocity.y < 0){
        player.physics.acceleration.x = 0;
        return std::make_unique<Falling>(gravity);
    }
    return nullptr;
}

void Standing::enter(Player& player,Engine&){
    player.next_command = std::make_unique<Stop>();
    player.standing.reset();
    player.standing.flip(player.sprite.flip);
}


///////////
// Jumping
///////////

Jumping::Jumping(double velocity)
    :velocity{velocity}{}

std::unique_ptr<State> Jumping::handle_input(Player& player, Engine&, const SDL_Event& event) {
      if (event.type == SDL_KEYDOWN){
        SDL_Keycode key = event.key.keysym.sym;
        if (key == SDLK_DOWN || key == SDLK_s){
            player.physics.velocity.y += (gravity*2);
        }
        else if (key == SDLK_LEFT || key == SDLK_a){
            player.physics.velocity.x = -2;
        }
         else if (key == SDLK_RIGHT || key == SDLK_d){
            player.physics.velocity.x = 2;
        }
    }
    // reduced movement in left / right directions
    // Boosted jump by holding spacebar space -> add a small vy
    // down -> drop faster
    return nullptr;
}

std::unique_ptr<State> Jumping::update(Player& player, Engine& engine, double dt){
    State::update(player,engine,dt);
    player.jumping.update(dt);
    player.sprite = player.jumping.get_sprite();

    if (player.physics.velocity.y == 0){
        return std::make_unique<Standing>();
    }
    else if (player.physics.velocity.y < 0){
        return std::make_unique<Falling>(gravity);
    }
    else if (player.physics.velocity.x < 0){
        player.jumping.flip(true);
    }
    else if (player.physics.velocity.x > 0){
        player.jumping.flip(false);
    }
    return nullptr;
}

void Jumping::enter(Player& player,Engine& engine){
    engine.audio.play_sound("jumping");
    player.color = {0,0,255,255};
    player.next_command = std::make_unique<Jump>(velocity);
    player.jumping.flip(player.sprite.flip);
}

void Jumping::exit(Player& player,Engine&){
    player.physics.acceleration.x = 0;
}

////////////
// Running
////////////

Running::Running(double acceleration)
    :acceleration{acceleration}{}

std::unique_ptr<State> Running::handle_input(Player& player, Engine&, const SDL_Event& event) {
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

std::unique_ptr<State> Running::update(Player& player, Engine& engine, double dt){
    State::update(player,engine,dt);

    player.physics.velocity.x *= damping; // Physics::damp()
    player.running.update(dt);
    player.sprite = player.running.get_sprite();


    if (player.physics.acceleration.x == 0){
        return std::make_unique<Standing>();
    }    
    return nullptr;
}

void Running::enter(Player& player,Engine& engine){
    engine.audio.play_sound("running");
    player.running.reset();
    player.color = {255,0,255,255};
    player.next_command = std::make_unique<Accelerate>(acceleration);
    player.running.flip(acceleration < 0);
}

void Running::exit(Player& player,Engine&){
    player.physics.acceleration.x = 0;
}

// ////////////
// // Shooting
// ////////////

std::unique_ptr<State> Shooting::handle_input(Player& player, Engine&, const SDL_Event& event) {
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
        if (key == SDLK_f){
            return std::make_unique<Shooting>();
        }
    }
    else if (event.type == SDL_KEYUP){
        return std::make_unique<Standing>();
    }
    return nullptr;
}

std::unique_ptr<State> Shooting::update(Player& player, Engine& engine, double dt){
    State::update(player,engine,dt);
    if (shot == false){
        Vec<double> position {player.physics.position.x+player.size.x, player.physics.position.y+(player.size.y/1.5)};
        Vec<double> velocity {15,2};
        velocity.x += randint(-1,1);
        velocity.y += randint(-1,1);
        if (player.sprite.flip){
            position = {player.physics.position.x, player.physics.position.y+player.size.y};
            velocity.x *= -1;
            player.bow.shift.x = -20;
            player.bow.flip = true;
            player.bow.angle = 20;
        }
        player.next_command = std::make_unique<FireProjectile>(player.arrow,position,velocity);
        engine.audio.play_sound("shooting");
        shot = true;
    }
    

    return nullptr;
}
void Shooting::exit(Player& player,Engine& engine){
    shot = false;
    player.bow = engine.graphics.get_sprite("none");
}

void Shooting::enter(Player& player,Engine& engine){
    player.bow = engine.graphics.get_sprite("bow");
    player.bow.angle = 350;
    if (player.sprite.flip){
        player.bow.shift.x = 0;
        player.bow.shift.y -= 5;
    }
    else{
        player.bow.shift.x += 10;
        player.bow.shift.y -= 6;
    }
    
    engine.audio.play_sound("drawing");
}

// ////////////
// // Swinging
// ////////////

std::unique_ptr<State> Swinging::handle_input(Player& player, Engine&, const SDL_Event& event) {
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
        if (key == SDLK_f){
            return std::make_unique<Swinging>();
        }
    }
    else if (event.type == SDL_KEYUP){
        return std::make_unique<Standing>();
    }
    return nullptr;
}

std::unique_ptr<State> Swinging::update(Player& player, Engine& engine, double dt){
    State::update(player,engine,dt);
    if (distance < 50){
        if (player.sprite.flip){
            player.sword.sprite.flip = true;
            player.sword.sprite.shift.x = -25;
            player.sword.sprite.angle -= 10;
            player.sword.sprite.shift.y += .05;
        }
        else{
            player.sword.sprite.flip = false;
            player.sword.sprite.angle += 10;
            player.sword.sprite.shift.y += .05;
        }
        distance += 5;
    }

    AABB player_box{player.physics.position, {2.0*player.size.x, 1.0*player.size.y}};
    std::vector<Object*> enemies = engine.world->quadtree.query_range(player_box);
    if (enemies.size() > 0){
        auto enemy = enemies.front();
        player.combat.attack(*enemy);
    }
    return nullptr;
}
void Swinging::exit(Player& player,Engine& engine){
    player.sword.sprite = engine.graphics.get_sprite("none");
    engine.audio.play_sound("swinging");
    distance = 0;
}

void Swinging::enter(Player& player,Engine& engine){
    player.sword.sprite = engine.graphics.get_sprite("sword");
    if (player.sprite.flip){
        player.sword.sprite.shift.x = -10;
        player.sword.sprite.angle = 340;
    }
    else{
        player.sword.sprite.shift.x = 15;
        player.sword.sprite.angle = 10;
    }
    player.sword.sprite.shift.y -= 2;
}

//////////
// Sliding 
//////////

Sliding::Sliding(double speed)
    :speed{speed}{}

std::unique_ptr<State> Sliding::handle_input(Player& player, Engine&, const SDL_Event& event) {
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
void Sliding::enter(Player& player,Engine&){
    if (player.physics.velocity.x < 0){
        player.next_command = std::make_unique<Slide>(-(speed*4));    
    }
    else if (player.physics.velocity.x > 0){
        player.next_command = std::make_unique<Slide>(speed * 4);    
    }
}

void Sliding::exit(Player& player,Engine&){
    player.physics.acceleration.x = 0;
}
std::unique_ptr<State> Sliding::update(Player& player, Engine& engine, double dt){
    State::update(player,engine,dt);
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

std::unique_ptr<State> Falling::handle_input(Player& player, Engine&, const SDL_Event& event){
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
std::unique_ptr<State> Falling::update(Player& player, Engine& engine, double dt) {
    State::update(player,engine,dt);
    player.falling.update(dt);
    player.sprite = player.falling.get_sprite();
    if (player.physics.velocity.y == 0){
        return std::make_unique<Standing>();
    }
    else if (player.physics.velocity.x < 0){
        player.falling.flip(true);
    }
    else if (player.physics.velocity.x > 0){
        player.falling.flip(false);
    }
    return nullptr;
}
void Falling::enter(Player& player,Engine& engine) {
    engine.audio.play_sound("falling");
    player.next_command = std::make_unique<Fall>(speed);
    player.falling.reset();
    player.falling.flip(player.sprite.flip);
}

/////////////
// Attack All 
////////////

std::unique_ptr<State> AttackAll::handle_input(Player&, Engine&, const SDL_Event& event) {
    if (event.type == SDL_KEYUP){
        SDL_Keycode key = event.key.keysym.sym;
        if (key == SDLK_q){
            return std::make_unique<Standing>();
        }
    }
    return nullptr;
}
void AttackAll::enter(Player& player,Engine& engine) {
    for (auto enemy : engine.world->enemies){
        player.combat.attack(*enemy);
    }
}

/////////////
// Hurting
////////////

std::unique_ptr<State> Hurting::handle_input(Player& player, Engine&, const SDL_Event& event) {
    if (event.type == SDL_KEYUP){
    SDL_Keycode key = event.key.keysym.sym;
    if (key == SDLK_LEFT){
        player.next_command = std::make_unique<Accelerate>(-player.walk_acceleration / .5);
        player.sprite.flip = true;
    }
    else if (key == SDLK_RIGHT){
        player.next_command = std::make_unique<Accelerate>(player.walk_acceleration / .5);
        player.sprite.flip = false;
    }
    }
    if (event.type == SDL_KEYUP){
        SDL_Keycode key = event.key.keysym.sym;
        if (key == SDLK_LEFT || key == SDLK_RIGHT){
            player.next_command = std::make_unique<Accelerate>(0);
        }

    }
return nullptr;
}
std::unique_ptr<State> Hurting::update(Player& player, Engine& engine, double dt) {
    elapsed_time += dt;
    if (elapsed_time >= cooldown){
        return std::make_unique<Standing>();
    }
    State::update(player, engine, dt);
    player.sprite.flip = !player.sprite.flip;
    return nullptr;
}

 void Hurting::enter(Player& player,Engine&){
    elapsed_time = 0;
    player.combat.invincible = true;

 }
 void Hurting::exit(Player& player,Engine&) {
    player.combat.invincible = false;
 }
