#pragma once
#include "vec.h"

// Forward declarations
class Graphics;
class Tilemap;
class Color;

class Camera {
public:
    Camera(Graphics& graphics, int tilesize);
    void move_to(const Vec<double>& new_location);
    Vec<int> world_to_screen(const Vec<double>& world_position) const;
    void render(const Vec<double>&position, const Color& color, bool filled=true) const;
    void render(const Tilemap& tilemap, bool grid_on=false) const;

private:
    Graphics& graphics;
    int tilesize;
    Vec<double> location; // Camera position in world coordinates
    void calculate_visible_tiles();
    Vec<int> visible_min, visible_max;
};