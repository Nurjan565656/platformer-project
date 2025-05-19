#include "raylib.h"
#include "globals.h"
#include "player.h"
#include "graphics.h"
#include "enemy_manage.h"
#include "level_manage.h"
#include "assets.h"

void update_game() {
    game_frame++;

    switch (game_state) {
        case MENU_STATE:
            if (IsKeyPressed(KEY_ENTER)) {
                SetExitKey(0);
                game_state = GAME_STATE;
                LevelManage::getInstanceLevel().load_level(0);
            }
            break;

        case GAME_STATE:
            if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)) {
                Player::getInstancePlayer().move_player_horizontally(PLAYER_MOVEMENT_SPEED);
            }

            if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A)) {
                Player::getInstancePlayer().move_player_horizontally(-PLAYER_MOVEMENT_SPEED);
            }

            // Calculating collisions to decide whether the player is allowed to jump
        Player::getInstancePlayer().set_is_player_on_ground(
        LevelManage::getInstanceLevel().is_colliding(
    {Player::getInstancePlayer().get_player_posX(), Player::getInstancePlayer().get_player_posY() + 0.1f},
    WALL
            )
        );
            if ((IsKeyDown(KEY_UP) || IsKeyDown(KEY_W) || IsKeyDown(KEY_SPACE)) && Player::getInstancePlayer().is_player_on_ground()) {
                player_y_velocity = -JUMP_STRENGTH;
            }

            Player::getInstancePlayer().update_player();
            EnemyManage::getInstance().update_enemies();

            if (IsKeyPressed(KEY_ESCAPE)) {
                game_state = PAUSED_STATE;
            }
            break;

        case PAUSED_STATE:
            if (IsKeyPressed(KEY_ESCAPE)) {
                game_state = GAME_STATE;
            }
            break;

        case DEATH_STATE:
            Player::getInstancePlayer().update_player_gravity();

            if (IsKeyPressed(KEY_ENTER)) {
                if (player_lives > 0) {
                    LevelManage::getInstanceLevel().load_level(0);
                    game_state = GAME_STATE;
                }
                else {
                    game_state = GAME_OVER_STATE;
                    PlaySound(game_over_sound);
                }
            }
            break;

        case GAME_OVER_STATE:
            if (IsKeyPressed(KEY_ENTER)) {
                LevelManage::getInstanceLevel().reset_level_index();
                Player::getInstancePlayer().reset_player_stats();
                game_state = GAME_STATE;
                LevelManage::getInstanceLevel().load_level();
            }
            break;

        case VICTORY_STATE:
            if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_ESCAPE)) {
                LevelManage::getInstanceLevel().reset_level_index();
                Player::getInstancePlayer().reset_player_stats();
                game_state = MENU_STATE;
                SetExitKey(KEY_ESCAPE);
            }
            break;
    }
}

void draw_game() {
    switch(game_state) {
        case MENU_STATE:
            ClearBackground(BLACK);
            draw_menu();
            break;

        case GAME_STATE:
            ClearBackground(BLACK);
            draw_parallax_background();
            LevelManage::getInstanceLevel().draw_level();
            draw_game_overlay();
            break;

        case DEATH_STATE:
            ClearBackground(BLACK);
            draw_death_screen();
            break;

        case GAME_OVER_STATE:
            ClearBackground(BLACK);
            draw_game_over_menu();
            break;

        case PAUSED_STATE:
            ClearBackground(BLACK);
            draw_pause_menu();
            break;

        case VICTORY_STATE:
            draw_victory_menu();
            break;
    }
}

int main() {
    SetConfigFlags(FLAG_VSYNC_HINT);
    InitWindow(1024, 500, "Platformer");
    SetTargetFPS(60);
    HideCursor();

    load_fonts();
    load_images();
    load_sounds();
    LevelManage::getInstanceLevel().loadLevelsFromFile("data/levels.rll");
    LevelManage::getInstanceLevel().load_level();
while (!WindowShouldClose()) {
        BeginDrawing();

        update_game();
        draw_game();

        EndDrawing();
    }


    LevelManage::getInstanceLevel().unload_level();
    unload_sounds();
    unload_images();
    unload_fonts();

    CloseAudioDevice();
    CloseWindow();

    return 0;
}
