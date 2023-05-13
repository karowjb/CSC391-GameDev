#include <engine.h>
#include "settings.h"
#include "player.h"
#include <chrono>
#include <iostream>
#include "level.h"
#include "world.h"
#include "projectile.h"
#include "loadscreen.h"

Engine::Engine(const Settings& settings)
    :graphics{settings.title, settings.screen_width, settings.screen_height},
      camera{graphics, settings.tilesize} {
    
    load_level(settings.starting_level);
}

void Engine::load_level(const std::string& level_filename) {
    Level level{level_filename, graphics, audio};
    audio.play_sound("background", true);
    world = std::make_shared<World>(level);
    // load player
    player = std::make_shared<Player>(*this, level.player_start_location, Vec<int>{1, 1});
    // move camera to start position
    camera.move_to(player->physics.position);
}

// Death end screen
void Engine::setup_death_screen() {
    running = true;
    Loadscreen game_over{"assets/game-over.txt", graphics, audio};
    audio.play_sound("endgame");

    world->backgrounds = game_over.backgrounds;
}

// Victory end screen
void Engine::setup_victory_screen() {
    running = true;
    Loadscreen game_over{"assets/victory.txt", graphics, audio};
    audio.play_sound("endgame");

    world->backgrounds = game_over.backgrounds;
}
void Engine::input(){
    // Input
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            // handle windows and systems events first
            if (event.type == SDL_QUIT) {
                running = false;
                break;
            }
            if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_g){
                grid_on = !grid_on;
            }
            // pass the rest of the events to the player who will
            // react to keypresses by moving
            auto command = player->handle_input(*this,event);
            if (command){
                command->execute(*player, *this);
            }
        }
        for (auto enemy : world->enemies){
            auto command = enemy->next_action(*this);
            if (command){
                command->execute(*enemy, *this);
            }
        }
}
void Engine::update(double dt){
    player->update(*this, dt);
    camera.update(dt);

    for (auto enemy : world->enemies){
        auto command = enemy->update(*this, dt);
        if (command){
            command->execute(*enemy, *this);
        }
    }
    for (auto& projectile : world->projectiles){
        projectile.update(*this, dt);
    }

    //handle collisions between enemy and player
    world->build_quadtree();
    AABB player_box{player->physics.position, {1.0*player->size.x, 1.0*player->size.y}};
    std::vector<Object*> enemies = world->quadtree.query_range(player_box);
    if (enemies.size() > 0){
        auto enemy = enemies.front();
        enemy->combat.attack(*player);
        //enter the hurting state
        player->state->exit(*player, *this);
        player->state = std::make_unique<Hurting>();
        player->state->enter(*player, *this);
    }
    // Handle projectile collisions
    for (auto& projectile : world->projectiles){
        AABB projectile_box{projectile.physics.position, {1.0*projectile.size.x, 1.0*projectile.size.y}};
        std::vector<Object*> enemies = world->quadtree.query_range(projectile_box);
        for (auto enemy : enemies){
            projectile.combat.attack(*enemy);
        }
    }
    //end game
    if (!player->combat.is_alive){
        EndGame endgame;
        endgame.execute(*player, *this);
        return;
    }
    if (victory){
        Victory victory;
        victory.execute(*player, *this);
    }
     //check for enemy death
     world->remove_inactive();
   
}

void Engine::render() {
    // draw the new scene
    graphics.clear();
    camera.render(world->backgrounds);
    camera.render(world->tilemap);
    // Render bars
    camera.render_potion(player->combat.potions, player->combat.max_potions);
    camera.render_life(player->combat.health, player->combat.max_health);
    camera.render(*player);
    for (auto enemy : world->enemies){
        camera.render(*enemy);
    }
    for (auto& projectile : world->projectiles){
        camera.render(projectile);
    }
    graphics.update();
}

void Engine::run(){
    running = true;
    auto previous = std::chrono::high_resolution_clock::now();
    double lag{0.0};
    while (running) {
        if (next_level){
            load_level(next_level.value());
            next_level.reset();
        }
        auto current = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed = current - previous;
        previous = current;
        lag += elapsed.count();
        constexpr double dt = 1.0/60.0;
        input();
        while (lag >= dt){
            update(dt);
            camera.move_to(player->get_sprite().first);
            lag -= dt;
        }
        render();
    }
    // After quit checking which screen to render
    if (victory){
        setup_victory_screen();
    }
    else{
        setup_death_screen();
    }

    while (running) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {  // closing the window
                running = false;
                break;
            }
        }
        graphics.clear();
        camera.render(world->backgrounds);
        graphics.update();
    }
}

void Engine::stop(){
    running = false;
}