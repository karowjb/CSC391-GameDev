#include "engine.h"
#include "settings.h"

int main() {
    // Settings settings{"../settings.txt"};
    Settings settings{"settings.txt"};

    Engine engine(settings);
    engine.run();
}

