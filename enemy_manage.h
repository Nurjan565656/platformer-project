#ifndef ENEMY_MANAGE_H
#define ENEMY_MANAGE_H
#include <vector>
#include <raylib.h>
#include "enemy.h"

class EnemyManage {
public:
    [[nodiscard]] std::vector<Enemy> get_enemies() const {
        return enemies;
    }

    static EnemyManage &getInstance() {
        static EnemyManage instance;
        return instance;
    };
    EnemyManage(const EnemyManage&) = delete;
    EnemyManage operator=(const EnemyManage&) = delete;
    EnemyManage(EnemyManage&&) = delete;
    EnemyManage operator=(EnemyManage&&) = delete;
    void spawn_enemies();
    void update_enemies();
    bool is_colliding_with_enemies(Vector2 pos) const;
    void remove_colliding_enemy(Vector2 pos);

    static void draw_enemies();

private:
    EnemyManage() = default;
    ~EnemyManage() = default;
    std::vector<Enemy> enemies{};
};
#endif //ENEMIES_MANAGER_H