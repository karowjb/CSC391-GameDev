#include "command.h"
#include "object.h"
#include "world.h"
#include <memory>
#include <iostream>
#include "engine.h"

////////////////////
// Stop
///////////////////
void Stop::execute(Object& object, Engine& ){
    // object.color = {255,0,0,255};
    object.physics.velocity.y = 0;
    object.physics.acceleration.x = 0;
}

///////////////////
// Jump
///////////////////
Jump::Jump(double velocity)
    :velocity{velocity}{}

void Jump::execute(Object& object, Engine& ){
    // object.color = {0,0,255,255};
    object.physics.velocity.y = velocity;
}

///////////////////
// Accelerate
///////////////////
Accelerate::Accelerate(double acceleration)
    :acceleration{acceleration}{}

void Accelerate::execute(Object& object, Engine& ){
    // object.color = {0,51,0,255};
    object.physics.acceleration.x = acceleration;
}

///////////////////
// Slide
///////////////////
Slide::Slide(double speed)
    :speed{speed}{}

void Slide::execute(Object& object, Engine& ){
    // object.color = {0,204,204,255};
    object.physics.acceleration.x = speed;
}

///////////////////
// Shoot
///////////////////
Shoot::Shoot(double speed)
    :speed{speed}{}

void Shoot::execute(Object& object, Engine&){
    // object.color = {230,76,0,255};
    object.physics.acceleration.x = speed;
}

///////////////////
// Fall
///////////////////
Fall::Fall(double speed)
    :speed{speed}{}

void Fall::execute(Object& object, Engine&){
    // object.color = {125,125,125,255};
    object.physics.acceleration.y = speed;
    // engine.audio.play_sound("landing");
}

///////////////////
// Game Changes
///////////////////

void EndGame::execute(Object&, Engine& engine){
    engine.stop();
}

PlaySound::PlaySound(std::string sound_name, bool is_background)
    :sound_name{sound_name}, is_background{is_background}{}
void PlaySound::execute(Object&, Engine& engine){
    engine.audio.play_sound(sound_name, is_background);
}

LoadLevel::LoadLevel(const std::string& filename)
    :filename{filename}{}

void LoadLevel::execute(Object&, Engine& engine) {
    // std::cout << "Loading level" << std::endl;
    engine.load_level("assets/" + filename);
    // std::cout << "assets/" << filename << std::endl;


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
    else if (command_name == "load_level"){
        if (arguments.size() != 1){
            throw std::runtime_error("Load level must be followed a level filename");
        }
        std::string filename = arguments.at(0);
        // std::cout << "Filename:" << filename << std::endl;
        return std::make_shared<LoadLevel>(filename);
    }
    throw std::runtime_error("Unknown command name: " + command_name);
}

