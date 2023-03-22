#pragma once
#include "engine.h"
class Player;
class World;

class Command {
public:
    virtual ~Command() {}
    virtual void execute(Player& player, Engine& engine) = 0;
};

class Stop : public Command {
public:
    void execute(Player& player, Engine& engine) override;
};

class Jump : public Command {
public:
    Jump(double velocity);
    void execute(Player& player, Engine& engine) override;
private:
    double velocity;
};

class Accelerate : public Command {
public:
    Accelerate(double acceleration);
    void execute(Player& player, Engine& engine) override;

private:
    double acceleration;
};


class Slide : public Command{
public:
    Slide(double speed);
    void execute(Player& player, Engine& engine) override;
private:
    double speed;
};

class Shoot : public Command {
public:
    Shoot(double speed);
    void execute(Player& player, Engine& engine) override;
private:
    double speed;
};

class Fall : public Command {
public:
    Fall(double speed);
    void execute(Player& player, Engine& engine) override;
private:
    double speed;
};