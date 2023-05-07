#include "projectile.h"
#include "engine.h"
#include <cmath>
 
void Projectile::update(Engine& engine, double dt){
    Physics old = physics;
    physics.update(dt);
    // Collisions
    Vec<double> future{physics.position.x, old.position.y};
    Vec<double> vx{physics.velocity.x,0};
    engine.world->move_to(future, size, vx);

    Vec<double> vy{0, physics.velocity.y};
    future.y = physics.position.y;
    engine.world->move_to(future, size, vy);

    physics.position = future;
    physics.velocity = {vx.x, vy.y};

    // rotate arrow if moving
    if (physics.velocity.x != 0 && physics.velocity.y != 0){
        sprite.angle = 90 - std::atan2(physics.velocity.y, physics.velocity.x)*180/M_PI;
    }

    // Collided with something so stop moving
    if (physics.velocity.x == 0 || physics.velocity.y == 0){
        physics.velocity.x = physics.velocity.y = 0;
        physics.acceleration.y = 0;
    }
    if (physics.velocity.x == 0 && physics.velocity.y == 0){
        combat.attack_damage = 0;
        elapsed += dt;
        if (elapsed >= lifetime){
            combat.is_alive = false;
        }
    }
}
