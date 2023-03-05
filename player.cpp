#include "player.h"
#include "world.h"


Player::Player(const Vec<double>& position, const Vec<int>& size)
    :size{size} {
        physics.position = position;
        state = std::make_unique<Standing>();
        state->enter(*this);
    }

void Player::handle_input(const SDL_Event& event) {
    auto new_state = state->handle_input(*this, event);
    if (new_state){
        state->exit(*this);
        state = std::move(new_state);
        state->enter(*this);
    }
    
}

void Player::update(World& world, double dt){
    auto new_state = state->update(*this, world, dt);
    if (new_state){
        state->exit(*this);
        state = std::move(new_state);
        state->enter(*this);
    }
}

std::pair<Vec<double>, Color> Player::get_sprite() const {
    return {physics.position, color};
}
