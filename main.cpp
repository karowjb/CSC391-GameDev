#include "graphics.h"
#include "player.h"
#include "world.h"
#include "camera.h"
#include <SDL2/SDL.h>
#include <iostream>
#include <chrono>
#include "engine.h"
#include "settings.h"

int main() {
    Settings settings{"settings.txt"};
    Engine engine(settings);
    engine.run();
}

