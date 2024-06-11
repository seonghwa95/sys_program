#ifndef GAME_STATE_H
#define GAME_STATE_H

#define MAX_HEALTH 100

typedef struct {
    int health;
    int experience;
} GameState;

void save_game(GameState *state);
void load_game(GameState *state);

#endif // GAME_STATE_H
