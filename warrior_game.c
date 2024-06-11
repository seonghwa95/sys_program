#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include "game_state.h"
#include "signal_handler.h"

#define SHM_KEY 12345
#define DAMAGE 10
#define EXP_GAIN 20

extern int drink_potion();

int main() {
    GameState *state;
    int shmid;
    pid_t pid;

    // 공유 메모리
    shmid = shmget(SHM_KEY, sizeof(GameState), 0666 | IPC_CREAT);
    state = (GameState *) shmat(shmid, NULL, 0);

    // 저장된 게임 로드
    load_game(state);

    printf("게임을 시작합니다!\n현재 체력: %d\n경험치: %d 퍼센트\n", state->health, state->experience);

    // 시그널 핸들러
    signal(SIGUSR1, handle_signal);

    // 게임 루프
    while (1) {
        char input;
        printf("공격하려면 'a'키를 누르세요 (종료하려면 'q', 물약을 먹으려면 'p'): ");
        scanf(" %c", &input);

        // q 입력시 게임 저장 후 종료
        if (input == 'q') {
            save_game(state);
            break;
        }
        // a 입력시 공격 프로세스
        else if (input == 'a') {
            if ((pid = fork()) < 0) {
                perror("fork");
                exit(1);
            }

            if (pid == 0) {
                // 자식 프로세스가 attack 프로그램 실행
                char shmid_str[10];
                sprintf(shmid_str, "%d", shmid);
                execl("./attack.out", "attack.out", shmid_str, NULL);
                perror("execl");
                exit(1);
            }

            wait(NULL);  // 자식 프로세스 종료 대기
            printf("현재 체력: %d\n경험치: %d 퍼센트\n", state->health, state->experience);

            if (state->health <= 0) {
                // 캐릭터 사망 시 게임 종료
                printf("캐릭터가 사망하였습니다. 게임 오버!!\n");
                save_game(state);
                system("rm game_state.txt");
                break;
            }
        } else if (input == 'p') {
            // 물약을 마시면 체력 회복
            int recovery = drink_potion();
            state->health += recovery;
            if (state->health > MAX_HEALTH) {
                state->health = MAX_HEALTH;
            }
            printf("물약을 마셨습니다! 체력 +%d\n현재 체력: %d\n", recovery, state->health);
        }
    }

    // 공유 메모리 분리 및 제거
    shmdt(state);
    shmctl(shmid, IPC_RMID, NULL);

    return 0;
}
