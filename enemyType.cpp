#include "enemy.h"
#include "enemyType.h"

EnemyType create_enemy_type(Graphics& graphics, std::string type_name){
    if (type_name == "troll"){
        return create_troll(graphics);
    }
    else if (type_name == "monster"){
        return create_monster(graphics);
    }
    else if(type_name == "dwarf"){
        return create_dwarf(graphics);
    }
    else if(type_name == "skeleton"){
        return create_skeleton_patrol(graphics);
    }
    else {
        throw std::runtime_error("Unknown enemy type " + type_name);
    }
}

std::unique_ptr<Command> default_behavior(Engine&, Enemy& enemy){
    if (abs(enemy.last_edge_position.x - enemy.physics.position.x) > 5){
        enemy.last_edge_position = enemy.physics.position.x;
        enemy.physics.acceleration.x = -enemy.physics.acceleration.x;
    }
    return std::make_unique<Accelerate>(enemy.physics.acceleration.x);
}

std::unique_ptr<Command> standing_behavior(Engine&, Enemy&){
    return std::make_unique<Stop>();
}

std::unique_ptr<Command> hurting(Engine&, Enemy&){
    return std::make_unique<Stop>();
}


// Creating enemy types 
EnemyType create_troll(Graphics& graphics){
    Vec<double> acceleration {-15,gravity};
    AnimatedSprite sprite = graphics.get_animated_sprite("orc_running",0.15, true, false);
    // sprite, acceleration, health, damage, hurting, elapsed time
    return EnemyType{sprite, acceleration,4,2,1,0, default_behavior};
}
EnemyType create_monster(Graphics& graphics){
    Vec<double> acceleration {0,gravity};
    AnimatedSprite sprite = graphics.get_animated_sprite("skeleton_standing",0.15, true, false);
    // sprite, acceleration, health, damage, hurting, elapsed time
    return EnemyType{sprite, acceleration,5,3,1,0, standing_behavior};
}

EnemyType create_dwarf(Graphics& graphics){
    Vec<double> acceleration {-20,gravity};
    AnimatedSprite sprite = graphics.get_animated_sprite("dwarf_running", 0.15, true, false);
    return EnemyType{sprite, acceleration, 20,1,1,0, default_behavior};
}
EnemyType create_skeleton_patrol(Graphics& graphics){
    Vec<double> acceleration {-15, gravity};
    AnimatedSprite sprite = graphics.get_animated_sprite("skeleton_running", 0.15, true, false);
    return EnemyType{sprite, acceleration, 10,3,1,0, default_behavior};
}
