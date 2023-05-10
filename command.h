#pragma once
class Engine;
class Object;
class Player;
#include <memory>
#include <vector>
#include "projectile.h"
#include <string>
// class World;

class Command {
public:
    virtual ~Command() {}
    virtual void execute(Object& object, Engine& engine) = 0;
};

class Stop : public Command {
public:
    void execute(Object& object, Engine& engine) override;
};

class Jump : public Command {
public:
    Jump(double velocity);
    void execute(Object& object, Engine& engine) override;
private:
    double velocity;
};

class Accelerate : public Command {
public:
    Accelerate(double acceleration);
    void execute(Object& object, Engine& engine) override;

private:
    double acceleration;
};


class Slide : public Command{
public:
    Slide(double speed);
    void execute(Object& object, Engine& engine) override;
private:
    double speed;
};

class Shoot : public Command {
public:
    Shoot(double speed);
    void execute(Object& object, Engine& engine) override;
private:
    double speed;
};

class FireProjectile : public Command{
public: 
    FireProjectile(Projectile projectile, Vec<double> position, Vec<double> velocity);
    void execute(Object& object, Engine& engine) override;
private:
    Projectile projectile;
};

// class SwingSword : public Command{
// public:
//     SwingSword(Object& object, Engine& engine, Player& player);
//     void execute(Object& object, Engine& engine) override;
// };

class Fall : public Command {
public:
    Fall(double speed);
    void execute(Object& object, Engine& engine) override;
private:
    double speed;
};

class EndGame : public Command {
public:
    void execute(Object&, Engine& engine) override;
};

class PlaySound : public Command{
public: 
    PlaySound(std::string sound_name, bool is_background);
    void execute(Object& object, Engine& engine) override;
private:
    std::string sound_name;
    bool is_background;
};

class LoadLevel : public Command{
public:
    LoadLevel(const std::string& filename);
    void execute(Object& object, Engine& engine) override;
private:
    std::string filename;
};
std::shared_ptr<Command> create_command(std::string command_name, std::vector<std::string> arguments);
