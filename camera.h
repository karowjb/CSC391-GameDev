#pragma once
#include "vec.h"

// Forward declarations
class Graphics;
class Tilemap;
class Color;
class Sprite;
class Object;
class Player;

class Camera {
public:
    Camera(Graphics& graphics, int tilesize);
    void move_to(const Vec<double>& new_location);
    Vec<int> world_to_screen(const Vec<double>& world_position) const;
    void render(const Vec<double>&position, const Color& color, bool filled=true) const;
    void render(const Tilemap& tilemap, bool grid_on=false) const;
    void render(const std::vector<std::pair<Sprite, int>>& backgrounds) const;
    void update(double dt);
    void render( const Vec<double>& position, const Sprite& sprite) const;
    void render(const Object& object) const;
    void render(const Player& player) const;
    // Life and potions bars
    void render_life(int life, int max_life);
    void render_potion(int potions, int max_potions);
private:
    Graphics& graphics;
    int tilesize;
    Vec<double> velocity{0};
    Vec<double> location; // Camera position in world coordinates
    void calculate_visible_tiles();
    Vec<int> visible_min, visible_max;
};