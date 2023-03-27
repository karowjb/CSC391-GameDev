#include "player.h"
#include "world.h"


Player::Player(Engine& engine, const Vec<double>& position, const Vec<int>& size)
    :size{size} {
        physics.position = position;
        state = std::make_unique<Standing>();
        state->enter(*this);
        sprite = engine.graphics.get_sprite("knight");
    }

std::unique_ptr<Command> Player::handle_input(const SDL_Event& event) {
    auto new_state = state->handle_input(*this, event);
    if (new_state){
        state->exit(*this);
        state = std::move(new_state);
        state->enter(*this);
    }
    auto next = std::move(next_command);
    next_command = nullptr;
    return next;
    
}

void Player::update(Engine& engine, double dt){
    auto new_state = state->update(*this, engine, dt);
    if (new_state){
        state->exit(*this);
        state = std::move(new_state);
        state->enter(*this);
    }

    if (next_command){
        next_command->execute(*this, engine);
        next_command = nullptr;
    }
}

std::pair<Vec<double>, Color> Player::get_sprite() const {
    return {physics.position, color};
}
