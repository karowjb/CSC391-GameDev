#pragma once

#include <string>
#include <unordered_map>
#include <sstream>

class Settings {
public:
    Settings(const std::string& filename);
    std::string filename;
    std::string title;
    int screen_width, screen_height;
    int tilesize;
    std::string starting_level;
    std::string characters;
    std::string sounds;

private:
    void load();
    std::unordered_map<std::string, std::string> parameters;
    template <typename T>
    void load(const std::string& key, T& value){
        try{
            std::stringstream ss{parameters.at(key)};
            ss >> value;
        }
        catch(std::out_of_range&){
            throw std::runtime_error("Key '" + key + "' not found in " + filename);
        }
    }
};