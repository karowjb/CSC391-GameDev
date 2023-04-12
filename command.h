#pragma once
class Engine;
class Player;
#include <memory>
#include <vector>
#include <string>
// class World;

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

class EndGame : public Command {
public:
    void execute(Player&, Engine& engine) override;
};

class PlaySound : public Command{
public: 
    PlaySound(std::string sound_name, bool is_background);
    void execute(Player& player, Engine& engine) override;
private:
    std::string sound_name;
    bool is_background;
};

std::shared_ptr<Command> create_command(std::string command_name, std::vector<std::string> arguments);