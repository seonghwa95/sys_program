#include <stdio.h>
#include <stdlib.h>
#include "game_state.h"

// 게임 상태를 저장하는 함수
void save_game(GameState *state) {
    FILE *file = fopen("game_state.txt", "w");
    if (file == NULL) {
        perror("fopen");
        exit(1);
    }

    fprintf(file, "%d %d\n", state->health, state->experience);
    fclose(file);
}

// 게임 상태를 로드하는 함수
void load_game(GameState *state) {
    FILE *file = fopen("game_state.txt", "r");
    if (file == NULL) {
        state->health = MAX_HEALTH;
        state->experience = 0;
        return;
    }

    fscanf(file, "%d %d", &state->health, &state->experience);
    fclose(file);
}
