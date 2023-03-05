#pragma once

#include<SDL2/SDL.h>
#include <memory>

//Forward decs
class Player;
class World;

class State{
public:
    virtual ~State(){}
    virtual std::unique_ptr<State> handle_input(Player& player, const SDL_Event& event) = 0;
    virtual std::unique_ptr<State> update(Player& player, World& world, double dt);
    virtual void enter(Player&) {}
    virtual void exit(Player&) {}
};

class Standing : public State {
public:
    std::unique_ptr<State> handle_input(Player& player, const SDL_Event& event) override;
    virtual std::unique_ptr<State> update(Player& player, World& world, double dt) override;

    void enter(Player& player) override;

};

class Jumping : public State {
public:
    std::unique_ptr<State> handle_input(Player& player, const SDL_Event& event) override;
    virtual std::unique_ptr<State> update(Player& player, World& world, double dt) override;

    void enter(Player& player) override;

};

class Running : public State {
public:
    std::unique_ptr<State> handle_input(Player& player, const SDL_Event& event) override;
    virtual std::unique_ptr<State> update(Player& player, World& world, double dt) override;

    void enter(Player& player) override;
};

class Sliding : public State {
public:
    std::unique_ptr<State> handle_input(Player& player, const SDL_Event& event) override;
    virtual std::unique_ptr<State> update(Player& player, World& world, double dt) override;

    void enter(Player& player) override;

};


class Shooting : public State {
public:
    std::unique_ptr<State> handle_input(Player& player, const SDL_Event& event) override;
    virtual std::unique_ptr<State> update(Player& player, World& world, double dt) override;

    void enter(Player& player) override;

};
