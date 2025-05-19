#ifndef LEVEL_H
#define LEVEL_H
#include <cstddef>
class LevelManage;
class Level {
public:
    Level(const size_t rows, const size_t columns, char* data)
    : rows(rows), columns(columns), data(data) {};
    Level() : rows{0}, columns{0}, data(nullptr) {};



    [[nodiscard]] size_t get_rows() const {
        return rows;
    }

    void set_rows(size_t rows) {
        this->rows = rows;
    }

    [[nodiscard]] size_t get_columns() const {
        return columns;
    }

    void set_columns(size_t columns) {
        this->columns = columns;
    }

    [[nodiscard]] const char* get_data() const {
        return data;
    }
    void set_data(char *data) {
        this->data = data;
    }
    static char& get_level_cell(size_t row, size_t column);

private:
    size_t rows;
    size_t columns;
    char *data = nullptr;
};

#endif //LEVEL_H