#pragma once

class Object;

class Combat{
public:
    int health{0}, max_health{0}, attack_damage{0};
    int potions{0}, max_potions{0};
    bool is_alive = true;
    bool invincible = false;
    void attack(Object& object);
    void take_damage(int damage);
};