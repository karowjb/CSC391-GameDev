#include "player.h"
#include "world.h"


Player::Player(Engine& engine, const Vec<double>& position, const Vec<int>& size)
    :size{size} {
        physics.position = position;
        physics.acceleration.y = gravity;
        combat.health = 9;
        combat.max_health = 9;
        combat.attack_damage = 3;
        combat.max_potions = 3;
        combat.potions = 3;
        state = std::make_unique<Standing>();
        state->enter(*this,engine);
        // getting the default sprite
        standing = engine.graphics.get_animated_sprite("knight_standing",0.15,false,false);
        jumping = engine.graphics.get_animated_sprite("knight_jumping",0.15,false,false);
        running = engine.graphics.get_animated_sprite("knight_running",0.05,false,false);
        falling = engine.graphics.get_animated_sprite("knight_falling",0.15,false,false);

        sprite = standing.get_sprite();
        // Setting the combat sprites
        arrow.sprite = engine.graphics.get_sprite("arrow");
        bow = engine.graphics.get_sprite("none");
        shield = engine.graphics.get_sprite("none");
        sword.sprite = engine.graphics.get_sprite("none");
        spear.sprite = engine.graphics.get_sprite("none");
        // Setting attack damages
        spear.combat.attack_damage = 4;
        arrow.combat.invincible = true;
        arrow.combat.attack_damage = 2;
        sword.combat.attack_damage = 1;
        }

std::unique_ptr<Command> Player::handle_input(Engine& engine, const SDL_Event& event) {
    
    auto new_state = state->handle_input(*this,engine, event);
    if (new_state){
        state->exit(*this,engine);
        state = std::move(new_state);
        state->enter(*this,engine);
    }
    auto next = std::move(next_command);
    next_command = nullptr;
    return next;
    
}

void Player::update(Engine& engine, double dt){
    auto new_state = state->update(*this, engine, dt);
    if (new_state){
        state->exit(*this,engine);
        state = std::move(new_state);
        state->enter(*this,engine);
    }

    if (next_command && running.number_of_frames() > 0){
        next_command->execute(*this, engine);
        next_command = nullptr;
    }
}

std::pair<Vec<double>, Color> Player::get_sprite() const {
    return {physics.position, color};
}

// Change the current sprite
void Player::mutate(Engine& engine){
    standing = engine.graphics.get_animated_sprite("ninja_standing",0.15,false,false);
    jumping = engine.graphics.get_animated_sprite("ninja_jumping",0.15,false,false);
    running = engine.graphics.get_animated_sprite("ninja_running",0.05,false,false);
    falling = engine.graphics.get_animated_sprite("ninja_falling",0.15,false,false);
}
