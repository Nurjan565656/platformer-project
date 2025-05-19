#include "globals.h"
#include "player.h"
#include  "enemy_manage.h"
#include "level.h"
#include "level_manage.h"

#include "player.h"
void Player::reset_player_stats() {
    player_lives = MAX_PLAYER_LIVES;
    for (int i = 0; i < LEVEL_COUNT; i++) {
        player_level_scores[i] = 0;
    }
}

void Player::increment_player_score() {
    PlaySound(coin_sound);
    player_level_scores[level_index]++;
}

int Player::get_total_player_score() {
    int sum = 0;

    for (int i = 0; i < LEVEL_COUNT; i++) {
        sum += player_level_scores[i];
    }

    return sum;
}

void Player::spawn_player() {
    player_y_velocity = 0;

    for (size_t row = 0; row < LevelManage::getInstanceLevel().get_current_level().get_rows(); ++row) {
        for (size_t column = 0; column < LevelManage::getInstanceLevel().get_current_level().get_columns(); ++column) {
            char cell = Level::get_level_cell(row, column);;

            if (cell == PLAYER) {
                Player::getInstancePlayer().set_player_posX(column);
                Player::getInstancePlayer().set_player_posY(row);
                LevelManage::getInstanceLevel().set_level_cell(getInstancePlayer().get_player_posX(), getInstancePlayer().get_player_posY(), AIR);
                return;
            }
        }
    }
}

void Player::kill_player() {
    // Decrement a life and reset all collected coins in the current level
    PlaySound(player_death_sound);
    game_state = DEATH_STATE;
    player_lives--;
    player_level_scores[level_index] = 0;
}

void Player::move_player_horizontally(float delta) {
    // See if the player can move further without touching a wall;
    // otherwise, prevent them from getting into a wall by rounding their position
    float next_x = Player::getInstancePlayer().get_player_posX() + delta;
    if (!LevelManage::getInstanceLevel().is_colliding({next_x, Player::getInstancePlayer().get_player_posY()}, WALL)) {
        Player::getInstancePlayer().set_player_posX(next_x);
    }
    else {
        player_pos.x = roundf(player_pos.x);
        return;
    }

    // For drawing player animations
    looks_forward = delta > 0;
    if (delta != 0) moves = true;
}

void Player::update_player_gravity() {
    // Bounce downwards if approaching a ceiling with upwards velocity
    if (LevelManage::getInstanceLevel().is_colliding({Player::getInstancePlayer().get_player_posX(), Player::getInstancePlayer().get_player_posY() - 0.1f}, WALL) && player_y_velocity < 0) {
        player_y_velocity = CEILING_BOUNCE_OFF;
    }

    // Add gravity to player's y-position

    player_pos.y += player_y_velocity;
    player_y_velocity += GRAVITY_FORCE;


    // If the player is on ground, zero player's y-velocity
    // If the player is *in* ground, pull them out by rounding their position
   player_on_ground = LevelManage::getInstanceLevel().is_colliding({Player::getInstancePlayer().get_player_posX(), Player::getInstancePlayer().get_player_posY() + 0.1f}, WALL);
    if (player_on_ground) { // Use the getter to check the state
        player_y_velocity = 0;
        player_pos.y = roundf(player_pos.y);
    }
}

void Player::update_player() {
    getInstancePlayer().update_player_gravity();

    // Interacting with other level elements
    if (LevelManage::getInstanceLevel().is_colliding(player_pos, COIN)) {
        LevelManage::getInstanceLevel().get_collider(player_pos, COIN) = AIR; // Removes the coin
        Player::getInstancePlayer().increment_player_score();
    }

    if (LevelManage::getInstanceLevel().is_colliding(player_pos, EXIT)) {
        // Reward player for being swift
        if (timer > 0) {
            // For every 9 seconds remaining, award the player 1 coin
            timer -= 25;
            time_to_coin_counter += 5;
if (time_to_coin_counter / 60 > 1) {
                increment_player_score();
                time_to_coin_counter = 0;
            }
        }
        else {
            // Allow the player to exit after the level timer goes to zero
            LevelManage::getInstanceLevel().load_level(1);
            PlaySound(exit_sound);
        }
    }
    else {
        // Decrement the level timer if not at an exit
        if (timer > 0) {
            timer--;
        } else if (timer == 0) {
            game_state = GAME_OVER_STATE;

        }

    }

    // Kill the player if they touch a spike or fall below the level
    if (LevelManage::getInstanceLevel().is_colliding(player_pos, SPIKE) || Player::getInstancePlayer().get_player_posY() > LevelManage::getInstanceLevel().get_current_level().get_rows()) {
        Player::getInstancePlayer().kill_player();
    }

    // Upon colliding with an enemy...
    if (EnemyManage::getInstance().is_colliding_with_enemies(Player::getInstancePlayer().get_player_pos())) {
        // ...check if their velocity is downwards...
        if (player_y_velocity > 0) {
            // ...if yes, award the player and kill the enemy
            EnemyManage::getInstance().remove_colliding_enemy(Player::getInstancePlayer().get_player_pos());
            PlaySound(kill_enemy_sound);

            increment_player_score();
            player_y_velocity = -BOUNCE_OFF_ENEMY;
        }
        else {
            // ...if not, kill the player
            Player::getInstancePlayer().kill_player();
        }
    }
}

void Player::draw_player() {
    horizontal_shift = (screen_size.x - cell_size) / 2;

    // Shift the camera to the center of the screen to allow to see what is in front of the player
    Vector2 pos = {
        horizontal_shift,
        Player::getInstancePlayer().get_player_posY() * cell_size
};

    // Pick an appropriate sprite for the player
    if (game_state == GAME_STATE) {
        if (!(Player::getInstancePlayer().is_player_on_ground())) {
            draw_image((Player::getInstancePlayer().is_looking_forward() ? player_jump_forward_image : player_jump_backwards_image), pos, cell_size);
        }
        else if (Player::getInstancePlayer().is_moving()) {
            draw_sprite((Player::getInstancePlayer().is_looking_forward() ? player_walk_forward_sprite : player_walk_backwards_sprite), pos, cell_size);
            Player::getInstancePlayer().set_is_moving(false);
        }
        else {
            draw_image((Player::getInstancePlayer().is_looking_forward() ? player_stand_forward_image : player_stand_backwards_image), pos, cell_size);
        }
    }
    else {
        draw_image(player_dead_image, pos, cell_size);
    }
}
