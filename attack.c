#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <signal.h>
#include "game_state.h"

#define DAMAGE 10
#define EXP_GAIN 20

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <shmid>\n", argv[0]);
        exit(1);
    }

    int shmid = atoi(argv[1]);
    GameState *state = (GameState *) shmat(shmid, NULL, 0);
    if (state == (void *) -1) {
        perror("shmat");
        exit(1);
    }

    state->health -= DAMAGE;
    state->experience += EXP_GAIN;

    printf("적을 공격했습니다! 체력 -%d, 경험치 +%d\n", DAMAGE, EXP_GAIN);

    shmdt(state);
    kill(getppid(), SIGUSR1);  // 부모 프로세스에 시그널 전송

    return 0;
}
