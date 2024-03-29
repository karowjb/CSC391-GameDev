#include "object.h"
#include "combat.h"

void Combat::attack(Object& object){
    object.combat.take_damage(attack_damage);
}
void Combat::take_damage(int damage){
    if (invincible){
        return;
    }
    invincible = true;
    health -= damage;
    if (health <= 0){
        is_alive = false;
    }
}