#include "camera.h"
#include "graphics.h"
#include "tilemap.h"
#include "vec.h"
#include <iostream>

constexpr double acceleration = 2;
constexpr double middlePoint = 5;
Camera::Camera(Graphics& graphics, int tilesize)
    : graphics{graphics}, tilesize{tilesize} {
    calculate_visible_tiles();
}

void Camera::move_to(const Vec<double>& new_location) {
    velocity = (new_location - location) * acceleration;
    // location = new_location;

    // calculate_visible_tiles();
}

Vec<int> Camera::world_to_screen(const Vec<double>& world_position) const {
    Vec<double> scaled =
        (world_position - location) * static_cast<double>(tilesize);
    Vec<int> pixel{static_cast<int>(scaled.x), static_cast<int>(scaled.y)};

    // shift to the center of the screen
    pixel += Vec<int>{graphics.width / 2, graphics.height / 2};

    // flip the y axis
    pixel.y = graphics.height - pixel.y;
    return pixel;
}

void Camera::render(const Vec<double>& position, const Color& color,
                    bool filled) const {
    Vec<int> pixel = world_to_screen(position);
    pixel -= Vec{tilesize / 2, tilesize / 2};  // centered on tile
    SDL_Rect rect{pixel.x, pixel.y, tilesize, tilesize};
    graphics.draw(rect, color, filled);
}

void Camera::render(const Tilemap& tilemap, bool grid_on) const {
    int xmin = std::max(0, visible_min.x);
    int ymin = std::max(0, visible_min.y);
    int xmax = std::min(visible_max.x, tilemap.width - 1);
    int ymax = std::min(visible_max.y, tilemap.height - 1);

    // draw tiles
    for (int y = ymin; y <= ymax; ++y) {
        for (int x = xmin; x <= xmax; ++x) {
            const Tile& tile = tilemap(x, y);
            Vec<double> position{static_cast<double>(x),
                                 static_cast<double>(y)};
            if (tile == Tile::Platform) {
                render(position, {0, 255, 0, 255});
            } else {
                render(position, {0, 127, 127, 255});
            }

            if (grid_on) {
                render(position, {0, 0, 0, 255}, false);
            }
        }
    }
}
 void Camera::update(double dt){
    location += velocity * dt;
     if (location.y < middlePoint){
        location.y = middlePoint;
    }
    // else if (location.y < 5){
    //     location.y = ;
    // }
    // if (static_cast<double>(visible_max.y)/2 > location.y){
    //     location.y = static_cast<double>(visible_max.y)/2;
    // }
    // else if (static_cast<double>(visible_max.y)/2 < location.y){
    //     location.y = 5;
    // }
    // else if(location.y < visible_max.y/2){
    //     location.y = 3;
    // }
    calculate_visible_tiles();
    // else {
    //     location.y = static_cast<double>(visible_max.y/4);
    // }
}

void Camera::calculate_visible_tiles() {
    // number of tiles visible (plus one for the edges)
    Vec<int> num_tiles = Vec{graphics.width, graphics.height};
    num_tiles /= (2 * tilesize);
    num_tiles += Vec{1, 1};
    // (Vec{graphics.width, graphics.height} / (2 * tilesize)) + Vec{1, 1};

    Vec<int> center{static_cast<int>(location.x), static_cast<int>(location.y)};
    visible_max = center + num_tiles;
    visible_min = center - num_tiles;
}