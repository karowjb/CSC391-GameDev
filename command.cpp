#include "command.h"
#include "player.h"
#include "world.h"
#include <memory>

////////////////////
// Stop
///////////////////
void Stop::execute(Player& player, Engine& ){
    player.color = {255,0,0,255};
    player.physics.velocity.y = 0;
    player.physics.acceleration.x = 0;
}

///////////////////
// Jump
///////////////////
Jump::Jump(double velocity)
    :velocity{velocity}{}

void Jump::execute(Player& player, Engine& ){
    player.color = {0,0,255,255};
    player.physics.velocity.y = velocity;
}

///////////////////
// Accelerate
///////////////////
Accelerate::Accelerate(double acceleration)
    :acceleration{acceleration}{}

void Accelerate::execute(Player& player, Engine& ){
    player.color = {0,51,0,255};
    player.physics.acceleration.x = acceleration;
}

///////////////////
// Slide
///////////////////
Slide::Slide(double speed)
    :speed{speed}{}

void Slide::execute(Player& player, Engine& ){
    player.color = {0,204,204,255};
    player.physics.acceleration.x = speed;
}

///////////////////
// Shoot
///////////////////
Shoot::Shoot(double speed)
    :speed{speed}{}

void Shoot::execute(Player& player, Engine&){
    player.color = {230,76,0,255};
    player.physics.acceleration.x = speed;
}

///////////////////
// Fall
///////////////////
Fall::Fall(double speed)
    :speed{speed}{}

void Fall::execute(Player& player, Engine&){
    player.color = {125,125,125,255};
    player.physics.acceleration.y = speed;
    // engine.audio.play_sound("landing");
}

///////////////////
// Game Changes
///////////////////

void EndGame::execute(Player&, Engine& engine){
    engine.stop();
}

PlaySound::PlaySound(std::string sound_name, bool is_background)
    :sound_name{sound_name}, is_background{is_background}{}
void PlaySound::execute(Player&, Engine& engine){
    engine.audio.play_sound(sound_name, is_background);
}

std::shared_ptr<Command> create_command(std::string command_name, std::vector<std::string> arguments) {
    if (command_name == "end_game"){
        return std::make_shared<EndGame>();
    }
    else if (command_name == "play_sound"){
        bool is_background = arguments.at(1) == "true" ? true : false;
        std::string sound_name = arguments.at(0);
        return std::make_shared<PlaySound>(sound_name, is_background);
    }
    throw std::runtime_error("Unknown command name: " + command_name);
}

