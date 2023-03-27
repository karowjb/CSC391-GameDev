#pragma once

#include<SDL2/SDL.h>
#include <memory>
//Forward decs
class Player;
class Engine;
class World;


class State{
public:
    virtual ~State(){}
    virtual std::unique_ptr<State> handle_input(Player& player, const SDL_Event& event) = 0;
    virtual std::unique_ptr<State> update(Player& player, Engine& engine, double dt);
    virtual void enter(Player&) {}
    virtual void exit(Player&) {}
};

class Standing : public State {
public:
    std::unique_ptr<State> handle_input(Player& player, const SDL_Event& event) override;
    virtual std::unique_ptr<State> update(Player& player, Engine& engine, double dt) override;

    void enter(Player& player) override;

};

class Jumping : public State {
public:
    Jumping(double velocity);
    std::unique_ptr<State> handle_input(Player& player, const SDL_Event& event) override;
    virtual std::unique_ptr<State> update(Player& player, Engine& engine, double dt) override;

    void enter(Player& player) override;
    void exit(Player& player) override;
private:
    double velocity;
};

class Running : public State {
public:
    Running(double acceleration);
    std::unique_ptr<State> handle_input(Player& player, const SDL_Event& event) override;
    virtual std::unique_ptr<State> update(Player& player, Engine& engine, double dt) override;
    void enter(Player& player) override;
    void exit(Player& player) override;
private:
    double acceleration;
};

class Sliding : public State {
public:
    Sliding(double speed);
    std::unique_ptr<State> handle_input(Player& player, const SDL_Event& event) override;
    virtual std::unique_ptr<State> update(Player& player, Engine& engine, double dt) override;
    void enter(Player& player) override;
    void exit(Player& player) override;
private:
    double speed;

};

class Shooting : public State {
public:
    std::unique_ptr<State> handle_input(Player& player, const SDL_Event& event) override;
    virtual std::unique_ptr<State> update(Player& player, Engine& engine, double dt) override;

    void enter(Player& player) override;

};

class Falling : public State {
public:
    Falling(double speed);
    std::unique_ptr<State> handle_input(Player& player, const SDL_Event& event) override;
    virtual std::unique_ptr<State> update(Player& player, Engine& engine, double dt) override;
    void enter(Player& player) override;
private:
    double speed;
};
