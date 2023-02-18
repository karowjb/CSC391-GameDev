#include "world.h"

World::World(int width, int height)
    :tilemap{width, height}{}

void World::add_platform(int x, int y, int width, int height) {
    for (int i = 0; i < height; ++i){
        for (int j = 0; j < width;j++){
            tilemap(x+j, y+i) = Tile::Platform;
        }
    }
}

const std::vector<SDL_Rect>& World::get_platforms() const {
    return platforms;
}

bool World::has_any_intersections(const SDL_Rect& bounding_box) const {
    // test if the given bounding_box intersects any of the platforms
    bool collides = std::any_of(std::begin(platforms),std::end(platforms),[&](const SDL_Rect& platform){
        return SDL_HasIntersection(&platform, &bounding_box);
    });
    return collides;
}