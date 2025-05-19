#ifndef LEVEL_MANAGE_H
#define LEVEL_MANAGE_H
#include "level.h"
#include "raylib.h"
#include <string>
#include <vector>

class LevelManage {
public:
    [[nodiscard]] std::vector<Level> get_levels() const {
        return LEVELS;
    }

    void set_levels(const std::vector<Level> &levels) {
        LEVELS = levels;
    }

    Level &get_current_level() {
        return current_level;
    }

    [[nodiscard]] char * get_current_level_data() const {
        return current_level_data;
    }

    static LevelManage &getInstanceLevel() {
        static LevelManage instance;
        return instance;
    };
    LevelManage(const LevelManage&) = delete;
    LevelManage operator=(const LevelManage&) = delete;
    LevelManage(LevelManage&&) = delete;
    LevelManage operator=(LevelManage&&) = delete;
    bool is_inside_level(int row, int column);
    bool is_colliding(Vector2 pos, char look_for);
    char& get_collider(Vector2 pos, char look_for);
    static void reset_level_index();
    void draw_level();
    void load_level(int offset = 0);

    static void unload_level();

    void set_level_cell(size_t row,  size_t column, char chr);
    void set_current_level(const Level &current_level);
    Level parseLevelRLE(const std::string& rleData);
    std::vector<Level> loadLevelsFromFile(const std::string& filename);
private:
    LevelManage() = default;
    ~LevelManage() = default;
    Level current_level;
    char* current_level_data;
    std::vector<Level> LEVELS;
};
#endif //LEVEL_MANAGE_H