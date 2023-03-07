#pragma once

class Player;
class World;

class Command {
public:
    virtual ~Command() {}
    virtual void execute(Player& player, World& world) = 0;
};

class Stop : public Command {
public:
    void execute(Player& player, World& world) override;
};

class Jump : public Command {
public:
Jump(double velocity);
    void execute(Player& player, World& world) override;
private:
    double velocity;
};